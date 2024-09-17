/***
	A function that, with a received row and column, returns a letter state.
	
	author: zoe worrall
	version: 9/12/2024
	contact: zworrall@g.hmc.edu
*/

module letter_receiver(
	 input  logic [1:0] row_index, col_index,
	 output logic [3:0] letter
);
	
	always_comb begin
		case (row_index*4 + col_index)
			0: letter = 1;
			1: letter = 2;
			2: letter = 3;
			3: letter = 10;
			4: letter = 4;
			5: letter = 5;
			6: letter = 6;
			7: letter = 11;
			8: letter = 7;
			9: letter = 8;
			10: letter = 9;
			11: letter = 12;
			12: letter = 14;
			13: letter = 0;
			14: letter = 15;
			15: letter = 13;
			default: letter = 14;
			endcase
		
		end
	
endmodule