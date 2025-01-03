/***
	Generates and multiplexes a seven-segment display using one seven segment display module
	
	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
	
*/

//typedef enum logic { S0=1'b0, S1=1'b1 } disp_statetype;
module dual_seven_seg(
     input   logic display_state,  // statetype variable letting us know whether display 0 or display 1 is on
	 input   logic [3:0] past_letter, curr_letter,  // whatever letter we are going to be displaying
	 output  logic [6:0] seg,  // GPIO power for seg1 and seg 2
	 output logic 		anode_display1, anode_display2	// if we are powering first (0) or second (1) segment
);	  
	  logic [3:0] letter;

	  // seven segment display logic
	  seven_segment segment(letter, seg);
	  
	  // controls which switch will be used by the seven segment display
	  always_comb begin
		  if (display_state == S1) begin // display1 on: this one depends on s1
				letter = curr_letter;
				anode_display1 = 1; // allow current flow
				anode_display2 = 0; // common anode of 2 is off
		  end else if (display_state == S0) begin // depends on input of s2
				letter = past_letter;
				anode_display1 = 0;
				anode_display2 = 1;  
		  end else begin // This means that the finite state machine is broken
				letter = 0;
				anode_display1 = 1;
				anode_display2 = 1;
		end
	  end 
  
endmodule