import {Pane} from 'tweakpane';
import { modelOptions } from '../src/input';

const pane = new Pane(
  {container: document.getElementById('tweakpane-params'),}
);

const paramsFolder = pane.addFolder({
  title: 'Parameters',
});

const brushFolder = paramsFolder.addFolder({
  title: 'Brush',
});

brushFolder.addBinding(window.clientInputComponent.brush, 'airPressure', {
  step: 0.1,
  min: 1,
  max: 8,
});

brushFolder.addBinding(window.clientInputComponent.brush, 'nozzleFov', {
  step: 5,
  min: 5,
  max: 45,
});

brushFolder.addBinding(window.clientInputComponent.brush, 'paintColor', {
  color: {type: 'float'},
  picker: 'inline',
  expanded: true,
});

const modelFolder = paramsFolder.addFolder({
  title: 'Model',
});

modelFolder.addBinding(window.clientStateComponent, 'model', {
  options: modelOptions.reduce((acc, value) => ({ ...acc, [value]: value,}), {}),
}).on('change',
  (value) => {
    window.clientEventComponent.changeModel = modelOptions.indexOf(value.value);
  }
);


const actionsFolder = paramsFolder.addFolder({
  title: 'Actions'
});

const resetPaintButton = actionsFolder.addButton({
  title: 'Reset Paint'
});

resetPaintButton.on('click',
  () => {
    window.clientEventComponent.reset = true;
  }
);
