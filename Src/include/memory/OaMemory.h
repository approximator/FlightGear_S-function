/*!
 * @file OaMemory.h
 *
 * @brief Functions for working with memory.
 *
 * @author Oleksii Aliakin (alex@nls.la)
 * @created Nov 14, 2014
 */

#ifndef _OAMEMORY_H_
#define _OAMEMORY_H_

void* oa_malloc(int size);

void oa_free(void* pMem);

#endif /* _OAMEMORY_H_ */
