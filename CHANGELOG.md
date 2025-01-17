# CHANGELOG:

Date is in YYYY/MM/DD format.

**v.A.B.C**

**A**: Major Feature / Major Bug Fix

**B**: Minor Feature / Minor Bug Fix

**C**: Refactoring / Tiny Feature / Patch


## [v.2.3.2-beta] - 2024-01-16
**Author:** Antoine
-   Refactoring
-   Added switch cases for button modes
    -   **Dynamic (lights up progressively)**
    -   **Static (ON/OFF)** <br><br>
  
    -   Christmas
    -   Halloween
    -   Chroma
    -   Flashlight
-   Added **bonus led data pin** in case `5` doesnt work use `4`
-   Default button mode also added, along with `BUTTON_ENABLED`
> Note, signal and brake inputs are set to `INPUT_PULLUP` for now to ignore noise. These will have to be swapped to `INPUT`. Signals input are also inverted for debugging purposes
> Also note that, for testing purposes, gyroscope is taking in raw input (gravity is not disregarded)

## [v.2.2.1-beta] - 2024-01-05
**Author:** Antoine
-   Finalized Signals
-   Added flashlight
-   Refactoring
-   Changed format to RGB
-   Included circuit diagram and other media
> Note, signal and brake inputs are set to `INPUT_PULLUP` for now to ignore noise. These will have to be swapped to `INPUT`. Signals input are also inverted for debugging purposes

## [v.2.2.0-beta] - 2024-12-29
**Author:** Antoine
-   Fixed brake light flashing logic that wasnt working
-   Made it so the center leds always flash when braking
-   Made it so signals have variable length (header)
-   Made it so if signal is on, brake becomes static (solid, not progressive)
-   Implemented detecting of signal from wire input of bike
> Note all colours are in GRB for now since different leds are used

## [v.2.1.0-prealpha] - 2024-12-26
**Author:** Antoine
-   Gyro logic, including filter, completed.
    -   Now ignores gravity (calculated during calibration phase on startup)
    -   filters out big spikes in gyro values
        -   if bumps are detected for an extending period of time, overrides
        -   if bumps seem to have a consistent acceleration value, overrides
    -   filters by taking an average from sample size
    -   filters by applying smoothing affect to acceleration
-   Gyro must how only be RELATIVELY flat. 
-   Gyro must do its calibration with no external acceleration present (not accelerating)
> Lots of parameters can be changed in the `gyro.h` header file to get more favorable results


## [v.1.1.0-prealpha] - 2024-11-30
**Author:** Antoine
-   **ALL CONTROL BASED ON GYRO AS INTENDED**
-   ADDED CONRTOLS BASED ON GYRO Y AXIS
-   MUST ENSURE THE GYRO LIGHT IS FACING UP, WITH THE PINS ON THE RIGHT SIDE
-   Issue with brake light, too sensitive, sometimes ini flash will be continuous
> ENSURE GYRO IS FLAT. IT WILL PRINT INITIAL VALUES TO LET YOU KNOW

## [v.1.0.0] - 2024-11-28
**Author:** Antoine
-   **ALL CONTROL BASED ON ENCODER FOR TESTING PURPOSES**
-   Sequential brake lights work
-   Turn signals work
-   Sequention acceleration works, can be turned off in header
-   Mario Star mode for acceleration, can be turned off in header
-   Flash when initialize braking
-   Flash continuously when hard braking

https://github.com/user-attachments/assets/23fc0a4a-a3bb-4595-a7d4-98dc2ed54147

