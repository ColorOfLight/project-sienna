import {
  ClientInputComponent,
  ClientEventComponent,
  ClientStateComponent,
  modelOptionStrings,
} from "./types";

const canvas = document.getElementById("canvas");

if (canvas == null) {
  throw new Error("Canvas not found");
}

const getCanvasSize = () => [
  canvas.clientWidth * window.devicePixelRatio,
  canvas.clientHeight * window.devicePixelRatio,
];

const getPointerPosition = (event: PointerEvent) => [
  event.clientX * window.devicePixelRatio,
  event.clientY * window.devicePixelRatio,
];

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

window.addEventListener("keydown", (event) => {
  clientInputComponent.pressedKeyMap[event.code] = true;
});

window.addEventListener("keyup", (event) => {
  clientInputComponent.pressedKeyMap[event.code] = false;
});

window.addEventListener("pointerdown", (event) => {
  if (event.target === canvas) {
    clientInputComponent.isPointerDown = true;
  }

  clientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener("pointerup", (event) => {
  clientInputComponent.isPointerDown = false;
  clientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener("pointermove", (event) => {
  clientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener("DOMContentLoaded", () => {
  clientEventComponent.updateCanvasSize = getCanvasSize();
});

window.addEventListener("resize", () => {
  clientEventComponent.updateCanvasSize = getCanvasSize();
});

window.clientInputComponent = clientInputComponent;
window.clientStateComponent = clientStateComponent;
window.clientEventComponent = clientEventComponent;
