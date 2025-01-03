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
`define N_TV 256

module lab02_tb();

 // Set up test signals
 logic clk, reset;  // clk is necessary in all test benches
 logic [3:0] switch1, switch2;		// the switches. On the board, each bit represents a switch
 logic [4:0] led, led_expected; // the leds; on the board, D42, D38, and D28
 logic [6:0] segment, seg1_expected, seg2_expected; // the segments of the matrix. 
 logic anode1, anode2;				// will not be checked because this is clock dependent (send help plz)
 logic [253:0] vectornum, errors;     // the way that we iterate through the table of test vectors
 logic [26:0] testvectors [10000:0]; // Vectors of format s1[3:0]_s2[3:0]_switch1[6:0]_switch2[6:0]_led[4:0]

/***
reset,
	 input	logic [3:0]  s1, s2,
     output  logic [4:0] led,
	 output  logic [6:0] seg1, seg2,
	 output logic 		seg1_pow, seg2_pow */

 // Instatiation of lab 1's top module
 top dut (
	.reset (reset), 
	.s1 (switch1),
	.s2 (switch2),
	.led (led),
	.seg (segment),
	.anode1 (anode1),
	.anode2 (anode2)
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
     $readmemb("C:/Users/zoeworrall/Desktop/projects_LRS/lab02/source/impl_1/lab02_testvectors.tv", testvectors, 0, `N_TV - 1);
     vectornum = 0; errors = 0;
     reset = 1; #27; reset = 0;
   end

  // Apply test vector on the rising edge of clk
 always @(posedge clk)
   begin
       #1; {switch1, switch2, seg1_expected, seg2_expected, led_expected} = testvectors[vectornum];
   end
  initial

 begin
   // Create dumpfile for signals; this will help with debugging in later labs
   $dumpfile("lab02_tb.vcd");
   $dumpvars(0, lab02_tb);
 end

  // Check results on the falling edge of clk
 always @(negedge clk)
   begin
     if (~reset) // skip during reset
       begin
         if (led != led_expected || (segment != seg1_expected && segment != seg2_expected))
           begin
             $display("Error: inputs: switch1=%b, switch2=%b", switch1, switch2);
             $display(" outputs: led=%b (%b expected), seg=%b (%b or %b expected)", led, led_expected, segment, seg1_expected, seg2_expected);
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