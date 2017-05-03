# Pipesim - Pipeline Simulator

UFRN - Federal University of Rio Grande do Norte


Made by João Vitor Filgueira Albuquerque


This project is to simulate a simple 5-stage pipeline.


stages:

		IF		
		ID		
		EX	
		MEM
		WB

How to test:

	1. put your code in the "code.txt"
	2. open the terminal and go to the file
	3. write "make", than "./pipesim"


ps.:
the code need to be writen like this


	add $t0, $zero, $v0
	add $t1, $t0, $t2
	sub $s0, $t3, $t1
	lw $t4, 4($s0)
	beq $s0, $t4, DENTRO
	add $t1, $t2, $t3
	lw $t0, 128($t1)
	j EXIT
	DENTRO:
	sw $t8, 0($s8)
	sub $t1, $t2, $t3
	lw $t0, 8($s1)

with out the "header" and with out spaces.
and the Label need to be alone in a line

