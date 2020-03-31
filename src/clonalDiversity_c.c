#ifndef C_CPP
#define R
#endif

#include "helper_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "clonalDiversity_c.h"
#include <stddef.h>
#ifdef R
#include <R.h> // R memory io, in the standard list of system directories
//#include "XDemo.h"  //the user defined header file. it will search the current working directory and the system directories as in "< >"
#include <Rinternals.h>
#endif

#ifdef R 

//'it is outside caller's responsibility to make sure the parameters are passed in as correct type. (check for types)
// return a matrix/vector holding the intraClonalDiversities for each sample  (paired list ???) 
SEXP intraClonalDiversities_c(
			SEXP clones_sampleNames, //clone summary, sample name, string char** 
			SEXP clones_cloneID, //clone summary, clones ids, int *
			SEXP clones_xMembers, //clone summary, number of members, int *
			
			SEXP cloneAssigns_sampleNames, //clone assignments for sequences, sample names, char**
			SEXP cloneAssigns_cloneID, //clone assignments, clone Ids, int*
			SEXP cloneAssigns_ReadID, //clone assignment, read ID, char**
			
			//now in mutations table we don't include the sampleNames, since READID is good to identify
			SEXP mutations_ReadID, //seq muations VRG, read ID, char** 
			SEXP mutations_Type,   //   Type , string substitution, char**
			SEXP mutations_Position,  //Poisiotin, int* 
			
			//now in Ig_RecSums table we don't include the sampleNames, since READID (UID) is good to identify
			SEXP Ig_RecSums_UID, //Ig Recsum for VBases, UID, string, char**  
			SEXP Ig_RecSums_XVBase, //             XVBase, int* 
			
			//now in vlengths table we don't include the sampleNames, since READID is good to identify
			SEXP vlengths_ReadID,  //total lengths of v, ReadID, char**  
			SEXP vlengths_totalVBase, //         int*, totalVBase
			
			SEXP indel_penalty //for penalty of indel. a numeric scalar in R and double value in C
		)
	{
		
		//preparing/converting the inputs
		double indel_penalty_c =asReal(indel_penalty);

			//make a copy of R input data <-----NO, we don't have to do this since we don't modify the input
			//we only need to make c pointers to the object 
			// this will be easier to manipulate  and pass around
		//clones summary
		size_t lenClones=xlength(clones_sampleNames);  //all other members of clones are equally long
		
		//obtaining the string arrays from R sample 
		const char** c_clones_SampleNames=(const char**)R_alloc(lenClones, sizeof(char*));
		int* c_clone_cloneID=INTEGER(clones_cloneID); //R_alloc(lenClones, sizeof(int));
		int* c_clones_xMembers=INTEGER(clones_xMembers);
		if(c_clones_SampleNames==NULL) //failed???
		{
			error("error in allocating memories for clone data, please check for error");
			return R_NilValue;
		}
		//now make pointers to each Rstring into c data
		size_t i=0;
		for(i=0;i<lenClones;i++)
		{
			c_clones_SampleNames[i]=CHAR(STRING_ELT(clones_sampleNames,i));
		}
		
		//cloneAssigns
		size_t lenCloneAssigns = xlength(cloneAssigns_sampleNames);
		const char** c_cloneAssigns_sampleNames = (const char**) R_alloc(lenCloneAssigns, sizeof(char*));
		int* c_cloneAssigns_cloneID = INTEGER(cloneAssigns_cloneID);
		const char** c_cloneAssigns_ReadID = (const char**)R_alloc(lenCloneAssigns, sizeof(char*));
		if(c_cloneAssigns_sampleNames==NULL||c_cloneAssigns_ReadID==NULL) //failed???
		{
			error("error in allocating memories for clone assignment data, please check for error");
			return R_NilValue;
		}
		
		//now make pointers to each Rstring into c data 
		for(i=0;i<lenCloneAssigns;i++)
		{
			c_cloneAssigns_sampleNames[i]=CHAR(STRING_ELT(cloneAssigns_sampleNames,i));
			c_cloneAssigns_ReadID[i]=CHAR(STRING_ELT(cloneAssigns_ReadID,i));
		}

		//mutations 
		size_t lenMutations=xlength(mutations_ReadID);
		const char** c_mutations_ReadID=(const char**)R_alloc(lenMutations, sizeof(char*));
		const char**  c_mutations_Type=(const char**)R_alloc(lenMutations, sizeof(char*));
		int* c_mutations_Position=INTEGER(mutations_Position);
		if(c_mutations_ReadID==NULL||c_mutations_Type==NULL) //failed???
		{
			Rprintf("error in allocating memories for mutations data, please check for error");
			return R_NilValue;
		}
		//now make pointers to each Rstring into c data 
		for(i=0;i<lenMutations;i++)
		{
			c_mutations_ReadID[i]=CHAR(STRING_ELT(mutations_ReadID,i));
			c_mutations_Type[i]=CHAR(STRING_ELT(mutations_Type,i));
		}

		//Ig_RecSums
		size_t lenRecSums=xlength(Ig_RecSums_UID);
		const char** c_Ig_RecSums_UID=(const char**)R_alloc(lenRecSums, sizeof(char*));
		int* c_Ig_RecSums_XVBase=INTEGER(Ig_RecSums_XVBase); 
		if(c_Ig_RecSums_UID==NULL) //failed???
		{
			Rprintf("error in allocating memories for Ig_RecSums data, please check for error");
			return R_NilValue;
		}
		//now make pointers to each Rstring into c data 
		for(i=0;i<lenRecSums;i++)
		{
			c_Ig_RecSums_UID[i]=CHAR(STRING_ELT(Ig_RecSums_UID,i));
		}
		
		//vlengths
		size_t lenVlengths=xlength(vlengths_ReadID);
		const char**  c_vlengths_ReadID= (const char**)R_alloc(lenVlengths, sizeof(char*));   
		int* c_vlengths_totalVBase=INTEGER(vlengths_totalVBase); 
		if(c_vlengths_ReadID==NULL) //failed???
		{
			Rprintf("error in allocating memories for Ig_RecSums data, please check for error");
			return R_NilValue;
		}
		//now make pointers to each Rstring into c data 
		for(i=0;i<lenVlengths;i++)
		{
			c_vlengths_ReadID[i]=CHAR(STRING_ELT(vlengths_ReadID,i));
		}	

		//now we are done with data preparation 
		//now go through each samples
        const char** unique_sampleNames=(const char**)R_alloc(lenClones, sizeof(char*));		
		size_t lenSampleNames=unique(c_clones_SampleNames, lenClones, sizeof(char*), compareStr, unique_sampleNames);
		//idis<-NULL;

//allocate the memory for the temporary holder in each round 
		size_t* clone_index_sample=(size_t*)R_alloc(lenClones, sizeof(size_t));
		size_t len_clone_index_sample;
		int * clone_ID_sample=(int*)R_alloc(lenClones, sizeof(int));
		size_t len_clone_ID_sample;
		int* clone_xMembers_sample=(int*)R_alloc(lenClones, sizeof(int));
		size_t len_clone_xMembers_sample;
		size_t* cloneAssigns_index_sample=(size_t*)R_alloc(lenCloneAssigns, sizeof(size_t));
		size_t len_cloneAssigns_index_sample;
		int* cloneAssigns_cloneID_sample=(int*)R_alloc(lenCloneAssigns, sizeof(int));
		size_t len_cloneAssigns_cloneID_sample;
		char** cloneAssigns_ReadID_sample=(char**)R_alloc(lenCloneAssigns, sizeof(char*));
		size_t len_cloneAssigns_ReadID_sample;
		
		//start get larger clones 
		int sizeOfClone=3;
		size_t len_clone_large =getIndexOfElement(c_clones_xMembers, lenClones, sizeof(int), &sizeOfClone,
											compareInt, clone_index_sample, 'g');  //here we only make use of th elen_clone_large but not clone_index_sample
									//this is something to do in the future. move to do this large clone here. <----------TO DO.
		//prepare the output data frame 
		//desing for the output data frame 
		//       sample clone idi XMembers
		size_t numOfCols=4 ; //
		SEXP idis =PROTECT(allocVector(VECSXP, numOfCols));
		SEXP cls =PROTECT(allocVector(STRSXP, 1)); // class attribute
        SET_STRING_ELT(cls, 0, mkChar("data.frame"));
        classgets(idis, cls);

		SEXP nam = PROTECT( allocVector(STRSXP, 4)); // names attribute (column names)
		SET_STRING_ELT(nam, 0, mkChar("sampleName"));
		SET_STRING_ELT(nam, 1, mkChar("cloneID"));
		SET_STRING_ELT(nam, 2, mkChar("idi"));
		SET_STRING_ELT(nam, 3, mkChar("xMembers"));
        namesgets(idis, nam);
		

			SEXP sn=PROTECT(allocVector(STRSXP, len_clone_large));
			SEXP cid=PROTECT(allocVector(INTSXP, len_clone_large));
			SEXP ind =PROTECT(allocVector(REALSXP, len_clone_large));
			SEXP xm=PROTECT(allocVector(INTSXP, len_clone_large));

		//allocate memory for "struct sequence" for each read (by readID)
		struct sequence* seq=(struct sequence*) R_alloc(lenCloneAssigns, sizeof(struct sequence));
		//struct sequence* seq=(struct sequence*) R_alloc(len_cloneAssigns_ReadID_sample_j, sizeof(sequence));
		//this allocation of sequence could be done inside the loop of clone ID at **chunk of 1**
		//but we allocate here in order to save time by not repeatedly allocating the memories
		//the tradeoff is that we need to allocate a much larger space, literally the space that can holder
		//all the ReadID records for this purpose. 
		
		//allocate the space for make each sequence in the loop k inside
		//could also do this in **chunk 1**, but there is no benefit of doing that, since we need to 
		//allocate a large space anyway, a space that can hold all the records in the mutation table.
		const char** mutations_Type_k=(const char**)R_alloc(lenMutations, sizeof(char*));
		size_t* mutations_Position_k=(size_t*)R_alloc(lenMutations, sizeof(size_t));
		size_t* mutations_index_k=(size_t*)R_alloc(lenMutations, sizeof(size_t));
		size_t len_mutations_index_k=0;
		size_t len_mutations_Position_k=0;
		size_t len_mutations_Type_k=0;
		
		size_t* Ig_RecSums_index_k=(size_t*)R_alloc(lenMutations, sizeof(size_t));
		//size_t Ig_RecSums_XVBase_k=0;//(size_t*)R_alloc(lenMutations, sizeof(size_t));
		size_t len_Ig_RecSums_index_k=0;
		//size_t len_Ig_RecSums_XVBase_k=0;
		
		size_t* vlengths_index_k=(size_t*)R_alloc(lenMutations, sizeof(size_t));
		//size_t vlengths_totalVBase_k=0;//(size_t*)R_alloc(lenMutations, sizeof(size_t));
		size_t len_vlengths_index_k=0;
		//size_t len_vlengths_totalVBase_k=0;

		
		size_t  j=0;
		size_t index=0; //used doing counting of how mang total outputs
		for(i =0;i<lenSampleNames;i++)  //looping through each samples
		{
			Rprintf("\t**doing sample %s......\n", unique_sampleNames[i]);
			R_FlushConsole();
			
			//Get all the information for this sample, cloneIDs and xMembers,  cloneAssigns cloneID and readID, 
			//mutation ReadID and type and position    (in mutation table we did not include sampleName, since READID is good
			//Ig_RecSums UID and xvbase (no sampleName, see above)
			//vlengths ReadID and xvbase (no sampleNmae, see above)
			
			//--first -- Get clone sample index with the name
Rprintf(" before calling any showing lenClones:%d; c_clones_SampleNames: %s;\n", lenClones, c_clones_SampleNames[0]);			
			len_clone_index_sample =getIndexOfElement(c_clones_SampleNames, lenClones, sizeof(char*), unique_sampleNames+i,
											compareStr, clone_index_sample,'e');
			Rprintf("\t**doing 1.1......len_clone_index_sample:%d\n", len_clone_index_sample);
			R_FlushConsole();
			//clone ID							
			 len_clone_ID_sample=getElementByIndex(c_clone_cloneID, lenClones, sizeof(int),
														clone_index_sample, len_clone_index_sample, clone_ID_sample);
	Rprintf("\t**doing 1.2......len_clone_ID_sample:%d\n", len_clone_ID_sample);
			R_FlushConsole();
			 len_clone_xMembers_sample=getElementByIndex(c_clones_xMembers, lenClones, sizeof(int),
														clone_index_sample, len_clone_index_sample, clone_xMembers_sample);
			if(len_clone_ID_sample!=len_clone_index_sample||len_clone_xMembers_sample!=len_clone_index_sample)
			{
				Rprintf("********WARNING: the number of obtain records in clone table are not what we expect! Please check.......");
			}		
			Rprintf("\t**doing 2......len_clone_xMembers_sample:%d\n", len_clone_xMembers_sample);
			R_FlushConsole();
			//----Second, do the cloneAssigns, sampleName, cloneID and readID			
			 len_cloneAssigns_index_sample=getIndexOfElement(c_cloneAssigns_sampleNames, lenCloneAssigns, sizeof(char*), unique_sampleNames+i,
											compareStr, cloneAssigns_index_sample, 'e');
			Rprintf("\t**doing 2.0......len_cloneAssigns_index_sample:%d\n", len_cloneAssigns_index_sample);
			//now get sample specific cloneID and readID 
			len_cloneAssigns_cloneID_sample=getElementByIndex(c_cloneAssigns_cloneID , lenCloneAssigns, sizeof(int),
														cloneAssigns_index_sample, len_cloneAssigns_index_sample, cloneAssigns_cloneID_sample);
			Rprintf("\t**doing 2.01......len_cloneAssigns_cloneID_sample:%d\n", len_cloneAssigns_cloneID_sample);
			len_cloneAssigns_ReadID_sample=getElementByIndex(c_cloneAssigns_ReadID , lenCloneAssigns, sizeof(char**),
														cloneAssigns_index_sample, len_cloneAssigns_index_sample, cloneAssigns_ReadID_sample);
			Rprintf("\t**doing 2.02......len_cloneAssigns_ReadID_sample:%d\n", len_cloneAssigns_ReadID_sample);											
			if(len_cloneAssigns_cloneID_sample!=len_cloneAssigns_index_sample||len_cloneAssigns_ReadID_sample!=len_cloneAssigns_index_sample)
			{
				Rprintf("********WARNING: the number of obtain records in clone assign table are not what we expect! Please check.......");
			}		
			Rprintf("\t**doing 3......\n");
			R_FlushConsole();
			//clones.sample<-clones[clones$sampleName==s,]
			//clone.assigns.sample<-clone.assigns[clone.assigns$sampleName==s,]
		   //	mutations.sample<-mutations[mutations$sampleName==s,]
			//Ig.RecSums.sample<-Ig.RecSums[Ig.RecSums$sampleName==s,]
			//vlengths.sample<-vlengths[vlengths$sampleName==s,]
			
			//get bigger clones in this sample
			//int sizeOfClone=3;
			size_t len_clone_index_sample_large =getIndexOfElement(clone_xMembers_sample, len_clone_index_sample, sizeof(int), &sizeOfClone,
											compareInt, clone_index_sample, 'g');
			Rprintf("\t**doing 3.1......len_clone_index_sample_large:%d\n", len_clone_index_sample_large);
			R_FlushConsole();
			//clone ID
			//clone_ID_sample=R_alloc(len_clone_index_sample, sizeof(int));	
			int* clone_ID_sample_large= (int*)R_alloc(len_clone_index_sample_large, sizeof(int));
			int* clone_xMembers_sample_large=(int*)R_alloc(len_clone_index_sample_large, sizeof(int));
			len_clone_ID_sample =getElementByIndex(clone_ID_sample, len_clone_index_sample, sizeof(int),
														clone_index_sample, len_clone_index_sample_large, clone_ID_sample_large);
			len_clone_xMembers_sample =getElementByIndex(clone_xMembers_sample, len_clone_index_sample, sizeof(int),
														clone_index_sample, len_clone_index_sample_large, clone_xMembers_sample_large);
			Rprintf("\t**doing 3.2......len_clone_ID_sample(large):%d\n", len_clone_ID_sample);
			R_FlushConsole();
            if(len_clone_ID_sample!=len_clone_index_sample_large)
			{
				Rprintf("********WARNING: the number of obtain records in clone large data are not what we expect! Please check.......");
			}
			if(len_clone_ID_sample!=len_clone_xMembers_sample)
			{
				error("********ERROR: the number of obtain records in clone large data are not of same size as xMembers! Please check.......");
				return R_NilValue;
			}
			//clones.sample<-clones.sample[clones.sample$X.Members>1,]
			//idis.sample<-data.frame(clones.sample$CloneID,sampleName=s, idi=rep(0, length(clones.sample$CloneID)),  XMembers=0, cloneSize=0);
			//allocate the memory space for using in the loop j
			char** cloneAssigns_ReadID_sample_j=(char**)R_alloc(len_cloneAssigns_ReadID_sample, sizeof(char*));
			size_t * cloneAssigns_index_sample_j=(size_t*) R_alloc(len_cloneAssigns_ReadID_sample,sizeof(size_t));
Rprintf("\t**doing 4......\n");
			R_FlushConsole();
			j=0;
			
			for(j=0;j<len_clone_ID_sample;j++)  //looping through each cloneID within each sample
			{
				
				int XMembers_j=clone_xMembers_sample_large[j];
				int cloneID_j=clone_ID_sample_large[j];
				
				//Set up for output
				SET_STRING_ELT(sn, index, mkChar(unique_sampleNames[i]));
				Rprintf("\t**doing 4.2......clone_ID_j:%d: XMembers_j:%d.\n", clone_ID_sample_large[j], XMembers_j);
			    
			R_FlushConsole();
			INTEGER(cid)[index]=cloneID_j;
//				SET_VECTOR_ELT(cid, index, ScalarInteger(cloneID_j));
				Rprintf("\t**doing 4.3......cloneID_j\n");
			R_FlushConsole();
			INTEGER(xm)[index]=XMembers_j;
//				SET_VECTOR_ELT(xm, index, ScalarInteger(XMembers_j));
				Rprintf("\t**doing 4.4......index:%d\n",index);
			R_FlushConsole();
			
			Rprintf("-----sn: %s;    cid : %d\n", CHAR(STRING_ELT(sn, index)), INTEGER(cid)[index]);
				index++;
				//cloneSize<-clones.sample[clones.sample$CloneID==j,"cloneSize"];
				
				//get information for each clone, READIDs, mutations (of each READID, type and position), totalVBase (of each READID), XVBase (of each READID)  
				//get READIDs from clone assignment table
				size_t len_cloneAssigns_ReadID_sample_j=getIndexOfElement(cloneAssigns_cloneID_sample, len_cloneAssigns_cloneID_sample
											, sizeof(int), &cloneID_j, compareInt, cloneAssigns_index_sample_j, 'e'
												);
				Rprintf("\t**doing 4.41......len_cloneAssigns_ReadID_sample_j: %d\n", len_cloneAssigns_ReadID_sample_j);
				size_t len_cloneAssigns_ReadID_sample_j_2=getElementByIndex(cloneAssigns_ReadID_sample, len_cloneAssigns_ReadID_sample,
											sizeof(char*), cloneAssigns_index_sample_j, len_cloneAssigns_ReadID_sample_j, 
											cloneAssigns_ReadID_sample_j);
				Rprintf("\t**doing 4.4.2.....len_cloneAssigns_ReadID_sample_j2: %d\n", len_cloneAssigns_ReadID_sample_j_2);
				if(len_cloneAssigns_ReadID_sample_j!=len_cloneAssigns_ReadID_sample_j_2)
				{
					Rprintf("**********WARNING: the # of records return from sample and clone specific are not what we expected********\n");
				}
				size_t k=0;

				 //--------------------------------
				// *****Chunk 1**********
				//---------------------------------

				for(k=0;k<len_cloneAssigns_ReadID_sample_j;k++)
				{ //loop through for each sequence identified by READID 
					seq[k].readID=cloneAssigns_ReadID_sample_j[k];
					
					//now get the mutation information, get index first for READID records
					len_mutations_index_k=getIndexOfElement(c_mutations_ReadID, lenMutations, sizeof(char*),
													&(seq[k].readID), compareStr, mutations_index_k, 'e');
					len_mutations_Position_k=getSizetFromIntByIndex(c_mutations_Position, lenMutations, 
													mutations_index_k, len_mutations_index_k, mutations_Position_k);
				
					len_mutations_Type_k=getElementByIndex(c_mutations_Type, lenMutations, sizeof(char*),
													mutations_index_k, len_mutations_index_k, mutations_Type_k);
					//suppose now we have mutations information, with the same length
					if(len_mutations_Position_k!=len_mutations_Type_k||len_mutations_Position_k!=len_mutations_index_k)
					{
						error("*****ERROR: returning different length of records (position and type)!!!please check\n");
						return R_NilValue;
					}
					//now we need to copy over the values, because the two arrays mutations_Position_k and mutatoins_Type_k will be reused
					seq[k].numMu=len_mutations_Position_k;
					seq[k].positions=mutations_Position_k;  //here we don't need to do deep copy, we will use this and update manually and accordingly
					seq[k].types=mutations_Type_k; //see above.
					
					//now this is very important, we don't want to keep allocating the memory, so we have to use the previously 
					//allocated memory and carefully pointing to the correct location, so to speak reuse it.
					//the the mutations_Position_k/mutations_Type_k are large and can hold all the records.
					//so need update and keep the used one alive, by just moving to the unused part.
					mutations_Position_k += len_mutations_Position_k;
					mutations_Type_k += len_mutations_Position_k;
//					size_t m=0;
//					seq[k].positions=(char**)R_alloc(len_mutations_Position_k,sizeof(char*));
//					seq[k].muTypes=(size_t*)R_alloc(len_mutations_Position_k,sizeof(size_t));
//					for(m=0;m<seq[k].numMu;m++)
//					{
//						seq[k].positions[m]=mutations_Position_k[m];
//						seq[k].muTypes[m]=mutations_Type_k[m];
//					}
					
					//now do the Ig_RecSums
					len_Ig_RecSums_index_k=getIndexOfElement(c_Ig_RecSums_UID, lenRecSums, sizeof(char*),
													&(seq[k].readID), compareStr, Ig_RecSums_index_k, 'e');
					//len_Ig_RecSums_XVBase_k=getElementByIndex(Ig_RecSums_XVBase, lenRecSums, sizeof(size_t),
					//								Ig_RecSums_index_k, len_Ig_RecSums_index_k, Ig_RecSums_XVBase_k);
					if(len_Ig_RecSums_index_k!=1)
					{
						Rprintf("WARNING*****:more than one element selected for XVBase, only the first one will be used \n");
					}
					seq[k].xVBase=c_Ig_RecSums_XVBase[Ig_RecSums_index_k[0]];
					
					//now do the total vBase
					len_vlengths_index_k=getIndexOfElement(c_vlengths_ReadID, lenVlengths, sizeof(char*),
													&(seq[k].readID), compareStr, vlengths_index_k, 'e');
					//len_Ig_RecSums_XVBase_k=getElementByIndex(Ig_RecSums_XVBase, lenRecSums, sizeof(size_t),
					//								Ig_RecSums_index_k, len_Ig_RecSums_index_k, Ig_RecSums_XVBase_k);
					if(len_vlengths_index_k!=1)
					{
						Rprintf("WARNING*****:more than one element selected for total VBase, only the first one will be used \n");
					}
					seq[k].totalVBase=c_vlengths_totalVBase[vlengths_index_k[0]];
					
					//clone.assigns.clone<-clone.assigns.sample[clone.assigns.sample$CloneID==j,];
					//mutations.clone<-mutations.sample[is.element(mutations.sample$ReadID, clone.assigns.clone$ReadID),]
					//Ig.RecSums.clone<-Ig.RecSums.sample[is.element(Ig.RecSums.sample$UID, clone.assigns.clone$ReadID),]
					//vlengths.clone<-vlengths.sample[is.element(vlengths.sample$ReadID, clone.assigns.clone$ReadID),]
				}//for loop for k READIDs 
				double idi=0;
				Rprintf("&&&&&&&&&&&DONE &&&&&&  idi :%f\n", idi);
					idi=		intraClonalDiversity(seq, len_cloneAssigns_ReadID_sample_j, indel_penalty_c
							); //get idi for each clone
				Rprintf("&&&&&&&&&&&DONE &&&&&&  idi :%f\n", idi);

					//here we need to store into the data frame , to do !!!
//------> need to do this one 					idis[index,c(3,4,5)]  <-c(idi,XMembers,cloneSize )
				REAL(ind)[index-1]=idi;
				//SET_VECTOR_ELT(ind, index-1, ScalarReal(idi));
			}//for loop clones 
				//here store into the data frame 
			//idis<-rbind(idis, idis.sample);
			
		}//end of outer for loop samples.
		//
		Rprintf("Done and set up the output .............");
		SET_VECTOR_ELT(idis, 0, sn);
		SET_VECTOR_ELT(idis, 1, cid);
		SET_VECTOR_ELT(idis, 2, ind);
		SET_VECTOR_ELT(idis, 3, xm);
		SEXP rownam;
		PROTECT(rownam=allocVector(STRSXP, index));
		char* s=(char*)R_alloc(50, sizeof(char));
		for(i=0;i<index;i++)
		{
			sprintf(s, "%zu", i+1);
			SET_STRING_ELT(rownam, i, mkChar(s));
		}
		 setAttrib(idis, R_RowNamesSymbol, rownam);
		if(index>1)		 	
			Rprintf("--index:%zu---sn: %s;    cid : %d ;    idi: %f;     xMember: %d\n", index, CHAR(STRING_ELT(sn, 0)), INTEGER(cid)[0], REAL(ind)[0], INTEGER(xm)[0]);

		//write attributes of the data frame 
		UNPROTECT(8);
		return idis;
	}//end of function.
