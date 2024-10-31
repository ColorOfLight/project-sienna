import { ensureNonNullable } from "../utils";

const isDesktop = () => {
  const userAgent = navigator.userAgent.toLowerCase();
  const isTouchDevice =
    "ontouchstart" in window || navigator.maxTouchPoints > 0;

  return !/mobile|android|iphone|ipad|tablet/.test(userAgent) && !isTouchDevice;
};

export const initMobilePopup = () => {
  const mobilePopupElement = ensureNonNullable(
    document.getElementById("mobile-warning-popup"),
    "Mobile warning popup element"
  );

  const mobilePopupCloseElement = ensureNonNullable(
    document.getElementById("mobile-warning-popup-close"),
    "Mobile warning popup close element"
  );

  if (!isDesktop()) {
    const classList = mobilePopupElement.classList;
    classList.remove("display-none");
  }

  mobilePopupCloseElement.addEventListener("click", () => {
    mobilePopupElement.classList.add("display-none");
  });
};
