#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

char outbuf[128] SECTION("shared_dram");

int main(void) {
	e_coreid_t coreid;

	// Query the CoreID from hardware.
	coreid = e_get_coreid();

	//write hello world to the buffer
	sprintf(outbuf, " 0x%03x Reporting", coreid);

	return EXIT_SUCCESS;
}
