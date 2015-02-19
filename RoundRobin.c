#include <stdio.h>
#include <xc.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

#define LEDA        PORTA
#define LEDB        PORTB
#define LEDC        PORTC
#define ON          0xFF
#define OUTPUT_PINS 0x00
#define OFF         0x00

//Function Declarations
static void task1(void *pvparameter);
static void task2(void* pvparameter);
static void task3(void* pvparameter);
eTaskState eTaskGetState( TaskHandle_t xTask );

//Global Variables used
xTaskHandle A, B, C, arr[10];
int z=0,i;
eTaskState status ;

int main()
{
    //Creating three tasks with taskHandle A,B,C 
    xTaskCreate( task3, "NAME3", 2000, NULL, 1,A);
    xTaskCreate( task1, "NAME", 2000, NULL, 1,B);
    xTaskCreate( task2, "NAME2",2000, NULL, 1,C);
    //Clearing portB & portC for LED Glow
    TRISA = OUTPUT_PINS;
    TRISB = OUTPUT_PINS;
    TRISC = OUTPUT_PINS;
    //Starting Scheduler
    vTaskStartScheduler();
    return 0;
}

static void task1(void *pvparameter)
{
     LEDB = ON;
}

static void task2(void*pvparameter)
{
     LEDC = ON;
}

static void task3(void*pvparameter)
{
    //eTaskGetState returns current status of passed task handle
    status= eTaskGetState( A );
    //status is zero for Ready State
    if(status==0) {
        vTaskSuspend(A) ;
        arr[z++]=A;
        //adding the suspended task to our array, so that we know which tasks we suspended
    }
    status= eTaskGetState( B );
    if(status==0) {
        vTaskSuspend(B) ;
        arr[z++]=B;
        //adding the suspended task to our array, so that we know which tasks we suspended
    }
    //Running it for specific time duration, which will later on be converted into number of ticks
    while(100000) {
        LEDA = ON;
    }

    for(i=0;i<z;i++)
        vTaskResume(arr[i]) ;
    vTaskSuspend(C);
}

void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
	vCoRoutineSchedule();
}


