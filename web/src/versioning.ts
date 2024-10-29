const versionElement = document.getElementById("version");

if (versionElement != null) {
  versionElement.innerText = `v${window.__APP_VERSION__}`;
}
