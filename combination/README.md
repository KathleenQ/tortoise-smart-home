# Combination Process of Website & Separate Functions

Add each separate function in [''separate_function'' folder](https://github.com/KathleenQ/tortoise-smart-home/tree/master/separate_function) into the basic website framework in ["html_framework" folder](https://github.com/KathleenQ/tortoise-smart-home/tree/master/html_framework), and come to ["final version"](https://github.com/KathleenQ/tortoise-smart-home/tree/master/final_version) at last.

There are slight changes to "separate functions" through the way, especially for the board change from *Arduino Uno R3* to *Arduino Uno WIFI R2*.

In special, considering efficiency and product life, **"temperature" & "land-water swap" functions only work for even minutes, "water changing" function only works for odd minutes**.

**Used sensors / actuators**, **brief explanation**, **line connection** are shown at the beginning of each file. Codes are expanded in function order: temperature, water changing (with real time), lighting, feeding *(without remotely setting time)*, land-water swap.

The folder is made to show my real working process and better to **test separate functions with Wi-Fi connection** for Arduino Uno WIFI R2 board.
