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

// 1. We register size change callback
// 2. Renders & paint happen
// 3. User performs action changing view height
// 4. C++ module is notified of change (somehow)
// 5. We want to block paint and execute callback from Step 1.

NativeSampleModule::NativeSampleModule(std::shared_ptr<CallInvoker> jsInvoker)
    : NativeSampleModuleCxxSpec(std::move(jsInvoker)), onBoundsChange_(std::nullopt) {
	}

void NativeSampleModule::reverseString(jsi::Runtime& rt, double target, jsi::Function callback) {
	auto& uiManager = getUIManagerFromRuntime(rt);
	auto shadowNode = uiManager.findShadowNodeByTag_DEPRECATED(target);
	auto offset = dom::getOffset(getCurrentShadowTreeRevision(rt, shadowNode->getSurfaceId()), *shadowNode);
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
  
		runtime_->queueMicrotask(onBoundsChange_.value());
  return newRootShadowNode;
}


} // namespace facebook::react
