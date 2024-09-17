/***
	The top-level module for lab 3 of MicroP's.
	
	Runs a clock that will control the lab03 module itself

	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
*/



module top(
	 input	logic [3:0]  col,
     output  logic [3:0] row,
	 output  logic [6:0] seg,
	 output logic 		anode1, anode2
);

		// create a way to iterate through the clock and track the number of counts until 2.4 Hz is reached
	   logic int_osc;
	   logic reset;
	   
	   assign reset = 0;
	   
	   // copied from the initial iCE40_Demo file
	   // Internal high-speed oscillator
	   HSOSC #(.CLKHF_DIV(2'b01))  // by setting bits to 01, it tells the clock to run at 24MHz
			 hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	  
	  lab03 lab_runner(int_osc, reset, col, row, seg, anode1, anode2);
	  
endmodule