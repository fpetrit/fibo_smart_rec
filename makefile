all: simulateur

main:
	gcc -o simulateur -Wall main.c ./assembler/assembler.c ./assembler/opstring_mapping.c 

mrproper:
	rm simulateur hexa.txt