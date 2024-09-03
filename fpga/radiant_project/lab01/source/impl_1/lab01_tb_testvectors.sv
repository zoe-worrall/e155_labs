`timescale 1ns/1ns
`default_nettype none
`define N_TV 8

module lab01_tb();
 // Set up test signals
 logic clk, reset;
 logic [3:0] switch;
 logic [2:0] led, led_expected;
 logic [6:0] seg, seg_expected;
 logic [31:0] vectornum, errors;
 logic [13:0] testvectors[10000:0]; // Vectors of format switch[3:0]__led[2:0]_seg[6:0]

 // Instantiate the device under test
 top dut(.reset(reset), .s(switch), .led(led), .seg(seg));

 // Generate clock signal with a period of 10 timesteps.
 always
   begin
     clk = 1; #5;
     clk = 0; #5;
   end
  
  initial
	begin
		switch = 0; # 10;
		switch = 1; # 10;
		switch = 2; # 10;
		switch = 3; # 10;
		switch = 4; # 10;
		switch = 5; # 10;
		switch = 6; # 10;
		switch = 7; # 10;
		switch = 8; # 10;
		switch = 9; # 10;
		switch = 10; # 10;
		switch = 11; # 10;
		switch = 12; # 10;
		switch = 13; # 10;
		switch = 14; # 10;
		switch = 15; # 10;
	end
  
  // Apply test vector on the rising edge of clk
 always @(posedge clk)
   begin
	   led_expected[0] = (switch[0]==switch[1]) ? 0 : 1;  // a xor gate, for better lack of terms
	   led_expected[1] = (switch[2] && switch[3]);  		  // an 'and' gate -- only turned on when both are 1
       case (switch)
			0: 		 seg_expected = 7'b111_1001;
			1: 		 seg_expected = 7'b010_0100; 
			2: 		 seg_expected = 7'b011_0000;
			3: 		 seg_expected = 7'b001_1001;
			
			4: 		 seg_expected = 7'b001_0010;
			5: 		 seg_expected = 7'b000_0010;
			6: 		 seg_expected = 7'b111_1000;
			7: 		 seg_expected = 7'b000_0000;
			
			8: 		 seg_expected = 7'b001_0000;
			9:		 seg_expected = 7'b100_0000;
			10:		 seg_expected = 7'b000_1000;
			11:		 seg_expected = 7'b000_0011;
			
			12:		 seg_expected = 7'b100_0110;
			13:		 seg_expected = 7'b010_0001;
			14:		 seg_expected = 7'b000_0110;
			default: seg_expected = 7'b000_1110;
		endcase
   end
   
  initial
	 begin
	   // Create dumpfile for signals
	   $dumpfile("lab01_tb.vcd");
	   $dumpvars(0, lab01_tb);
	 end
	 
  // Check results on the falling edge of clk
 always @(negedge clk)
   begin
     if (~reset) // skip during reset
       begin
         if (led != led_expected || seg != seg_expected)
           begin
             $display("Error: inputs: s=%b", switch);
             $display(" outputs: led=%b (%b expected), seg=%b (%b expected)", led, led_expected, seg, seg_expected);
             errors = errors + 1;
           end

      
       // vectornum = vectornum + 1;
      
       // $display("%d tests completed with %d errors.", vectornum, errors);
     end
   end
endmodule