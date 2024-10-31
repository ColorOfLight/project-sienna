import { Pane } from "tweakpane";
import { ControlStrings } from "../types";
import { ensureNonNullable } from "../utils";

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

export const initControlsPane = () => {
  const tweakpaneControlsElement = ensureNonNullable(
    document.getElementById("tweakpane-controls"),
    "Tweakpane controls element"
  );

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
};
