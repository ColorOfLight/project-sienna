import {Pane} from 'tweakpane';


const pane = new Pane(
  {container: document.getElementById('tweak-pane'),}
);

const brushFolder = pane.addFolder({
  title: 'Brush',
});

brushFolder.addBinding(window.ClientInputComponent.brush, 'airPressure', {
  step: 0.01,
  min: 0.5,
  max: 2,
});

brushFolder.addBinding(window.ClientInputComponent.brush, 'nozzleFov', {
  step: 5,
  min: 15,
  max: 45,
});

brushFolder.addBinding(window.ClientInputComponent.brush, 'paintColor', {
  color: {type: 'float'},
  picker: 'inline',
});
