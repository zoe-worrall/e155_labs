/***
	Generates and multiplexes a seven-segment display using one seven segment display module
	
	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
*/

module dual_seven_seg(
     input   logic state,  // statetype variable letting us know whether display 0 or display 1 is on
	 input	logic  [3:0] s1, s2,
	 output  logic [6:0] seg,  // GPIO power for seg1 and seg 2
	 output logic 		anode_display1, anode_display2	// if we are powering first (0) or second (1) segment
);	  
	  logic [3:0] s;

	  // the generated seven segmenet display. Displays according to the current switch.
	  seven_segment segment(s, seg);
	  
	  // controls which switch will be used by the seven segment display
	  always_comb begin
		  if (state == S1) begin // display1 on: this one depends on s1
				s = s1;
				anode_display1 = 0; // allow current flow
				anode_display2 = 1; // common anode of 2 is off
		  end else if (state == S0) begin // depends on input of s2
				s = s2;
				anode_display1 = 1;
				anode_display2 = 0;  
		  end else begin // This means that the finite state machine is broken
				s = 0;
				anode_display1 = 1;
				anode_display2 = 1;
		end
	  end 
  
endmodule