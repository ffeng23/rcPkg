///////////////////////////////
// XDemo.cc
// C++ Function File
//#include "XDemo.h"
#include <R.h> // R memory io, in the standard list of system directories
//#include "XDemo.h"  //the user defined header file. it will search the current working directory and the system directories as in "< >"
#include <Rinternals.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//used to be called by .C() 
void add_rc(double* x, double* y, double* o)
{
	Rprintf("feng in side c function\n");
	*o=*x+*y;
	
}

//used to be called by .Call() 
SEXP add_rc_call(SEXP x, SEXP y)
{
	Rprintf("feng *call2222* in side c function\n");
	//start converting the inputs
	double xc=asReal(x); double yc=asReal(y);
	//for output
	double o=xc+yc;
	
	//adding 
	Rprintf("****new adding vector\n");
	
	SEXP xv=PROTECT(allocVector(INTSXP, 3));
	INTEGER(xv)[0]=10;
	INTEGER(xv)[1]=20;
	INTEGER(xv)[2]=30;
	//SET_VECTOR_ELT(xv, 0, scalarInteger(10));
	//SET_VECTOR_ELT(Xv, 1,20);
	SEXP xs=PROTECT(allocVector(STRSXP, 3));
	SET_STRING_ELT(xs, 0, mkChar("Feng"));
	SET_STRING_ELT(xs, 1, mkChar("Zihan"));
	SET_STRING_ELT(xs, 2, mkChar("Chris"));
	
	SEXP out =PROTECT(allocVector(VECSXP,2));
	SET_VECTOR_ELT(out, 0,xv);
	SET_VECTOR_ELT(out, 1,xs);
	SEXP cls, nam, rownam;
	PROTECT(cls = allocVector(STRSXP, 1)); // class attribute
   SET_STRING_ELT(cls, 0, mkChar("data.frame"));
   classgets(out, cls);
   
   PROTECT(nam = allocVector(STRSXP, 2)); // names attribute (column names)
   SET_STRING_ELT(nam, 0, mkChar("a"));
   SET_STRING_ELT(nam, 1, mkChar("b"));
   namesgets(out, nam);
   
   char* s=(char*) R_alloc(10, sizeof(char));
   
   PROTECT(rownam = allocVector(STRSXP, 3)); // row.names attribute
   s[0]='1';
   s[1]='\0';
   sprintf(s, "%d", 1);
   SET_STRING_ELT(rownam, 0, mkChar(s));
   s[0]='2';
   SET_STRING_ELT(rownam, 1, mkChar(s));
   s[0]='3';
   //itoa(1, s, 10);
   SET_STRING_ELT(rownam, 2, mkChar(s));
   setAttrib(out, R_RowNamesSymbol, rownam);
   
	UNPROTECT(6);
	//return ScalarReal(o);
	return out;
}

//used to access/modify the numeric/double vector
SEXP accessVector_rc_call(SEXP x)
{
	Rprintf("feng *call accessVec* in side c function\n");
	//Getting the length
	R_xlen_t n=xlength(x);
		Rprintf("n is %d\n",n);
	if(n==0)
		return R_NilValue;
	SEXP out = PROTECT(allocVector(REALSXP, n));
	for(int i =0;i<n;i++)
	{
		REAL(out)[i]=REAL(x)[i]+2;
	}
	//double xc=asReal(x); double yc=asReal(y);
	double* xi=REAL(x);
	Rprintf("Printing using double pointer, %f :: %f :: %f\n", xi[0], xi[1], xi[2]); 
	//for output
	UNPROTECT(1);
	return out;
	
}

//used to access/modify the string vector
SEXP accessStrVec_rc_call(SEXP x)
{
	Rprintf("feng *call access string vector * in side c function\n");
	//Getting the length
	R_xlen_t n=xlength(x);
	Rprintf("n is %d\n",n);
	if(n==0)
		return R_NilValue;
	SEXP out = PROTECT(allocVector(STRSXP, n));
	for(int i =0;i<n;i++)
	{
		SET_STRING_ELT(out, i,(STRING_ELT(x, i)));
		
		Rprintf("the element %d: %s\n", i, CHAR((STRING_ELT(x, i))));
	}
	//double xc=asReal(x); double yc=asReal(y);
	
	//now let's see how we can access convert R string to C strings/char array
	//char** x_ptr=STRING_PTR(x);
	//Rprintf("testing the string pointer, STRING_PTR: first is %s === %s === %s ====\n", x_ptr[0],x_ptr[1],x_ptr[2]);
	//for output
	SET_STRING_ELT(x, 0, mkChar("ABS"));
	UNPROTECT(1);
	return out;
	
}


