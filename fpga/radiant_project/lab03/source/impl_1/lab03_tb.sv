/***
  A Testbench meant to test all possible cases of switch pins used within the lab.
	Based off of the fulladder testbench inside of the MicroP's page.

  Calls on a text file "lab02_testvectors.tv", however due to odd pathing the full
	address of the file to computer needed to be written. For future users, it
	will be necessary to replace this current pathing with their own to ensure
	the right connection and no errors being thrown during Simulation.

  @author: zoe worrall
	   zworrall@g.hmc.edu
  @version: 09/06/2024
  
*/
`timescale 1ns/1ns
`default_nettype none
`define N_TV 272


module lab03_tb();
 // Set up test signals
 logic clk, reset;  // clk is necessary in all test benches
 logic [3:0]  row, col; 			 				// 
 logic [6:0] seg; 				// 
 logic anode1, anode2;				// 
 logic [271:0] vectornum, errors;     			// 
 logic [49:0]  testvectors [10000:0];  			// 

 logic [6:0] seg_arr [16] = { 7'b1111001, 7'b0100100, 7'b0110000, 7'b0001000, 7'b0011001, 7'b0010010, 7'b0000010, 7'b0000011, 7'b1111000, 7'b0000000, 7'b0010000, 7'b1000110, 7'b0000110, 7'b1000000, 7'b0001110, 7'b0100001};
   

 // Instatiation of lab 1's top module
 lab03 dut (
	.clk (clk), 
	.reset (reset),
	.col (col),
	.row (row),
	.seg (seg),
	.anode1 (anode1),
	.anode2 (anode2)
   );
   

 // Generate clock signal with a period of 10 timesteps (5 up, 5 down)
 always
   begin
     clk = 1; #5;
     clk = 0; #5;
   end
  
initial 
	begin
		
	end

 // At the start of the simulation:
 //  - Load the testvectors
 //  - Pulse the reset line
 initial
	begin
	 col = 0; reset = 1; 
	 #27;
	 assert(row == 4'b0000) else $error("E0 %b", row);
		 
	 col = 0; reset = 0;
	 assert (row == 4'b0000) else $error("E1 0001 expected, %b received", row);
	
	// [WAIT] check that the row cycle is working
	 for(int i=0; i<4; i++) begin
		 #10;
		 assert (row == 2**i) else $error("E%b Cycle", 2**i);
	 end
	 
	reset = 1; #10; reset = 0;
	// check all log states: we'll check each row and each column and row to make sure segment is right
	for (int r=0; r<4; r++) begin
	  	for (int c=0; c<4; c++) begin
		//  [WAIT] 	
		for (int i=0; i<r+1; i++) #10;  // go to the correct row
		if (r != 0) begin assert(row == 2**r) else $error("correct_root %b, expected=%b, actual=%b", r, 2**r, row); end
		else		  assert(row == 4'b0001) 	else $error("correct_root %b", r);
		
		// [LOG] set col when row = r -- we should enter a cycle now	
		col = 2**c; #10;
		assert(row == 2**r) else $error("E_loop1A%b, expected=%b, actual=%b", r, 2**r, row);
	
		//  [CYCLE] confirm that we are still at row == r (this is inside the cycle)
		#10;
		assert(row == 2**r) else $error("E_loop2A%b, expected=%b, actual=%b", r, 2**r, row); 

		// [CYCLE] current letter should be set now
		#10; 
		if (r==0 && c==0) assert(seg == 0)  else $error("E_Loop3%b%b, expected segment=%b vs received = %b", r, c, 0, seg);
		else assert(seg == seg_arr[r*4+c-1])  else $error("E_Loop3%b%b, expected segment=%b vs received = %b", r, c, seg_arr[r*4+c-1], seg);

		//  [CYCLE MORE] wait for a very long time (another 2^14)*10. should in tx state now. confirm row is expected.
		#40960;
		col = 0;
		#40960;
		col = 2**c;
		#40960;
		col = 0;
		#40960;
		col = 2**c;
		#204800;
		assert(row == 2**r) else $error("E_loop4A%b, expected=%b, actual=%b", r, 2**r, row); 

		// Should return to cycling through the rows
		col = 0; #10;
		assert(row == 0) else $error("E_loop5A%b, expected=%b, actual=%b", r, 2**r, row);
	end
     end

   #1000;
   vectornum = 0; errors = 0;
   reset = 1; #27; reset = 0;

	$stop;
   end
   
  initial
   begin
   // Create dumpfile for signals; this will help with debugging in later labs
   $dumpfile("lab03_tb.vcd");
   $dumpvars(0, lab03_tb);
 end

endmodule