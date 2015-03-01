/*Copyright (c) 2015, Adam Taylor
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h> //harsware abstraction library 

#define _BufSize   (128)
#define _BufOffset (0x01000000)

int main()
{
  //counters for row and colum, cored id and loop counter
  unsigned row, col, id, row_loop,col_loop;
  // this will contain the epiphany platform configuration
  e_platform_t epiphany;
  e_epiphany_t dev;
  e_mem_t memory;
  char message[_BufSize];
  e_return_stat_t result;

  e_init(NULL); // initialise the system establish connection to the Device
  e_reset_system(); // reset the epiphnay chip
  e_get_platform_info(&epiphany);//gets the configuration info for the parallella platofrm
  // allocatethe shared memory for recieivng  the message from the core 
  e_alloc(&memory, _BufOffset, _BufSize);

  row = 0;
  col = 0;

  for(row_loop=0; row_loop <4; row_loop ++)
  {
    for(col_loop=0; col_loop <4; col_loop ++)
      {
	//one core within the parallella work group is 1 x 1 i.e single core
	e_open(&dev,row_loop,col_loop,1,1); 
	//reset the group
	e_reset_group(&dev);
	//load the group
	result =  e_load("hello_world.srec",&dev,0,0,E_TRUE);
	if (result != E_OK){
	  fprintf(stderr,"Error Loading the Epiphany Application %i\n", result);
	}
	usleep(10000);
	e_read(&memory,0,0,0x0, message, _BufSize);
	fprintf(stderr,"message from  core = %s\n", message);
	e_close(&dev);
      }
  }
 
  e_free(&memory);
  e_finalize();

  return 0;

}
