/***
	A testbench for lab01.sv.
	
	Based heavily off of the code used for the fulladder testbench found on HMC's E155's tutorial, 
	located at https://hmc-e155.github.io/tutorials/modelsim-simulation-tutorial/index.html#testbench-simulation
	
	author: zoe worrall
	version: 9/1/2024
	contact: zworrall@g.hmc.edu
*/

`timescale 1ns/1ns
`default_nettype none
`define N_TV 8

module seven_segment_tb();
 // Set up test signals
 logic clk, reset;
 logic [3:0] switch;
 logic [2:0] led;
 logic [6:0] seg;

 // Instantiate the device under test
 top dut(reset, .SWITCH(switch), .LED(led), .SEGMENT(seg));

 // Generate clock signal with a period of 10 timesteps.
 always
   begin
     clk = 1; #5;
     clk = 0; #5;
   end
  
 // At the start of the simulation:
 //  - Load the testvectors
 //  - Pulse the reset line (if applicable)
 initial
   begin
     $readmemb("fulladder_testvectors.tv", testvectors, 0, `N_TV - 1);
     vectornum = 0; errors = 0;
     reset = 1; #27; reset = 0;
   end
  // Apply test vector on the rising edge of clk
 always @(posedge clk)
   begin
       #1; {a, b, cin, s_expected, cout_expected} = testvectors[vectornum];
   end
  initial
 begin
   // Create dumpfile for signals
   $dumpfile("fulladder_tb.vcd");
   $dumpvars(0, fulladder_tb);
 end
  // Check results on the falling edge of clk
 always @(negedge clk)
   begin
     if (~reset) // skip during reset
       begin
         if (cout != cout_expected || s != s_expected)
           begin
             $display("Error: inputs: a=%b, b=%b, cin=%b", a, b, cin);
             $display(" outputs: s=%b (%b expected), cout=%b (%b expected)", s, s_expected, cout, cout_expected);
             errors = errors + 1;
           end

      
       vectornum = vectornum + 1;
      
       if (testvectors[vectornum] === 11'bx)
         begin
           $display("%d tests completed with %d errors.", vectornum, errors);
           $finish;
         end
     end
   end
endmodule