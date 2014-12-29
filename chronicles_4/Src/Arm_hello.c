#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h> //harsware abstraction library 

int main()
{
  //counters for row and colum, cored id and loop counter
  unsigned   row_loop,col_loop;
  // this will contain the epiphany platform configura tion
  e_platform_t epiphany;
  e_epiphany_t dev;
  e_mem_t memory;      
  e_mem_t memory2;
  int  message;
  int  core1;
  int  core2;
  int  message2;
  e_return_stat_t result;
 

  e_init(NULL); // initialise the system establish connection to the Device
  e_reset_system(); // reset the epiphnay chip
  e_get_platform_info(&epiphany);//gets the configuration info for the parallella platofrm


   for(row_loop=0; row_loop <4; row_loop ++)
   {
     for(col_loop=0; col_loop <3; col_loop = col_loop+2)
     {
	//one core within the parallella work group is 1 x 2 i.e dual core
	e_open(&dev,row_loop,col_loop,1,2); 
	//reset the group
	e_reset_group(&dev);
	//load the group
	result =  e_load("hello_world.srec",&dev,0,0,E_FALSE);
       	if (result != E_OK){
	  fprintf(stderr,"Error Loading the Epiphany Application 1 %i\n", result);
       	}
          	
	result =  e_load("hello_world2.srec",&dev,0,1,E_FALSE);
      	if (result != E_OK){
	  fprintf(stderr,"Error Loading the Epiphany Application 2 %i\n", result);
       	}
	e_start_group(&dev);
	usleep(10000);
	e_read(&dev,0,0,0x3000, &message, sizeof(int));
	e_read(&dev,0,0,0x3004, &core1,sizeof(int));
       	e_read(&dev,0,1,0x3000, &message2, sizeof(int));
	e_read(&dev,0,1,0x3004, &core2,sizeof(int));
       	fprintf(stderr,"message from  core %d ", core1);
	fprintf(stderr,"core id  = 0x%03x \n", message);
 	fprintf(stderr,"message from  core %d ", core2);
	fprintf(stderr,"core id = 0x%03x \n", message2);
	e_close(&dev);
     }
    }
 
  e_free(&memory);
  e_finalize();

  return 0;

}
