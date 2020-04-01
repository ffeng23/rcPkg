#R wrapper for c code

#'@export
#'@useDynLib rcPkg add_rc
#
add_r_w<-function(x,y)
{
#here in this example, we using .C call, of which C function normally does not return values.
# and also pointer only as input. 
#but instead we using the input pointer to function as output. Inputs are passed as 
# list of pointers. so we get the output as the 3rd input.
	.C(add_rc, x, y, numeric(1))[[3]];
}
#only apply to pakcages with name spaces.
#.onUnload <- function (libpath) {
#	cat("***********doing unloading\t")
 # library.dynam.unload("rcPkg", libpath)
#}

#'@export
#'@useDynLib rcPkg add_rc_call
add_rcall_w<-function(x,y)
{
	.Call(add_rc_call, x, y)
}
#'@export
#'@useDynLib rcPkg accessVector_rc_call accessStrVec_rc_call
accessVector_rc_w<-function(x)
{
	y<-NULL;
	if((class(x)!="numeric"&&class(x)!="integer")&&length(x)!=0)
	{
		if(class(x)=="character")
		{
			y<-.Call(accessStrVec_rc_call,x)
		}
		else
		{
			stop("we need either a numeric or integer array as input");
		}
	} else {
		if(class(x)=="integer")
		{
			x=as.numeric(x);
		}
		y<-.Call(accessVector_rc_call,x);
	}
	
	return(y);
}

#'@title R wrapper of C function to get subset of indexes by full combination
#'@description a fast C function wrapper to generate a full combinations of index
#'	based on the number of sequences and size of the group.
#'@param x integer number of the elements in the pool to be drawn from
#'@param y integer number of elements in the group of indexes we are generating
#'@return a matrix holder the indexes. It will arranged so that columns are the different subsets/groups,
#'	and each column is one index in the subset.
#'@export
#'@useDynLib rcPkg subsetFullCombination_c
#
subsetFullCombination_w<-function(x,y)
{
	.Call(subsetFullCombination_c, x, y, round(factorial(x)/factorial(y)/factorial(x-y)))
}

#'@title R wrapper of c function to search for the diverse barcodes
#'@description it passes in the barcodes and indexes of barcodes. The called
#'	c function will search all index group for diverse/balanced barcodes.
#'@details the divergence score is calculated as sum^2(freq-c(0.25,0.25,0.25,0.25)) for 
#'	positions and then normalized by sequence length.
#'@param s character matrix, assuming its each is for one barcodes sequences and 
#'	columns are the positions of nucleotide in the barcode sequence.
#'@param index numeric matrix, holding the combinations of indexes to search from the barcode matrix above.
#'	it is arranged similiar to the output from the function \code{\link{subsetFullCombination}}. Each column
#'	is for one subset of index to be seach for diverse barcodes.
#'@return a list of index combinations will give us the balanced barcodes and also the divergence score
#'
#'@export
#'@useDynLib rcPkg getDiverseSet_c
getDiverseSet_w<-function(s,  index)
{
	#cat("dim",dim(s)[1]);
	.Call(getDiverseSet_c, s, dim(s)[1], dim(s)[2],index, dim(index)[2], dim(index)[1]
			);
}

#now define a wrapper to call intraDiversities
#'@title R wrapper of c functions to do intraDiversities
#'@description doing c code based on the input to calculate the intraDiversities of clones for each sample
#'	it is too slow to do it in R
#'@param clones dataframe the clone summary table . 
#'	Data schema is [1]:sample name;[2]clone Id; [3]xMembers (number of clone members);
#'@param clone.assignments data frame the clone assignment table 
#'	Data schema: [1]sample name;[2] clone Id; [3] read ID
#'@param mutations data frame the mutation table 
#'	Data schema: [1] ReadID; [2] type ; [3] positions
#'@param Ig.RecSums data frame the Ig recombination table 
#'	Data schema: [1] ReadID; [2] xVBase
#'@param vlengths data frame the vlengths table
#'	Data schema: [1] ReadID; [2] totalVBase 
#'@param mCloneSize integer a minimum clone size that is used to filter out the smaller clones from the 
#'	calculation 
#'@param indel.penalty numeric the indel penalty score. The default is 1. 
#'@return a data frame contains "sampleID" and "cloneID" and "idi" and "xMembers" (number of clone)
#'@examples 
#'	# Please check the DataAnalysis_v1.2_intraClonalDiversit_nonSilentMu_cfun.R 
#'	#for code examples.
#'	#Basically we try to /must make sure the data frame to be correctly formatted 
#'	#PLEASE DO NOT RUN
#'	##start prepare the input for calling c intraClonalDiversities
#'	##df.clone.test<-df.clones [,c("sampleName", "CloneID", "X.Members")]
 #'	##df.cloneAssign.test<-df.cloneAssign[, c("sampleName", "CloneID", "ReadID")]
