
#ifndef MEMORY_TYPES_H_
#define MEMORY_TYPES_H_

#ifdef RAYDEBUG
	#define AssertLengthMsg(length, start, msg) 	if ( ((start + length) & 0x00000FFF) != 0x00000000) { \
	KernelThrowException(msg, EXC_ILLEGAL_STATE, length, 0, 0); \
						  }
	#define AssertLength(length, start) AssertLengthMsg(length, start, "Unaligned length in "  __AT)
#else
	#define AssertLength(length, start)
#endif

typedef struct {
	UINT32 length;
	UINT32 pages;
	UINT32 paddStartPages;
	UINT32 paddEndPages;
} ALLOCATION_INFO_USER;

typedef struct {
	UINT32 length;
	UINT32 pages;
	UINT32 usablePages;
	UINT32 addressStart;
	UINT32 usableAddressStart;
	UINT32 paddStartPages;
	UINT32 paddEndPages;
} ALLOCATION_INFO;



#endif /* MEMORY_TYPES_H_ */
