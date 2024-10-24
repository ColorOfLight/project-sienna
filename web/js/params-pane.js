import {Pane} from 'tweakpane';


const pane = new Pane(
  {container: document.getElementById('tweakpane-params'),}
);

const paramsFolder = pane.addFolder({
  title: 'Parameters',
});

const brushFolder = paramsFolder.addFolder({
  title: 'Brush',
});

brushFolder.addBinding(window.ClientInputComponent.brush, 'airPressure', {
  step: 0.1,
  min: 1,
  max: 8,
});

brushFolder.addBinding(window.ClientInputComponent.brush, 'nozzleFov', {
  step: 5,
  min: 5,
  max: 45,
});

brushFolder.addBinding(window.ClientInputComponent.brush, 'paintColor', {
  color: {type: 'float'},
  picker: 'inline',
  expanded: true,
});

const actionsFolder = paramsFolder.addFolder({
  title: 'Actions'
});

const resetButton = actionsFolder.addButton({
  title: 'Reset'
});

resetButton.on('click',
  () => {
    window.ClientEventComponent.reset = true;
  }
);
