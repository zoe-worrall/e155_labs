/***
	The second-level module for lab 2 of MicroP's.
	
	Consists of four inputs: a reset, a clock, and two sets of switches.
	The output is to control on-board LED's and a bread-boarded dual 7-segment led matrix depending
		on the output of the two DIP switch boards.
	
	Calls on three separate modules:
	* seven_segment: A module that controls a 7-led array according to the input of four on-board switches
						- values range from 0-9 and A-F, representing a total of 16 bits on the array
	
	Note on multiplexing; a human eye normally perceives up to 60-90 Hz.
		https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.howtogeek.com/888948/how-many-fps-can-the-human-eye-see/%23:~:text%3DIn%2520a%2520study%2520conducted%2520to,things%2520at%2520up%2520to%2520500Hz.&ved=2ahUKEwiSpuWWqquIAxXjLkQIHdrPAfcQFnoECBYQAw&usg=AOvVaw3ZOBfnKBmDzKNPTVcBMtmf
		We want to multiplex between the two segments fast enough that it 
		seems imperceivable. Therefore, we'll aim for about 480 kHz oscillation between the two.
			With the clock operating at 48 MHz, mathematically this equates to switching between LED's
			every 100_000 counts (about 480 kHz).
	
	* led_sum: A that controls five on-board LED's to display the sum of the two hex values in bits
			
	* 
	
	author: zoe worrall
	version: 9/3/2024
	contact: zworrall@g.hmc.edu
*/

module lab01(
     input   logic clk, reset,
	 input	 logic [3:0] s1, s2,
     output  logic [3:0] led,
	 output  logic [6:0] det_switch_1, det_switch_2,
	 output logic 		seg1_pow, seg2_pow	// if we are powering first (0) or second (1) segment
);

	   logic [16:0] counter;  // generates roughly 732 Hz
	   logic oscillate;
	   
	    // keeps track of current "lit" segment via informing segments using switch
	   logic [6:0] seg1, seg2;
	   
			   
	  multiplex_dual run_dual	   
	  
	  // controls the seven segment array
	  
	  // A:P20 || B:P12 || C:P13 || D:P45 || E:P9 || F:P44 || G:P3
	  seven_segment seg_one( s1, seg1);
	  
	  seven_segment seg_two( s2, seg2);
  
	  // controls the three on-board LED's (for me, LED's D6, D7, D8, on pins P42, P38, and P28)
	  led_summation leds(s1, s2, led);
		
	   // Counter: used by led_control to blink led[2] at  480 kHz
	   always_ff @(posedge clk) begin
		 if(reset == 0)  counter <= 0;
		 else 
			 counter <= counter + 1;
	   end
endmodule

// seven_segs this_seg
  // how to make testbench
	// seven_seg_tb.sv
	// don't compile it -- exclude from implementation