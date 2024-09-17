/***
	Generates the combinational logic necessary for a finite state machine
	
	author: zoe worrall
	version: 09/06/2024
	contact: zworrall@g.hmc.edu
*/

typedef enum logic [1:0] { WAIT=2'b00, LOG=2'b01, CYCLE=2'b10, TILL_RX_STOP=2'b11 } statetype;

module lab03_fsm(
	 input   logic 	 clk, reset,			// reset
	 input   logic [3:0]    col,  		// the input of row and column
	 input 	 logic [14:0] counter, 		// in order to keep track of how many ms have passed
	 
	 output logic [3:0]  row,
	 output logic [3:0] past_letter,   	// past letter saved
	 output logic [3:0] curr_letter,		// the current or most recently pressed letter
	 output logic [1:0] state,
	 output logic [20:0] waiter
);
		logic [1:0] row_index, next_row_index, col_index, next_col_index;  // used to store and keep track of current row
		
		logic [3:0] next_row;
		
		logic [1:0] next_state;//state,  next_state;		// statetype variable - WAIT, LOG, CYCLE, RX
		
		logic [3:0] next_past_letter, next_curr_letter;  // keeps track of next_letter, which is updated only when in LOG
		logic [3:0] next_letter;  // keeps track of next_letter, which is updated only when in LOG
		
		logic 			 in_loop;		// used to help with metastability
		logic [7:0][3:0] stab_mat;		// store the stabilized col
		
		logic [3:0] stable_col;
		logic [20:0] next_waiter;
		
		letter_receiver getLetter(row_index, col_index, next_letter);  // just some logic to get the next letter
		
		flop_me stabilize_col(clk, reset, in_loop, col, stab_mat);

	  always_ff@(posedge clk) begin
		  
		  if(reset)  begin  	// reset all the values if we reset
			state <= 0;
			
			row <= 0;
			row_index <= 0;
			col_index <= 0;
			
			curr_letter <= 0;
			past_letter <= 0;
			waiter <= 0;
			end else begin
				row_index <= next_row_index;
				col_index <= next_col_index;
				
				past_letter <= next_past_letter;
				curr_letter <= next_curr_letter;
				
				waiter <= next_waiter;
				state <= next_state;
				row <= next_row;
			end
		end

	  always_comb begin
		   case (state)
		   // WAITING STATE
		   WAIT:  
			  begin
				next_curr_letter = curr_letter;
				next_past_letter = past_letter;
				
				if (col != 0 && row !=0) begin
					in_loop = 1;
					next_waiter = 8;
					next_state = LOG;
					next_row   = row;
					next_col_index = col_index;
					next_row_index = row_index;
					
					
				end else begin
					case (row)
					   0: next_row = 1;
					   1: next_row = 2;
					   2: next_row = 4;
					   4: next_row = 8;
					   8: next_row = 1;
					   default: next_row = 0;
					endcase
					
					next_state = WAIT;
					next_waiter = waiter;
					
					next_col_index = col_index;
					next_row_index = row_index;
					in_loop = 0;
					end
				 // else if (counter==stab_time && (stable_col[3] == 4'b0001 || stable_col[3] == 4'b0010 || stable_col[3] == 4'b0100 || stable_col[3] == 4'b1000) && row != 0) begin
				end
			   
		   // BUTTON PUSHED: save the data, set timer
		   LOG:
		     begin 
			     if (waiter != 0) begin // for metastability, we are waiting for a series of four flip flops to save the data
					next_state = LOG;
					next_row = row;
					
					next_waiter = waiter - 1;
					next_past_letter = past_letter;
					next_curr_letter = curr_letter;
					
					next_col_index = col_index;
					next_row_index = row_index;
					
					in_loop = 1;
				end else begin  //  we've now received a signal -- check to see if its a single button, otherwise leave
					if (stab_mat[0] == 4'b0001 || stab_mat[0] == 4'b0010 || stab_mat[0] == 4'b0100 || stab_mat[0] == 4'b1000) begin
						if 		(stab_mat[3][0]) next_col_index = 0;
						else if (stab_mat[3][1]) next_col_index = 1;
						else if (stab_mat[3][2]) next_col_index = 2;
						else 			         next_col_index = 3;  // only true if col[3] == 1 (can only get to this state if that's the case)
							
						if 	    (row[0]) next_row_index = 0;
						else if (row[1]) next_row_index = 1;
						else if (row[2]) next_row_index = 2;
						else 			next_row_index = 3; // only true if row[3] == 1 (can only get to this state if that's the case)
						 
						next_state = CYCLE;
						next_waiter = 2**19;
						next_past_letter = past_letter;
						next_curr_letter = curr_letter;
						
						in_loop = 0;
						next_row = row;
					end else begin  // the button that was pushed was multiple columns, go back
						 
						next_state = WAIT;
						
						in_loop = 0;
						next_row = 0;
						
						next_row_index = row_index;
						next_col_index = col_index;
						
						next_waiter = 0;
						
						next_past_letter = past_letter;
						next_curr_letter = curr_letter;
					end
				end
				
			 end
			 
		   // WAIT FOR A MS
		   CYCLE:
			 begin
				next_row = row;
				
				
				if (waiter == 0) begin next_state = TILL_RX_STOP;      next_past_letter = curr_letter; next_curr_letter = next_letter; end
				else			 begin next_state = CYCLE; 		      next_past_letter = past_letter; next_curr_letter = curr_letter; end	 
				
				next_row_index = row_index;
				next_col_index = col_index;
				
				in_loop = 1;
				
				next_waiter = waiter - 1;
				
			end
			   
			// WAIT FOR THE BUTTON WE WERE LOOKING AT TO NO LONGER BE PRESSED
		   TILL_RX_STOP:
			 begin
				in_loop = 0;
				
				next_waiter = 0;
				next_past_letter = past_letter;
				next_curr_letter = curr_letter;
				
				next_row_index = row_index;
				next_col_index = col_index;
			   
			   if (col[col_index]) begin  next_state = TILL_RX_STOP; next_row = row;	end	// still pressed
			   else 				  begin  next_state = WAIT; 		   next_row=0;	end		// go back to the waiting state
			end
				   
			default: 
			   begin   // something has gone terribly wrong if this happens (there should only be four states)				
					next_row = 0; 
					next_state = WAIT;
					
					next_row_index = row_index;
					next_col_index = col_index;
					next_waiter = waiter;
					next_past_letter = past_letter;
					next_curr_letter = curr_letter;
					in_loop = 0;
				end
			endcase
		end
  
endmodule