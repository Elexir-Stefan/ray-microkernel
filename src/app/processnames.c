/**
 * @file processnames.c
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Fr Aug 1 2008
 * @see processnames.h
 * @brief Associates (descriptive) names with processes
 */

#include <typedefs.h>
#include <mtask/pid_types.h>
#include <memory/strings/strings.h>
#include <memory/memory.h>
#include <debug.h>

#define MAX_DESCRIPTION_LENGTH	128
#define MAX_NAME_LENGTH		64
#define MIN_NAME_LENGTH		3

typedef struct _DESCNAME {
	PID pid;
	String name;
	String description;
	struct _DESCNAME *next;
} DESCNAME;

DESCNAME *root = NULL;

/**
 * Returns the PID of a process by its associated descriptive name
 * @param name of the process to look for
 * @return PID of the process or 0 if name does not exist
 */
PID GetPIDByDescriptiveName(CString name) {
	DESCNAME *current = root;

	while(current) {
		if (strcmp(current->name, name) == 0) {
			return current->pid;
		}
		current = current->next;
	}
	return PID_DOES_NOT_EXIST;
}

static DESCNAME *GetItemByPID(PID pid) {
	DESCNAME *current = root;

	while(current) {
		if (current->pid == pid) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

String GetNameByPID(PID pid) {
	DESCNAME *find = GetItemByPID(pid);
	if (find) {
		return find->name;
	} else {
		return NULL;
	}
}

String GetDescriptionByPID(PID pid) {
	DESCNAME *find = GetItemByPID(pid);
	if (find) {
		return find->description;
	} else {
		return NULL;
	}
}

BOOL RegisterDescriptiveName(PID pid, CString name, CString description) {
	if (GetPIDByDescriptiveName(name) == PID_DOES_NOT_EXIST) {
		// create new entry for the list
		UINT32 nameLength = strnlen(name, MAX_NAME_LENGTH) + 1;
		UINT32 descLength = strnlen(description, MAX_DESCRIPTION_LENGTH) + 1;

		if (LIKELY(nameLength >= MIN_NAME_LENGTH)) {
			DESCNAME *newEntry = KMalloc(sizeof(DESCNAME) + nameLength + descLength, MEMTYPE_DATA);

			newEntry->name = (String)(newEntry + 1);
			newEntry->description = newEntry->name + nameLength;
			newEntry->pid = pid;

			memcpy(newEntry->name, name, nameLength);
			memcpy(newEntry->description, description, descLength);

			// insert element in the list
			newEntry->next = root;
			root = newEntry;
		} else {
			// Name is too short
			return FALSE;
		}
		return TRUE;
	} else {
		ASSERT(FALSE);
		return FALSE;
	}
}
