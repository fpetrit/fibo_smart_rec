/**
 * @file
 * @brief Open the input file, parses it, compiles if it does not produce any error, then runs it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "assembler/assembler.h"
#include "assembler/Label_vector.h"
#include "runtime/runtime.h"

regex_t label_regex;

int main(int argc, char ** argv){

    // test the argcount, we need 2 (the binary name + 1 filename)

    if (argc != 2){
        fprintf(stderr, "Usage: ./simulateur <filename>\n"); //stderr:
        exit(EXIT_FAILURE);
    }

    // open the source file, open a new output file in the same folder, test file descriptors

    FILE * src, * output;

    src = fopen(argv[1], "r");

    if (! src){
        perror("source code opening");
        exit(EXIT_FAILURE);
    }

    output = fopen("hexa.txt", "w+");

    if (! output){
        perror("output file opening");
        fclose(src);
        exit(EXIT_FAILURE);
    }

    regcomp(&label_regex, LABEL_PATTERN, REG_NOSUB);

    // parsing and assembling
    // parsing is reading the input source code to verify the absence of error

    // a label: "une étiquette", translates the address number where it is declared in the source code to a referable string
    // a vector is a data structure where data is contiguously stored in RAM, it is different from an array
    // it automatically uses malloc calls to dynamically allocate more memory when the vector is full
    Label_vector labels;
    Label_vector_init(&labels);

    // the response is non zero if any error occured
    int response = parse(src, &labels);

    // the assemble function
    if (response == 0)
        assemble(src, output, &labels);

    fclose(src);

    // must be called when the variable is useless to free dynamically allocated memory (malloc --> free)
    Label_vector_deconstruct(&labels);

    regfree(&label_regex);

    // if no error (response is 0), begin execution of ./hexa.txt
    if (response == 0)
        run(output);

    fclose(output);

    exit(EXIT_SUCCESS);
}