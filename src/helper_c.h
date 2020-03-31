#ifndef HELPER_C_H
#define HELPER_C_H

#include <stddef.h>
//'in here we define the headers for helper c functions, that might be called by other c functions
//' note these are not going to be called R (?)
//'call to do set difference following R style (? setdiff in R to see the description)
//' asymetric difference  
//' passing in two arrays
//'returning the elements that is in x, but not in y 
//size_t  setdiff_c_int(const int * const x, const size_t nx, const int * const y, const size_t ny, int* z);
//size_t  setdiff_c_int(const size_t * const x, const size_t nx, const size_t * const y, const size_t ny, size_t* z);
size_t  setdiff_c(const void* const x, const size_t nx,  //first set pointer and # of records
											const void* const y, const size_t ny,  //2nd set pointer and #
											const size_t size,  //size of each element 
											int (*compar)(const void *, const void *), 
											void* const z  // output 
											);
int 
compareSizet(const void *p1, const void *p2);

int 
compareDouble(const void *p1, const void *p2);

int 
compareFloat(const void *p1, const void *p2);

int
compareInt(const void *p1, const void *p2);

int compareStr(const void *p1, const void *p2);

size_t unique(void * const  p, const size_t nmem, const size_t size, int (*compar)(const void*,const void*), void* r) ;



size_t getElementByIndex(const void* const p, const size_t nmem, const size_t size, 
											const size_t* const  index, const size_t nindex,  void* const element);

size_t getIndexOfElement(const void* const p, const size_t nmem, const size_t size,
										const void* const element,	
										int (*compar)(const void *, const void *),//, ">="),  
										size_t* const index , char relation);
										
size_t getSizetFromIntByIndex(const int * const p, const size_t nmem,  
											const size_t* const  index, const size_t nindex,  size_t* const element);

#endif