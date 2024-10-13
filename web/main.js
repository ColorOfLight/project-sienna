const canvas = document.getElementById('canvas');

const getCanvasSize = () => [canvas.clientWidth * window.devicePixelRatio, canvas.clientHeight * window.devicePixelRatio];
const getPointerPosition = (event) => [event.clientX * window.devicePixelRatio, event.clientY * window.devicePixelRatio];
const ClientInputComponent = {
  pressedKeyMap: {
    'KeyW': false,
    'KeyA': false,
    'KeyS': false,
    'KeyD': false
  },
  isPointerDown: false,
  pointerPosition: [0, 0],
  canvasSize: getCanvasSize(),
};

const ClientEventComponent = {
  reset: false,
  changeCanvasSize: false,
};

window.addEventListener('keydown', (event) => {
  ClientInputComponent.pressedKeyMap[event.code] = true;
});

window.addEventListener('keyup', (event) => {
  ClientInputComponent.pressedKeyMap[event.code] = false;
});

window.addEventListener('pointerdown', (event) => {
  ClientInputComponent.isPointerDown = true;
  ClientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener('pointerup', (event) => {
  ClientInputComponent.isPointerDown = false;
  ClientInputComponent.pointerPosition = getPointerPosition(event);
});

window.addEventListener('pointermove', (event) => {
  ClientInputComponent.pointerPosition = getPointerPosition(event);
});

document.getElementById('reset').addEventListener('click', () => {
  ClientEventComponent.reset = true;
});


window.addEventListener('DOMContentLoaded', () => {
  ClientEventComponent.changeCanvasSize = true;
  ClientInputComponent.canvasSize = getCanvasSize();
});

window.addEventListener('resize', () => {
  ClientEventComponent.changeCanvasSize = true;
  ClientInputComponent.canvasSize = getCanvasSize();
});

window.ClientInputComponent = ClientInputComponent;
window.ClientEventComponent = ClientEventComponent;
