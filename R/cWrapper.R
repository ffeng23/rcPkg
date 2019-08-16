#R wrapper for c code

#'@export
#'@useDynLib rcPkg add_rc
#
add_r<-function(x,y)
{
	.C(add_rc, x, y, numeric(1))[[3]]
}
#only apply to pakcages with name spaces.
#.onUnload <- function (libpath) {
#	cat("***********doing unloading\t")
 # library.dynam.unload("rcPkg", libpath)
#}

#'@export
#'@useDynLib rcPkg add_rc_call
add_rcall<-function(x,y)
{
	.Call(add_rc_call, x, y)
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
subsetFullCombination<-function(x,y)
{
	.Call(subsetFullCombination_c, x, y, round(factorial(x)/factorial(y)/factorial(x-y)))
}

#'@title R wrapper of c function to search for the diverse barcodes
#'@description it passes in the barcodes and indexes of barcodes. The called
#'	c function will search all index group for diverse/balanced barcodes.
#'@param s character matrix, assuming its each is for one barcodes sequences and 
#'	columns are the positions of nucleotide in the barcode sequence.
#'@param index numeric matrix, holding the combinations of indexes to search from the barcode matrix above.
#'	it is arranged similiar to the output from the function \code{\link{subsetFullCombination}}. Each column
#'	is for one subset of index to be seach for diverse barcodes.
#'@return a list of index combinations will give us the balanced barcodes.
#'
#'@export
#'@useDynLib rcPkg getDiverseSet_c
getDiverseSet<-function(s,  index)
{
	#cat("dim",dim(s)[1]);
	.Call(getDiverseSet_c, s, dim(s)[1], dim(s)[2],index, dim(index)[2], dim(index)[1]
			);
}
