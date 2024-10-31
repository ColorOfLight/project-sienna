import {
  ClientEventComponent,
  ClientInputComponent,
  ClientStateComponent,
  modelOptionStrings,
} from "./types";
import { initInputHandlers } from "./scripts/init-input";
import { initParamsPane } from "./scripts/init-params-pane";
import { initControlsPane } from "./scripts/init-controls-pane";
import { initVersionText } from "./scripts/init-version-text";
import { initMobilePopup } from "./scripts/init-mobile-popup";

const clientInputComponent: ClientInputComponent = {
  pressedKeyMap: {
    KeyW: false,
    KeyA: false,
    KeyS: false,
    KeyD: false,
    KeyR: false,
    KeyF: false,
  },
  isPointerDown: false,
  pointerPosition: [0, 0],
  brush: {
    nozzleFov: 5,
    airPressure: 1.0,
    paintColor: { r: 1, g: 0.5, b: 0 },
  },
};

const clientStateComponent: ClientStateComponent = {
  model: modelOptionStrings[0],
};

const clientEventComponent: ClientEventComponent = {
  reset: undefined,
  updateCanvasSize: undefined,
  changeModel: undefined,
};

// Expose components to the global scope for WASM to access
window.clientInputComponent = clientInputComponent;
window.clientStateComponent = clientStateComponent;
window.clientEventComponent = clientEventComponent;

initInputHandlers(clientInputComponent, clientEventComponent);
initParamsPane(
  clientInputComponent,
  clientStateComponent,
  clientEventComponent
);
initControlsPane();

initVersionText();
initMobilePopup();
