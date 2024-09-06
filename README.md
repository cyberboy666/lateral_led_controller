# lateral_led_controller

### hardware network controller for addressable led installations 

![image](https://github.com/user-attachments/assets/b56ed530-bede-486a-a84b-ba3950885025)

- this circuit is distributed by __UNDERSCORES__ - _an open video hardware label_ : it is available to purchase - as a pcb, kit or assembled unit - at [underscores.shop](https://underscores.shop/lateral_led_controller/)
- the schematic for the circuit can be found [here](/hardware/schematic.pdf)
- the pcb gerber files for the lastest version can be found [here](/hardware/gerber_latest.zip)
- interactive BOM is [here](https://htmlpreview.github.io/?https://github.com/cyberboy666/lateral_led_controller/blob/main/hardware/bom/ibom.html)
- consider [donating](https://opencollective.com/underscores) to the underscores project to help us continue creating for the commons

## description

_lateral_led_controller_ is a hardware interface that converts [Art-Net](https://art-net.org.uk/), a network protocol for transmitting DMX data over network, into signals that drive addressable LEDs. it can be combined with mapping software (such as my [artnet_led_mapper](https://github.com/cyberboy666/artnet_led_mapper/)) to send arbitrary video onto an led strip canvas

features include:

- receives artnet data via ETHERNET, WIFI or ACCESS_POINT
- natively drive addressable leds [WS281X](https://www.aliexpress.com/item/4001322411818.html) and [APA102](https://www.aliexpress.com/item/32322326979.html)
- supports up to `8` parallel data outs and up to `12` DMX universes for max control over `2000` leds !
- local web interface for easily configuring network / led settings
- flexible dc power input - between 5v to 12v with low-heat / efficient on-board step-down converter
- open source hardware and firmware - can be customized to meet your specific needs

## background

this controller is a companion to my [artnet_led_mapper](https://github.com/cyberboy666/artnet_led_mapper/) application that converts a video feed into artnet for addressable led strip installations. it was built for [lateral movement](https://cyberboy666.com/lateralmovement/) club nights such as seen here:

![image](http://underscores.shop/wp-content/uploads/2024/06/leds.gif)


you can find some more background on this install and process here: [WORKS IN PROGRESS 04 - talk about controlling addressable leds for art installation](https://videos.scanlines.xyz/w/p/tUhDfYqJJPfjovscNMvFRL) 

## demo video

[coming soon]

## hardware options

<details><summary>hardware options</summary>

besides this controller there are some more bits of hardware you will need to realize an _addressable led installation_ - i will outline some suggestions here based on my experience but this will depend on your specific requirements - feel free to [email me](mailto:tim@cyberboy666.com) if you would like individual consulting on this.

### led strips

this controller is configured to support two types of addressable led strips (more can be added by firmware update):

### __WS281X__

ie [neopixels](https://www.adafruit.com/product/2541),  [WS2813 (5v) or WS2815 (12v)](https://www.aliexpress.com/item/4001322411818.html)

- these strips are cheapest - around 5usd for 60/m on aliexpress
- data to them is sent over a single wire (D) but is quite slow (around _800kbps_)
- this means that a maximum of around 240 of these leds can be addressed in series before the refresh rate drops below 30fps
- this controller is designed to be able to address up to 8 seperate WS281X led strips in parallel (outputs D0-D7 on board) for maximum 1920 number of leds in total


![image](https://github.com/user-attachments/assets/fd99ba35-70c5-444a-bb6a-b914387d39c2)

### __APA102__

ie [dotstar](https://www.adafruit.com/product/2574) ,  [SK9822](https://www.aliexpress.com/item/32322326979.html)

- these strips cost more - around 10usd for 60/m on aliexpress
- data to them is sent over two wires : ( __D__ata and __C__lock ) but is _very fast_ - (around _24Mbps_ )
- the controller is designed to address a single APA10 led strip by connecting D0 -> Data, D1 -> Clock
- at those speeds there is no need to have parrallel outputs can easily address 2000 leds in series (may need to look into power injection tho)
- they also claim to draw around 1/5 of the power as WS281X (i havnt tested this myself)

![image](https://github.com/user-attachments/assets/4de6dc08-b744-477d-b597-58106c115f09)

## power supply

we ended up using a switching power supply [like this](https://www.aliexpress.com/item/1005002843829663.html) 

- choose voltage based on your strip type (likely to be either 5v or 12v)
- and then power based on the requirement for total number of leds you are using
- you should power the _lateral_led_controller_ from the same supply thats powering the leds (its designed to safely take 5v - 12v input) or atleast ensure the GNDs are connected
- take note on the recommended power wire thickness based on how much current is passing through it (if you use 12v strips then less current is required and therefore thinner wires)

![image](https://github.com/user-attachments/assets/a14c4c9a-6c22-4981-a39e-36884a5f9992)

## light diffusers

for most installation settings it is prefered to use some kind of diffuser to help to evenly distribute light. there are many options and information about this topic online

## wiring and connectors


the data lines from _lateral_led_controller_ uses Pluggable Screw Terminals - standard wire (22awg etc) can connect these lines from controller to the strip

![image](https://github.com/user-attachments/assets/4b82d6b9-e639-4413-a254-619010723e6c)

theres also the option to use cat5/6 cables to carry multiple data lines from the _lateral_led_controller_ - these are quite cheap, come in long distances and have twisted pairs which should allow for longer runs

![image](https://github.com/user-attachments/assets/211844cf-94b4-4366-baef-74deefd615ba)


personally i prefer running power lines seperately (on [red+black paired wire](https://www.aliexpress.com/item/4000625095039.html) ) - and using [these kinds](https://www.aliexpress.com/item/1005004267277214.html) of barrel / screw connectors to easily plug in / unplug from the installation

![image](https://github.com/user-attachments/assets/a575ffa4-9df2-4658-974c-fc0e0b74afea)

most led strips come with these [SM JST 4](https://www.aliexpress.com/item/1005002516911742.html?) connectors pre-soldered to both ends. one option if you dont want to resolder wires / new connectors onto each strip would be to just get some extra of these and create breakouts

![image](https://github.com/user-attachments/assets/95455f20-8f90-485b-b1c4-b9bf2ced58d4)


### custom adapter boards

i made two little pcb adatpers that may help you with format convertion also - see below for some examples of how you could wire them

![image](https://github.com/user-attachments/assets/23ffb9b2-3eb2-40f2-93ea-8bda2d1dc31b)

![image](https://github.com/user-attachments/assets/7045149a-8fe8-46df-9e44-41ffc32d59f9)

![image](https://github.com/user-attachments/assets/27cdb41e-c229-4add-86df-b33254a3c030)


</details>

## software options

<details><summary>software options</summary>

any software that can output artnet over a network can be used with _lateral_led_controller_. some programs i have tested it with include:

- my [artnet_led_mapper](https://github.com/cyberboy666/artnet_led_mapper) application
- [resolume](https://resolume.com/)
- [touch designer](https://derivative.ca/UserGuide/TouchDesigner)
- [QLC+](https://www.qlcplus.org/)

</details>

# documentation

this project is fully _open-source hardware_ - all the files required to build it are included in this repo for free. if you have the time and/or skill you can contribute back by collaborating on / testing new designs, improving these docs, making demo videos/other creative content etc. you can also support the project financially by [donating](https://opencollective.com/underscores) directing, or purchasing through the [web shop](https://underscores.shop).

depending on whether you are going fully diy or buying an assembled and tested unit, some of the following guides will be relavent to you. the flow would be:

## ordering parts

<details><summary><b>parts sourcing guide (w/ notes on pcb fabracation )</b> - start here if you are building fully from scatch or have purchased a pcb</summary>
  

i try to source all the parts i can from either:
- [tayda](https://www.taydaelectronics.com/) ; cheaper for common parts like resistors etc, also good for mechanical parts like switches and buttons
- [mouser](https://www.mouser.de/) ; has lots more options, speciality video ic's, can sometimes cost more (free shipping on orders over 50euros)
- other ; ocationally there will be parts which will need to be sourced elsewhere - usaully either aliexpress, ebay or amazon etc...

take a look at the [full_bom](/hardware/bom/full_bom.csv) for this project to see where i am sourcing each part from

## import into tayda

- go to the [tayda quick order](https://www.taydaelectronics.com/quick-order/) and in bottom corner choose _add from file_
- select the file [tayda_bom.csv](./hardware/bom/tayda_bom.csv) in the BOM folder (you will have to download it first or clone this repo)
- after importing select _add to cart_
- __NOTE:__ the minimum value for resistors is 10, so you may need to modify these values to add to cart (or if they are already modified here you will need to see the  full_bom for actual part QTY) 

- OPTIONAL: it is a good idea to add some dip-ic sockets and 2.54pin headers/sockets to your tayda order if you dont have them around already

## sourcing other parts

this circuit has a few _specific_ parts that can not be sourced from tayda/mouser:

### WT32-ETH01 micro-controller
we are using a [WT32-ETH01](https://www.aliexpress.com/item/1005007279607510.html) ESP32 controller with the LAN8720A chip / ethernet port already attached - this is because it requires specific routing that is easier to get in a module like this - [heres](https://github.com/egnor/wt32-eth01) some more general info about this uC - it should be easy to find with a search on ali/amazon/ebay/etc - if you get it without header pins you can solder the controller straight on to the pcb (using the castellated edges) 

![image](https://github.com/user-attachments/assets/6898c883-868a-46ce-b203-ca0a9fc6f7b0)

### MP1584EN_5v_stepdown

another pre-soldered module used in this circuit is this [MP1584EN_5v_stepdown]([MP1584EN_5v_stepdown](https://www.aliexpress.com/item/1005006005888518.html)) - 3A Step-down 5v

its a useful module that allows a wide range of voltage inputs and is much more efficent than standard linear regulators (so producing less heat - which is important for installation work) - you can also solder the board directly onto our pcb with the castellated edges - take note of the direction indicated by arrow on bottom of module

![image](https://github.com/user-attachments/assets/519af38c-6c49-4a2e-8758-ee4c2f00e200)

### WeMos_CH340G_USB_to_SERIAL

since the WT32-ETH01 doesnt have an on-board _usb-to-serial_ on the module for uploading firmware this board is designed to work with this [WeMos_CH340G_USB_to_SERIAL](https://www.aliexpress.com/item/1005006642575408.html) one - we are using this because it outputs the command signals that allow for auto-uploading (so we dont need to manually switch to boot mode every time) - you may need to join some pads with solder on the module to select DTS mode for it to work

![image](https://github.com/user-attachments/assets/0647dc8c-2f8c-4b08-b302-41dc97b6437a)


![Screenshot from 2024-09-06 13-56-25](https://github.com/user-attachments/assets/48900809-941f-4626-86d2-d2ff11dc5ee2)

## terminal jacks

the footprints on the pcb for the data line outputs are spaced for __3.5MM__

- im using [KF2EDG RIGHT ANGLE 3.5MM PLUGGABLE](https://www.aliexpress.com/item/1005005511990041.html) terminals in my kits, but this is up to you - standard screw terminal (non-pluggable) like [this](https://www.aliexpress.com/item/1005001333343844.html) could also fit..

![image](https://github.com/user-attachments/assets/8c9686a7-936e-47c8-957d-84c8a8925cf8)


## ordering pcbs

you can support this project by buying individual pcbs from the [shop](https://underscores.shop). if you would rather have pcbs fabricated from gerbers directly the file you need is [here](/hardware/gerber_latest.zip)

- i get my pcbs fabricated from [jlcpcb](https://cart.jlcpcb.com/quote) - 5 is the minumum order per design
- upload the zip file with the `add gerber file` button
- the default settings are mostly fine - set the __PCB Qty__ and __PCB Color__ settings (you can check that the file looks correct with pcb veiwer)
- it may be best to combine orders with other pcbs you want to have fab'd since the shipping can cost more than the items - also orginising group buys is a good way to distribute the extra pcbs /costs 
  
i often use jlcpcb because they are reliable, cheap and give you an option of colours. remember though that the cheapest Chinese fab houses are not always the most ethical or environmently friendly - if you can afford it consider supporting local companies. 

  </details>
  
## assembly guide
  
<details><summary><b>assembly guide</b> - start here if you have purchased a diy kit</summary>

## interactive BOM for build guiding

follow this link to view the [interactive BOM](https://htmlpreview.github.io/?https://github.com/cyberboy666/lateral_led_controller/blob/main/hardware/bom/ibom.html)

## general solder advice

- remember to heat pad first (2-3seconds), then add solder, then continue to heat (1-2seconds)

- Checkout the web-comic [soldering is easy](https://mightyohm.com/files/soldercomic/FullSolderComic_EN.pdf) for more soldering advice

## order of assembly

- both the _WT32-ETH01_ controller and the _MP1584EN_5v_stepdown_ module can be soldered directly to the board using the castellated edges (if you would rather use pins/sockets this is fine also - just be aware you may need to use higher standoffs if also mounting a front panel) - these should be soldered before anything else

- next i would solder the resistors, capictors (take note of direction), diodes, transistors etc

- finally would finish with ic's and interface parts - sockets, buttons etc...

</details>

## firmware guide

<details><summary><b>firmware guide</b> - for editing the code & flashing it to your micro-controller</summary>
  
## flashing firmware onto the micro-controller
  
if you have got a kit from the shop the default firmware will be pre-configured - still it could be useful to know how to update firmware / customize your controller.
  
### install guide
  
all _underscores_ projects with micro-controllers use [platformio](https://platformio.org/) with [visual studio code](https://code.visualstudio.com/) to edit, flash and monitor the code.
  
- first download (and unzip) the code in this repo - easiest is [as a zip](https://github.com/cyberboy666/lateral_led_controller/archive/refs/heads/main.zip) or you can clone using git if you are comfortable with this
- next download, install and open [visual studio code](https://code.visualstudio.com/#alt-downloads)
- now open the extension tab within vscode on left vertical menu (or press ctrl-shift-x) and search for `platformio` to install this extension
  
![image](https://user-images.githubusercontent.com/12017938/158495161-7c3114fc-814b-4acc-b142-4a9522370473.png)

- connect the micro-controller to computer via usb with the __WeMos_CH340G_USB_to_SERIAL__ port, open the software folder of this project (ctrl-k ctrl-o) in vscode and find the _platformio_ commands (either in left vertical menu under _platformio_ or little tick/arrow symbols along bottom blue bar) - `PlatformIO: Upload` should flash the default code to your micro-controller
  
![image](https://user-images.githubusercontent.com/12017938/158495844-99466196-086a-47d2-b803-2b5941d33ac5.png)

</details>

</details>

## operating guide
  
<details><summary><b>operating guide</b> - start here if you have purchased an assembled unit</summary>

![image](https://github.com/user-attachments/assets/24260926-f897-4c14-985b-2637f945ce2b)


## access settings interface for first time

- power on the circuit by plugging in a 5-12v dc supply into the POWER IN
- using a computer / phone connect to the ACCESS_POINT created by the controller: default name `leds` pw `ledsleds` - you should see the blue _network connected_ led come on when you have connected to this network
- in a browser go to `http://leds.local` (also try `http://192.168.4.1` if that doesnt work ) - you should now see the web interface!
- from here you can set up which connection mode to use, see your controllers ip and also set which led protocol & layout to use - make sure the led settings (led type, number of data lines and number of leds per line ) reflect your installation setup.
- for most installation settings it will be preferable to use wired ethernet connection to send data into this controller - make sure the IP/SUBMASK/GATEWAY settings reflect the ethernet settings being used on the computer this controller is connecting to. __auto assign ethernet connection__ worked from my linux machine but i could not get windows to assign an ip (act as dhcp) - maybe there is some setting or maybe it needs to be done manually from some operating systems such as windows.

![image](https://github.com/user-attachments/assets/8b49bebf-3b1b-4bed-84be-f57d4eba3ade)

![image](https://github.com/user-attachments/assets/c307ca27-7ae2-421c-893b-cc47e49fe88f)


## reset button

### soft reset

on the controller board next to the `NETWORK CONNECTED` led is a `CONTROLLER RESET` button - pushing this will reboot the controller -> you should see the _network connected_ light go off for a momement while it resets

### hard reset

if you hold down the `CONTROLLER RESET` button for 5 seconds then release it you should see the `NETWORK CONNECTED` led flashing -> then short pressing the `CONTROLLER RESET` again from this mode will perform a __hard reset__ this means that the _NETWORK TYPE_ is reset back to __ACCESS_POINT__ and all settings are restored to default (you will need to connect to `leds` with pw `ledsleds` again to set up your controller)

this can be helpful if you accidently put the controller into a state with network settings that you can not access the page any more.

(if you get to the flashing led state but do not want to do a hard reset just power down the controller while flashing and your settings will not be wiped)


</details>

### more info

<details><summary><b>how the circuit works</b></summary>
  
[coming soon]
                                                                                                                             
</details>

<details><summary><b>contributing guide</b></summary>
  
if you would like to contribute back to these projects in some way but dont know how the best thing (for now) would be to reach out to me directly ( tim@cyberboy666.com or @cyberboy666 on scanlines forum) - i will be happy to help
  
</details>


## credits & more info


This circuit is distributed through UNDERSCORES – open video hardware label – visit [underscores.shop](https://underscores.shop) for more info

The pcb was designed using KICAD , the booklet was created in LibreOffice Draw

Everything from gerbers, cad files, panels and documentation is freely available online and distributed under CC-BY-SA / open-source licenses – help us contribute to the commons !

Ask any questions or start discussions related to this project on the [scanlines.xyz](https://scanlines.xyz) forum – an online community space dedicated to diy av / electronic media art

You can contact me directly at tim (at) cyberboy666 (dot) com 
Please get in touch if you are interested in hosting a workshop !

![image](https://github.com/user-attachments/assets/9879806e-2057-46f2-97a5-5c3c61af2ddf)


thanks to Tor for user testing + detailed feedback and suggestions + help with web interface design. thanks to the Lateral Movement Crew: Leo, Teesh, Wendy, Luke, Sean - for all the work into making these parties happen
