/***
	The seven_segment module used in lab 1 of MicroP's.
	
	Uses combinational logic to control a 7-segment LED matrix
	
	@input: s   :: a vector of what switches are turned on on the board
	@output seg :: a vector of seven bits that light up according to the order of on-board DIP switches
					Ranges from 0-9, A-F 
					(mapping wise: maps 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, A, b, C, d, E, F to corresponding switch orders)
	
	author: zoe worrall
	version: 9/1/2024
	contact: zworrall@g.hmc.edu
*/

// For future improvement: typedef enum logic[6:0] { num1=7'b111_1001, num2=7'b010_0100, num3=7'b011_0000, num4=7'b001_1001, num5=7'b001_0010, num6=7'b000_0010, num7=7'b111_1000, num8=7'b000_0000, num9=7'b001_0000, num0=7'b100_0000, numA=7'b000_1000, numB=7'b000_0011, numC=7'b100_0110, numD=7'b010_0001, numE=7'b000_0110, numF=7'b000_1110} num_type;

module seven_segment(
	 input   logic [3:0] s,
	 output  logic [6:0] seg);
	
	// num_type nums;
	// makes a logic table that is called whenever a rule is made, which sets all relevant segments
		// although it is possible to simplify the boolean equations for the segments, it was simpler
		// for me to make it using a case statement
	always_comb begin
		// seg = num_type'(s);
		
		case (s)
			0: 		 seg = 7'b100_0000;		 
			1:		 seg = 7'b111_1001;
			2: 		 seg = 7'b010_0100; 
			3: 		 seg = 7'b011_0000;
			
			4: 		 seg = 7'b001_1001;
			5: 		 seg = 7'b001_0010;
			6: 		 seg = 7'b000_0010;
			7: 		 seg = 7'b111_1000;
			
			8: 		 seg = 7'b000_0000;
			9: 		 seg = 7'b001_0000;
			10:		 seg = 7'b000_1000;
			11:		 seg = 7'b000_0011;
			
			12:		 seg = 7'b100_0110;
			13:		 seg = 7'b010_0001;
			14:		 seg = 7'b000_0110;
			default: seg = 7'b000_1110;
		endcase
		
	end
	
endmodule