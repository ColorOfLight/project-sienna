export enum ModelOptions {
  Cube = 0,
  Plane = 1,
  Sphere = 2,
}

export const modelOptionStrings = Object.keys(ModelOptions).filter((key) =>
  isNaN(Number(key))
) as (keyof typeof ModelOptions)[];

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

export type ClientEventComponent = {
  reset: boolean | undefined;
  updateCanvasSize: number[] | undefined;
  changeModel: ModelOptions | undefined;
};

export type ClientStateComponent = {
  model: (typeof modelOptionStrings)[number];
};

export type ControlStrings = {
  [key: string]: {
    [key: string]: string;
  };
};
