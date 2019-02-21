# Final Demo Codes Version

Here is the **final** codes combination for the artefact! (Comes from ["combination" process](https://github.com/KathleenQ/tortoise-smart-home/tree/master/combination).)

Brief instructions are shown in the beginning annotation of file. Detailed explanations can be found in intermediate folders ["separate_function"](https://github.com/KathleenQ/tortoise-smart-home/tree/master/separate_function) and ["html_framework"](https://github.com/KathleenQ/tortoise-smart-home/tree/master/html_framework).

Wi-Fi SSID and corresponding password need to be inserted in "wifi_secret.h" file first! *(Default for WPA setting, for WEP, slight changes are needed in "final_version.ino"(-> have pointed out~).)*

Open with Arduino IDE using *Arduino UNO WiFi Rev.2* board.

## Line Connection Summary

For the whole demo, you need to first upload program in Arduino IDE without connecting the LED lamp, then connect the USB port with *a portable battery* and connect the power line of LED as the last step. *(Due to "strong power demand" problem)*

![Line Connection](https://github.com/KathleenQ/tortoise-smart-home/blob/master/pictures/line-connection.jpg)

## Website Monitor 
*(updated website version on 20/2/2019)*:

![Website](https://github.com/KathleenQ/tortoise-smart-home/blob/master/pictures/website-monitor-200219.jpg)

***Real time*** could be read first. 

***Temperature*** shows current water temperature value. 

***Land-water Swap*** shows current brightness level and pressure condition. With judgement, it'll then show whether the ladder board is up or down. 

***Water Changing*** shows whether the water is too turbid to turn on the pump.

***Feeding*** function needs you to reset and change the feeding time remotely. After double checking the turbidity voltage condition, the feeding device (which mainly controlled by a micro servo) will be shown whether it works or not. 

In ***lighting***, 3 different brightness levels are remotely controlled through website button and its condition will be shown as well.

## Simple Test Demo

Here is a simple test demo taken on 15/02/2019. It's not a final demo, please pay attention on the exhibition on 22/02/2019~

As condition limitation, it only mainly test "water changing", "lighting" and "land-water swap" functions without showing the real-time website.

[![Test Demo](http://img.youtube.com/vi/y2FGb_0lQKI/0.jpg)](http://www.youtube.com/watch?v=y2FGb_0lQKI)
