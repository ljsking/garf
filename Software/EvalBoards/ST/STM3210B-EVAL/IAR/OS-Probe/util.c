/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>

CPU_INT08U MakeRandomNumber(CPU_INT08U arg){
    CPU_INT08U clk;
    clk = OSTimeGet();
    return clk%arg;
}