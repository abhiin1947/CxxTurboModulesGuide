import { TurboModule, TurboModuleRegistry } from "react-native";

export interface Spec extends TurboModule {
    readonly reverseString: (target: number, callback: (top: number) => void) => void;
}

export default TurboModuleRegistry.getEnforcing<Spec>("NativeSampleModule");
