#ifndef FLAGS_H_
#define FLAGS_H_

#define disable_interrupts asm { pushf; cli; }
#define enable_interrupts asm popf

extern volatile int lockFlag;
extern volatile int context_change_request;

void dispatch();

#define lock ++lockFlag
#define unlock if(--lockFlag == 0 && context_change_request == 1) {dispatch();}

#endif /* FLAGS_H_ */