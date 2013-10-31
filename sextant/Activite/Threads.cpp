/*
 * Thread.cpp
 *
 *  Created on: 1 oct. 2008
 *      Author: Jean-Marc Menaud
 */
#include "Threads.h"

void Threads::suspend(int suspID){
	sched_set_waiting(thread_get_current(),suspID);
}

void Threads::resume(int suspID){
	sched_set_ready(pop_in_waiting_queue(suspID));
}



