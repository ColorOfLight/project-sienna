export const getCanvasSize = (canvas: HTMLCanvasElement) => [
  canvas.clientWidth * window.devicePixelRatio,
  canvas.clientHeight * window.devicePixelRatio,
];

export const getPointerPosition = (event: PointerEvent) => [
  event.clientX * window.devicePixelRatio,
  event.clientY * window.devicePixelRatio,
];

export const ensureNonNullable = <T>(
  value: T | null | undefined,
  fallbackName?: string
): T => {
  if (value == null) {
    throw new Error(`${fallbackName ?? "Value"} cannot be null or undefined`);
  }

  return value;
};
