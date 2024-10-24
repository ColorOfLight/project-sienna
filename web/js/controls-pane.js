import {Pane} from 'tweakpane';

const controlStrings = {
  'Object Rotation': {
    'Up': 'W',
    'Down': 'S',
    'Left': 'A',
    'Right': 'D',
  },
  'Camera': {
    'Zoom in': 'R',
    'Zoom out': 'F',
  },
  'Paint': {
    'Paint': 'Hold Mouse Click',
    'Reset': 'Click Reset Button'
  }
}


const pane = new Pane(
  {container: document.getElementById('tweakpane-controls'),}
);

const controlsFolder = pane.addFolder({
  title: 'Controls (read-only)',
});

for (const [groupName, groupValue] of Object.entries(controlStrings)) {
  const groupFolder = controlsFolder.addFolder({
    title: groupName,
  });

  for (const [name, _] of Object.entries(groupValue)) {
    groupFolder.addBinding(controlStrings[groupName], name, {
      readonly: true,
    });
  }
}
