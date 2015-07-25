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


    
