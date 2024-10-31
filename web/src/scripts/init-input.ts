import { ensureNonNullable, getCanvasSize, getPointerPosition } from "../utils";
import { ClientInputComponent, ClientEventComponent } from "../types";

export const initInputHandlers = (
  clientInputComponent: ClientInputComponent,
  clientEventComponent: ClientEventComponent
) => {
  const canvas = ensureNonNullable(
    document.getElementById("canvas"),
    "Canvas"
  ) as HTMLCanvasElement;

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
    clientEventComponent.updateCanvasSize = getCanvasSize(canvas);
  });

  window.addEventListener("resize", () => {
    clientEventComponent.updateCanvasSize = getCanvasSize(canvas);
  });
};