#endif
//'Calculate the intra-clonal diversity for each clone.
//'@param seq struct sequence array (pointer) holding the sequences for one clone in one sample. 
//'			it contains the mutation information for sequence that will be used to calculate diversity.
//'@param nmem size_t number of sequences in the seq array
//'@param indel_penalty, score for the indels.
//'@return a diversity  
double intraClonalDiversity( struct sequence * seq, 
														size_t nmem,  //# of sequenecs fo 
														double indel_penalty   //penalty or score for indel
							)
	{
		size_t i=1;
		//checking points for doing the pairewise diversity
		double idi=0;
		size_t num_pair=0;
		if(nmem <=1)
		{
			return 0;
		}
		
		//prepare for selecting the valid difference
		size_t len_index_mutation_max=0;
		for(i=0;i<nmem;i++)
		{
			if(seq[i].numMu>len_index_mutation_max)
			{
				len_index_mutation_max=seq[i].numMu;
			}
		}
		printf("the len_index_mutation_max:%zu\n", len_index_mutation_max);
		if(len_index_mutation_max==0) //all mutations are zero 
		{
			return (0);
		}
		
		//The following memory locations will be reused over the loop i
		size_t len_index_mutation_seq2=0;
		size_t* mutations_position_seq2_valid_diff=NULL;
		size_t* mutations_position_seq1_valid_diff=NULL;
		const char** mutations_type_seq2_valid=NULL;
		const char** mutations_type_seq1_valid=NULL;
		size_t* mutations_position_seq2_valid=NULL;
		size_t* mutations_position_seq1_valid=NULL;
		size_t len_index_mutation_seq1=0;
		size_t* index_mutation_seq2=NULL;
		size_t* index_mutation_seq1=NULL;
		
#ifdef R
		if(len_index_mutation_max>0){
			index_mutation_seq1=(size_t*) R_alloc(len_index_mutation_max, sizeof(size_t));
			index_mutation_seq2=(size_t*) R_alloc(len_index_mutation_max, sizeof(size_t));
			mutations_position_seq1_valid=(size_t*) R_alloc(len_index_mutation_max, sizeof(size_t));
			mutations_position_seq2_valid=(size_t*) R_alloc(len_index_mutation_max, sizeof(size_t));
			mutations_type_seq1_valid=(const char**) R_alloc(len_index_mutation_max, sizeof(char*));
			mutations_type_seq2_valid=(const char**) R_alloc(len_index_mutation_max, sizeof(char*));
			mutations_position_seq1_valid_diff=(size_t*) R_alloc(len_index_mutation_max,sizeof(size_t));
			mutations_position_seq2_valid_diff=(size_t*) R_alloc(len_index_mutation_max,sizeof(size_t));
		}
#endif 
#ifdef C_CPP
	if(len_index_mutation_max>0){
		index_mutation_seq1=(size_t*) malloc(len_index_mutation_max* sizeof(size_t));
		index_mutation_seq2=(size_t*) malloc(len_index_mutation_max* sizeof(size_t));
		mutations_position_seq1_valid=(size_t*) malloc(len_index_mutation_max*sizeof(size_t));
		mutations_position_seq2_valid=(size_t*) malloc(len_index_mutation_max*sizeof(size_t));
		mutations_type_seq1_valid=(const char**) malloc(len_index_mutation_max*sizeof(char*));
		mutations_type_seq2_valid=(const char**) malloc(len_index_mutation_max*sizeof(char*));
		mutations_position_seq1_valid_diff=(size_t*) malloc(len_index_mutation_max*sizeof(size_t));
		mutations_position_seq2_valid_diff=(size_t*) malloc(len_index_mutation_max*sizeof(size_t));
	}
#endif 
		size_t temp=0;
		size_t j=0;
		printf("Ready to do the looping............\n");
		for(j=0;j<nmem-1;j++)
		{ //1st one of the pair 
			for( i=j+1;i<nmem;i++)//starting from one, to make a pair.
			{//2nd one of the pair
				num_pair++;
				printf("doing the loop %zu........\n" , i );
#ifdef R				
				R_FlushConsole();
#endif 
				/*num_pairs<-num_pairs+1;
				mutation.seq1<-mutations[mutations$ReadID==id.seq1,c("Type", "Position")]
				totalV.seq1<-vlengths[vlengths$ReadID==id.seq1,"totalVBase"];
				#read the mutations from the mutations file
				#the logic is that the mutations is comparing with the UCA, 
				#list the point mutations at each
				#first total number of nts in the sequences
				id.seq2<-clone.assignment[j,"ReadID"];*/
				size_t num_nt, num_nt_seq1, num_nt_seq2;
				num_nt_seq1=seq[j].xVBase;
				num_nt_seq2=seq[i].xVBase;
				
				//totalV.seq2<-vlengths[vlengths$ReadID==id.seq2,"totalVBase"];
				//mutation.seq2<-mutations[mutations$ReadID==id.seq2,c("Type", "Position")]
				
				num_nt=num_nt_seq2;
				printf("\t the num_nt :%zu ; num_nt_seq1: %zu ; num_nt_seq2: %zu\n", num_nt, num_nt_seq1, num_nt_seq2);
				if(num_nt_seq2 > num_nt_seq1){
					printf("\tConditions 1\n");
					num_nt=num_nt_seq1;
					//pick the valid mutations based on their position
					
					temp=seq[j].totalVBase-seq[j].xVBase;
					if(seq[j].totalVBase<seq[j].xVBase)
					{
						temp=0;
					}
					len_index_mutation_seq2=getIndexOfElement(seq[i].positions, seq[i].numMu, sizeof(size_t),
																	&(temp), compareInt, index_mutation_seq2, 'h');
					//according to index, get elements 
					num_nt_seq2=getElementByIndex(seq[i].positions, seq[i].numMu, sizeof(size_t), 
																	index_mutation_seq2, len_index_mutation_seq2, mutations_position_seq2_valid);
					//seq[i].positions=mutations_position_seq2_valid;  //shallow copy here, hopefully this should work 
					num_nt_seq2=getElementByIndex(seq[i].types, seq[i].numMu, sizeof(char*), 
																	index_mutation_seq2, len_index_mutation_seq2, mutations_type_seq2_valid);
					//seq[i].types=mutations_type_seq2_valid;  //shallow copy here, hopefully this should work 												
					num_nt_seq1=seq[j].numMu;
					memcpy(mutations_position_seq1_valid, seq[j].positions, sizeof(size_t)*num_nt_seq1);
					memcpy(mutations_type_seq1_valid, seq[j].types, sizeof(char*)*num_nt_seq1);
					//# the reason that we do this is because the totalV is the expect length, but sequencing might give us a shortened/truncated one.
					//#  num_nt.seq1 or num_nt.seq2 are the observed length, could be shorter.  (totalV.Seq- num_nt.seq1) telling us the unsequenced nts.
					//# the mutation has to be falling into the sequenced region. 
				}else {
					printf("\tConditions 2\n");
					num_nt=num_nt_seq2;
					//pick the valid mutations based on their position
					temp=seq[i].totalVBase-seq[i].xVBase;
					if(seq[i].totalVBase<seq[i].xVBase)
					{
						temp=0;
					}
					 printf("based on the position :%zu; numMu: %zu\n", temp, seq[j].numMu);
					size_t m=0;
					
					for(m=0;m<seq[j].numMu;m++)
					{
						printf("m:%zu -- positions :%zu..........", m, seq[j].positions[m]);
						fflush(stdout);
					}
					len_index_mutation_seq1=getIndexOfElement(seq[j].positions, seq[j].numMu, sizeof(size_t),
																	&temp, compareSizet, index_mutation_seq1, 'h');
					
					
					printf("len_index_mutation_seq1:%zu\n", len_index_mutation_seq1);
						if(seq[j].numMu>0)
						{
							printf(" positions: %zu\n", seq[j].positions[0]);
						}
						for(m=0;m<len_index_mutation_seq1;m++)
						{
							printf("m:%zu -- element :%zu..........", m, index_mutation_seq1[m]);
							fflush(stdout);
						}
						printf("hello\n");
						fflush(stdout);
					
					
					//according to index, get elements 
					num_nt_seq1=getElementByIndex(seq[j].positions, seq[j].numMu, sizeof(size_t), 
																	index_mutation_seq1, len_index_mutation_seq1, mutations_position_seq1_valid);
					//seq[i-1].positions=mutations_position_seq1_valid;  //shallow copy here, hopefully this should work 
					num_nt_seq1=getElementByIndex(seq[j].types, seq[j].numMu, sizeof(char*), 
																	index_mutation_seq1, len_index_mutation_seq1, mutations_type_seq1_valid);
					//seq[i-1].types=mutations_type_seq1_valid;  //shallow copy here, hopefully this should work
					num_nt_seq2=seq[i].numMu;
					memcpy(mutations_position_seq2_valid, seq[i].positions, sizeof(size_t)*num_nt_seq2);
					memcpy(mutations_type_seq2_valid, seq[i].types, sizeof(char*)*num_nt_seq2);
				}
				if(seq[j].numMu>1)
					printf("seq[i-1].numMu:%zu; seq[i-1].positions:%zu;seq.types:%s\n", seq[j].numMu, seq[j].positions[0], seq[j].types[0]); 
				if(seq[i].numMu>1)
					printf("seq[i].numMu:%zu; seq[i].positions:%zu;seq.types:%s\n", seq[i].numMu, seq[i].positions[0], seq[i].types[0]);
				printf("ready to move to next step\n");
#ifdef C_CPP
				fflush(stdout);
#endif
#ifdef R
					R_FlushConsole();
#endif
				//now count the different, seq[i-1] vs seq[i]
				const char* mtype="Substitution";
				len_index_mutation_seq1=getIndexOfElement(mutations_type_seq1_valid, num_nt_seq1, sizeof(char*),
																	&mtype, compareStr, index_mutation_seq1, 'e');
				//get positions from seq1 
				
				size_t len_position_dif_seq1=getElementByIndex(mutations_position_seq1_valid,  num_nt_seq1, sizeof(size_t), 
																	index_mutation_seq1, len_index_mutation_seq1, mutations_position_seq1_valid_diff);
				printf("ready to move to next step 2\n");
#ifdef C_CPP
				fflush(stdout);
#endif
#ifdef R
					R_FlushConsole();
#endif
				//Seq2
				if(num_nt_seq2>1)
				{
					printf("seq[i].numMu:%zu; seq[i].positions:%zu;seq.types:%s\n", num_nt_seq2, mutations_position_seq2_valid[0], mutations_type_seq2_valid[0]);
					fflush(stdout);
				}
				len_index_mutation_seq2=getIndexOfElement(mutations_type_seq2_valid, num_nt_seq2,sizeof(char*),
																	&mtype, compareStr, index_mutation_seq2, 'e');
				printf("ready to move to next step 2.5\n");
#ifdef C_CPP
				fflush(stdout);
#endif
#ifdef R
					R_FlushConsole();
#endif
				//get positions from seq2
				size_t len_position_dif_seq2=getElementByIndex(mutations_position_seq2_valid,  num_nt_seq2, sizeof(size_t), 
																	index_mutation_seq2, len_index_mutation_seq2, mutations_position_seq2_valid_diff);
				printf("ready to move to next step 3\n");
				if(seq[j].numMu>1)
					printf("seq[i-1].numMu:%zu; seq[i-1].positions:%zu;seq.types:%s\n", seq[j].numMu, seq[j].positions[0], seq[j].types[0]);
				if(seq[i].numMu>1)
					printf("seq[i].numMu:%zu; seq[i].positions:%zu;seq.types:%s\n", seq[i].numMu, seq[i].positions[0], seq[i].types[0]);
				
#ifdef C_CPP
				fflush(stdout);
#endif
#ifdef R
					R_FlushConsole();
#endif
				//compare the difference
#ifdef C_CPP
				fflush(stdout);
#endif
#ifdef R
				Rprintf("&&&&&&show: len_positions_dif_seq1:%zu;   len_position_dif_seq2: %zu\n", len_position_dif_seq1, len_position_dif_seq2);
					R_FlushConsole();
#endif

				double diff=setdiff_c(mutations_position_seq1_valid_diff, len_position_dif_seq1, 
												mutations_position_seq2_valid_diff, len_position_dif_seq2, 
												sizeof(size_t), compareSizet,
												index_mutation_seq1 //note, this is just a holder that we are not going to using its value any way
												);			
#ifdef R
				Rprintf("&&&&&&show: len_positions_dif_seq1:%zu;   len_position_dif_seq2: %zu\n", len_position_dif_seq1, len_position_dif_seq2);
					R_FlushConsole();
#endif

				diff+=setdiff_c(mutations_position_seq2_valid_diff, len_position_dif_seq2, 
												mutations_position_seq1_valid_diff, len_position_dif_seq1,
												sizeof(size_t), compareSizet,
												index_mutation_seq1 //note, this is just a holder that we are not going to using its value any way
												);
				if(seq[j].numMu>1)
					printf("*****seq[i-1].numMu:%zu; seq[i-1].positions:%zu;seq.types:%s\n", seq[j].numMu, seq[j].positions[0], seq[j].types[0]); 
				if(seq[i].numMu>1)
					printf("seq[i].numMu:%zu; seq[i].positions:%zu;seq.types:%s\n", seq[i].numMu, seq[i].positions[0], seq[i].types[0]);
#ifdef C_CPP
				fflush(stdout);
#endif
#ifdef R
					R_FlushConsole();
#endif
				//*-------------------------------------------									
				//--------********do indels**********
				//**********************************
				len_index_mutation_seq1=getIndexOfElement(mutations_type_seq1_valid, num_nt_seq1, sizeof(char*),
																	&mtype, compareStr, index_mutation_seq1, 'n');//for the case where not substitution.
				//get positions from seq1 
				len_position_dif_seq1=getElementByIndex(mutations_position_seq1_valid,  num_nt_seq1, sizeof(size_t), 
																	index_mutation_seq1, len_index_mutation_seq1, mutations_position_seq1_valid_diff);
				printf("ready to move to next step 2\n");
				fflush(stdout);
				//Seq2
				len_index_mutation_seq2=getIndexOfElement(mutations_type_seq2_valid, num_nt_seq2,sizeof(char*),
																	&mtype, compareStr, index_mutation_seq2, 'n');
				printf("ready to move to next step 2.5\n");
				fflush(stdout);
				//get positions from seq2
				len_position_dif_seq2=getElementByIndex(mutations_position_seq2_valid,  num_nt_seq2, sizeof(size_t), 
																	index_mutation_seq2, len_index_mutation_seq2, mutations_position_seq2_valid_diff);
				
				//compare the difference 
				diff+=indel_penalty*setdiff_c(mutations_position_seq1_valid_diff, len_position_dif_seq1, 
												mutations_position_seq2_valid_diff, len_position_dif_seq2, 
												sizeof(size_t), compareSizet,
												index_mutation_seq1 //note, this is just a holder that we are not going to using its value any way
												);			
				diff+=indel_penalty*setdiff_c(mutations_position_seq2_valid_diff, len_position_dif_seq2, 
												mutations_position_seq1_valid_diff, len_position_dif_seq1, 
												sizeof(size_t), compareSizet,
												index_mutation_seq1 //note, this is just a holder that we are not going to using its value any way
												);		
					if(seq[j].numMu>1)
							printf("%%%%%%seq[i-1].numMu:%zu; seq[i-1].positions:%zu;seq.types:%s\n", seq[j].numMu, seq[j].positions[0], seq[j].types[0]); 
					if(seq[i].numMu>1)
						printf("seq[i].numMu:%zu; seq[i].positions:%zu;seq.types:%s\n", seq[i].numMu, seq[i].positions[0], seq[i].types[0]);
				fflush(stdout);
				//num.diff.indel<- num.diff.indel*indel.penalty;
				printf("diff:%f; num_nt:%zu\n", diff, num_nt);
				idi+=diff/num_nt;
			}//end of loop through sequences/pairs
		}
#ifdef C_CPP
	free(index_mutation_seq1);
	free(index_mutation_seq2);
	free(mutations_position_seq1_valid);
	free(mutations_position_seq2_valid);
	free(mutations_position_seq1_valid_diff);
	free(mutations_position_seq2_valid_diff);
	free(mutations_type_seq1_valid);
	free(mutations_type_seq2_valid);
#endif		
//		printf("seq[i-1].numMu:%zu; seq[i-1].positions:%zu;seq.types:%s\n", seq[0].numMu, seq[0].positions[0], seq[0].types[0]); 
//			printf("seq[i].numMu:%zu; seq[i].positions:%zu;seq.types:%s\n", seq[1].numMu, seq[1].positions[0], seq[1].types[0]);
		Rprintf("!!!!!!!!!!!!!!!!!!!!!1(IntraClonalDiversity) <->Done\n");
			fflush(stdout);
		return idi/num_pair;
	}//end of function 								