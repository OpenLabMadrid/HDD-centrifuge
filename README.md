# HDD centrifuge

### The design of this machine have been made by OpenLab Madrid and *getting the initial idea* in the *hackteria wiki* (https://hackteria.org/wiki/Hacked_Hard-disc_Centrifuge), adding some changes to adapt the machine to our needs.

## Making the solid parts (case and rotor)
### Step 1: Case
The case files are in the "Laserable and 3D printable" folder. You have a SolidWorks assembly of all the files you will need and the .dxf files to cut in a laser cutter machine. If you don´t have the SolidWorks software don´t worry, we have attached a .jpg capture of all the case mounted in the "Images of the mounted device" folder. 

The material must be 3mm thickness and we have used MDF for the HDD enclosing and transparent acrylic for the cover. 

### Step 2: Rotor
You can find the .stl file to 3D print it in the "Laserable and 3D printable" folder. We have printed it in PLA, 0.2mm thickness and 20% infill without supports.

It is true that is not perfectly simmetric and in the case that we are managing more g forces, it will be dangerous to intall this rotor in a centrifuge, but with the g-force that we are managing, you won´t have problems. A more simmetric rotor made from acrilic and laser cutting machine, could be found in the Hackteria wiki. 

## Installing the code
### Step 1: Installing the libraries
Download the u8glib library and install it following the normal Arduino process to install libraries o copy it to "\Documents\Arduino\libraries" path.