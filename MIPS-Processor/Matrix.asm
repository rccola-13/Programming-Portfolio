# Name			: Ryan Cole
# Username		: rccole@mtu.edu
# Description	: This program takes input for the size and default contents of a square matrix
#				  of integers. Size ranges between 1 and 20, while the elements' values may range
#				  from 0 to 9. Afterwards, it reads in input for coordinates within the matrix.
#				  Any specified locations in the matrix then have their integer value increased
#				  by one. Finally, once an invalid coordinate is inputted, the entire matrix is
#				  printed out, row by row.

.text		
# Read in the size of the matrix, square the value to find the total number of
# elements, then multiply the value by 4 to find the total memory space required. 		
		li		$v0, 5
		syscall
		move		$t0, $v0
		mult		$v0, $v0
		mflo		$t1
		add		$t1, $t1, $t1
		add		$t1, $t1, $t1
		
# Terminate if size is less than 1 or greater than 20.		
		li		$s0, 20
		blez 	$t0, done
		bgt		$t0, $s0, done
		
# Read in the default value for the matrix's elements.		
		li		$v0, 5
		syscall
		move		$t2, $v0
		
# Terminate if the default value is less than 0 or greater than 9.
		li		$s0, 9
		bltz 	$t2, done
		bgt		$t2, $s0, done
		
		li		$t3, 0					# Memory counting variable, to be incremented by 4
		
fill_default:
# Store the default value in every element of the matrix.
		bge		$t3, $t1, increment_element
		sw		$t2, matrix($t3)
		addi		$t3, $t3, 4
		b		fill_default
		
increment_element:
# Read in the row number of an element to increment.
		li		$v0, 5
		syscall
		move		$t4, $v0

# Continue on if the row number is less than 0 or greater than or equal to the matrix's size.
		bltz 	$t4, prepare_print
		bge		$t4, $t0, prepare_print
		
# Read in the column number of an element to increment.
		li		$v0, 5
		syscall
		move		$t5, $v0

# Continue on if the column number is less than 0 or greater than or equal to the matrix's size.
		bltz 	$t5, prepare_print
		bge		$t5, $t0, prepare_print
		
# Calculate the memory address of the specified element:	(4(size) * rowNum) + 4(columnNum)
		li		$t6, 0
		add		$t6, $t0, $t0
		add		$t6, $t6, $t6
		mult		$t4, $t6
		mflo		$t6
		add		$t5, $t5, $t5
		add		$t5, $t5, $t5
		add		$t6, $t6, $t5
		
# Fetch the element and increment it by 1.
		lw		$t7, matrix($t6)
		addi		$t7, $t7, 1
		
# If the value of the element was 9, loop the value back to 0. If not, continue onwards.
		bne		$t7, 10, basic_increment
		li		$t7, 0
		
basic_increment:
# Insert the incremented element back into the matrix (memory), then loop.
		sw		$t7, matrix($t6)
		b		increment_element

prepare_print:
# Print a line feed.
		li		$v0, 11
		addi		$a0, $0, 10
		syscall

# Reset the values of two registers.
		li		$t2, 0					# Row counting variable, to be incremented by 1
		li		$t3, 0					# Memory counting variable, to be incremented by 4

print_matrix:
# Terminate once every element in the matrix has been printed.
		bge		$t3, $t1, done

# Print the next element of the matrix.
		lw		$a0, matrix($t3)
		li		$v0, 1
		syscall

# Increment the counting variables.
		addi		$t2, $t2, 1
		addi		$t3, $t3, 4

# Take necessary action if the end of a row has been reached. If not, then loop.
		beq		$t2, $t0, next_row
		b		print_matrix
		
next_row:
# Reset the row counting variable, print a line feed to move to the next row, and loop.
		li		$t2, 0
		li		$v0, 11
		addi		$a0, $0, 10
		syscall
		b		print_matrix

# Termination station
done:

.data
matrix:	.space	1600					# Enough allocated memory to store a 20x20 matrix.