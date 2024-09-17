/***
  Acts as a flip flop for the given system

  @author: zoe worrall
	   zworrall@g.hmc.edu
  @version: 09/06/2024
*/

module flop_me #(parameter TIMES_FLOP=8, parameter VALUE_SIZE=4)
	(input  logic 		clk, reset,
	 input  in_loop,				// boolean value for whether you're in a loop or not
	 input  logic [VALUE_SIZE-1:0]				  value,
	 output logic [TIMES_FLOP-1:0][VALUE_SIZE-1:0] stable_value);
 
	integer i;
	
	always_ff @(posedge clk) begin
		if (reset) begin
			for (i=0; i<TIMES_FLOP; i++) begin
				stable_value[i] <= '0;
			end
		end else if (in_loop) begin 
			for (i=0; i<TIMES_FLOP-1; i++) begin // go through TIMES time, pushing into the next box
				stable_value[i+1] <= stable_value[i];
			end
			stable_value[0] <= value;  // prevents infinite loop
		end
	end
 
endmodule