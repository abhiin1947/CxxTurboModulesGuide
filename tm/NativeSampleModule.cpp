#include "NativeSampleModule.h"
#include <react/renderer/core/ShadowNode.h>
#include <react/renderer/uimanager/UIManagerBinding.h>
#include <react/renderer/uimanager/primitives.h>
#include <react/renderer/dom/DOM.h>


namespace facebook::react {

static UIManager& getUIManagerFromRuntime(jsi::Runtime& runtime) {
  return UIManagerBinding::getBinding(runtime)->getUIManager();
}

static RootShadowNode::Shared getCurrentShadowTreeRevision(
	facebook::jsi::Runtime& runtime,
	SurfaceId surfaceId) {
  auto& uiManager =
	  facebook::react::UIManagerBinding::getBinding(runtime)->getUIManager();
  auto shadowTreeRevisionProvider = uiManager.getShadowTreeRevisionProvider();
  return shadowTreeRevisionProvider->getCurrentRevision(surfaceId);
}

static ShadowNode::Shared getShadowNodeInTree(
    const ShadowNode& shadowNode,
    const ShadowNode& newTree) {
  auto ancestors = shadowNode.getFamily().getAncestors(newTree);
  if (ancestors.empty()) {
    return nullptr;
  }

  auto pair = ancestors.rbegin();
  return pair->first.get().getChildren().at(pair->second);
}

// 1. We register size change callback
// 2. Renders & paint happen
// 3. User performs action changing view height
// 4. C++ module is notified of change (somehow)
// 5. We want to block paint and execute callback from Step 1.

NativeSampleModule::NativeSampleModule(std::shared_ptr<CallInvoker> jsInvoker)
    : NativeSampleModuleCxxSpec(std::move(jsInvoker)), onBoundsChange_(std::nullopt) {
	}

void NativeSampleModule::registerBoundsChangeCallback(jsi::Runtime& rt, double target, jsi::Function callback) {
	auto& uiManager = getUIManagerFromRuntime(rt);
	shadowNode = uiManager.findShadowNodeByTag_DEPRECATED(target);
	auto offset = dom::getOffset(getCurrentShadowTreeRevision(rt, shadowNode.value()->getSurfaceId()), *shadowNode.value());
	uiManager.registerCommitHook(*this);
	runtime_ = &rt;

	onBoundsChange_ = std::move(callback);
//	callback.call(rt, offset.top);
//	rt.queueMicrotask(callback);
}

void NativeSampleModule::commitHookWasRegistered(
	const UIManager& uiManager) noexcept {}
void NativeSampleModule::commitHookWasUnregistered(
	const UIManager& uiManager) noexcept {}

RootShadowNode::Unshared NativeSampleModule::shadowTreeWillCommit(
	const ShadowTree& shadowTree,
	const RootShadowNode::Shared& oldRootShadowNode,
	const RootShadowNode::Unshared& newRootShadowNode) noexcept {

        if (shadowNode.has_value()) {
            auto oldRect = dom::getBoundingClientRect(oldRootShadowNode, getShadowNodeInTree(shadowNode.value(), *oldRootShadowNode), false);
            auto newRect = dom::getBoundingClientRect(newRootShadowNode, *shadowNode.value(), false);
            auto name = shadowNode.value()->getDebugName();
            
            if ((oldRect.width != newRect.width || oldRect.height != newRect.height || oldRect.x != newRect.x || oldRect.y != newRect.y) && onBoundsChange_.has_value()) {
                auto oldRectObject = jsi::Object(*runtime_);
                auto newRectObject = jsi::Object(*runtime_);
                oldRectObject.setProperty(*runtime_, "width", jsi::Value(oldRect.width));
                oldRectObject.setProperty(*runtime_, "height", jsi::Value(oldRect.height));
                oldRectObject.setProperty(*runtime_, "x", jsi::Value(oldRect.x));
                oldRectObject.setProperty(*runtime_, "y", jsi::Value(oldRect.y));
                
                newRectObject.setProperty(*runtime_, "width", jsi::Value(newRect.width));
                newRectObject.setProperty(*runtime_, "height", jsi::Value(newRect.height));
                newRectObject.setProperty(*runtime_, "x", jsi::Value(newRect.x));
                newRectObject.setProperty(*runtime_, "y", jsi::Value(newRect.y));
                
                auto value = jsi::Object(*runtime_);
                value.setProperty(*runtime_, "oldRect", oldRectObject);
                value.setProperty(*runtime_, "newRect", newRectObject);
                auto shouldCommit = onBoundsChange_.value().call(*runtime_, std::move(value));
                if (shouldCommit.isBool()) {
                    return shouldCommit.asBool() ? newRootShadowNode : nullptr;
                } else {
                    return newRootShadowNode;
                }
            }
        }
        
        return newRootShadowNode;
}


} // namespace facebook::react
