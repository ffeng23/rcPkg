#ifndef C_CPP
#define R
#endif
//#define C_CPP
#include <stdlib.h>
#include <stdbool.h>
#include "helper_c.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>

#ifdef R
#include <R.h> // R memory io, in the standard list of system directories
//#include "XDemo.h"  //the user defined header file. it will search the current working directory and the system directories as in "< >"
#include <Rinternals.h>
#endif

//Get index by element (single element)
// assume we are picking the indexes that evaluated according to the compare function passed 
// input : 
//				p, the source array
//				nmem, number of elements
//				size, size of each element 
//
//				the element, assuming a SINGLE element to be compared with 
//               
//				compar, function pointer. the function we used to evaluted with.
//              index , the pointer to the index array to which output will be written to .
//             relation, relation between the element and member of array as a criteria to pick indexes.
//									char, l(<), m(<=)e(=), g(>=),  h(>). , n (!=). For example, relation=e, meaning we will pick 
//									the indexs of the array where the member of array equals to element; relation=l, 
//									we will pick where the memeber of array less than the element. m is [array <=element] 
// Output: integer index as output, index is at least of size nmem;
//
//Note: 
size_t getIndexOfElement(const void* const p, const size_t nmem, const size_t size,	
														const void* const element, 
														int (*compar)(const void *, const void *), 
														size_t* const index, char relation)
{
	size_t n=0;
	short update_flag;
	int compare_result;
	for(size_t i=0;i<nmem;i++)
	{
		update_flag=0; //no writing.
		compare_result=compar((char*)p+i*size, element);
		//compare each element to pick out the good ones
		if(compare_result==0&&(relation=='e'||relation =='m'||relation=='g'))
		{
			update_flag=1;
		}
		
		if(compare_result<0&&(relation =='l'||relation=='m'||relation=='n'))
		{
			update_flag=1;
		}
		
		if(compare_result>0&&(relation =='g'||relation=='h'||relation=='n'))
		{
			update_flag=1;
		}
		if(update_flag==1)
		{
			index[n]=i;
			n++;
		}
	}//end of for loop
	return n;
}

//Get elements by index, 
//It is the outside caller's responsibility to initialize and free the memory
//input : p, the source array 
//			nmem, total number of input source array 
//			size, size of each element by bytes (use sizeof to determine)
//		
//			index, the index array that indicates what elements we want to pick 
//			nindex, the number of indexes of the index array 
//Output: 
//			element, the output array should contains the nindex elements,
//Note: this function might 
//will throw exception if the index is out of range, in this case we will return -1. 
size_t getElementByIndex(const void* const p, const size_t nmem, const size_t size, 
											const size_t* const  index, const size_t nindex,  void* const element)
{
		/*if(nindex==0)
		{
			return 0;
		}*/
		//now do it
		for(int i=0;i<nindex;i++)
		{
			if(index[i]>=nmem)
			{
				printf("******ERROR: request an element out of range..........\b\n");
				return -1; //exception
			}
			memcpy((char*)element+i*size, (char*)p+index[i]*size, size);
		}
		return nindex;
}
//Get elements (size_t) from p (int array) by index, 
//It is the outside caller's responsibility to initialize and free the memory
//input : p, the source array (int array)
//			nmem, total number of input source array 
//			size, size of each element by bytes (use sizeof to determine)
//		
//			index, the index array that indicates what elements we want to pick 
//			nindex, the number of indexes of the index array 
//Output: 
//			element, the output (size_t) array should contains the nindex elements,
//Note: this function might 
//will throw exception if the index is out of range, in this case we will return -1. 
size_t getSizetFromIntByIndex(const int * const p, const size_t nmem,  
											const size_t* const  index, const size_t nindex,  size_t* const element)
{
		/*if(nindex==0)
		{
			return 0;
		}*/
		//now do it
		for(int i=0;i<nindex;i++)
		{
			if(index[i]>=nmem)
			{
#ifdef C_CPP
				printf("******ERROR: request an element out of range..........\b\n");
				return -1; //exception
#endif
#ifdef R
			error("******ERROR: request an element out of range..........\b\n");
#endif
			}
			*(element+i)= *(p+index[i]);
		}
		return nindex;
}

