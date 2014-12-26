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
