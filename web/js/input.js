const canvas = document.getElementById('canvas');

const getCanvasSize = () => [canvas.clientWidth * window.devicePixelRatio, canvas.clientHeight * window.devicePixelRatio];
const getPointerPosition = (event) => [event.clientX * window.devicePixelRatio, event.clientY * window.devicePixelRatio];
const ClientInputComponent = {
  pressedKeyMap: {
    'KeyW': false,
    'KeyA': false,
    'KeyS': false,
    'KeyD': false,
    'KeyR': false,
    'KeyF': false,
  },
  isPointerDown: false,
  pointerPosition: [0, 0],
  brush: {
    nozzleFov: 5,
    airPressure: 1.0,
    paintColor: {r: 1, g: 0.5, b: 0},
  }
};

const ClientEventComponent = {
  reset: undefined,
  updateCanvasSize: undefined,
};

window.addEventListener('keydown', (event) => {
  ClientInputComponent.pressedKeyMap[event.code] = true;
});

window.addEventListener('keyup', (event) => {
  ClientInputComponent.pressedKeyMap[event.code] = false;
});

window.addEventListener('pointerdown', (event) => {
  if (event.target === canvas) {
    ClientInputComponent.isPointerDown = true;
  }
  
  ClientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener('pointerup', (event) => {
  ClientInputComponent.isPointerDown = false;
  ClientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener('pointermove', (event) => {
  ClientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener('DOMContentLoaded', () => {
  ClientEventComponent.updateCanvasSize = getCanvasSize();
});

window.addEventListener('resize', () => {
  ClientEventComponent.updateCanvasSize = getCanvasSize();
});

window.ClientInputComponent = ClientInputComponent;
window.ClientEventComponent = ClientEventComponent;
