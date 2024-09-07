/***
  A Testbench meant to test all possible cases of switch pins used within the lab.
	Based off of the fulladder testbench inside of the MicroP's page.

  Calls on a text file "lab02_testvectors.tv", however due to odd pathing the full
	address of the file to computer needed to be written. For future users, it
	will be necessary to replace this current pathing with their own to ensure
	the right connection and no errors being thrown during Simulation.

  @author: zoe worrall
	   zworrall@g.hmc.edu
  @version: 09/05/2024
  
*/
`timescale 1ns/1ns
`default_nettype none
`define N_TV 512

module dual_seven_seg_tb();

 // Set up test signals
 logic clk, reset; 					// clk is necessary in all test benches
 logic 		 state;
 logic [3:0] switch1, switch2;		// the switches. On the board, each bit represents a switch
 logic [6:0] segment, seg_expected, seg2_expected; // the segments of the matrix. 
 logic anode1, anode1_expected, anode2, anode2_expected;				// will not be checked because this is clock dependent (send help plz)
 logic [511:0] vectornum, errors;     // the way that we iterate through the table of test vectors
 logic [19:0] testvectors [10000:0]; // Vectors of format s1[3:0]_s2[3:0]_display1_seg[6:0]_anode1_anode2

/***
			logic state,  // statetype variable letting us know whether display 0 or display 1 is on
	 input	logic  [3:0] s1, s2,
	 output  logic [6:0] seg,  // GPIO power for seg1 and seg 2
	 output logic 		anode_display1, anode_display2 */

 // Instatiation of lab 1's top module
 dual_seven_seg dut (
	.state (state), 
	.s1 (switch1),
	.s2 (switch2),
	.seg (segment),
	.anode_display1 (anode1),
	.anode_display2 (anode2)
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
     $readmemb("C:/Users/zoeworrall/Desktop/projects_LRS/lab02/source/impl_1/dualseg_lab02_testvectors.tv", testvectors, 0, `N_TV - 1);
     vectornum = 0; errors = 0;
     reset = 1; #27; reset = 0;
   end

  // Apply test vector on the rising edge of clk
 always @(posedge clk)
   begin
       #1; {switch1, switch2, state, seg_expected, anode1_expected, anode2_expected} = testvectors[vectornum];
   end
  initial

 begin
   // Create dumpfile for signals; this will help with debugging in later labs
   $dumpfile("dual_seven_seg_tb.vcd");
   $dumpvars(0, dual_seven_seg_tb);
 end

  // Check results on the falling edge of clk
 always @(negedge clk)
   begin
     if (~reset) // skip during reset
       begin
         if (segment != seg_expected || anode1 != anode1_expected || anode2 != anode2_expected)
           begin
             $display("Error: inputs: switch1=%b, switch2=%b", switch1, switch2);
             $display(" outputs: seg=%b (%b expected), anode1=%b (%b expected), anode2=%b (%b expected)", segment, seg_expected, anode1, anode1_expected, anode2, anode2_expected);
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