
''' Used to produce the lab02_testvectors testbench in Lab 2, Micro P's
# Number of bits to generate
switch1 = 2**4
switch2 = 2**4

s_vals = ["0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"]
sum_vals = ["00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111", "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111", "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111", "11000", "11001", "11010", "11011", "11100", "11101", "11110", "11111"]
segment = ["1000000", "1111001", "0100100", "0110000", "0011001", "0010010", "0000010", "1111000", "0000000", "0010000", "0001000", "0000011", "1000110", "0100001", "0000110", "0001110"] 
seg_val = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

for s1 in range(switch1):
    for s2 in range(switch2):
        sum = seg_val[s1] + seg_val[s2]
        string = s_vals[s1] + "_" + s_vals[s2] + "_" + segment[s1] + "_" + segment[s2] + "_" + sum_vals[sum]
        print(string)

'''

# Number of bits to generate
switch = 2**4
state0 = True

s_vals = ["0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"]
sum_vals = ["00000", "00001", "00010", "00011", "00100", "00101", "00110", "00111", "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111", "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111", "11000", "11001", "11010", "11011", "11100", "11101", "11110", "11111"]
segment = ["1000000", "1111001", "0100100", "0110000", "0011001", "0010010", "0000010", "1111000", "0000000", "0010000", "0001000", "0000011", "1000110", "0100001", "0000110", "0001110"] 
seg_val = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

## Desired format: switch1_switch2_<if display 1 is on or off>_<the correct segment variables>_anode1_anode2

# s = ""
# anodes = ""
# string = ""
# for i in range(2):
#     for s1 in range(switch):
#         for s2 in range(switch):
#             if state0: 
#                 s = "1_" + segment[s1]
#                 anodes = "0_1"
#             else:      
#                 s = "0_" + segment[s2]
#                 anodes = "1_0"


#             string = s_vals[s1] + "_" + s_vals[s2] + "_" + s + "_" + anodes
#             print(string)
#     state0 = not state0

# To check Lab3's Finite State Machine

'''  
    input   logic  [2:0] state,  		// statetype variable - WAIT, LOG, CYCLE, RX
	 input   logic [3:0] row, col;  	// the input of row and column
	 output logic  [3:0] next_row,   	// the next row we will go to (if need be)
	 output logic 		next_state,		// if we are powering first (0) or second (1) segment
	 output logic [3:0]  next_letter	// puts out the next letter logic; will only be used if we're in LOG state
     '''

# states = ["00", "01", "10", "11"]  # WAIT, LOG, CYCLE, WAIT_RX_STOP
# rows = ["0001", "0010", "0100", "1000"]
# # row can only be powered in five ways

# cols = ["0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"]
# # only  five are ideal in logging state, but we should check them all
#     #               ^               ^               ^               ^               ^               ^               ^               ^     the matrix will think col[0] is pressed
#     #                       ^                               ^                               ^                               ^             the matrix will think col[1] is pressed
#     #                                       ^                                                               ^                             the matrix will think col[2] is pressed
#     #                                                                       ^                                                             col[3] is considered pressed

# counter = ["00000_00000_00000", "00000_10000_00000"]

# letters = ["0001", "0010", "0011", "1010", "0100", "0101", "0110", "1011", "0111", "1000", "1001", "1100", "1110", "0000", "1111", "1101"]
# ## Potential inputs/outputs:
# '''
# - test for all state types
# - test for all variations of row and column
# '''

# # vector: state_counter_tilltime_row_col_nextrow_nextstate_nextletter

# for state in states:
#     for r_index in range(len(rows)):
#         for c_index in range(len(cols)):
#             if (state == "00"):
#                 if (cols[c_index] == "0000"):
#                     string = state+"_"+counter[0]+"_"+counter[1]+"_"+rows[r_index]+"_"+cols[c_index]
#                     string = string + "_" + rows[(r_index+1) % 4] + "_" + states[0] + "_0000"
#                     print(string)
#                 else:
#                     col = cols[c_index]
#                     c=-1
#                     if (col == "0001"):     c = 0
#                     elif (col == "0010"):   c = 1
#                     elif (col == "0100"):   c = 2
#                     elif (col == "1000"):   c = 3
#                     if (c != -1):
#                         string = state+"_"+counter[0]+"_"+counter[1]+"_"+rows[r_index]+"_"+cols[c_index]
#                         string = string + "_" + rows[r_index] + "_" + "01" + "_0000"
#                         print(string)
#                     else:
#                         string = state+"_"+counter[0]+"_"+counter[1]+"_"+rows[r_index]+"_"+cols[c_index]
#                         string = string + "_" + rows[(r_index+1) % 4] + "_" + "00" + "_0000"
#                         print(string)
#             if (state == "01"):     # I refuse to deal with reaching the waiting state with no column being triggered
#                 col = cols[c_index]
#                 c=-1
#                 if (col == "0001"):     c = 0
#                 elif (col == "0010"):   c = 1
#                 elif (col == "0100"):   c = 2
#                 elif (col == "1000"):   c = 3
#                 if (c != -1):
#                     string = state+"_"+counter[0]+"_"+counter[1]+"_"+rows[r_index]+"_"+cols[c_index]
#                     string = string + "_" + rows[r_index] + "_" + states[2] + "_" + letters[r_index*4 + c]
#                     print(string)
#             if (state == "10"): ## test both when counter is different from till_time and when counter is the same
#                 string = state+"_"+counter[0]+"_"+counter[1]+"_"+rows[r_index]+"_"+cols[c_index]
#                 string = string + "_" + rows[r_index] + "_" + states[2] + "_0000"
#                 print(string)
#                 string = state+"_"+counter[1]+"_"+counter[1]+"_"+rows[r_index]+"_"+cols[c_index]
#                 string = string + "_" + rows[r_index] + "_" + states[3] + "_0000"
#                 print(string)
#             if (state == "11"): ## test both when the column is on and when the column is off
#                 string = state+"_"+counter[1]+"_"+counter[1]+"_"+rows[r_index]+"_"+"0000"
#                 string = string + "_" + rows[r_index] + "_" + states[0] + "_0000"
#                 print(string)


## Generates the test vectors for the lab03 testbench
### ut   logic clk, reset, col,        -> output row, seg, anode1, anode2

states = ["00", "01", "10", "11"]  # WAIT, LOG, CYCLE, WAIT_RX_STOP
rows = ["0001", "0010", "0100", "1000"]

for i in range(4): # go through loop for each potential state
    # (don't need to check anodes)
    # starting in S=0
    # reset = 0, col = "0000" -> "0001", x
    # iterate through every row; confirm that row cycles to 2 (i.e. first time, second, third, fourth, fifth) before changing col
    # on row = "0100" (third time), have col = "0110" to confirm that it won't begin a loop

    # on  sixth - reset = 0, col = "0001" -> "0010", "0100" (i.e. "4" on the keypad)
    # on seventh -reset = 0, col = "0001" -> "0010", "0010"   (current state is LOG)

    # on eighth - reset = 0, col = "0001" -> "0010", "0010" (current state is CYCLE)
    # increase ninth so that clk is incremented proper number of times in the loop (still loop)
    # on ninth   - reset= 0, col = "0001" 0> "0010", "0010" (current state is TX)
    # on tenth -  reset = 0, col = "0000" -> "0010", "0010"