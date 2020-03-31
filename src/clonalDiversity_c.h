#ifndef CLONAL_DIVERSITY_H
#define CLONAL_DIVERSITY_H
#include <stddef.h>
struct sequence{
	const char* readID; //READ ID one string
	size_t numMu; //# of muttions, could be zero
	const char** types; // 1D char array of size numMu, point to strings indication substitution or insertion/deletion
	size_t* positions;//1D int array, position of each mutation
	size_t xVBase; // scalar, number VBase
	size_t totalVBase; //scalar, total nuber of VBases 
};

double intraClonalDiversity( struct sequence* seq, 
														size_t nmem,  //# of sequenecs fo 
														double indel_penalty   //penalty or score for indel
							);
							

#endif