#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

#define ptr_address 0x80903000
#define int_address 0x80903004

int *outbuf; 
int *msg;


int main(void) {
	e_coreid_t coreid;
	//point to area of memory with to store the string 
	

	// Query the CoreID from hardware.
	coreid = e_get_coreid();

	outbuf = (unsigned int *) ptr_address;
        msg = (unsigned int *) int_address;
	*outbuf = coreid; 
	*msg = 1;

	return EXIT_SUCCESS;
}