//used to be called by .C() 
//'Generate a subset of indices by full combination (C code).
//' an example of usage is that we have 10 indexes and we 
//' we want to get 5 group of subset of these indexes each of which
//' contains 3 indexes, starting pool A={1,2,3,....,10}
//'  expecting output B={{1,2,3}, {1,2,4}, ....... , 
//'       {2,3,4}, {2,3,5}......,
//'  }
//' So the parameter to this function in this case is 
//'  x=10, y=3, z=[1:  C10_3]
//'@param x integer total number of indexes
//'@param y integer number of elements in the subset
//'@param z integer number of subsets in the subset to be output. The full set is calculate based on 
//'	permutation without replacement and without order. If less than this number it will 
//'	be truncated.
//'@return interger matrix
SEXP subsetFullCombination_c(SEXP x, SEXP y, SEXP z)
{
	Rprintf("calling C to the \n");
	//start converting the inputs
	int len=asInteger(x); int num=asInteger(y);
	if(num>len)
	{
		error(" the num of elements of the permutatoins has to be smaller than the length of the input array.");
	}
	SEXP temp=PROTECT(allocVector(INTSXP, num));
	int* index_array=INTEGER(temp);//c(1:num); //the starting index sequence
	//set the starting values
	for(int i=0;i<num;i++)
	{
		index_array[i]=i+1;
		Rprintf("i+1:%d",i+1);
	}
	
	//#sset<-list(); #keep the sequences
	//#sset.index<-data.frame(); #keep the indexes
	//int len<-s;
	int count=0;
	int flag_finished=0;
	int total=asInteger(z);//<-factorial(len)/factorial(num)/factorial(len-num);
	//#inside the while loop we keep checking/updating index.array
	//#   index.array keeps the index for each of the num of elements.
	//#	we check for it and determine what to increment the current index order
	//#	go back up to incrementing the upper(previous) index
	
	//#pre-allocate the space
	SEXP out=PROTECT(allocMatrix(INTSXP,  num,total));
	int* sset_index=INTEGER(out);//matrix(0, nrow=round(total), ncol=num)
		memset(sset_index,0,total*num*sizeof(int));
	while(TRUE)
	{
		//#need to make the first combination can be run!!!
		
		//#let's do the job to pick one element for one subset.
		count++;//<-count+1;
		//Rprintf("***count:%d\n", count);
		
		//flush.console();
		memcpy(sset_index+(count-1)*num,index_array,num*sizeof(int));
		//Rprintf("\t");
		//for(int k=0;k<num;k++)
		//{
		//	Rprintf("reading mem:k:%d:%d;", k, sset_index[(count-1)*num+k]);
		//}
		//Rprintf("\n");
		//for(int k=0;k<num;k++)
		//{
		//	Rprintf("reading index_array:k:%d:%d;", k, index_array[k]);
		//}
		//Rprintf("\n");
		
		//#sset[[count]]<-s[c(index.array)]
		
		if(count%5000==0)
		{
			Rprintf("subsetFullCombination():%d/%d....\n", count, total );
			R_FlushConsole();
			R_CheckUserInterrupt();
			//flush.console();
		}
		//cat("\tbefore updating......\n")
		//flush.console();
		//# updating condition
	
		for(int i=num-1;i>=0;i--)// in num:1) #checking all indexing positions in the output array
		{
			if(i==num-1)
			{
				index_array[i]++;//=index.array[i]+1;
				if(index_array[i]<=(len-((num-1)-i)))
				{
					//#in this case, we simply increment ignore and go next round of update, skipping out of the for loop 
					//#index.array[i]<-index.array[i]+1;
					break; 
				} //#else we will update the next array index in the for loop
			} else { //#need to check the previous one (big number index)
				if(index_array[i+1]>(len-((num-1)-(i+1)))) //#the big number one is "overflowed", we will keep incrementing this current one
				{
					index_array[i]++;//<-index.array[i]+1; //#update the current one
					
					if(index_array[i]<=(len-((num-1)-i)))
					{
						//#update the previouse index/ reset them to it "original"
						//index.array[num:(i+1)]<-c((num-i):1)+index.array[i]
						//using for loops now to update the 
						for(int j=(num-1);j>=(i+1);j--)
						{
							index_array[j]=index_array[i]+(j-i);
						}
						//#in this case, we simply increment ignore and go next round of update, skipping out of the for loop 
						//#index.array[i]<-index.array[i]+1;
						break; 
					} else {//#we will update the next array index in the for loop
						if(i==0)
						{
							//#we are done;
							flag_finished=1;
						}
					}
				} else { //#in this case, it is impossible. we simply do nothing 
					//#index.array[i]<-index.array[i]+1;
					//#break;
					Rprintf("imppossible case\n");
				}
			}
			
		}//#end of for loop
		//Rprintf("\tdone for one run\n");
		if(flag_finished)
		{
			break; //#done!!
		}
	}
	Rprintf("DONE!!!!\n");
	UNPROTECT(2);
	return out;
}

