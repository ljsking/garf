/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : app.c
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "util.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_LINE_0                                        0
#define  APP_LINE_1                                       24
#define  APP_LINE_2                                       48
#define  APP_LINE_3                                       72
#define  APP_LINE_4                                       96
#define  APP_LINE_5                                      120
#define  APP_LINE_6                                      144
#define  APP_LINE_7                                      168
#define  APP_LINE_8                                      192
#define  APP_LINE_9                                      216

#define  APP_IDLE                                          0
#define  APP_SHOW_SEQ                                      1
#define  APP_WAIT_INPUT                                    2
#define  APP_RECOGNIZE                                     3


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK         App_TaskUserIFStk[APP_TASK_USER_IF_STK_SIZE];
static  OS_STK         App_TaskKbdStk[APP_TASK_KBD_STK_SIZE];
static  OS_STK         App_Task7SegsStk[APP_TASK_7SEGS_STK_SIZE];

static  OS_EVENT      *App_UserIFMbox;
static  OS_EVENT      *App_ProcessMbox;
static  OS_EVENT      *App_7SegsMbox;

#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (APP_PROBE_COM_EN  == DEF_ENABLED) && \
     (PROBE_COM_STAT_EN == DEF_ENABLED))
static  CPU_FP32       App_ProbeComRxPktSpd;
static  CPU_FP32       App_ProbeComTxPktSpd;
static  CPU_FP32       App_ProbeComTxSymSpd;
static  CPU_FP32       App_ProbeComTxSymByteSpd;

static  CPU_INT32U     App_ProbeComRxPktLast;
static  CPU_INT32U     App_ProbeComTxPktLast;
static  CPU_INT32U     App_ProbeComTxSymLast;
static  CPU_INT32U     App_ProbeComTxSymByteLast;

static  CPU_INT32U     App_ProbeComCtrLast;
#endif

#if (APP_OS_PROBE_EN == DEF_ENABLED)
static  CPU_INT32U     App_ProbeCounts;
static  CPU_BOOLEAN    App_ProbeB1;
static  CPU_BOOLEAN    App_ProbeJoystickLeft;
static  CPU_BOOLEAN    App_ProbeJoystickRight;
static  CPU_BOOLEAN    App_ProbeJoystickUp;
static  CPU_BOOLEAN    App_ProbeJoystickDown;
static  CPU_BOOLEAN    App_ProbeJoystickCenter;
#endif


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskCreate       (void);
static  void  App_EventCreate      (void);

static  void  App_TaskStart        (void        *p_arg);
static  void  App_TaskUserIF       (void        *p_arg);
static  void  App_TaskKbd          (void        *p_arg);
static  void  App_Task7Segs        (void        *p_arg);

#if ((APP_PROBE_COM_EN == DEF_ENABLED) || \
     (APP_OS_PROBE_EN  == DEF_ENABLED))
static  void  App_InitProbe        (void);
#endif

#if (APP_OS_PROBE_EN == DEF_ENABLED)
static  void  App_ProbeCallback    (void);
#endif

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

