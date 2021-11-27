# Name			: Ryan Cole
# Username		: rccole@mtu.edu
# Description	: This program first searches a linked list of employee records for a keyword among
#				  the employees' names. It prints out the keyword being searched for, which may
#				  contain any ASCII characters. If the keyword, which is case insensitive, appears
#				  anywhere in a particular employee's name, then they are a match for the search.
#				  For each employee who matches the keyword search, their name, employee ID number,
#				  and age are printed. Each matching employee's information is printed in the order
#				  in which they appear in in the linked list.

.text
# Print out that a search is in progress
			la		$a0, search_txt
			li		$v0, 4
			syscall
			
# Print the keyword being searched for
			la		$a0, keyword
			li		$v0, 4
			syscall

# Print a line feed
			li		$v0, 11
			addi		$a0, $0, 10
			syscall
			
# Store the ASCII values of A, Z, a, and z so we can check for case later on
			li		$s1, 65
			li		$s2, 90
			li		$s3, 97
			li		$s4, 122
			
# Load the memory addresses of the keyword and the first node in the linked list
			la		$t0, keyword
			la		$t1, first
			
hold_address:
# Store the address of the node so we don't lose track of it as we iterate through memory
# during the keyword search.
			li		$s0, 0
			addi		$s0, $t1, 0
			
match_loop:
# Load the next available ASCII character in the keyword and in this employee's name
			lb		$t2, 0($t0)
			lb		$t3, 9($t1)
			
# If we have iterated to the end of the keyword, print the employee's information
			beqz		$t2, print_loop
			
# If we have iterated to the end of this employees name, there is no keyword match
# We must move onto the next node in the linked list
			beqz		$t3, next_entry
			
# If the current character in the employee's name is not a letter, move on to check for a keyword match
			blt		$t3, $s1, check_match
			bgt		$t3, $s4, check_match
			
# If the current character in the employee's name is a letter, proceed according to its case
			ble		$t3, $s2, uppercase
			bge		$t3, $s3, lowercase
			b		check_match
			
uppercase:
# If uppercase, check for a character match with its lowercase equivalent
			li		$t4, 0
			addi		$t4, $t3, 32
			beq		$t2, $t4, match
			
lowercase:
# If lowercase, check for a character match with its uppercase equivalent
			li		$t4, 0
			addi		$t4, $t3, -32
			beq		$t2, $t4, match

check_match:
# Check the two characters to see if they match
			beq		$t2, $t3, match
			bne		$t2, $t3, no_match
			
match:
# If the two characters match, iterate to the next character in each respective string and loop
			addi		$t0, $t0, 1
			addi		$t1, $t1, 1
			b		match_loop
			
no_match:
# If the two characters don't match, go back to the first character in the keyword
# and iterate to the next character in the employee's name, then loop
			la		$t0, keyword
			addi		$t1, $t1, 1
			b		match_loop

print_loop:
# Return to the first character in the keyword for future searches
			la		$t0, keyword

# Print the matching employee's name
			la		$a0, 9($s0)
			li		$v0, 4
			syscall
			
# Print out a string to format the output correctly
			la		$a0, ID_txt
			li		$v0, 4
			syscall

# Print out the employee's ID number
			lw		$a0, 4($s0)
			li		$v0, 1
			syscall

# Print out a string to format the output correctly
			la		$a0, age_txt
			li		$v0, 4
			syscall
			
# Print out the employee's age
			lb		$a0, 8($s0)
			li		$v0, 1
			syscall
			
# Print a line feed
			li		$v0, 11
			addi		$a0, $0, 10
			syscall

next_entry:
# Iterate to the next entry in the linked list
# If the end of the list has not yet been reached, loop back to the keyword search
			lw		$t1, 0($s0)
			bne		$t1, -1, hold_address

.data
# Various strings that help format the program's output
search_txt:	.asciiz		"Searching for: "
ID_txt:		.asciiz		", #"
age_txt:		.asciiz		", age "

### START DATA ###!
.data
keyword:		.asciiz		"an"                    # String to search for, (this may be empty)
first:		.word     	node2                   # Next pointer
          	.word     	20030422                # Employee ID
          	.byte     	27                      # Age
          	.asciiz   	"Jim D. Halpert"        # Name (you can assume this will NOT be empty)
node2:     	.word     	node3
          	.word     	20030435
          	.byte     	26
          	.asciiz   	"Pam M. Beesly"
node3:    	.word     	node4
          	.word     	20010984
          	.byte     	41
          	.asciiz   	"Michael G. Scott"
node4:    	.word     	node5
          	.word     	20030580
          	.byte     	31
          	.asciiz   	"Dwight K. Schrute III"
node5:    	.word     	node6
          	.word     	20010321
          	.byte     	24
          	.asciiz   	"Ryan B. Howard"
node6:    	.word     	node7
          	.word     	20051229
          	.byte     	32
          	.asciiz   	"Andy B. Bernard Jr."
node7:    	.word     	-1
          	.word     	20084724
          	.byte     	45
          	.asciiz   	"Robert California"
### END DATA ###
