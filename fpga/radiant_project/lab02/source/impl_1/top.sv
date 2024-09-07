/***
	The top-level module for lab 2 of MicroP's.
	
	Consists of two inputs: a reset and a set of switches. Is combinational logic.
	The output is to control on board LED's and a bread-boarded 7-segment led matrix.
	
	Calls on two separate modules:
	* HSOSC: generates a clock running at 48MHz
	* lab02: generates multiplexed seven-segment led matrices and their summation on diodes
				dependent on five bread-boarded DIP switches.

	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
*/



module top(
     input   logic 		 reset,
	 input	logic [3:0]  s1, s2,
     output  logic [4:0] led,
	 output  logic [6:0] seg,
	 output logic 		anode1, anode2
);

		// create a way to iterate through the clock and track the number of counts until 2.4 Hz is reached
	   logic int_osc;
	   
	   // copied from the initial iCE40_Demo file
	   // Internal high-speed oscillator
	   HSOSC #(.CLKHF_DIV(2'b00))  // by setting bits to 00, it tells the clock to run at 48MHz
			 hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	  
	  lab02 lab_runner(int_osc, reset, s1, s2, led, seg, anode1, anode2);
	  
endmodule