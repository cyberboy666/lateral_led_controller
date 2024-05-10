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

### accessing the setting
