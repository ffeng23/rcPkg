#ifndef CLONAL_DIVERSITY_H
#define CLONAL_DIVERSITY_H
#include <stddef.h>

//this is more about mutations plus discontinued section and length 
struct sequence{
	const char* readID; //READ ID one string
	size_t numMu; //# of muttions, could be zero
	const char** types; // 1D char array of size numMu, point to strings indication substitution or insertion/deletion
	size_t* positions;//1D int array, position of each mutation
	size_t xVBase; // scalar, number VBase
	size_t totalVBase; //scalar, total nuber of VBases 
    size_t discPositionStart; //scale, the end position of discontinued section, -1 if no discPosition
    size_t discPositionEnd; //scalar, the starting postion of discontinued section, -1 if no discPositon 
};

double intraClonalDiversity( struct sequence* seq, 
														size_t nmem,  //# of sequenecs fo 
														double indel_penalty   //penalty or score for indel
							);

// c function to calcuate the actual sequence length based on discontinued section length 
//this is the one assume 1)ASSUME seqLength is the SHORTER length between seq1 and seq2 and we don't have to know whether it is seq1 or seq2.
//2) ASSUME the seq1 disc section (discPositionStart1) is before seq2 disc section.
//3) both discontinued sections are present                            
//
//check the frLib R code "getSeqLength_inclusive_seq1Disc_ahead " for details
//in c code, we use -1 to indicate not available (NA) for discPositionStart and end 
//
size_t getSeqLength_inclusive_seq1Disc_ahead (
        size_t seqLength,
        size_t discPositionStart1,
        size_t discPositionEnd1,
        size_t discPositionStart2,
        size_t discPositionEnd2
    );
							
size_t    getSeqLength_disc1_existShort(
        size_t seqLength,
        size_t totalLength,
        size_t discPositionStart1,
        size_t discPositionEnd1,
        size_t discPositionStart2,
        size_t discPositionEnd2
    );
    
    //we process the case where seq 1 is shorter, 
// and no discountinue section. 
    size_t getSeqLength_disc1_NAShort(
      size_t  seqLength,
      size_t  totalLength,
      size_t  discPositionStart2,
      size_t  discPositionEnd2
    );
#endif