//'C struct to define the deversity of a group of barcodes
//'@description this struct used to define the "data" type to pass 
//'	around the diversity information between c functions. Not intended 
//'	to pass to the R codes.
//'@param 
struct diversity{
	double divergence;
	bool* diverse;
	int length;
};

int sum_int_c(const int *const  c, const int length)
{
	int o=0;
	for(int i=0;i<length;i++)
	{
		o+=c[i];
	}
	return o;
}
double sum_double_c(const double * const  c, const int length)
{
	double o=0;
	for(int i=0;i<length;i++)
	{
		o+=c[i];
	}
	return o;
}
double* normalize_array_c(const int * const  a, const int seqLen, const int sum, double* o)
{
	//double* tmp=(double*) R_alloc(seqLen, sizeof(double));
	for(int i=0;i<seqLen;i++)
	{
		o[i]=((double)a[i])/sum;
	}
	return o;
}
//sum^2(a1-c(0.25,0.25,0.25,0.25)
double array_diff_ss_c(const double * const a1)
{
	int length=4;
	double tmp[]={0.25,0.25,0.25,0.25};
		for(int i=0;i<length;i++)
		{
				tmp[i]=(a1[i]-tmp[i])*(a1[i]-tmp[i]);
		}
	double x=sum_double_c(tmp, length);	
	//Free(tmp);
	return(x);
}

//check to whether the integer array has zero elements in there
bool hasZeros(const int * const  freq, const int num)
{
	bool out=false;
	for(int i =0;i<num; i++)
	{
		if(freq[i]==0){
			out=true;
			break;
		}
	}
	return out;
}
//'to calculate the diversity of a subset of sequence matrix.
//'@description use to calculate the diversity of selected sequences out of the sequence matrix
//'@param seq_matrix STRSXP (SEXP) this is the R object matrix arranged into 1D vector in C code. 
//'	In R matrix format, it is arranged so that each row is for one sequence barcode and each column
//'	for the nucleotides at different position of the sequence. In 1D format, it is by column, meaning
//'	the nucleotides at the same position across the different sequences are arranged into 
//' one continous block
//'		nt at position 1{seq1, seq2, ..., seqN}, nt at position 2 {seq1,....,seqN}...
//'		nt at position M{seq1, seq2, ..., seqN};
//'@param numOfSeq integer total number of sequences (N) in the matrix
//'@param seqLength integer sequence length in terms of the number of the nucleotides
//'@param index pointer to index vector. it is of length "numOfIndex" (1D). The numOf index 
//'	should be smaller than total number of sequences in the matrix (numOfSeq)
//'@return a diversity struct contains two fields, 
//'		diverse and diverge score (diverging from perfect balanced )
 
struct diversity* getDiversity(SEXP seq_matrix, const int numOfSeq, const int seqLength
								, const int * const  index, const int numOfIndex)
							
