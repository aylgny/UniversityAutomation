const {
  app,
  BrowserWindow,
} = require("electron");

const {
  spawn,
} = require("child_process");

const path =
  require("path");


let backendProcess = null;


const backendPath =
  path.resolve(
    __dirname,
    "../out/build/x64-debug/UniversityAutomation.exe"
  );

function startBackend() {

  if (
    backendProcess
  ) {
    return;
  }


  backendProcess =
    spawn(
      backendPath,
      [],
      {
        cwd:
          path.dirname(
            backendPath
          ),

        windowsHide: true,

        stdio: [
          "ignore",
          "pipe",
          "pipe",
        ],
      }
    );


  backendProcess.stdout.on(
    "data",
    (
      data
    ) => {

      console.log(
        `[Backend] ${data
          .toString()
          .trim()}`
      );
    }
  );


  backendProcess.stderr.on(
    "data",
    (
      data
    ) => {

      console.error(
        `[Backend Error] ${data
          .toString()
          .trim()}`
      );
    }
  );


  backendProcess.on(
    "error",
    (
      error
    ) => {

      console.error(
        "Could not start C++ backend:",
        error
      );

      backendProcess =
        null;
    }
  );


  backendProcess.on(
    "exit",
    (
      code
    ) => {

      console.log(
        `C++ backend exited with code ${code}`
      );

      backendProcess =
        null;
    }
  );
}


function stopBackend() {

  if (
    !backendProcess
  ) {
    return;
  }


  backendProcess.kill();


  backendProcess =
    null;
}


function createWindow() {

  const window =
    new BrowserWindow({
      width: 1200,
      height: 800,

      webPreferences: {
        contextIsolation: true,
        nodeIntegration: false,
      },
    });


  window.loadFile(
    path.join(
        __dirname,
        "../frontend/dist/index.html"
    )
  );
}


app.whenReady().then(
  () => {

    startBackend();

    createWindow();


    app.on(
      "activate",
      () => {

        if (
          BrowserWindow
            .getAllWindows()
            .length === 0
        ) {
          createWindow();
        }
      }
    );
  }
);


app.on(
  "before-quit",
  () => {

    stopBackend();
  }
);


app.on(
  "window-all-closed",
  () => {

    if (
      process.platform !==
      "darwin"
    ) {
      app.quit();
    }
  }
);