# Nuclear Lego - a Fun and Safe Nuclear Physics Tool

This is intended to be a modular system (base + cubes) which would make it  
easy to perform simple nuclear physics experiments without any radiation  
involved. The project is currently developed as a qualitative tool for  
demonstrations. However there is a large potential for customization, as this  
device is basically a particle transport solver with a physical real-time  
interface (cubes).  

Cubes (4 cm side) can be placed on a rectangular base. Every cube can represent  
either a radioactive source, material, or radiation detector. The base can  
contain 3 layers of 4 by 4 cubes maximum. Every cube can be programmed through  
I2C to change what it represents. Up to 256 types of isotopes and up to 256  
types of materials are supported, extending this is also possible. Radiation  
detectors can be programmed to detect any combination of alpha, beta, gamma  
and neutron radiation.

# Setup instructions
## Raspberry Pi 5:
1. Install g++ and cmake:
``` bash
sudo apt install g++ cmake
```
OpenMP should be included with g++ installation

2. Clone the repository and compile executable:
``` bash
cd ~
mkdir GitHub && cd GitHub
git clone https://github.com/Tetris2102/Nuclear_Lego
cd Nuclear_Lego
cd Particle_Transport
mkdir build && cd build
cmake ..
make
```

3. Make the executable launch at startup
``` bash
mkdir -p ~/.config/autostart
nano ~/.config/autostart/nuclear_lego.desktop
```
Then paste the following into editor:
```
[Desktop Entry]
Encoding=UTF-8
Type=Application
Comment=Start nuclear lego executable in terminal
Exec=lxterminal -e ~/GitHub/Nuclear_Lego/Particle_Transport/build/nuclear_lego
Hidden=false
Name=Nuclear Lego
StartupNotify=true
Terminal=true
```

5. Set poweroff button and make it act on the first press
To set poweroff button at GPIO pin 17:
``` bash
sudo nano /boot/firmware/config.txt
```

In editor, add the following line:
```
dtoverlay=gpio-shutdown,gpio_pin=17,gpio_pull=up,debounce=1000
```

To make the poweroff button act on first press:
``` bash
sudo nano /boot/firmware/cmdline.txt
```

In editor, add the following line:
```
command_poweroff=poweroff
```

Then REBOOT with
``` bash
sudo reboot
```

Now you can connect a poweroff button between GPIO pin 17 and ground.

7. (Optional) Enable SSH and/or Raspberry Pi Connect
To enable SSH through terminal:
``` bash
sudo raspi-config
```
Go to Interfacing Options -> SSH -> Yes

## Hardware:
1. Assemble cubes (3d printing, soldering and flashing software on ATTiny85).
It is convenient to use Arduino UNO via Arduino As ISP to program ATTiny 85

2. Assemble the base.
3D print base, then put in poweroff button and power status LED. Mount Raspberry Pi in place with 2 M3x6mm flathead bolts (M3 bolts used everywhere). Solder the 70x90 mm perf board with 2 I2C multiplexers and a level shifter and screw it in with a 3d-printed frame and four M3x6mm flathead bolts. Put header pins in corresponding openings in base lid and solder 30 cm wires with female ports at the other end. Wire everything accordingly. Screw on the lid with eight M3x10mm flathead bolts.

3. You should be good to go unless any major mistakes were made in the process!

# Usage
1. Now if you have at least two cubes, you can program one to be source and the other to be a detector with an Arduino Uno or Nano via I2C (see cube_attiny85.ino and test_write.ino for programming via I2C).
2. If you put these cubes close to each other on the base, or on top of each other, the detector should produce increased counts with its buzzer.
3. With 3 cubes or more and by adding more reactions/scatters/absorbs to data.cpp you should be able to experiment with particle absorbtion, reflection, production, fission, criticality, and more! Don't forget to recompile after any data was modified for changes to take effect.

# Credits:
1. My parents and my wonderful Special Projects teacher  
2. https://grabcad.com/library/double-sided-fr-4-perfboards-w-m2-corners-1  
3. https://grabcad.com/library/raspberry-pi-5-model-1  
4. https://pip-assets.raspberrypi.com/categories/892-raspberry-pi-5/documents/RP-008347-DS-1-raspberry-pi-5-mechanical-drawing.pdf?disposition=inline  
