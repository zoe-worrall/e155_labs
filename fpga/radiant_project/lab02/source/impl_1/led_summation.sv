/***
	A lab that sums the output of two switches, and reports this sum to the user via diodes.
	
	author: zoe worrall
	version: 9/1/2024
	contact: zworrall@g.hmc.edu
*/

// For future improvement: typedef enum logic[6:0] { num1=7'b111_1001, num2=7'b010_0100, num3=7'b011_0000, num4=7'b001_1001, num5=7'b001_0010, num6=7'b000_0010, num7=7'b111_1000, num8=7'b000_0000, num9=7'b001_0000, num0=7'b100_0000, numA=7'b000_1000, numB=7'b000_0011, numC=7'b100_0110, numD=7'b010_0001, numE=7'b000_0110, numF=7'b000_1110} num_type;

module led_summation(
	 input   logic [3:0] s1, s2,
	 output  logic [4:0] led);
	
	assign led = s1 + s2;
	
endmodule