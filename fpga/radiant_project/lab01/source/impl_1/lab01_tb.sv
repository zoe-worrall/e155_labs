/***
  A Testbench meant to test all possible cases of switch pins used within the lab.
	Based off of the fulladder testbench inside of the MicroP's page.

  Calls on a text file "lab01_testvectors.tv", however due to odd pathing the full
	address of the file to computer needed to be written. For future users, it
	will be necessary to replace this current pathing with their own to ensure
	the right connection and no errors being thrown during Simulation.

  @author: zoe worrall
	   zworrall@g.hmc.edu
  @version: 09/02/2024
  
*/
`timescale 1ns/1ns
`default_nettype none
`define N_TV 16

module lab01_tb();

 // Set up test signals
 logic clk, reset;  // clk is necessary in all test benches
 logic [3:0] switch;		// the switches. On the board, each bit represents a switch
 logic [2:0] led, led_expected; // the leds; on the board, D42, D38, and D28
 logic [6:0] seg, seg_expected; // the segments of the matrix. 
 logic [31:0] vectornum, errors;     // the way that we iterate through the table of test vectors
 logic [15:0] testvectors [10000:0]; // Vectors of format switch[3:0]__led[2:0]_seg[6:0]

 // Instatiation of lab 1's top module
 top dut (
	.reset (reset), 
	.s (switch), 
	.led (led),
	.seg (seg)
   );

 // Generate clock signal with a period of 10 timesteps (5 up, 5 down)
 always
   begin
     clk = 1; #5;
     clk = 0; #5;
   end
  
 // At the start of the simulation:
 //  - Load the testvectors
 //  - Pulse the reset line
 initial
   begin
     $readmemb("C:/Users/zoeworrall/Desktop/projects_LRS/lab01/source/impl_1/lab01_testvectors.tv", testvectors, 0, `N_TV - 1);
     vectornum = 0; errors = 0;
     reset = 1; #27; reset = 0;
   end

  // Apply test vector on the rising edge of clk
 always @(posedge clk)
   begin
       #1; {switch, led_expected, seg_expected} = testvectors[vectornum];
   end
  initial

 begin
   // Create dumpfile for signals; this will help with debugging in later labs
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
             $display("Error: inputs: switch=%b", switch);
             $display(" outputs: led=%b (%b expected), seg=%b (%b expected)", led, led_expected, seg, seg_expected);
             errors = errors + 1;
           end

       // iterate through each vector in the testbench
       vectornum = vectornum + 1;
      
       if (testvectors[vectornum] === 14'bx)
         begin
           $display("%d tests completed with %d errors.", vectornum, errors);
	   // $finish; commented out to see the waveforms
         end
     end
   end
endmodule