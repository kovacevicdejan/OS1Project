#include "timer.h"
#include <dos.h>
#include "pcb.h"
#include "SCHEDULE.H"
#include "flags.h"
#include "kernsem.h"

unsigned tsp;
unsigned tss;
unsigned tbp;
PCB* temp;

volatile int counter = defaultTimeSlice;
void tick();

void interrupt timer() {
	if(!context_change_request) {
		tick();
		if(lockFlag == 0) {
			KernelSem::tick();
			while(lockTickCounter > 0) {
				KernelSem::tick();
				--lockTickCounter;
            }
		}
		else
		    ++lockTickCounter;
	}
	
	if (!context_change_request && counter > 0) counter--;

	//if(!context_change_request) asm int 60h;

	if ((counter == 0 && running->timeSlice > 0) || context_change_request) {
		if(lockFlag == 0){
			context_change_request = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			if(running->getId() != idle.getId()) {
				if(running->status == RUNNING) {
					running->status = READY;
					Scheduler::put((PCB*)running);
				}
			}
			else {
				running->status = IDLE;
			}
        
			if((temp = Scheduler::get()) != 0)
				running = temp;
			else {
				if(idlePCB == 0)
					idlePCB = PCB::getPCBById(idle.getId());
				running = idlePCB;
			}

			running->status = RUNNING;
            
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			counter = running->timeSlice;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

		}
		else context_change_request = 1;
	}
	if(!context_change_request) asm int 60h;
}

unsigned oldTimerOFF, oldTimerSEG; 

void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0  
		mov es,ax

		mov ax, word ptr es:0022h 
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0020h	
		mov word ptr oldTimerOFF, ax	

		mov word ptr es:0022h, seg timer	
		mov word ptr es:0020h, offset timer 

		mov ax, oldTimerSEG 	
		mov word ptr es:0182h, ax 
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}
