const canvas = document.getElementById('canvas');

const getCanvasSize = () => [canvas.clientWidth * window.devicePixelRatio, canvas.clientHeight * window.devicePixelRatio];

const ClientInputComponent = {
  pressedKeyMap: {
    'W': false,
    'A': false,
    'S': false,
    'D': false
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
  ClientInputComponent.pressedKeyMap[event.key.toUpperCase()] = true;
});

window.addEventListener('keyup', (event) => {
  ClientInputComponent.pressedKeyMap[event.key.toUpperCase()] = false;
});

window.addEventListener('pointerdown', (event) => {
  ClientInputComponent.isPointerDown = true;
  ClientInputComponent.pointerPosition = [event.clientX, event.clientY];
});

window.addEventListener('pointerup', (event) => {
  ClientInputComponent.isPointerDown = false;
  ClientInputComponent.pointerPosition = [event.clientX, event.clientY];
});

window.addEventListener('pointermove', (event) => {
  ClientInputComponent.pointerPosition = [event.clientX, event.clientY];
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