#'	##df.mutations.test<-df.mutations[, c("ReadID", "Type", "Position")]
#'	##IG.RecSum.test<-IG.RecSum[,c("UID", "X.VBases")]
#'	##df.vlength.test<-df.vlength[, c("ReadID", "totalVBase", "XVBases")]; 	
#'	##ready to call 
#'	##system.time(
#'	##	idis<-intraClonalDiversities(clones=df.clone.test, #clone summary, 
#'	##			clone.assigns=df.cloneAssign.test, #clone assignments for sequences
#'	##			mutations=df.mutations.test, #seq muations VRG
#'	##			Ig.RecSums=IG.RecSum.test, #Ig Recsum for VBases
#'	##			vlengths=df.vlength.test,  #total lengths of v 
#'	##			indel.penalty=1 #for penalty of indel.
#'	##			)
#'	##	);
#'@export
#'@useDynLib rcPkg intraClonalDiversities_c

intraClonalDiversities<-function(
			clones, #clone summary, 
			clone.assigns, #clone assignments for sequences
			mutations, #seq muations VRG
			Ig.RecSums, #Ig Recsum for VBases
			vlengths,  #total lengths of v
            mCloneSize=3,			
			indel.penalty=1 #for penalty of indel.
)
{
	#//check to make sure the data integrity
		if(missing(clones)||class(clones)!="data.frame"||dim(clones)[2]<3)
		{
			stop("clone data input is not valid, please check..... \n");
		}
		if(missing(clone.assigns)||class(clone.assigns)!="data.frame"||dim(clone.assigns)[2]<3)
		{
			stop("clone assign data input is not valid, please check..... \n");
		}
		if(missing(mutations)||class(mutations)!="data.frame"||dim(mutations)[2]<3)
		{
			stop("mutation data input is not valid, please check..... \n");
		}
		if(missing(Ig.RecSums)||class(Ig.RecSums)!="data.frame"||dim(Ig.RecSums)[2]<2)
		{
			stop("Ig recombination data input is not valid, please check..... \n");
		}
		if(missing(vlengths)||class(vlengths)!="data.frame"||dim(vlengths)[2]<2)
		{
			stop("v length data input is not valid, please check..... \n");
		}
		
		clones_sampleNames=as.character(clones[,1]) #clone summary, sample name, string char** 
		
		clones_cloneID=as.integer(clones[,2]) #clone summary, clones ids, int *
		clones_xMembers=as.integer(clones[,3])# //clone summary, number of members, int *
			
		cloneAssigns_sampleNames=as.character(clone.assigns[,1]);# //clone assignments for sequences, sample names, char**
		cloneAssigns_cloneID=as.integer(clone.assigns[,2])# //clone assignments, clone Ids, int*
		cloneAssigns_ReadID=as.character(clone.assigns[,3])# //clone assignment, read ID, char**
			
		#now in mutations table we don't include the sampleNames, since READID is good to identify
		mutations_ReadID=as.character(mutations[,1])#, //seq muations VRG, read ID, char** 
		mutations_Type=as.character(mutations[,2])#   //   Type , string substitution, char**
		mutations_Position=as.integer(mutations[,3])#  //Poisiotin, int* 
			
		#	//now in Ig_RecSums table we don't include the sampleNames, since READID (UID) is good to identify
		Ig_RecSums_UID=as.character(Ig.RecSums[,1])# //Ig Recsum for VBases, UID, string, char**  
		Ig_RecSums_XVBase=as.integer(Ig.RecSums[,2])#, //             XVBase, int* 
			
		#	//now in vlengths table we don't include the sampleNames, since READID is good to identify
		vlengths_ReadID=as.character(vlengths[,1])#,  //total lengths of v, ReadID, char**  
		vlengths_totalVBase=as.integer(vlengths[,2]);# //         int*, totalVBase
			
		
	.Call(
		intraClonalDiversities_c, 
					clones_sampleNames, clones_cloneID, clones_xMembers,
					cloneAssigns_sampleNames, cloneAssigns_cloneID, cloneAssigns_ReadID, 
					mutations_ReadID, mutations_Type, mutations_Position, 
					Ig_RecSums_UID, Ig_RecSums_XVBase, 
					vlengths_ReadID, vlengths_totalVBase,
					mCloneSize,
					indel.penalty
			);
}