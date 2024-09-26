const ClientInputComponent = {
  pressedKeyMap: {
    'W': false,
    'A': false,
    'S': false,
    'D': false
  },
  isPointerDown: false,
  pointerPosition: [0, 0],
};

const ClientEventComponent = {
  reset: false,
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

window.ClientInputComponent = ClientInputComponent;
window.ClientEventComponent = ClientEventComponent;
