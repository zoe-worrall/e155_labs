/***
	The top-level module for lab 1 of MicroP's.
	
	Consists of two inputs: a reset and a set of switches. Is combinational logic.
	The output is to control on board LED's and a bread-boarded 7-segment led matrix.
	
	Calls on two separate modules:
	* HSOSC: generates a clock running at 48MHz
	* lab01: runs and generates the led and segment outputs desired in lab 01

	author: zoe worrall
	version: 9/1/2024
	contact: zworrall@g.hmc.edu
*/

module top(
     input   logic 		 reset,
	 input	 logic [3:0] s,
     output  logic [2:0] led,
	 output  logic [6:0] seg
);

		// create a way to iterate through the clock and track the number of counts until 2.4 Hz is reached
	   logic int_osc;
	   
	   // copied from the initial iCE40_Demo file
	   // Internal high-speed oscillator
	   HSOSC #(.CLKHF_DIV(2'b00))  // by setting bits to 00, it tells the clock to run at 48MHz
			 hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	  
	  lab01 lab_runner(int_osc, reset, s, led, seg);
endmodule

// seven_segs this_seg
  // how to make testbench
	// seven_seg_tb.sv
	// don't compile it -- exclude from implementation