//#include <algorithm.h>
//' input : p, array 
//			nmem, number of member 
//			size, size of each member by bytes
//			compar, compare function that will be used for sorting 
//output :
//			r, array without repeats.
//assuming the return array is initialized by the caller  and has a size of nmem 
// note: This one is a little different from R unique, because it return a sorted array. R doesn't. But 
//			this is fine (?), since in R, the order (relative to original data is changed anyway, since the 
//			repeated entries are remove anyway. 
size_t unique(void * const  p, const size_t nmem, const size_t size, int (*compar)(const void*,const void*), void* r) 
{
	if(nmem==0)
		return 0;
	//we first sort the array
		qsort(p, nmem, size, compar);
	//then we go through 
	size_t n=0; 
		if(r==NULL)
		{
				return -1;
		}
	size_t i=0;
	int crt=0;
	memcpy (r, p, 1* size);
	n++;
	for(	i =1;i<nmem;i++)
	{
		//compare with the previous one 
		crt=compar((char*)p+(i-1)*size, (char*)p+i*size);
		if(crt!=0)
		{
			memcpy((char*)r+n*size, (char*)p+i*size, 1*size);
			//r[n]=p[i];
			n++;
		}
	}
	return n;
}
//'note we expect const char* as input 
int compareStr(const void *p1, const void *p2)
{
    /* The actual arguments to this function are "pointers to
       pointers to char", but strcmp(3) arguments are "pointers
       to char", hence the following cast plus dereference */
	//printf("#########calling inside comparestr:\n");
//	printf("\t###p1:%c ; p2:%c \n", (*(const char **)p1)[0], (*(const char**)p2)[0]);
   return strcmp(*((const char  ** ) p1) , *((const char **   ) p2));
}

//here, even this is called compare int, it works for both int, size_t, and double, float
//a potential problem, we might overflow the return value, say double ->int, size ->int
 int 
compareInt(const void *p1, const void *p2)
{
    
   return (* (const int *) p1) - (*(const int*) p2);
}

//here, even this is called compare int, it works for both int, size_t, and double, float
//a potential problem, we might overflow the return value, say double ->int, size ->int
int 
compareDouble(const void *p1, const void *p2)
{
    if((* (const double*) p1) - (*(const double*) p2)>1e-105)
		return 1;
	if((* (const double*) p1) - (*(const double*) p2)<-1e-105)
		return -1;
	//case of zero 
   return 0;
}

int 
compareFloat(const void *p1, const void *p2)
{
    if((* (const float *) p1) - (*(const float*) p2)>1e-105)
	{
		return 1;
	}
	if((* (const float *) p1) - (*(const float*) p2)<-1e-105)
	{
		return -1;
	}
   return 0;
}
int 
compareSizet(const void *p1, const void *p2)
{
	//printf("------calling inside compare size t:----\n");
/*#ifdef R
	Rprintf(" \t p1: %zu; p2: %zu;\n",* (const size_t *) p1 , (*(const size_t*) p2));
#endif
*/
    if((* (const size_t *) p1) >(*(const size_t*) p2))
	{
		//printf("return 1\n");
		return 1;
	}
	if((* (const size_t *) p1) < (*(const size_t*) p2))
	{
		//printf("return -1\n");
		return -1;
	}
	//printf ("return 0\n");
   return 0;
}

//z is a pointer to an int array, it is caller's responsibility to malloc the array, we assume it is at least 
// of a size of nx, 
//   
size_t  setdiff_c(const void* const x, const size_t nx,  //first set pointer and # of records
											const void* const y, const size_t ny,  //2nd set pointer and #
											const size_t size,  //size of each element 
											int (*compar)(const void *, const void *), 
											void* const z  // output 
											)
