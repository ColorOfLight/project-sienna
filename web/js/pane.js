import {Pane} from 'tweakpane';

const PARAMS = {
  speed: 50,
};

const pane = new Pane(
  {container: document.getElementById('tweak-pane'),}
);

// TODO: replace with the real parameters
pane.addBinding(PARAMS, 'speed', {
  min: 0,
  max: 100,
});

