import { TurboModule, TurboModuleRegistry } from "react-native";

interface DOMRect {
    x: number;
    y: number;
    width: number;
    height: number;
}

interface Params {
    oldRect: DOMRect;
    newRect: DOMRect;
}

export interface Spec extends TurboModule {
    readonly registerBoundsChangeCallback: (target: number, callback: (params: Params) => boolean) => void;
}

export default TurboModuleRegistry.getEnforcing<Spec>("NativeSampleModule");
