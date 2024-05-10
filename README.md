# ethernet_led_controller

WIP

## getting started:

### powering the controller

this circuit needs 5v to run. there are a few different ways you can send power to this controller:

- 5v over __2.1 barrel_jack__ (centre positive) - ensure the _power_input_select_ jumper is set to the left (this bypasses stepdown module - actually shouldnt matter if set to right sending 5v to stepdown should work and not be dangerous)
- 5v over __power terminal block__ (top = GND, bottom = 5v ) - ensure the _power_input_select_ jumper is set to the left...
- 12v over __2.1 barrel jack__ (centre positive) - ensure the __power_input_select__ jumper is set to the RIGHT - __important! this could fry mcu if sending 12v directly to it!__
- 12v over __power terminal block__ (top = GND, bottom = 12v ) - ensure the _power_input_select_ jumper is set to the RIGHT... __important! this could fry mcu if sending 12v directly to it!__

### WeMos_USB_to_SERIAL

on top middle of board there is 6 pins which can be connected to the __WeMos_USB_to_SERIAL__ adapter that comes with this circuit (micro-usb port facing up). this is useful for:

- updating the firmware on the micro controller
- monitoring the serial console for debugging

if you are not doing either of these things it is recommended to remove the serial module from the board.

if you are using the serial monitor there is another jumper ( _power over serial_ ) to select whether the board should:

- take 5v power directly from the micro-usb plugged into the _serial module_ - jumper set to __power over serial__ - this allows updating/monitoring the board with only 1 cable connected
- do not take power from serial micro-usb connection - jumper set away from __power over serial__ (or not connected at all) you will need to power the board also in one of the ways listed above

_when WeMos_USB_to_SERIAL module is connected and controller is running there is a known bug where ethernet will not work unless the serial_monitor is open - this is due to ETH01 boards using same pin for ethernet clock and program_low IO0_

## accessing the settings interface

this controller can connect to a network in three ways:

- ETHERNET
- WIFI
- ACCESS_POINT

you can tell when it is connected because the blue NETWORK CONNECTED led will be on.

the default connection mode is ACCESS_POINT (as this should be easiest to get on to in any situation)

from a computer look for the default wifi_network name `leds` and connect to it with the default password `ledsleds`

then you should see the blue led come on. now you are connected try accessing `http://leds.local` (also try `http://192.168.4.1` if that doesnt work )from a web-browser and you should see the config page.

from here you can set up which connection mode to use, see your controllers ip and also set which led protocol & layout to use.

_known issue: although it works fine from linux, i couldnt get __auto assign ethernet connection__ to work on windows - it seems like windows can not designate an ip for the controller if it does not set one itself -> maybe there is a way to fix this -> doesnt really matter tho... static ip is better in most cases anyway..._

## reset button

### soft reset

on the controller board next to the `NETWORK CONNECTED` led is a `CONTROLLER RESET` button - pushing this will hard reboot the controller -> you should see the _network connected_ light go off for a momement while it resets

### hard reset

if you hold down the `CONTROLLER RESET` button for 5 seconds then release it you should see the `NETWORK CONNECTED` led flashing -> short pressing the `CONTROLLER RESET` again from this mode will perform a __hard reset__ this means that the _NETWORK TYPE_ is reset back to __ACCESS_POINT__ and all settings are restored to default (you will need to connect to `leds` with pw `ledsleds` again to set up your controller)

this can be helpful if you accidently put the controller into a state with network settings that you can not access the page any more.

(if you get to the flashing led state but do not want to do a hard reset just power down the controller while flashing and your settings will not be wiped)

## addressing data to led strips

this controller is configured to support two types of addressable led strips:

### __WS281X__

ie [neopixels](https://www.adafruit.com/product/2541),  [WS2813 (5v) or WS2815 (12v)](https://www.aliexpress.com/item/4001322411818.html)

- these strips are cheapest - around 5usd for 60/m on aliexpress
- data to them is sent over a single wire (D) but is quite slow (around _800kbps_)
- this means that a maximum of around 240 of these leds can be addressed in series before the refresh rate drops below 30fps
- this controller is designed to be able to address up to 8 seperate WS281X led strips in parallel (outputs D0-D7 on board) for maximum 1920 number of leds in total


### __APA102__

ie [dotstar](https://www.adafruit.com/product/2574) ,  [SK9822](https://www.aliexpress.com/item/32322326979.html)

- these strips cost more - around 10usd for 60/m on aliexpress
- data to them is sent over two wires : ( __D__ata and __C__lock ) but is _very fast_ - (around _24Mbps_ )
- the controller is designed to address a single APA10 led strip by connecting D0 -> Data, D1 -> Clock
- at those speeds there is no need to have parrallel outputs can easily address 2000 leds in series (may need to look into power injection tho)
- they also claim to draw around 1/5 of the power as WS281X (i havnt tested this myself)

from the settings page you can select the _led type_:

if you choose WS281X then you also will have the choice to select how many parallel outputs to use and how many leds are connected to each of thoses outputs

## data line wiring

the eight data-line outputs on the controller (D0-D7) can be connected with wire directly to the led strips using the __spring-terminal blocks__ on the pcb

(you also could use some kind of [Quick Wire Conductor](https://www.aliexpress.com/item/1005003870395163.html?) on led strip end to easily connect and disconnect the wire on both ends)

for longer runs you also can use the two __rj45 data jacks__ (D0-D3 and D4-D7) to easily send multiple datalines (twisted paired with GND) over a distance (long cat cables are very cheap) 

there are also two types of _connector_boards_ to help with these different wiring options:

- __rj45_to_terminal_block__ : used to split out the 4 data lines on data_rj45 into individual wires
- __terminal_block_plus_barrel_to_JST-SM__ used to combine indivial data wires + power over barrel_jack to the JST-SM connector that comes with these led strips -> can be used if you do not want to change the connector that comes with the strips
