import { Pane } from "tweakpane";
import { ControlStrings } from "./types";

const controlStrings: ControlStrings = {
  "Object Rotation": {
    Up: "W",
    Down: "S",
    Left: "A",
    Right: "D",
  },
  Camera: {
    "Zoom in": "R",
    "Zoom out": "F",
  },
  Paint: {
    Paint: "Hold Mouse Click",
    Reset: "Click Reset Button",
  },
};

const tweakpaneControlsElement = document.getElementById("tweakpane-controls");

if (tweakpaneControlsElement == null) {
  throw new Error("Tweakpane controls element not found");
}

const pane = new Pane({
  container: tweakpaneControlsElement,
});

const controlsFolder = pane.addFolder({
  title: "Controls (read-only)",
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
