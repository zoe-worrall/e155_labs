## Lab 1


### Introduction

This is a lab that is meant to perform functions as define in [MicroP's Lab Module 1](https://hmc-e155.github.io/lab/lab1/).

The simulation for the test bench can be found within the project folder "lab01_vectorsim.mpf"

All code was written in Radiant Lattice and tested using Questa 2024.2.

Code was written on Parallels, Windows 11, mounted on a Mac with M1 chip. Files were uploaded to the given FPGA board using openFPGA in the Mac terminal. All additional files, if of interest, can be found in zoe-worrall's GitHub.

### File Structure

This lab features the following top-down structure:

lab01 structure:

	lab01
	|
	|__ lab01_tb.sv  (the test bench for top.sv)
	|
	|__ lab01_testvectors.tv  (the text vectors used in lab01_tb.sv)
	|
	|__ top.sv	  (compiles seven_segment and led_control. Runs a clock that is used for led[2])
		|
		|____ seven_segment.sv 		(controls a seven-segment array through bits 0x0 to 0xF)
		|
		|____ led_control.sv		(controls three diodes through xor, and, and clock)

  