{
	/*if(class(s)!="DNAStringSet")
	{
		stop("the input is not of DNAStringSet!!please check")
	}*/
	if(numOfIndex>numOfSeq)
	{
		error("number of Indexes selected to be checked for diversity is more than the total number of sequences, please check!!!");
	}
	
	if(TYPEOF(seq_matrix)!=STRSXP)
	{
		error("the input sequence matrix is not of STRSXP type, please check!!!");
	}
	int* freq=(int*) R_alloc(4, sizeof(int));
	double* freq_db=(double*)R_alloc(4,sizeof(int));
	freq_db[1]=0;
	memset(freq, 0, sizeof(int)*4);//{A,C,T,G}
	//#names(freq)<-c("A","C","T","G")
	
	//len<-length(s[[1]])
	//int* diverse=(int*) R_alloc(seqLength, sizeof(int));//bool array for diverse flag, not diversity <-rep(TRUE,len);
	//memset(diverse, 0, sizeof(int)*seqLength);
	
	struct diversity* sd=(struct diversity*)R_alloc(1, sizeof(struct diversity));
	sd->length=seqLength;
	sd->divergence=0;
	sd->diverse=(bool*)R_alloc(seqLength, sizeof(bool));
	//x<-toupper(as.matrix(s))
	for(int i=0;i<seqLength;i++)
	{	//reset the freq array.
		memset(freq, 0, sizeof(int)*4);//{A,C,T,G}
		//check for each sequence for the diversity at the position i
		for(int j=0;j<numOfIndex;j++)
		{
			//Rprintf("i:%d;j:%d--%s\t", i,j,CHAR(STRING_ELT(seq_matrix,i*seqLength+j)));//[i*seqLength+j]);
			char xi=CHAR(asChar(STRING_ELT(seq_matrix,i*numOfSeq+(index[j]-1))))[0];//CHAR((seq_matrix,(i-1)*seqLength+index[j]))[0];
			//Rprintf("i:%c;",xi);
			switch(xi)
			{
				case 'A'://1
				case 'a':
					freq[0]++;
					break;
				case 'C'://2
				case 'c':
					freq[1]++;
					break;
				case 'T'://3
				case 't':
					freq[2]++;
					break;
				case 'G'://4
				case 'g':
					freq[3]++;
					break;
				default: //for all other 
					//do nothing.
					break;
					
			}//end of switch
			
		}//end of inner for loop, for each position but different sequence
		//Rprintf("\n");
		//freq[0]++;
		
		int sum=sum_int_c(freq,4);
		freq_db=normalize_array_c(freq, 4, sum, freq_db);
		
		sd->divergence += array_diff_ss_c(freq_db);
		//Rprintf("freq ss :%f; divergence:%f\n", array_diff_ss_c(freq_db),sd->divergence);
		/*for(int k=0;k<4;k++)
		{
			Rprintf("freq %d:%d;",k,freq[k]);
		}*/
		sd->diverse[i]=!hasZeros(freq,4);

		//Rprintf("bool:%s\n",sd->diverse[i]?"true":"false");
				
			
		/*diverse[i]<-setequal(unique(x[,i]), names(freq))
		for(j in 1:length(freq))
		{
			freq[j]<-sum(x[,i]==names(freq)[j])
		}
		#now get the freq
		freq<-freq/sum(freq)
		freq<-freq-c(0.25,0.25,0.25,0.25)
		diverge<-diverge+sum(freq^2)*/
	}
	
	//list(diverse=diverse, freq=diverge/len)
	return sd;
}
bool isSeqsDiverse(const bool * const d, const int len )
{
	//bool o=true;
	Rprintf("len is %d\t", len);
	for(int i=0;i<len;i++)
	{
		Rprintf("d[%d]:%s; ", i,d[i]?"true":"false");
		if(!d[i])
		{
			//o=false;
			Rprintf("\n");
			return false;
		}
	}
	Rprintf("\n");
	return true;
}
//'@title used to search for the diverseSet in the barcode pool
//'@param s character matrix of R objects (R STRSXP) holding the sequences, the row is the sequence of char vector 
//'@param numOfSeq int scalar h
SEXP getDiverseSet_c(SEXP s, SEXP numOfSeq, SEXP seqLength, 
					SEXP subsetIndex, SEXP numOfSubset, SEXP subsetLength
				)// include, s.pair, include.pair )
{
	int numSeq=asInteger(numOfSeq);
	int sLen=asInteger(seqLength);
	if(TYPEOF(s)!=STRSXP)
	{
		Rprintf("The passed in R object is not of type STRSXP, please check.....\n");
	}
	else 
	{
		Rprintf("A STRSXP object read in with length of %d (%d x %d).\n", length(s), numSeq, sLen);
	}
	//STRING_ELT(s,1);
/*	for(int i=0;i<30;i++)
	{
		Rprintf("s [%d]:%s\n", i, CHAR(STRING_ELT(s,i)));
	}
	
	Rprintf("**numSeq:%d;sLen:%d; \n", numSeq, sLen);
	*/
	/*if(!missing(s.pair))
	{#combine both Reads
		s.com<-DNAStringSet(paste0(as.character(s), as.character(s.pair)))
		names(s.com)<-names(s);
	}
	#*/
	//cat("generating the permutated sequence set.......\n")
	int numSub=asInteger(numOfSubset);
	int subsetLen=asInteger(subsetLength);
	if(TYPEOF(subsetIndex)!=INTSXP)
	{
		error("The passed in R object is not of type INTSXP, please check.....\n");
	}
	else 
	{
		Rprintf("A INTSXP object read in with length of %d (%d x %d).\n", length(subsetIndex), numSub, subsetLen);
	}
	int* st0=INTEGER(subsetIndex);//<-subsetFullCombination(s.com, num)
	
	//st<-st0[[1]]
	//st.index<-st0[[2]]

	//Rprintf("**numSub:%d;  subsetLen:%d\n",  numSub,subsetLen);
	Rprintf("\tst0:%d;\n", st0[1]);//, s_com);
	
	//here start testing the pairlist for out	
	//	/*----------pair list ------
	SEXP st_diverse_out=PROTECT(allocList(0));
	int count_out=0;
	
	/*	s.com.include<-NULL;
	if(!missing(include)&&!missing(include.pair))
	{
		s.com.include<-DNAStringSet(paste0(as.character(include), as.character(include.pair)))
		names(s.com.include)<-names(include);
	}
	
	index<-c();*/
	Rprintf("Find the balanced sets.....\n");
	
	struct diversity* sd;//=getDiversity(s, numSeq, sLen,st0+subsetLen,subsetLen);
	//Rprintf("sd diverse 0:%d;sd diverse[1]:%d; sd diverge:%f\n", sd->diverse[0],sd->diverse[1], sd->divergence);
	
	//go through the index and calculate the diversity of each group and write the output if this is good.
	int p_count=0;
	for(int i=0;i<numSub;i++)// in 1:length(st))
	{
		//#get diversity
		if((i/numSub*100)>=p_count*10)
		{
			
			Rprintf("getDiverseSet():%d\n", i/numSub*100 );
			p_count++;
			R_FlushConsole();
		}
		//now let's do the calculate
		sd=getDiversity(s, numSeq, sLen,st0+(subsetLen*i),subsetLen);
		Rprintf("i:%d; ", i);//, isSeqsDiverse(sd->diverse,sLen)?"true":"false");
		Rprintf("sd diverse:%s; ", isSeqsDiverse(sd->diverse,sLen)?"true":"false");
		Rprintf("sd divergence:%f\n", sd->divergence);
		//get diversity for one group of barcodes, now check to see if we need to write to the output
		if(isSeqsDiverse(sd->diverse, sLen))
		{
			//save it to the output, we will write the sequence indexes for each group and the divergence score.
			//Rprintf("leng of pairlist is :%d\n", length(st_diverse_out));
			SEXP xtmp=PROTECT(allocVector(VECSXP,2));
			//set up R object of vector for index
			SEXP ind_tmp=PROTECT(allocVector(INTSXP,subsetLen));
			memcpy(INTEGER(ind_tmp), st0+(subsetLen*i),sizeof(int)*subsetLen);
			//set up R object scalar for divergence
			SET_VECTOR_ELT(xtmp,1,PROTECT(ScalarReal(sd->divergence)));
			SET_VECTOR_ELT(xtmp,0,ind_tmp);
			
			if(count_out==0)
			{
				st_diverse_out=PROTECT(CONS(xtmp,R_NilValue));
				//Rprintf("nil value detect in the first pairlist element!!!\n");
			}
			else {//larger than zero, meaning we have 
				SETCDR(nthcdr(st_diverse_out,count_out-1),PROTECT(CONS(xtmp,R_NilValue)));
			}
			count_out++;
			if(count_out*4>=10000-1) //10000 is the maximum of the PROTECT object stack.
			{
				Rprintf("Reaching the maximum number of PROTECT object stack,need to stop here."); 
				Rprintf("	You might need to split the indexes vector for a new search.......");
			}
			//Rprintf("leng of pairlist is :%d\n", length(st_diverse_out));
		}
		
		
		/*score<-NULL;
		if(!missing(s.com.include))
		{
			score<-diversity(append(st[[i]], s.com.include))
		} else {
			score<-diversity(st[[i]]);
		}
		if(all(score[[1]]))
		{
			cat("\tgetDiverseSet():find one diversed set\n");
			flush.console();
			diverge.score<-c(diverge.score, score[[2]])
			st.diverse[[length(st.diverse)+1]]<-st[[i]]
			index<-c(index,i)
		}*/
	}
	Rprintf("Done!\n");
	
	/*if(length(diverge.score)==0){
		return(list())
	}
	st.diverse<-st.diverse[order(diverge.score, decreasing=F)];
	index<-index[order(diverge.score, decreasing=F)];
	
	return (list(diverse=st.diverse, index=st.index[index,]))*/
	UNPROTECT(count_out*4+1);//the total count of PROTECT objects.
	return st_diverse_out;
}
