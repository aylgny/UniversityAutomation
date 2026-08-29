/*
 * Application entry point.
 * Provides hash-based routing for the Electron application.
 */

import {
  StrictMode,
} from "react";

import {
  createRoot,
} from "react-dom/client";

import {
  HashRouter,
} from "react-router";

import "./index.css";

import App from "./App";


createRoot(
  document.getElementById(
    "root"
  )!
).render(

  <StrictMode>

    <HashRouter>

      <App />

    </HashRouter>

  </StrictMode>
);