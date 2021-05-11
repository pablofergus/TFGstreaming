<!DOCTYPE html>
<html lang="en">
  <head>
    <title>example</title>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      #container { margin-right: auto; margin-left: auto; max-width: 480px; }
      #info { background: #e0f0f0; border-radius: .5em; padding: 2em;  }
      img { width: 100%; margin-top: 1em; border: 1px solid #ccc; }

    </style>
  </head>
  <body>
    <div id="container">
      <div id="info">
        A div below shows live MJPEG video stream from the server, which
        is simulated by reading files from the images/ directory.
        <br/><br/>
        You can also use <code>curl</code> command-line utility:
        <br/><code>curl localhost:801/api/video1</code>
      </div>
      <img src="/api/video1" />
    </div>
  </body>
</html>