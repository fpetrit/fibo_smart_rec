/**
 * @file
 * @brief A resizable array struct that stores the labels data.
 */

#ifndef LABEL_VECTOR_H
#define LABEL_VECTOR_H

#include "../constants.h"


/**
 * @brief Represents a label: a string that we can reference in the source code to indiactes the address number where it was declared.
 */
typedef struct {

    char name[LABEL_MAX_LEN + 1];
    int address;
    unsigned int line_no;

} Label ;


/**
 * @brief A resizable array structure that stores the labels data.
 */
typedef struct {

    /// @brief The current maximal number of labels the array can store.
    int length;

    /// @brief The actual number of labels stored in the array.
    int count;

    /// @brief The array is reprensented by a pointer to a @ref Label struct.
    Label * arr;

} Label_vector ;


/// @brief  Initialize the passed @ref Label_vector.
void Label_vector_init(Label_vector * vect);


/// @brief Add a @ref Label strucure in the passed @ref Label_vector. The vector is automatically resized if full. 
Label * Label_vector_create_label(Label_vector * vect, char * name, int address, int line_no);


/**
 * @brief Search for label name in the vector.
 * @return The index of the searched label in the @ref Label_vector::arr if found, else -1.
 */
int Label_vector_search(Label_vector * vect, char * name);


/**
 * @brief Free the @ref Label_vector dynamically allocated memory.
 */
void Label_vector_deconstruct(Label_vector * vect);

#endif