//size_t  setdiff_c_int(const size_t * const x, const size_t nx, const size_t * const y, const size_t ny, size_t* z)
{
	//size_t nz=0;
	if(nx==0)
	{
		return 0;
	}
#ifdef C_CPP	
	void* xc=malloc(nx*size);
	
	void* yc=malloc(ny*size);
#endif
#ifdef R	
	void* xc=R_alloc(nx, size);
	
	void* yc=R_alloc(ny, size);
#endif    
/*
#ifdef R
	Rprintf("------INSIDE setdiff_c------: nx: %zu; ny:%zu; size: %zu\n", nx, ny, size);
		Rprintf("------INSIDE setdiff_c------pointers: xc: %p; yc:%p; xc[0] :%zu\n", xc, yc, ((size_t*)xc)[0] );
	R_FlushConsole();
#endif
*/
	//*p_z=(int*)malloc(nx*sizeof(int)); //prepare the output array with a size of the input array , will resize in the end 
	
	//copy over to do sorting 
	memcpy(xc, x, nx*size);
	memcpy(yc, y, ny*size);
#ifdef DEBUG
#ifdef R
		Rprintf("------INSIDE setdiff_c---11111after memcpy---pointers: xc: %p; yc:%p; \n", xc, yc);
/*Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[0] :%zu; yc[0] :%zu\n", xc, yc, ((size_t*)xc)[0], ((size_t*)yc)[0]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[1] :%zu; yc[1] :%zu\n", xc, yc, ((size_t*)xc)[1], ((size_t*)yc)[1]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[1] :%zu; yc[1] :%zu\n", xc, yc, ((size_t*)xc)[2], ((size_t*)yc)[2]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[1] :%zu; yc[1] :%zu\n", xc, yc, ((size_t*)xc)[3], ((size_t*)yc)[3]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[1] :%zu; yc[1] :%zu\n", xc, yc, ((size_t*)xc)[4], ((size_t*)yc)[4]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[1] :%zu; yc[1] :%zu\n", xc, yc, ((size_t*)xc)[5], ((size_t*)yc)[5]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[6] :%zu; yc[6] :%zu\n", xc, yc, ((size_t*)xc)[6], ((size_t*)yc)[6]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[7] :%zu; xc[8] :%zu\n", xc, yc, ((size_t*)xc)[7], ((size_t*)xc)[8]);
Rprintf("------INSIDE setdiff_c----after memcpy--pointers: xc: %p; yc:%p; xc[9] :%zu; yc[7] :%zu\n", xc, yc, ((size_t*)xc)[9], ((size_t*)yc)[7]);
*/
	R_FlushConsole();
#endif	
#endif
	//sort the input arrays in order to do the quick checking
	qsort(xc, nx, size, compar);
	qsort(yc, ny, size, compar);
/*#ifdef R	
	Rprintf("------INSIDE setdiff_c-222-----: nx: %zu; ny:%zu; size: %zu\n", nx, ny, size);
	//Rprintf("------INSIDE setdiff_c------: xc[0]: %zu; yc:%zu; size: %zu\n", ((size_t*)xc)[0], ((size_t*)yc)[0], size);
	size_t m=0;
	for(m=0;m<nx;m++)
	{
		Rprintf("@@@printing xc [%zu] pointer: %p  ; content :%zu\n", m, xc+m*size, ((size_t*)xc)[m]);
	}
	Rprintf("Done here\n");
	R_FlushConsole();
#endif
*/
	//going through the array x and check for difference in y
	//
	size_t index_x=0;
	size_t index_y=0;
	size_t index_z=0;
	for(;;) //loop through
	{
		if(index_x >=nx )
		{
			break;//done
		}
		
		//check for duplicate, only do it on x aray 
		if(index_x>0&&compar((char*)xc+index_x*size, (char*)xc+(index_x-1)*size)==0)
		{ //find a duplicates, skip to next one
			index_x++;
			continue;
		}
		
		if(index_y>=ny) //in this case, we run out of y, so copy over all xs and then we are done. 
		{
			memcpy((char*)z+index_z*size, (char*)xc+index_x*size, size);
			index_z++;
			index_x++;	
			continue;
		}
		
		//we are here, we need to compare 
		if(compar((char*)xc+index_x*size, (char*)yc+index_y*size)==0)
		{
			index_x++;
			index_y++;
		} 
		else  //unequal case, 
		{
			if(compar((char*)xc+index_x*size, (char*)yc+index_y*size)<0)
			{//this must be a difference 
				memcpy((char*)z+index_z*size, (char*)xc+index_x*size, size);
				
				index_z++;
				index_x++;
			}
			else //in this case, xc>yc, xc could still not be a different element , need to increment y for checking 
			{
				index_y++;
				//if(index_y>=ny) //no more elements, so xc now is a different element, need to write it down
				//{
				//	z[index_z]=xc[index_x];
				//	index_z++;
				//}
			}
		}
		
	}//end of for loop for checking
#ifdef DEBUG
#ifdef R	
	Rprintf("Done here\n");
	R_FlushConsole();
#endif
#endif

#ifdef C_CPP		
	free(xc);
	free(yc);
#endif
	return index_z;
}