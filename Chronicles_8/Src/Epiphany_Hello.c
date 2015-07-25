#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"
#define cnt_address 0x80803000


int *outbuf; 
int sig;
unsigned final = 1000;

void timer_hand(int sig);

void __attribute__ ((interrupt)) timer_hand(int sig){

  outbuf = outbuf +1;
  *outbuf = 0x55;

}


int main(void) {

  int loop;
  unsigned timer_value_0;
  unsigned timer_value_1;
  

  outbuf = (unsigned int *) cnt_address;

  //read the timer value it should be zero
  timer_value_0 = e_ctimer_get(E_CTIMER_0); 
  *outbuf = timer_value_0;
  outbuf = outbuf+1;
  
  //set the timer to a value it should return what it is set to 
  timer_value_0 = e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX);
  e_ctimer_set(E_CTIMER_1,E_CTIMER_MAX);
  *outbuf = timer_value_0;
  outbuf = outbuf+1;
  
  //start the timer initially to count down clocks
  e_ctimer_start(E_CTIMER_1, E_CTIMER_IALU_INST); 
  e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);

  //stop timer and determine the time taken 	
  timer_value_0 = E_CTIMER_MAX - e_ctimer_stop(E_CTIMER_0);
  timer_value_1 = E_CTIMER_MAX - e_ctimer_stop(E_CTIMER_1);
  *outbuf = timer_value_0;
  outbuf = outbuf+1;
  *outbuf = timer_value_1;

  

  //start timer again using clocks and trigger on interrupts 

  e_irq_attach(E_TIMER0_INT, timer_hand);
  
  e_irq_mask(E_TIMER0_INT, E_FALSE);
  e_irq_global_mask(E_FALSE);
  
  e_ctimer_set(E_CTIMER_0,  final);
  e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
  

  while(e_ctimer_get(E_CTIMER_0));

  return EXIT_SUCCESS;
}


    
