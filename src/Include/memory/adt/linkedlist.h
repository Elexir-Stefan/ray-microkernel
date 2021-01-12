/**
 * @file linkedlist.h
 * @author Stefan Nuernberger [NURNware Technologies - stefan@nurnware.de]
 * @date Mar 28, 2009
 * @brief ...
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/** data types **/

#define CreateList(name, innerName, nodeName) \
	typedef struct { \
		nodeName* innerName ## Root; \
		nodeName* innerName ## Tail; \
		UINT32 innerName ## Elements; \
	} name;

#define CreateListItem(name, innerName, nodeName) \
	typedef struct { \
		nodeName* innerName ## Next; \
		nodeName* innerName ## Prev; \
	} name;

/** (doubly) linked list functions **/

#define LListRemoveRoot(list, name) \
	_LListRemoveRoot(list ## Root, list ## Tail, list ## Elements, name ## Next);

#define LListInsertAtEnd(list, name, element) \
	_LListInsertAtEnd(list ## Root, list ## Tail, list ## Elements, element, name ## Next)

#define LListInsertAtFront(list, name, element) \
	_LListInsertAtFront(list ## Root, list ## Tail, list ## Elements, element, name ## Next)


#define DLListInsertAtFront(list, name, element) \
	_DLListInsertAtFront(list ## Root, list ## Tail, list ## Elements, element, name ## Next, name ## Prev)

#define DLListInsertAtEnd(list, name, element) \
	_DLListInsertAtEnd(list ## Root, list ## Tail, list ## Elements, element, name ## Next, name ## Prev)

#define DLListRemoveRoot(list, name) \
	_DLListRemoveRoot(list ## Root, list ## Tail, list ## Elements, name ## Next, name ## Prev);

#define DLListRemoveTail(list, name) \
	_DLListRemoveTail(list ## Root, list ## Tail, list ## Elements, name ## Next, name ## Prev);


#define DLListRemoveNode(list, name, element) \
	_DLListRemoveNode(list ## Root, list ## Tail, list ## Elements, name ## Next, name ## Prev, element);


/** actual implementations **/

#define _LListRemoveRoot(listRoot, listTail, numElements, elNextPtr) \
	listRoot = listRoot->elNextPtr; \
	if (listRoot == NULL) { \
		listTail = NULL; \
	} \
	numElements--;

#define _DLListRemoveRoot(listRoot, listTail, numElements, elNextPtr, elPrevPtr) \
	listRoot = listRoot->elNextPtr; \
	if (listRoot == NULL) { \
		listTail = NULL; \
	} else { \
		listRoot->elPrevPtr = NULL; \
	} \
	numElements--;

#define _DLListRemoveTail(listRoot, listTail, numElements, elNextPtr, elPrevPtr) \
	listTail = listTail->elPrevPtr; \
	if (listTail == NULL) { \
		listRoot = NULL; \
	} else { \
		listTail->elNextPtr = NULL; \
	} \
	numElements--;

#define _DLListRemoveNode(listRoot, listTail, numElements, elNextPtr, elPrevPtr, element) \
	if (element->elPrevPtr != NULL) { \
		element->elPrevPtr->elNextPtr = element->elNextPtr; \
	} \
	if (element->elNextPtr != NULL) { \
		element->elNextPtr->elPrevPtr = element->elPrevPtr; \
	} \
	if (listRoot == element) { \
		listRoot = element->elNextPtr; \
	} \
	if (listTail == element) { \
		listTail = element->elPrevPtr; \
	} \
	numElements--;

#define _LListInsertAtEnd(listRoot, listTail, numElements, element, elNextPtr) \
	element->elNextPtr = NULL; \
	if (listTail == NULL) { \
		listRoot = element; \
	} else { \
		listTail->elNextPtr = element; \
	} \
	listTail = element; \
	numElements++;

#define _LListInsertAtFront(listRoot, listTail, numElements, element, elNextPtr) \
	element->elNextPtr = listRoot; \
	if (listRoot == NULL) { \
		listTail = element; \
	} \
	listRoot = element; \
	numElements++;

#define _DLListInsertAtEnd(listRoot, listTail, numElements, element, elNextPtr, elPrevPtr) \
	element->elNextPtr = NULL; \
	element->elPrevPtr = listTail; \
	if (listTail == NULL) { \
		listRoot = element; \
	} else { \
		listTail->elNextPtr = element; \
	} \
	listTail = element; \
	numElements++;

#define _DLListInsertAtFront(listRoot, listTail, numElements, element, elNextPtr, elPrevPtr) \
	element->elNextPtr = listRoot; \
	element->elPrevPtr = NULL; \
	if (listRoot == NULL) { \
		listTail = element; \
	} else { \
		listRoot->elPrevPtr = element; \
	} \
	listRoot = element; \
	numElements++;

#endif
