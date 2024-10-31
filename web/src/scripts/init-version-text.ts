import { ensureNonNullable } from "../utils";

export const initVersionText = () => {
  const versionElement = ensureNonNullable(
    document.getElementById("version"),
    "Version element"
  );

  if (versionElement != null) {
    versionElement.innerText = `v${window.__APP_VERSION__}`;
  }
};
