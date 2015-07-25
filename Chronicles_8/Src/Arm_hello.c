#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h> //harsware abstraction library 


int main()
{
  //counters for row and colum, cored id and loop counter
  unsigned   row_loop,col_loop;
  // this will contain the epiphany platform configuration
  e_platform_t epiphany;
  e_epiphany_t dev;
  e_return_stat_t result;

 unsigned  message;
  int  message2;
  int loop;
  int addr;;

  e_init(NULL); // initialise the system establish connection to the Device
  e_reset_system(); // reset the epiphnay chip
  e_get_platform_info(&epiphany);//gets the configuration info for the parallella platofrm


	//one core within the parallella work group is 1 x 1 i.e single core
        e_open(&dev,0,0,1,1); 
	//reset the group
	e_reset_group(&dev);
	//load the group
	result =  e_load("hello_world.srec",&dev,0,0,E_FALSE);
       	if (result != E_OK){
	  fprintf(stderr,"Error Loading the Epiphany Application 1 %i\n", result);
       	}
          	
	e_start_group(&dev);
	usleep(10000);

	fprintf(stderr,"timer loop \n ");
	addr = 0x3000;

	for(loop = 0; loop <5; loop ++){

	  e_read(&dev,0,0,addr, &message, sizeof(unsigned));
	  fprintf(stderr,"message from  core %u \n ", message);
	  addr = addr+0x04;
	}
	e_close(&dev);
	e_finalize();
	fprintf(stderr,"demo complete \n ");
	return 0;

}
