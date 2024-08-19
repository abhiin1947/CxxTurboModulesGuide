#pragma once

#if __has_include(<React-Codegen/AppSpecsJSI.h>) // CocoaPod headers on Apple
#include <React-Codegen/AppSpecsJSI.h>
#elif __has_include("AppSpecsJSI.h") // CMake headers on Android
#include "AppSpecsJSI.h"
#endif
#include <memory>
#include <string>
#include <react/renderer/core/ShadowNode.h>
#include <react/renderer/uimanager/UIManagerBinding.h>
#include <react/renderer/uimanager/primitives.h>
#include <react/renderer/uimanager/UIManagerCommitHook.h>
#include <react/renderer/uimanager/UIManager.h>

namespace facebook::react {

class NativeSampleModule : public NativeSampleModuleCxxSpec<NativeSampleModule>, public UIManagerCommitHook {
 public:
  NativeSampleModule(std::shared_ptr<CallInvoker> jsInvoker);

  void registerBoundsChangeCallback(jsi::Runtime& rt, double target, jsi::Function callback);
	
#pragma mark - UIManagerCommitHook

  void commitHookWasRegistered(const UIManager& uiManager) noexcept override;
  void commitHookWasUnregistered(const UIManager& uiManager) noexcept override;

  RootShadowNode::Unshared shadowTreeWillCommit(
	  const ShadowTree& shadowTree,
	  const RootShadowNode::Shared& oldRootShadowNode,
	  const RootShadowNode::Unshared& newRootShadowNode) noexcept override;
	
    std::optional<ShadowNode::Shared> shadowNode;
	std::optional<jsi::Function> onBoundsChange_;
	jsi::Runtime* runtime_ = nullptr; // Pointer to the runtime
};

} // namespace facebook::react
