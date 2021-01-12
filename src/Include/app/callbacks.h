#ifndef _CALLBACKS_H
#define _CALLBACKS_H

/**
 * @file callbacks.h
 * @date 10-29-2006
 * @brief Defines structures for return types of RMI functions
 */

typedef enum {
	RAY_SUCCESS = 0,
	RAY_WAKEUP_OCCUPIED = 1,
	RAY_WAKEUP_ILLEGAL
} RAY_WAKEUP;

#endif