int  main (void)
{
    CPU_INT08U  os_err;


    BSP_IntDisAll();                                            /* Disable all ints until we are ready to accept them.  */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel".         */

    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    return (0);
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
    CPU_INT32U  i;
    CPU_INT08U  nstate;
    CPU_INT32U  nbutton;
    CPU_INT08U  err;
    CPU_INT08U  now_stage;
    CPU_INT08U  remained_input;
    
    CPU_INT08U  answer[200];
    
    (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions.                            */
    OS_CPU_SysTickInit();                                       /* Initialize the SysTick.                              */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity.                              */
#endif

#if ((APP_PROBE_COM_EN == DEF_ENABLED) || \
     (APP_OS_PROBE_EN  == DEF_ENABLED))
    App_InitProbe();
#endif

    App_EventCreate();                                          /* Create application events.                           */
    App_TaskCreate();                                           /* Create application tasks.                            */
    nstate = APP_IDLE;
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */

        switch(nstate){
        case APP_IDLE:
            now_stage = 0;
            OSMboxPend(App_ProcessMbox, 0, &err);
            if(OS_ERR_NONE == err)
                nstate = APP_SHOW_SEQ;
            break;
        
        case APP_SHOW_SEQ:
            now_stage++;
            OSMboxPost(App_7SegsMbox, (void *)now_stage);
            BSP_LED_On(0);
            OSTimeDlyHMSM(0, 0, 0, 250);
            BSP_LED_Off(0);
            answer[now_stage-1] = MakeRandomNumber(4);
            for (i = 0; i < now_stage; i++) {
                OSTimeDlyHMSM(0, 0, 0, 500);
                BSP_LED_On(answer[i]+1);
                OSTimeDlyHMSM(0, 0, 0, 500);
                BSP_LED_Off(answer[i]+1);
            }
            remained_input = now_stage;
            nstate = APP_WAIT_INPUT;
            OSMboxAccept(App_ProcessMbox); // clear mail box
            break;
            
        case APP_WAIT_INPUT:
            nbutton = (CPU_INT32U)OSMboxPend(App_ProcessMbox, OS_TICKS_PER_SEC*3, &err);
            if(OS_ERR_NONE == err){
                nstate = APP_RECOGNIZE;
            }else if(OS_ERR_TIMEOUT == err){
                BSP_LED_On(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                BSP_LED_Off(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                BSP_LED_On(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                BSP_LED_Off(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                nstate = APP_IDLE;
            }
            break;
          
        case APP_RECOGNIZE:
            remained_input--;
            if(nbutton == answer[now_stage-remained_input-1]){
                if(0 == remained_input)
                    nstate = APP_SHOW_SEQ;
                else
                    nstate = APP_WAIT_INPUT;
            }else{
              BSP_LED_On(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                BSP_LED_Off(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                BSP_LED_On(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                BSP_LED_Off(0);
                OSTimeDlyHMSM(0, 0, 0, 250);
                nstate = APP_IDLE;
            }
            break;
        }

    }
}



/*
*********************************************************************************************************
*                                             App_EventCreate()
*
* Description : Create the application events.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_EventCreate (void)
{
#if (OS_EVENT_NAME_SIZE > 12)
    CPU_INT08U  os_err;
#endif


    App_UserIFMbox = OSMboxCreate((void *)0);                   /* Create MBOX for communication between Kbd and UserIF.*/
    App_ProcessMbox = OSMboxCreate((void *)0);                   /* Create MBOX for communication between Kbd and Process.*/
    App_7SegsMbox = OSMboxCreate((void *)0);                   /* Create MBOX for communication between Process and 7Segs.*/
#if (OS_EVENT_NAME_SIZE > 12)
    OSEventNameSet(App_UserIFMbox, "User IF Mbox", &os_err);
    OSEventNameSet(App_ProcessMbox, "Process Mbox", &os_err);
    OSEventNameSet(App_7SegsMbox, "7Segs Mbox", &os_err);
#endif
}


/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
    CPU_INT08U  os_err;


    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskUserIF,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskUserIFStk[APP_TASK_USER_IF_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_USER_IF_PRIO,
                             (INT16U          ) APP_TASK_USER_IF_PRIO,
                             (OS_STK        * )&App_TaskUserIFStk[0],
                             (INT32U          ) APP_TASK_USER_IF_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_USER_IF_PRIO, "User I/F", &os_err);
#endif

    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskKbd,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskKbdStk[APP_TASK_KBD_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_KBD_PRIO,
                             (INT16U          ) APP_TASK_KBD_PRIO,
                             (OS_STK        * )&App_TaskKbdStk[0],
                             (INT32U          ) APP_TASK_KBD_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_KBD_PRIO, "Keyboard", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *)) App_Task7Segs,
                             (void          * ) 0,
                             (OS_STK        * )&App_Task7SegsStk[APP_TASK_7SEGS_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_7SEGS_PRIO,
                             (INT16U          ) APP_TASK_7SEGS_PRIO,
                             (OS_STK        * )&App_Task7SegsStk[0],
                             (INT32U          ) APP_TASK_7SEGS_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 9)
    OSTaskNameSet(APP_TASK_KBD_PRIO, "7 Segs", &os_err);
#endif
}


/*
*********************************************************************************************************
*                                            App_TaskKbd()
*
* Description : Monitor the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg       Argument passed to 'App_TaskKbd()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskKbd (void *p_arg)
{
    CPU_BOOLEAN  b_prev;
    CPU_BOOLEAN  b;

    (void)p_arg;

    b_prev = DEF_FALSE;

    while (DEF_TRUE) {
        b = BSP_PB_GetStatus();
        
        if (DEF_BIT_IS_SET(b, BSP_PUSH_BUTTON_1) && DEF_BIT_IS_CLR(b_prev, BSP_PUSH_BUTTON_1)) {
            OSMboxPost(App_ProcessMbox, (void *)0);
        }else if(DEF_BIT_IS_SET(b, BSP_PUSH_BUTTON_2) && DEF_BIT_IS_CLR(b_prev, BSP_PUSH_BUTTON_2)){
            OSMboxPost(App_ProcessMbox, (void *)1);
        }else if(DEF_BIT_IS_SET(b, BSP_PUSH_BUTTON_3) && DEF_BIT_IS_CLR(b_prev, BSP_PUSH_BUTTON_3)){
            OSMboxPost(App_ProcessMbox, (void *)2);
        }else if(DEF_BIT_IS_SET(b, BSP_PUSH_BUTTON_4) && DEF_BIT_IS_CLR(b_prev, BSP_PUSH_BUTTON_4)){
            OSMboxPost(App_ProcessMbox, (void *)3);
        }
        b_prev = b;
        OSTimeDlyHMSM(0, 0, 0, 20);
    }
}


/*
*********************************************************************************************************
*                                            App_TaskUserIF()
*
* Description : Updates LCD.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskUserIF()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskUserIF (void *p_arg)
{
    CPU_INT08U  *msg;
    CPU_INT08U   err;
    CPU_INT32U   nstate;
    CPU_INT32U   count;
    count = 0;
    (void)p_arg;

    OSTimeDlyHMSM(0, 0, 1, 0);
    nstate = 1;


    while (DEF_TRUE) {
        msg = (CPU_INT08U *)(OSMboxPend(App_UserIFMbox, 0, &err));
        if (err == OS_NO_ERR) {
            nstate = (CPU_INT32U)msg;
            switch (nstate) {
            case 0:
                count++;
                break;
            }
        }
    }
}

/*
*********************************************************************************************************
*                                            App_Task7Segs()
*
* Description : Updates 7 segments.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskUserIF()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Task7Segs (void *p_arg)
{
    CPU_INT32U   msg;
    CPU_INT08U   err;
    (void)p_arg;

    OSTimeDlyHMSM(0, 0, 1, 0);

    while (DEF_TRUE) {
        msg = (CPU_INT32U)(OSMboxPend(App_7SegsMbox, OS_TICKS_PER_SEC / 10, &err));
        if (err == OS_NO_ERR) {
            BSP_7Segs(msg);
        }
    }
}

/*
*********************************************************************************************************
*                                             App_InitProbe()
*
* Description : Initialize uC/Probe target code.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_PROBE_COM_EN == DEF_ENABLED) || \
     (APP_OS_PROBE_EN  == DEF_ENABLED))
static  void  App_InitProbe (void)
{
#if (APP_OS_PROBE_EN        == DEF_ENABLED)
    (void)App_ProbeCounts;
    (void)App_ProbeB1;
    (void)App_ProbeJoystickCenter;
    (void)App_ProbeJoystickDown;
    (void)App_ProbeJoystickLeft;
    (void)App_ProbeJoystickRight;
    (void)App_ProbeJoystickUp;

#if ((APP_PROBE_COM_EN      == DEF_ENABLED) && \
     (PROBE_COM_STAT_EN     == DEF_ENABLED))
    (void)App_ProbeComRxPktSpd;
    (void)App_ProbeComTxPktSpd;
    (void)App_ProbeComTxSymSpd;
    (void)App_ProbeComTxSymByteSpd;
#endif

    OSProbe_Init();
    OSProbe_SetCallback(App_ProbeCallback);
    OSProbe_SetDelay(250);
#endif

#if (APP_PROBE_COM_EN       == DEF_ENABLED)
    ProbeCom_Init();                                            /* Initialize the uC/Probe communications module.       */
#if (PROBE_COM_METHOD_RS232 == DEF_ENABLED)
    ProbeRS232_Init(115200);
    ProbeRS232_RxIntEn();
#endif
#endif
}
#endif

/*
*********************************************************************************************************
*                                         AppProbeCallback()
*
* Description : uC/Probe OS plugin callback.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : uC/Probe OS plugin task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_OS_PROBE_EN == DEF_ENABLED)
static  void  App_ProbeCallback (void)
{

#if ((APP_PROBE_COM_EN  == DEF_ENABLED) && \
     (PROBE_COM_STAT_EN == DEF_ENABLED))
    CPU_INT32U  ctr_curr;
    CPU_INT32U  rxpkt_curr;
    CPU_INT32U  txpkt_curr;
    CPU_INT32U  sym_curr;
    CPU_INT32U  symbyte_curr;
#endif
    
    App_ProbeCounts++;

    App_ProbeB1             = BSP_PB_GetStatus(1);

#if ((APP_PROBE_COM_EN  == DEF_ENABLED) && \
     (PROBE_COM_STAT_EN == DEF_ENABLED))
    ctr_curr     = OSTime;
    rxpkt_curr   = ProbeCom_RxPktCtr;
    txpkt_curr   = ProbeCom_TxPktCtr;
    sym_curr     = ProbeCom_TxSymCtr;
    symbyte_curr = ProbeCom_TxSymByteCtr;

    if ((ctr_curr - App_ProbeComCtrLast) >= OS_TICKS_PER_SEC) {
        App_ProbeComRxPktSpd      = ((CPU_FP32)(rxpkt_curr   - App_ProbeComRxPktLast)     / (ctr_curr - App_ProbeComCtrLast)) * OS_TICKS_PER_SEC;
        App_ProbeComTxPktSpd      = ((CPU_FP32)(txpkt_curr   - App_ProbeComTxPktLast)     / (ctr_curr - App_ProbeComCtrLast)) * OS_TICKS_PER_SEC;
        App_ProbeComTxSymSpd      = ((CPU_FP32)(sym_curr     - App_ProbeComTxSymLast)     / (ctr_curr - App_ProbeComCtrLast)) * OS_TICKS_PER_SEC;
        App_ProbeComTxSymByteSpd  = ((CPU_FP32)(symbyte_curr - App_ProbeComTxSymByteLast) / (ctr_curr - App_ProbeComCtrLast)) * OS_TICKS_PER_SEC;

        App_ProbeComCtrLast       = ctr_curr;
        App_ProbeComRxPktLast     = rxpkt_curr;
        App_ProbeComTxPktLast     = txpkt_curr;
        App_ProbeComTxSymLast     = sym_curr;
        App_ProbeComTxSymByteLast = symbyte_curr;
    }
#endif
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskCreateHook(ptcb);
#endif
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*
*               (2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskSwHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument(s) : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                        TICK HOOK (APPLICATION)
*
* Description : This function is called every tick.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TickHook();
#endif
}
#endif
#endif
