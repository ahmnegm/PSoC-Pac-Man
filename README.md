# PSoC-Pac-Man
Final project for MIT's Microcomputer Project Laboratory (6.115) course, a recreation of level one of the Pac-Man arcade game. The project interfaces two PSoCs (UART communication, VGA code), a two-axis joystick controller, a LCD screen (to display the game score), line driver chip, and VGA PS2 breakout board. Further information, including the project video and writeup, can be found at [personal website](http://www.mit.edu/~ahmnegm/projects/).

## psoc_vga_project final/vga.cydsn File
This file is for the main PSoC device, which runs the VGA and game logic code, ADCs for the analog controller, and UART communication of the game score with the second PSoC device.

## psoc_second_device_project final File
This file is for the second PSoC device, which communicates via UART with the main PSoC device and is solely used for the purpose of displaying the game score on the LCD screen.
