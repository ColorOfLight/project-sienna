export type ClientInputComponent = {
  pressedKeyMap: {
    [key: KeyboardEvent["code"]]: boolean;
  };
  isPointerDown: boolean;
  pointerPosition: number[];
  brush: {
    nozzleFov: number;
    airPressure: number;
    paintColor: { r: number; g: number; b: number };
  };
};

export type ModelOptions = "Cube" | "Plane" | "Sphere";

export type ClientEventComponent = {
  reset: boolean | undefined;
  updateCanvasSize: number[] | undefined;
  changeModel: ModelOptions | undefined;
};

export type ClientStateComponent = {
  model: ModelOptions;
};
