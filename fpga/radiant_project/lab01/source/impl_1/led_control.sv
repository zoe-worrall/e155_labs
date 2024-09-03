/***
	The led_control module for lab 1 of MicroP's.
	Controls three on-board LEDs, according to the logic defined on the lab page:
						- led0: an xor gate between switch 0 and switch 1
						- led1: a and gate between switch 2 and 3
						- led2: blinking at ~2.4 Hz
	
	author: zoe worrall
	version: 9/1/2024
	contact: zworrall@g.hmc.edu
*/

module led_control(
	 input   logic [24:0] counter,
	 input	 logic [3:0]  s,
     output  logic [2:0]  led);
	 
   // Assign LED outputs
   assign led[0] = (s[0]==s[1]) ? 0 : 1;  // a xor gate, for better lack of terms
   assign led[1] = s[2] && s[3];  		  // an 'and' gate -- only turned on when both are 1
   assign led[2] = counter[24];			  // also taken from initial iCE code, but it's
											//  taking the final bit in counter. In our case,
											//  we want it to blink at 2.4 Hz (math on website).
endmodule