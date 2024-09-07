/***
	The top-level module for lab 1 of MicroP's.
	
	Consists of two inputs: a reset and a set of switches. Is combinational logic.
	The output is to control on board LED's and a bread-boarded 7-segment led matrix.
	
	Calls on three separate modules:
	* HSOSC: generates a clock running at 48MHz
	* seven_segment: A module that controls a 7-led array according to the input of four on-board switches
						- values range from 0-9 and A-F, representing a total of 16 bits on the array
	
	* led_control: A that controls three on-board LEDs, according to the logic defined on the lab page:
						- led0: an xor gate between switch 0 and switch 1
						- led1: a and gate between switch 2 and 3
						- led2: blinking at ~2.4 Hz
	
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
	   logic [24:0] counter;
	  
	   // copied from the initial iCE40_Demo file
	   // Internal high-speed oscillator
	   HSOSC #(.CLKHF_DIV(2'b00))  // by setting bits to 00, it tells the clock to run at 48MHz
			 hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	  
	  // controls the seven segment array
	  // A:P20 || B:P12 || C:P13 || D:P45 || E:P9 || F:P44 || G:P3
	  seven_segment one_seg( s, seg);
  
	  // controls the three on-board LED's (for me, LED's D6, D7, D8, on pins P42, P38, and P28)
	  led_control one_cont(counter, s, led);
		
	   // Counter: used by led_control to blink led[2] at 2.4 Hz
	   always_ff @(posedge int_osc) begin
		 if(reset == 0)  counter <= 0;
		 else            counter <= counter + 1;
	   end
   
endmodule

// seven_segs this_seg
  // how to make testbench
	// seven_seg_tb.sv
	// don't compile it -- exclude from implementation