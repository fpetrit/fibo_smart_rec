all: simulateur

simulateur:
	gcc -g -o simulateur -Wall main.c assembler/assembler.c assembler/error_checking.c assembler/Label_vector.c assembler/opstring_mapping.c assembler/Error.c runtime/Instruction_vector.c runtime/runtime.c

mrproper:
	rm simulateur hexa.txt