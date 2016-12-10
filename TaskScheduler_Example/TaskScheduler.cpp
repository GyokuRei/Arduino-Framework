// 
// 
// 

#include "TaskScheduler.h"

void TaskSchedulerClass::Init()
{
	cli();                                  // Turn off global interrupt

	/* Reset Timer/Counter1 */
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 = 0;

	/* Setup Timer/Counter1 */
	TCCR1B |= (1 << CS11) | (1 << CS10);    // prescale = 64
	TCNT1 = 65286;
	TIMSK1 = (1 << TOIE1);                  // Overflow interrupt enable 

}

//Add a function into Thread Array and run every time

void TaskSchedulerClass::Add(void(*func)(), uint16_t time)
{
	threadNumber++;		// initial number is 0 and increase 1 value before add and function into array
	Thread *threadTemp = new Thread[threadNumber - 1];	// temporary array use to save olde thread array

	for (uint8_t index = 0; index < (threadNumber - 1); index++)
	{
		threadTemp[index] = ThreadArray[index];
	}

	if (ThreadArray != NULL)
	{
		delete[] ThreadArray;
	}

	ThreadArray = new Thread[threadNumber];
	for (uint8_t index = 0; index < (threadNumber - 1); index++)
	{
		ThreadArray[index] = threadTemp[index];
	}

	Thread thread;
	thread.Func = func;
	thread.Time = time;
	thread.CountDown = 0;

	// Add new thread into array
	ThreadArray[threadNumber - 1] = thread;
}

// Sort every thread increase from small to big base on time.

void TaskSchedulerClass::sort()
{
	Thread tempThread;
	for (uint8_t index = 0; index < threadNumber - 1; index++)
	{
		for (uint8_t index1 = 1; index1 < threadNumber; index1++)
		{
			if (ThreadArray[index].Time > ThreadArray[index1].Time)
			{
				tempThread = ThreadArray[index];
				ThreadArray[index] = ThreadArray[index1];
				ThreadArray[index1] = tempThread;

			}
		}
	}
}

void TaskSchedulerClass::Run()
{
	sort();
	sei();

}

void TaskSchedulerClass::Stop()
{
	cli();
}

void TaskSchedulerClass::Execute()
{
	if (!threadNumber)
		return;

	timeCounter++;
	if (timeCounter < funcCallingTime)
		return;

	//when enough time to call function

	timeCounter = 0;

	ThreadArray[nearestThreadOrder].Func();

	for (uint8_t index = 0; index < threadNumber; index++)
	{
		// Set count down time every thread
		ThreadArray[index].CountDown -= funcCallingTime;
	}

	ThreadArray[nearestThreadOrder].CountDown = ThreadArray[nearestThreadOrder].Time;

	nearestThreadOrder = 0;

	for (uint8_t index = 1; index < threadNumber; index++)
	{
		// Find thread order has count down time is smallest
		if (ThreadArray[index].CountDown < ThreadArray[nearestThreadOrder].CountDown)
		{
			nearestThreadOrder = index;
		}
	}

	// Set time to call function in next time
	funcCallingTime = ThreadArray[nearestThreadOrder].CountDown;

}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 65286;	// Interrupt function was called every 0.001 s
	TaskScheduler.Execute();
}

TaskSchedulerClass TaskScheduler;
