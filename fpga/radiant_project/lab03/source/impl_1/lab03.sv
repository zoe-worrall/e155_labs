/***
	The second-level module for lab 3 of MicroP's.
	
	runs the finite state machine and the seven segment display

	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
*/

typedef enum logic { S0=1'b0, S1=1'b1 } disp_statetype;

module lab03(
     input   logic 	  clk, reset,
     input   logic [3:0]  col,
     output  logic [3:0]  row,
     output  logic [6:0] seg,
     output  logic 	  anode1, anode2	// if we are powering first (0) or second (1) segment
);

	   logic [14:0] counter;
	   logic [20:0] waiter;
	   logic [3:0] next_row;
	   
	   disp_statetype disp_curr, disp_next;   // variables that control the state of anode1 and anode2
	   logic [1:0] state, next_state;   
	   logic [3:0] past_letter, curr_letter;
	   
	   // Switches back and forth between the on and off state. Resets if the user wants to reset.
	   always_ff @(posedge clk) begin
		if(reset)  begin  	// reset all the values if we reset
			disp_curr <= S0;
			counter <= 0;
			end
		else begin
			disp_curr <= disp_next;
			counter <= counter + 1;
			end
	   end
	   
	   always_comb begin
		   if (counter[9])     disp_next = S0; // basically trade every 2^4
		   else 	       		  disp_next = S1;
		end
	   
	   dual_seven_seg dualRunner(disp_curr, past_letter, curr_letter, seg, anode1, anode2);
	
	   lab03_fsm states(clk, reset, col, counter, row, past_letter, curr_letter, state, waiter);
	
endmodule

// seven_segs this_seg
  // how to make testbench
	// seven_seg_tb.sv
	// don't compile it -- exclude from implementation