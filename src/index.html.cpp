#include <pgmspace.h>
    char index_html[] PROGMEM = R"=====(
    <!DOCTYPE HTML>
    <html>
    <style>
        input[type='number'] {
            background-color: whitesmoke;
            color: #2b2a2a;
        }

        input[type='submit'] {
            background-color: rgb(45, 108, 243);
            width: 100px;
            color: #ffffff;
        }

        input {
            width: 30%;
            height: 44px;
            border-radius: 4px;
            margin: 10px auto;
            font-size: 15px;
        }

        input {
            border: 0;
            padding: 0 15px
        }

        body {
            text-align: center;
            font-family: verdana, sans-serif;
            border-radius: 5px;
            background-color: rgb(23, 23, 23);
            color: whitesmoke;
            padding: 20px;
        }
    </style>

    <head>
        <title>WaterTank Setting</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>

    <body>
        <h1>TANK SETTING</h1>

        <form action="/set">
            <br><br>
            <label>Overflow distance (in cm)</label><br>
            %setmin%
            <br><br><label>Tank height (in cm)</label><br>
            %setmax%
            <br><br><label>Motor start at (in cm)</label><br>
            %setthreshold%
            <br><br>
            <input type="submit" value="Save">
        </form>
    </body>

    </html>
    )=====";