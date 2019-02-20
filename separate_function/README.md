# Separate Functions Without "Internet" Connection

This folder shows how various main sensors and actuators work individually and what separate functions look like without Wi-Fi connection.

It's designed to help people test separate parts first, and thus ensure all components can work well individually.

The brief separate functions' descriptions and required tools are shown below: 

(***Detailed Explanations***, ***Line Connection*** & ***References*** are shown as the beginning annotation of each file!)

## Feeding
FILE "_feeding"

**Use**: 1 DS1302 real-time clock module, 1 micro servo, 1 turbidity sensor & 1 self-made plastic food container with a hole, 1 self-made plastic rotating board.

**Codes Union**: "turbiditySensor", "microServo", "realTime_clock" ("_OLD" for Arduino Uno R3 board, "_RtcDS1302" for Arduino Uno WIFI R2).

**Description**: Feed at pre-set fixed time point, if the water is not too turbid, the food will be released, depending on whether the hole of the food container will be hid with the help of micro servo (connected with other hand-made physical device).

![Initial Feeding Idea]()

## Land-water Swap
FILE "_land_water_swap"

**Use**: 1 photoresistor (light sensor), 2 relay modules, 1 pressure sensor & 1 pulley system, 2 plastic boards (a rotatable ladder board and a fixed above-water flat board).

**Codes Union**: "land_water_swap_without_pressure", "pressureSensor".

**Description**: If (NOT bright && condition "board down") && NO pressure (i.e. no tortoises currently on the flat board), open "pull-up relay" for a while. If bright && condition "board up", open "drop-down relay" for a while.

![Land-water Swap Idea]()

## Lighting
FILE "_lighting"

**Use**: 1 5730LED lamp.

**Description**: Remotely control the lamp and its 3 brightness levels by website command.

## Temperature
FILE "_temperature"

**Use**: 1 water-proof temperature sensor, (1 OLED -> not used for final demo version).

**Codes Union**: "temperature_sensor".

**Description**: Get water temperature data (and show it on the OLED screen), and send real-time message to website, to let owner take further actions by themselves (e.g. open heater remotely using "smart socket").

## Water Changing
FILE "_water_changing"

**Use**: 1 turbidity sensor, 1 pump (with MOS module).

**Codes Union**: "simple_pump_MOS", "turbiditySensor".

**Description**: If the water is turbid than standard (the turbidity voltage is smaller), turn on the pump.

---

The folder is created mainly for *Arduino Uno R3* board. Some codes and libraries are changed for final *Arduino Uno WIFI R2* board. The detailed changes are shown in ["combination"](https://github.com/KathleenQ/tortoise-smart-home/tree/master/combination) folder.
