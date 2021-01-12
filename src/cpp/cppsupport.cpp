/*
 * cppsupport.cpp
 *
 *  Created on: 04.01.2010
 *      Author: stefan
 */

extern "C" {
#include <typedefs.h>
#include <memory/memory.h>
#include <memory/paging.h>
#include <memory/stackmem.h>
#include <exceptions.h>
#include <video/video_char.h>

}

#include <cpp/cppsupport.h>
#include <cpp/specialmem_int.h>


//#include <reent.h>
struct _reent {
	int bla;
};
static struct _reent global_reent;
struct _reent *_impure_ptr = &global_reent;


// Special global struct
_NoMemProtection NoMemProtection;



void * stderr;

static BOOL usePagedMem = FALSE;

extern "C" {

extern void __register_frame(const void *);
extern void __exit(UINT32 reason);
extern void __register_frame_info (const void *, struct object *);
extern void *__deregister_frame_info (const void *);

void __cxa_pure_virtual();
int __cxa_atexit(void (*f)(void *), void *p, void *d);
void __cxa_finalize(void *d);
void *__dso_handle; // only the address of this symbol is taken by gcc

// local functions
static void __do_global_ctors_aux();
static void __do_global_dtors_aux();
static void frame_dummy (void);



struct object
{
	void (*f)(void*);
	void *p;
	void *d;
} object[32];
unsigned int iObject = 0;

void CPlusPlusInit()
{
	//__register_frame(&__EH_FRAME_BEGIN__);
	frame_dummy();
	__do_global_ctors_aux();

}

void CPlusPlusExit()
{
	__do_global_dtors_aux();
}

MEMPROFILE static POINTER InternalMalloc(UINT32 size);
MEMPROFILE static void InternalFree(POINTER p);

static POINTER InternalMalloc(UINT32 size) {
	if (usePagedMem) {
		return KMalloc(size, MEMTYPE_DATA);
	} else {
		return StackMemAlloc(size);
	}
}

static void InternalFree(POINTER p) {
	if (usePagedMem) {
		KFree(p, MEMTYPE_DATA, TRUE);
	} else {
		KernelThrowException("Cannot release stack mem", EXC_OUT_OF_MEMORY, (UINT32)p, FALSE, (INT_REG*)NULL);
	}
}

int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
	if (iObject >= 32) return -1;
	object[iObject].f = f;
	object[iObject].p = p;
	object[iObject].d = d;
	++iObject;
	return 0;
}

/* This currently destroys all objects */
void __cxa_finalize(UNUSED void *d)
{
	unsigned int i = iObject;
	for (; i > 0; --i)
	{
		--iObject;
		object[iObject].f(object[iObject].p);
	}
}

static void frame_dummy (void)
{

	static struct object object;
	__register_frame_info (&__EH_FRAME_BEGIN, &object);
}

static void __do_global_ctors_aux()
{
	unsigned long nptrs = (unsigned long) __CTOR_LIST[0];
	unsigned i;
	if (nptrs == (unsigned long)-1) {
		for (nptrs = 0; __CTOR_LIST[nptrs + 1] != 0; nptrs++);
		for (i = nptrs; i >= 1; i--) {
			__CTOR_LIST[i] ();
		}
	}
}

static void __do_global_dtors_aux (void)
{
	static func_ptr *p = __DTOR_LIST + 1;
	func_ptr f;

	static BOOL completed = FALSE;

	if (__builtin_expect (completed, FALSE))
		return;

	__cxa_finalize (__dso_handle);

	while ((f = *p))
	{
		p++;
		f ();
	}

	__deregister_frame_info (__EH_FRAME_BEGIN);

	completed = TRUE;
}


	void CPlusPlusPagedMemAvailable() {
		usePagedMem = TRUE;
	}


	void __cxa_pure_virtual()
	{
		// Do nothing or print an error message.
	}

	int sprintf (UNUSED char * str, UNUSED const char * format, ... )
	{
		return -1;
	}
}


void *operator new(size_t size)
{
	return InternalMalloc(size);
}


void *operator new[](size_t size)
{
	return InternalMalloc(size);
}

void operator delete(void *p)
{
	InternalFree(p);
}

void operator delete[](void *p)
{
	InternalFree(p);
}

POINTER operator new(size_t size, UNUSED const _NoMemProtection& mem)
{
	return KMallocUnProtected(size, MEMTYPE_DATA);
}

POINTER operator new[](size_t size, UNUSED const _NoMemProtection& mem)
{
	return KMallocUnProtected(size, MEMTYPE_DATA);
}

void operator delete (POINTER address, UNUSED const _NoMemProtection& mem)
{
	KFree(address, MEMTYPE_DATA, TRUE);
}

void operator delete[] (POINTER address, UNUSED const _NoMemProtection& mem)
{
	KFree(address, MEMTYPE_DATA, TRUE);
}


extern "C" {

	void __sprintf_chk() {
		KernelThrowException("sprintf_chk called", EXC_ILLEGAL_STATE, 0, FALSE, (INT_REG*)NULL);
	}

	void __stack_chk_fail_local() {
		KernelThrowException("Stack smashing protection", EXC_ILLEGAL_STATE, 0, FALSE, (INT_REG*)NULL);
	}

	void abort(void) {
		for(;;);
	}

	void * realloc (UNUSED void * ptr, UNUSED size_t size ) {
		KernelThrowException("realloc used", 0, 0, FALSE, (INT_REG*)NULL);
		return NULL;
	}

	void * malloc (UNUSED size_t size ) {
		return InternalMalloc(size);
	}

	void free (UNUSED void * ptr ) {
		InternalFree(ptr);
	}

	int fputs (UNUSED const char * str, UNUSED void * stream ) {
		return 0;
	}

	int fputc (UNUSED int character, UNUSED void * stream ) {
		return 0;
	}

	size_t fwrite (UNUSED const void * ptr, UNUSED size_t size, UNUSED size_t count, UNUSED void * stream ) {
		return 0;
	}

	int dl_iterate_phdr() {
		return -1;
	}

}
