import { Pane } from "tweakpane";
import {
  ClientInputComponent,
  ClientStateComponent,
  ClientEventComponent,
  ModelOptions,
  modelOptionStrings,
} from "../types";
import { ensureNonNullable } from "../utils";

export const initParamsPane = (
  clientInputComponent: ClientInputComponent,
  clientStateComponent: ClientStateComponent,
  clientEventComponent: ClientEventComponent
) => {
  const tweakpaneParamsElement = ensureNonNullable(
    document.getElementById("tweakpane-params"),
    "Tweakpane params element"
  );

  const pane = new Pane({
    container: tweakpaneParamsElement,
  });

  const paramsFolder = pane.addFolder({
    title: "Parameters",
  });

  const brushFolder = paramsFolder.addFolder({
    title: "Brush",
  });

  brushFolder.addBinding(clientInputComponent.brush, "airPressure", {
    step: 0.1,
    min: 1,
    max: 8,
  });

  brushFolder.addBinding(clientInputComponent.brush, "nozzleFov", {
    step: 5,
    min: 5,
    max: 45,
  });

  brushFolder.addBinding(clientInputComponent.brush, "paintColor", {
    color: { type: "float" },
    picker: "inline",
    expanded: true,
  });

  const modelFolder = paramsFolder.addFolder({
    title: "Model",
  });

  modelFolder
    .addBinding(clientStateComponent, "model", {
      options: modelOptionStrings.reduce(
        (acc, value) => ({ ...acc, [value]: value }),
        {}
      ),
    })
    .on("change", (value) => {
      clientEventComponent.changeModel = ModelOptions[value.value];
    });

  const actionsFolder = paramsFolder.addFolder({
    title: "Actions",
  });

  const resetPaintButton = actionsFolder.addButton({
    title: "Reset Paint",
  });

  resetPaintButton.on("click", () => {
    clientEventComponent.reset = true;
  });
};
