/***
	The second-level module for lab 2 of MicroP's.
	
	Consists of four inputs: a reset, a clock, and two sets of switches.
	The output is to control on-board LED's and a bread-boarded dual 7-segment led matrix depending
		on the output of the two DIP switch boards.
	
	Calls on three separate modules:
	* seven_segment: A module that controls a 7-led array according to the input of four on-board switches
						- values range from 0-9 and A-F, representing a total of 16 bits on the array
	
	Note on multiplexing; a human eye normally perceives up to 60-90 Hz.
	On another note, the LEDs on the seven-segmenet matrix are set to oscillate at 46875 Hz, well over our current
		ability to see Hz.
	
	* led_summation: A module that controls five on-board LED's to display the sum of the two hex values in bits
	* dual_seven_seg: A module that multiplexes between two seven segment matrices
	
	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
*/

typedef enum logic { S0, S1 } statetype;

module lab02(
     input   logic clk, reset,
	 input	logic [3:0] s1, s2,
     output  logic [4:0] led,
	 output  logic [6:0] seg,
	 output logic 		anode1, anode2	// if we are powering first (0) or second (1) segment
);

	   logic [10:0] counter;  // generates roughly 1.5 MHz
	   logic oscillate; d
	   statetype state, nextstate;
	   
	   // Switches back and forth between the on and off state. Resets if the user wants to reset.
	   always_ff @(posedge clk) begin
		if(reset == 0)  begin counter <= 0; state <= S0; end
		else 		    begin counter <= counter + 1; state <= nextstate; end
	   end

	   always_comb begin
		   if (counter[9]) nextstate = S0; // check second to last state -- basically trade every 2^9
		   else nextstate = S1;
		end

			   // state, s1, s2, seg, anode_display1, anode_display2
	  // controls the dual segment array
	  dual_seven_seg segments(state, s1, s2, seg, anode1, anode2);
  
	  // controls the three on-board LED's (for me, LED's D6, D7, D8, on pins P42, P38, and P28)
	  led_summation leds(s1, s2, led);
		
endmodule

// seven_segs this_seg
  // how to make testbench
	// seven_seg_tb.sv
	// don't compile it -- exclude from implementation