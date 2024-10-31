import { ensureNonNullable } from "../utils";

const isPopupSeen = () => {
  return localStorage.getItem("initialInfoPopupSeen") === "true";
};

export const initInitialInfoPopup = () => {
  const infoPopupElement = ensureNonNullable(
    document.getElementById("initial-info-popup"),
    "Initial info popup element"
  );

  const infoPopupCloseElement = ensureNonNullable(
    document.getElementById("initial-info-popup-close"),
    "Initial info popup close element"
  );

  if (isPopupSeen()) {
    infoPopupElement.classList.add("display-none");
  }

  infoPopupCloseElement.addEventListener("click", () => {
    infoPopupElement.classList.add("display-none");
    localStorage.setItem("initialInfoPopupSeen", "true");
  });
};
