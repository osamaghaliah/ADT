/*
	This program represents an adaptive data structure which is designed for any manually built structure.
	An instance of this data structure contains the following:

		1. A function that creates an empty structure including a dynamic allocation for its elements - starting from 0.
		2. A function to delete the whole structure from the memory.
		3. A function to retrieve a specified data at a specified given index. (Getter-like).
		4. A function to assign/change new/old elements at a specified given index. (Setter-like).
		5. A function to retrieve the size.
		6. A function to print the elements.

	In order to observe the functionality of this data structure, running Demo.c file is a choice that will be providing us a good output.
	@author: Osama M. Ghaliah
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdptArray.h"
#include "book.h"
#include "Person.h"

typedef struct AdptArray_ {
	int size;
	PElement* p_array_objects;
	DEL_FUNC generic_del_func;
	COPY_FUNC generic_copy_func;
	PRINT_FUNC generic_print_func;
} AdptArray;

/*
	This function initializes an empty adaptive array.
*/
PAdptArray CreateAdptArray(COPY_FUNC generic_copy_func, DEL_FUNC generic_del_func, PRINT_FUNC generic_print_func) {
	// A dynamic allocation attempt for the structure as a whole.
	PAdptArray p_Adpt_Array = (PAdptArray) malloc(sizeof(AdptArray));

	// Checking whether the allocation succeeded or not.
	if (p_Adpt_Array == NULL)
		return NULL;

	// Initizalizing the size as 0.
	p_Adpt_Array->size = 0;
	// A dynamic allocation attempt for the elements of the array.
	p_Adpt_Array->p_array_objects = (PElement*) malloc(sizeof(PElement));
	
	// Checking whether the allocation succeeded or not.
	if (p_Adpt_Array->p_array_objects == NULL)
        return NULL;

	// Assigning the generic-like functions with the given ones.
	p_Adpt_Array->generic_del_func = generic_del_func;
	p_Adpt_Array->generic_copy_func = generic_copy_func;
	p_Adpt_Array->generic_print_func = generic_print_func;
	
	return p_Adpt_Array;
}

/*
	This function retrieves data of a specified given index.
*/
PElement GetAdptArrayAt(PAdptArray p_Adpt_Array, int index) {
	// Taking into consideration illegal/NULL arguements.
	if (p_Adpt_Array->p_array_objects[index] == NULL || index < 0 || index >= p_Adpt_Array->size)
		return NULL;
	
	// Building a copy of the wanted data that must to be returned.
	else {
		PElement ans = p_Adpt_Array->generic_copy_func(p_Adpt_Array->p_array_objects[index]);
		return ans;
	}
}

/*
	This function retrieves the current size of the adaptive array.
*/
int GetAdptArraySize(PAdptArray p_Adpt_Array) {
	// Taking into consideration whether the given argument is NULL.
	if (p_Adpt_Array == NULL)
		return -1;
	else
		return p_Adpt_Array->size;
}

/*
	This function prints out the current existing elements of the adaptive array.
*/
void PrintDB(PAdptArray p_Adpt_Array) {
	for (int i = 0; i < p_Adpt_Array->size; i++) {
		// Printing relevant (Non-NULL) values ONLY.
		if (p_Adpt_Array->p_array_objects[i] != NULL)
			p_Adpt_Array->generic_print_func(p_Adpt_Array->p_array_objects[i]);
	}
}

/*
	This function assigns/replaces new given elements at a specified given index.
*/
Result SetAdptArrayAt(PAdptArray p_Adpt_Array, int index, PElement new_element) {
	// Taking into consideration whether the given index is legal or not.
	if (index < 0)
        return FAIL;
	
	// In case the index is not in the range of the array, an extension is performed.
    if (index >= p_Adpt_Array->size) {
		// Building a new array with the new suitable size to contain the new index.
        PElement* extended_array = (PElement*) realloc(p_Adpt_Array->p_array_objects, (index + 1) * sizeof(PElement));

		// Checking whether the re-allocation/extension was succeeded or not.
		if (extended_array == NULL)
			return FAIL;
		
		// Updating our elements array to be the new extended one.
		p_Adpt_Array->p_array_objects = extended_array;

		// Getting rid of unwanted recently added units to save some memory. 
		for (int i = p_Adpt_Array->size; i <= index; i++) {
			p_Adpt_Array->p_array_objects[i] = NULL;
            free(p_Adpt_Array->p_array_objects[i]);
		}

		// Assigning our given element at the given specified index.
    	p_Adpt_Array->p_array_objects[index] = p_Adpt_Array->generic_copy_func(new_element);

		// Updating our adaptive array size to the newly extended size.
		p_Adpt_Array->size = index + 1;
    }
	
	// In case the index already exists (No need to re-allocate), simply assign the new element or replace if there is an old one at that index. 
	else {
		// Assigning our given element at the given specified index.
		p_Adpt_Array->p_array_objects[index] = p_Adpt_Array->generic_copy_func(new_element);

		// Updating our adaptive array size to the newly extended size.
		p_Adpt_Array->size = index + 1;
	}

    return SUCCESS;
}

/*
	This function deletes a given adaptive array by completely freeing it from the memory.
*/
void DeleteAdptArray(PAdptArray p_Adpt_Array) {
	if (p_Adpt_Array == NULL)
		return;

	for (int i = 0; i < p_Adpt_Array->size; i++) {
		// Deleting relevant (Non-NULL) values ONLY.
		if(p_Adpt_Array->p_array_objects[i] != NULL)
			p_Adpt_Array->generic_del_func(p_Adpt_Array->p_array_objects[i]);
	}

	// Freeing the whole elements.
	free(p_Adpt_Array->p_array_objects);

	// Freeing the whole adaptive array.
	free(p_Adpt_Array);
}