# HDD centrifuge

The design of this machine have been made by OpenLab Madrid, *getting the initial idea* in the *hackteria wiki* (https://hackteria.org/wiki/Hacked_Hard-disc_Centrifuge) and adding some changes to adapt the machine to our needs.
## Electronics
### Understanding the components
![alt text](https://github.com/OpenLabMadrid/HDD-centrifuge/blob/master/Electronics/Photos/Electronic%20diagram.png?raw=true "Electronic diagram")


_Part 1: Esc controller_. This is the driver of the HDD Brushless motor. It send a triphasic PWM signal over 3 pins of the motor. The problem is that motors use to have 4 pins. You need to found the 3 pins attached to the 3 inductors of the motor and "discard" the pin associated to the common ground of them. To made this, you will need a polimeter to measure the resistances between the different pins. The pins of the three inductors will have the same resistance between them and the ground. You will need to keep measuring the resistance until you found the pin with which the other 3 have the same resistance. This is the ground, the other 3 must be the inductor pins. Connect them to the ESC controller.

![alt text](https://github.com/OpenLabMadrid/HDD-centrifuge/blob/master/Electronics/Photos/HDD%20measurement.gif?raw=true)

_Part 2: Arduino nano_. For mounting all the electronic chips, we have been using perforated PCBs. The Arduino nano is the main controller andd we are using the chinese clone (from Aliexpress, Banggood,...) with the TTL CH340 chip which needs a expecial driver to program it from the PC. You could find easily on google searching for "CH340 arduino driver".

![alt text](https://github.com/OpenLabMadrid/HDD-centrifuge/blob/master/Electronics/Photos/mounted%20pcb.png?raw=true "PCB")

The pins that we are using are:
- LCD
	- SDA: pin A4
	- SCL: pin A5
	- GND: Arduino GND
	- VCC: Arduino +5 V
- Encoder
	- Encoder output A: pin D3
	- Encoder output B: pin D4
	- Encoder button output: pin D2
- ESC
	- ESC digital control: pin D9
	- Enable ESC (to the base of mosfet that enable ESC controller): pin D6

To solder the components between eachothers we are using awg30 wire:

![alt text](https://github.com/OpenLabMadrid/HDD-centrifuge/blob/master/Electronics/Photos/awg.jpg?raw=true "awg pcb")

## Making the solid parts (case and rotor)
### Step 1: Case
The case files are in the "Laserable and 3D printable" folder. Here, you will find the SolidWorks assembly files of all the parts you will need and the .dxf files to cut in a laser cutter machine. If you don´t have the SolidWorks software don´t worry, we have attached a .jpg capture of all the case mounted in the "Images of the mounted device" folder: 

![alt text](https://github.com/OpenLabMadrid/HDD-centrifuge/blob/master/Laserable%20and%203D%20printable/Case/Images%20of%20the%20mounted%20device/Assembly2.JPG?raw=true "Assembembly example 2")

The material must be 3mm thickness and we have used MDF for the HDD enclosing and transparent acrylic for the cover. The MDF have been stuck with white glue and the acrylic with cloroform or 
cyanoacrylate (better the first one).  

### Step 2: Rotor
You can find the .stl file in the "Laserable and 3D printable" folder. We have printed it in PLA, 0.2mm thickness and 20% infill without supports.

It is true that the result of the 3D printer is not perfectly symmetrical and in the case that we are managing more g forces, it will be dangerous to install this rotor in a centrifuge, but with which we are managing, you won´t have problems. A more symmetrical rotor made from acrilic and laser cutting machine, could be found in the Hackteria wiki.   

## Installing the code
### Step 1: Installing the libraries
Download the u8glib library and install it following the normal Arduino process to install libraries o copy it to "\Documents\Arduino\libraries" path.
### Step 2: Burn the code in your Arduino
And enjoy it!

## Support
Any question could be made sending an email to info@openlabmadrid.es, feel free to ask anything.
