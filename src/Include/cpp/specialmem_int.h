/**
 * @file specialmem_int.h
 *
 * @date Aug 13, 2010
 * @author stefan
 * @brief (Summary)
 */

#ifndef SPECIALMEM_INT_H_
#define SPECIALMEM_INT_H_

typedef typeof (sizeof(int)) size_t;

struct _NoMemProtection
{

};
POINTER operator new(size_t size, const _NoMemProtection& mem);
POINTER operator new[](size_t size, const _NoMemProtection& mem);
void operator delete (POINTER address, const _NoMemProtection& mem);
void operator delete[] (POINTER address, const _NoMemProtection& mem);

#endif /* SPECIALMEM_INT_H_ */
