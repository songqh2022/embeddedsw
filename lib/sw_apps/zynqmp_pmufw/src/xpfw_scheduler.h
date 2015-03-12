/******************************************************************************
*
* Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#ifndef XPFW_SCHEDULER_H_
#define XPFW_SCHEDULER_H_


#include "xpfw_default.h"

#define XPFW_SCHED_MAX_TASK	10U


typedef void (*XPfw_Callback_t) (void);

struct XPfw_Task_t{
	u32 Interval;
	u32 OwnerId;
	XPfw_Callback_t Callback;
};

typedef struct {
	struct XPfw_Task_t TaskList[XPFW_SCHED_MAX_TASK];
	u32 TaskCount;
	u32 PitBaseAddr;
	u32 Tick;
	u32 Enabled;
} XPfw_Scheduler_t ;


void XPfw_SchedulerTickHandler(XPfw_Scheduler_t *SchedPtr);
XStatus XPfw_SchedulerInit(XPfw_Scheduler_t *SchedPtr, u32 PitBaseAddr);
XStatus XPfw_SchedulerStart(XPfw_Scheduler_t *SchedPtr);
XStatus XPfw_SchedulerStop(XPfw_Scheduler_t *SchedPtr);
XStatus XPfw_SchedulerProcess(const XPfw_Scheduler_t *SchedPtr);
XStatus XPfw_SchedulerAddTask(XPfw_Scheduler_t *SchedPtr, u32 OwnerId,u32 MilliSeconds, XPfw_Callback_t Callback);
XStatus XPfw_SchedulerRemoveTask(XPfw_Scheduler_t *SchedPtr, u32 OwnerId, u32 MilliSeconds, XPfw_Callback_t Callback);

#endif /* XPFW_SCHEDULER_H_ */
