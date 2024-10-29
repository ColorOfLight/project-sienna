import {
  ClientEventComponent,
  ClientInputComponent,
  ClientStateComponent,
} from "./types";

// Declare the global window object extension
declare global {
  interface Window {
    clientInputComponent: ClientInputComponent;
    clientStateComponent: ClientStateComponent;
    clientEventComponent: ClientEventComponent;
  }
}

// This is required to ensure the file is treated as a module
export {};
