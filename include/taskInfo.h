/*
 * taskInfo.h
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_TASKINFO_H_
#define INCLUDE_TASKINFO_H_

#include <stdint.h>
#include "dateTime.h"

#define MAX_POND_COUNT		(16)
#define MAX_TASK_COUNT		(10)
#define TASK_SIZE			(58)
#define TASK_START_ADDR		(0)

#pragma pack(push, 1)

typedef struct
{
	uint8_t pondIdx;
	uint16_t feedingTime;
} Pond;

typedef struct
{
	uint8_t isEnabled;
	DateTime startTime;
	uint8_t totalPond;
	Pond pondList[MAX_POND_COUNT];
} Task;

#pragma pack(pop)

#endif /* INCLUDE_TASKINFO_H_ */
