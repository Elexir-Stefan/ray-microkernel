/*
 * rmi_internals.h
 *
 *  Created on: 08.03.2009
 *      Author: stefan
 */

#ifndef RMI_INTERNALS_H_
#define RMI_INTERNALS_H_

#include <mtask/privileges.h>
#include <mtask/task_types.h>


typedef struct {
	PROCESS_PTR thread;
} RMI_PROCESS;


typedef struct {
	UINT32 maxSize;
	MSG_TYPE type;
} GIFT_BUFFER;



#endif /* RMI_INTERNALS_H_ */
