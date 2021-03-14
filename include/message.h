/*
 * message.h
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_MESSAGE_H_
#define INCLUDE_MESSAGE_H_

#define MSG_ID_START_CONFIG 	(0x01)
#define MSG_ID_END_CONFIG		(0x02)
#define MSG_ID_TASK_INFO		(0x03)
#define MSG_ID_SET_TIME			(0x04)
#define MSG_ID_GET_TIME			(0x05)
#define MSG_ID_GET_STATE		(0x06)
#define MSG_ID_DRIVE_FRWD		(0x07)
#define MSG_ID_DRIVE_BCWD		(0x08)
#define MSG_ID_DRIVE_STOP		(0x09)
#define MSG_ID_DRIVE_SET_SPEED	(0x0A)
#define MSG_ID_FEED_START		(0x0B)
#define MSG_ID_FEED_STOP		(0x0C)
#define MSG_ID_FEED_SET_SPEED	(0x0D)
#define MSG_ID_ABORT_TASK	 	(0x99)
#define MSG_ID_TASK_INFO_RESP	(0x0E)

#endif /* INCLUDE_MESSAGE_H_ */
