#include "CppUTest/TestHarness.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h> 

using namespace std;
extern "C" {
	#include "helper_c.h"
	#include "clonalDiversity_c.h"
}
// create a test group
TEST_GROUP(rc_test){
   
};

//create a test for that test group
TEST (rc_test, compareInt){
	printf("++++rc_test:testing compareInt............");
	int p1=0, p2=1;
	int result=compareInt(&p1,&p2);
    CHECK_EQUAL(-1,result);
	p1=1; p2=0;
	result=compareInt(&p1,&p2);
    CHECK_EQUAL(1,result);
	p1=1; p2=1;
	result=compareInt(&p1,&p2);
    CHECK_EQUAL(result,0);
	
	float f=25.1;
	float * fp=&f;
	cout<<"casting float to int :"<< *((int*)fp)<<" without casting:"<<*fp<<endl;
	
	size_t st1=1, st2=2;
	result =compareSizet(&st1, &st2);
	CHECK_EQUAL(-1, result);
	st2=0;
	result  =compareSizet(&st1, &st2);
	CHECK_EQUAL(1, result);
	
	st1=0;
	result  =compareSizet(&st1, &st2);
	CHECK_EQUAL(0, result);
	
	st1=1; st2=1;
	result =compareSizet(&st1, &st2);
	CHECK_EQUAL(0, result);
	
	//============
	double db1=0, db2=1e10;
	result =compareDouble(&db1, &db2);
	CHECK_EQUAL(-1, result);
	
	db1=1e10, db2=0;
	result =compareDouble(&db1, &db2);
	CHECK_EQUAL(1, result);
	
	db1=-1, db2=-1e10;
	result =compareDouble(&db1, &db2);
	CHECK_EQUAL(1, result);
	
	db1=0, db2=0;
	result =compareDouble(&db1, &db2);
	CHECK_EQUAL(0, result);
	
	//==++++++++++++
	float fb1=0, fb2=1e10;
	result =compareFloat(&fb1, &fb2);
	CHECK_EQUAL(-1, result);
	printf("**succed!!!!**\n");
}

//create a test for that test group
TEST (rc_test, qsortint){
	printf("++++rc_test:testing ^qsortint^............");
	printf("\n\ttesting first array .........."); 
	int array[]={0,1,2,3,8,0} ;
	
	qsort(array, 6, sizeof(int), compareInt);
	int i=0;
	int expected[]={0,0,1,2,3,8};
	for(;i<6;i++)
	{
		//printf("--%d:%d",i, array[i]);
		CHECK_EQUAL(expected[i],array[i]);
	}
	printf("...***DONE\n");
	
	printf("\n\ttesting second array .........."); 
	int array2[]={0,0,0,2,3,0} ;
	
	qsort(array2, 6, sizeof(int), compareInt);
	i=0;
	int expected2[]={0,0,0,0,2,3,};
	for(;i<6;i++)
	{
		//printf("--%d:%d",i, array[i]);
		CHECK_EQUAL(expected2[i],array2[i]);
	}
	printf("...***DONE\n");
	
	printf("\n\ttesting third array .........."); 
	int array3[]= {-1,0,0,2,3,-100};
	
	qsort(array3, 6, sizeof(int), compareInt);
	i=0;
	int expected3[]={-100,-1,0,0,2,3};
	for(;i<6;i++)
	{
		printf("--%d:%d",i, array3[i]);
		CHECK_EQUAL(expected3[i],array3[i]);
	}
	printf("...***DONE\n");
	
	printf("\n\ttesting fourth array .........."); 
	int array4[]={0,0,0,0,0,-0} ;
	
	qsort(array4, 6, sizeof(int), compareInt);
	i=0;
	int expected4[]={0,0,0,0,0,-0};
	for(;i<6;i++)
	{
		//printf("--%d:%d",i, array[i]);
		CHECK_EQUAL(expected4[i],array4[i]);
	}
	printf("...***DONE\n");
	
	printf("\t*******succed!!!!**\n");
	
	printf("\t*************compare size***********");
}

//create a test for that test group
TEST (rc_test, compareStr){
	printf("+++++testing ^compareStr^............");
	const char* exp="exp";
	const char* ret ="ret";
	int rt=compareStr(&exp, &ret);
	cout<<"rt:"<<rt <<"(exp vs. ret)"<<endl;
    CHECK_TRUE(rt<0);

//this following code used to demonstrate array pointer vs pointer array 
// and casting 
	char xa[]="ACDE";   //c character array 
	cout<<"-------c character array xa:"<<xa<<endl;
	
	//Testing array pointer 
	char x[]={'A','B','C','\0'};
	char y[]={'D','E','F','\0'};
	
	//array pointer 
	char (*xap)[4]=&x;
	cout<<"\n---------printing the array pointer without casting:"<<(*xap)[0]<<endl;
	
	cout<<"can NOT print with casting \"(*((const char**)(&x)))\""<<endl;
//	cout<<"\ntesting the array pointer:"<<(*((const char**)(&x)))<<endl;

	cout<<" To \"cast\" array pointer to pointer to pointer, we need to \n"
		<< "we need to make a new pointer to char and then obtain the address of this new pointer\n"
		<<" therefore, we have a pointer to pointer "<<endl;
	const char * xloc=(x);
	char * yloc=&(y[0]);
	
	//xloc=(const char**)(&xa);
	cout<<"xloc :"<<*xloc<<endl;
	rt=compareStr(&yloc,&xloc);
	cout<<"rt:"<<rt <<"("<<y<<"	vs."<< x<<")"<<endl;	
    CHECK_TRUE(rt>0);

//testing pointer array 
	const char* xp[]={"AXp","B","C"};
	cout<<"---------print pointer array without casting :"<<xp[0][0]<<endl;
	cout<<"casting pointer array:"<<((char**)xp)[0][0]<<endl;
	//cout<<"testing x: "<<x<<endl;
	
	

	rt=compareStr(&yloc,&yloc);
	cout<<"rt:"<<rt<<"(comparing two "<<y<<")"<<endl;
    CHECK_TRUE(rt==0);
	
	printf("succed!!\n");
}

//create a test for that test group
TEST (rc_test, qsortStr){
	printf("++++testing ^qsortStr^............");
	const char*  str[]={"A", "X","XK", "XA", "XX"};
	size_t n=5;
	char ** strm=(char**)malloc(n*sizeof(char*));
	for(size_t  i=0;i<n;i++)
	{
		strm[i] =(char*)malloc(2*sizeof(char));
	}
	
	cout<<"\nbefore sorting"<<endl;
	for(size_t  i=0;i<n;i++)
	{
		cout<<"str :"<<i<<":"<<str[i]<<";";
		(strm[i][0])=(str[i][0]);
		strm[i][1]=0;
		//STRCMP_EQUAL("A", str[0]);
	}
	 
	//size_t rt=compareStr("exp", "ret");
	cout<<"\n--------------sort str-----------"<<endl;
	qsort(str, n, sizeof(char*), compareStr);
	
	cout<<"\n--------------sort strmmmmm-----------"<<endl;
	qsort(strm, n, sizeof(char*),compareStr);
	cout<<"after sorting........."<<endl;
	for(size_t i=0;i<n;i++)
	{
		cout<<"str :"<<i<<":"<<str[i]<<";";
		
		//STRCMP_EQUAL("A", str[0]);
	}
	cout<<endl;
	for(size_t i=0;i<n;i++)
	{
		cout<<"strm :"<<i<<":"<<strm[i]<<";";
		
		//STRCMP_EQUAL("A", str[0]);
	}
	printf("succed!!\n");
	for (size_t i=0;i<n;i++)
		free(strm[i]);
	
	free(strm);
}

//create a test for that test group
TEST (rc_test, unique){
	printf("++++testing ^unique^............");
	const char*  str[]={"A", "X","XK", "XA", "XX", "X", "A"};
	size_t n=7;
	
	//need to declare and initialize the output array
	
	char** str_nu=(char**)malloc(n*sizeof(char**));
	size_t nu=unique(str, n, sizeof(char*), compareStr, str_nu);
	
	cout<<"\ntotal return :"<<nu<<endl;
	CHECK_EQUAL(5, nu);
	const char* expStr[]={"A", "X", "XA", "XK", "XX"};
	
	for(size_t i=0; i<nu;i++)
	{
		STRCMP_EQUAL(expStr[i], str_nu[i]);
	}

	//check for int array
	int  in[]={1,2,2,3,4,7,7,4};
	size_t ni=8;
	int in_nu[8];
	size_t niu=unique(in, ni, sizeof(int), compareInt, in_nu);
	cout<<"total return "<<niu<<endl;
	
	
	int in_exp[]={1,2,3,4,7};
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	free(str_nu);
	
	cout<<"-----Succed!!!\n"<<endl;
}

//create a test for that test group
TEST (rc_test, setdiff_c){
	printf("++++testing ^setdiff_c^............");
	
	//check for size_t array
	size_t  in1[]={1,2,2,3,4,7,7,4};
	size_t in2 []={1,2};
	size_t ni1=8;
	size_t ni2=2;
	size_t in_nu[8];
	size_t niu=setdiff_c(in1, ni1, in2, ni2, sizeof(size_t), compareInt, in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	size_t in_exp[]={3,4,7};
	for(size_t i=0;i<niu;i++)
	{
		cout<<"i-"<<i<<":"<<in_nu[i]<<endl;
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	//check for opposite of above
	niu=setdiff_c(in2, ni2, in1, ni1, sizeof(size_t), compareInt, in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	
	CHECK_EQUAL(0, niu);
	/*in_exp[]={3,4,7};
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}*/
	
	//check for equal case
	niu=setdiff_c(in1, ni1, in1, ni1,  sizeof(size_t), compareInt, in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	
	CHECK_EQUAL(0, niu);
	
	//check for zero length array of the first one.
	niu=setdiff_c(NULL, 0, in1, ni1,  sizeof(size_t), compareInt,in_nu);
	cout<<"\n\t (zero case) total return "<<niu<<endl;
	
	CHECK_EQUAL(0, niu);
	
	//check for zero length array of the first one.
	niu=setdiff_c( in1, ni1, NULL, 0, sizeof(size_t), compareInt, in_nu);
	cout<<"\n\t (zero case) total return "<<niu<<endl;
	
	CHECK_EQUAL(5, niu);
	unique(in1, ni1, sizeof(size_t),compareInt,in1);
	for(size_t i=0;i<niu;i++)
	{
		cout<<"in1["<<i<<"]:"<<in1[i]<<";in_nu["<<i<<"]:"<<in_nu[i]<<endl;
		CHECK_EQUAL(in1[i], in_nu[i]);
	}
	
		//check for zero length array of the first one. sizet arrays
		cout<<"_________do zero entry second array test................"<<endl;
	niu=setdiff_c( in1, ni1, NULL, 0, sizeof(size_t), compareSizet, in_nu);
	cout<<"\n\t (zero case size_t array ) total return "<<niu<<endl;
	
	CHECK_EQUAL(5, niu);
	//free(str_nu);
	cout<<"-----Succed!!!\n"<<endl;
}


//create a test for that test group
TEST (rc_test, getElementByIndex){
	printf("++++testing ^getElementByIndex()^............");
	
	//check for size_t array
	size_t  in1[]={1,2,2,3,4,7,7,4};
	size_t in2 []={1,2};
	size_t ni1=8;
	size_t ni2=2;
	size_t in_nu[8];
	size_t niu=getElementByIndex(in1, ni1, sizeof(size_t), in2, ni2,  in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	size_t in_exp[]={2,2};
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	//Testing picking all 
	size_t in3[]={0,1,2,3,4,5,6,7};
	niu=getElementByIndex(in1, ni1, sizeof(size_t), in3, ni1,  in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in1[i], in_nu[i]);
	}
	
	//Testing out of range 
	in2[1]=9;
	niu=getElementByIndex(in1, ni1, sizeof(size_t), in2, ni2,  in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL((size_t)(-1), niu);
	/*for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in1[i], in_nu[i]);
	}*/
	
	//Testing empty source 
	niu=getElementByIndex(NULL, 0, sizeof(size_t), in2, ni2,  in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL((size_t)(-1), niu);
	
	//Testing empty index
	niu=getElementByIndex(in1, ni1, sizeof(size_t), NULL, 0,  in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL((0), niu);
	
	//Start doing characters/strings
		cout<<"\t-$$$$$start testing characters $$$$$$$$$$$"<<endl;
	const char* str[]={"AB", "C","E","KKK","aa"};
	size_t str_id[]={1,2};
	size_t str_id_n=2;
	size_t str_n=5;
	char* str_nu[5];
	
	niu=getElementByIndex(str, str_n, sizeof(char*), str_id, str_id_n,  str_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	for(size_t i=0;i<niu;i++)
	{
		STRCMP_EQUAL(str[str_id[i]], str_nu[i]);
	}
	
	size_t str_id2[]={4,3,2,1,0};
	str_id_n=5;
	niu=getElementByIndex(str, str_n, sizeof(char*), str_id2, str_id_n,  str_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
	for(size_t i=0;i<niu;i++)
	{
		cout<<"str_nu:"<<i<<"--"<<str_nu[i]<<endl;
		STRCMP_EQUAL(str[str_id2[i]], str_nu[i]);
	}
	
	/*size_t in_exp[]={3,4,7};
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	//check for opposite of above
	niu=setdiff_c_size_t(in2, ni2, in1, ni1,  in_nu);
	cout<<"\n\ttotal return "<<niu<<endl;
		*/
	//free(str_nu);
	cout<<"-----Succed!!!\n"<<endl;
}

//create a test for that test group
TEST (rc_test, getIndexOfElement){
	printf("++++testing ^getIndexOfElement()^............");
	
	//check for size_t array
	size_t  in1[]={1,2,2,3,4,7,7,4};
	size_t e=7;
	size_t ni1=8;
	
	size_t in_nu[8];
	size_t niu=getIndexOfElement(in1, ni1, sizeof(size_t), &e, compareInt,  in_nu, 'e');
	cout<<"\n\ttotal return "<<niu<<endl;
	size_t in_exp[8]={5,6,0,0,0,0,0,0};
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	//Testing the greater than case 
	   e=4;
	   niu=getIndexOfElement(in1, ni1, sizeof(size_t), &e, compareInt,  in_nu, 'g');
	cout<<"\n\ttotal return  (case of \'g\'):"<<niu<<endl;
	in_exp[0]=5;
	in_exp[1]=6;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	e=7;
	niu=getIndexOfElement(in1, ni1, sizeof(size_t), &e, compareInt,  in_nu, 'h');
	cout<<"\n\ttotal return  (case of \'h\'):"<<niu<<endl;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	//testing less than 
	niu=getIndexOfElement(in1, ni1, sizeof(size_t), &e, compareInt,  in_nu, 'l');
	cout<<"\n\ttotal return  (case of \'l\'):"<<niu<<endl;
	in_exp[0]=0;
	in_exp[1]=1;
	in_exp[2]=2;
	in_exp[3]=3;
	in_exp[4]=4;
	in_exp[5]=7;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	//testing less than and equal  
	niu=getIndexOfElement(in1, ni1, sizeof(size_t), &e, compareInt,  in_nu, 'm');
	cout<<"\n\ttotal return  (case of \'m\'):"<<niu<<endl;
	in_exp[0]=0;
	in_exp[1]=1;
	in_exp[2]=2;
	in_exp[3]=3;
	in_exp[4]=4;
	in_exp[5]=5;
	in_exp[6]=6;
	in_exp[7]=7;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	e=0;
	//testing less than and equal  
	niu=getIndexOfElement(in1, ni1, sizeof(size_t), &e, compareInt,  in_nu, 'm');
	cout<<"\n\ttotal return  (case of \'m\'):"<<niu<<endl;
	CHECK_EQUAL(0, niu);
	//check for string array
	cout<<"\t------------Testing string array-----------"<<endl;
	const char*  str1[]={"AB", "C","E","KKK","Feng", "Feng", "Chris","C"};
	const char*  se="Feng";
	//size_t ni1=8;
	
	//in_nu[8];
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se, compareStr,  in_nu,'e');
	cout<<"\n\ttotal return "<<niu<<endl;
	 in_exp[0]=4;
	 in_exp[1]=5;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	//now testing the string, but zero return 
	//memcpy(se, "Iris", 4*sizeof(char));
	const char* se2="Iris";
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se2, compareStr,  in_nu,'e');
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL(0,niu);
	
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se2, compareStr,  in_nu,'g');
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL(1,niu);
	in_exp[0]=3;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se2, compareStr,  in_nu,'h');
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL(1,niu);
	in_exp[0]=3;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se2, compareStr,  in_nu,'l');
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL(7,niu);
	in_exp[0]=0;
	in_exp[1]=1;
	in_exp[2]=2;
	in_exp[3]=4;
	in_exp[4]=5;
	in_exp[5]=6;
	in_exp[6]=7;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se2, compareStr,  in_nu,'m');
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL(7,niu);
	
	niu=getIndexOfElement(str1, ni1, sizeof(char*), &se2, compareStr,  in_nu,'n');
	cout<<"\n\ttotal return "<<niu<<endl;
	CHECK_EQUAL(8,niu);
	in_exp[3]=3;
	in_exp[4]=4;
	in_exp[5]=5;
	in_exp[6]=6;
	in_exp[7]=7;
	for(size_t i=0;i<niu;i++)
	{
		CHECK_EQUAL(in_exp[i], in_nu[i]);
	}
	cout<<"-----Succed!!!\n"<<endl;
}

//create a test for that test group
TEST (rc_test, intraClonalDiversity){
	printf("++++testing ^^intraClonalDiversity^^............");
	
	//check for size_t array
	struct sequence* seq=NULL;
	double idi=intraClonalDiversity( seq, 
														0,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//start checking for sequeneces
	struct sequence  seq1;//=(struct sequence*)malloc(1*sizeof(struct sequence));
	const char * name1="seq1";
	(seq1).readID=name1;
	seq1.numMu=1;
	seq1.positions=(size_t*)malloc(10*sizeof(size_t));
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	seq1.positions[3]=200;
	seq1.positions[4]=210;
	seq1.types=(const char**)malloc(10*sizeof(char*));
	const char* muType[5]={"Substitution","Substitution","Substitution","Substitution","Substitution"};
	seq1.types[0]=muType[0];
	seq1.types[1]=muType[1];
	seq1.types[2]=muType[2];
	seq1.types[3]=muType[3];
	seq1.types[4]=muType[4];
	seq1.totalVBase=285;
	seq1.xVBase=284;
	//----------test with one sequence.
	cout<<"\n****Testing one sequences:"<<endl; 
	idi=intraClonalDiversity( &seq1, 
														1,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(0, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//-------testing with two sequences
	struct sequence  seq2;//=(struct sequence*)malloc(1*sizeof(struct sequence));
	const char * name2="seq2";
	(seq2).readID=name2;
	seq2.numMu=1;
	seq2.positions=(size_t*)malloc(10*sizeof(size_t));
	seq2.positions[0]=20;
	seq2.positions[1]=30;
	seq2.positions[2]=43;
	seq2.positions[3]=200;
	seq2.positions[4]=210;
	seq2.types=(const char**)malloc(10*sizeof(char*));
	const char* muType2[5]={"Substitution","Substitution","Substitution","Substitution","Substitution"};
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[1];
	seq2.types[2]=muType2[2];
	seq2.types[3]=muType2[3];
	seq2.types[4]=muType2[4];
	seq2.totalVBase=285;
	seq2.xVBase=278;
	
	seq=(struct sequence*)malloc(5*sizeof(struct sequence));
	memcpy(seq,&seq1, sizeof(struct sequence));
	memcpy(seq+1,&seq2, sizeof(struct sequence));
	seq[2].readID="seq3";
	seq[2].numMu=3;
	seq[2].positions=(size_t*)malloc(10*sizeof(size_t));
	seq[2].positions[0]=20;
	seq[2].positions[1]=30;
	seq[2].positions[2]=43;
	seq[2].positions[3]=200;
	seq[2].positions[4]=210;
	seq[2].types=(const char**)malloc(10*sizeof(char*));
	const char* muType3[5]={"Substitution","Substitution","Substitution","Substitution","Substitution"};
	seq[2].types[0]=muType3[0];
	seq[2].types[1]=muType3[1];
	seq[2].types[2]=muType3[2];
	seq[2].types[3]=muType3[3];
	seq[2].types[4]="Substitution";
	seq[2].totalVBase=285;
	seq[2].xVBase=279;
	
	cout<<"seq[0].xVBase:"<<seq[0].xVBase<<"seq[1].xVBase:"<<seq[1].xVBase<<endl;
	cout<<"seq2.xVBase:"<<seq2.xVBase<<"; seq2.readID:\""<<seq2.readID<<"\"; seq[1].readID:\'"<<seq[1].readID<<"\'"<<endl;
	cout<<"seq[2].xVBase:"<<seq[2].xVBase<<"; seq3.readID:\""<<seq[2].readID<<"\"; seq[2].readID:\'"<<seq[2].readID<<"\'"<<endl;
	//cout<<"hello:"<<endl;
	fflush(stdout);
	cout<<"\n****Testing two sequences with identical mutation:"<<endl; 
	seq2.positions[0]=20;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(0, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence 
	cout<<"\n****Testing two sequences with non-identical mutation position:"<<endl; 
	seq2.positions[0]=30;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(2.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with zero mutation
	cout<<"\n****Testing two sequences with zero mutation:"<<endl; 
	seq2.positions[0]=30;
	seq[0].numMu=0;
	seq[1].numMu=0;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(0, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with one zero mutation
	cout<<"\n****Testing two sequences with one zero mutation sequence:"<<endl; 
	seq2.positions[0]=30;
	seq[0].numMu=1;
	seq[1].numMu=0;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(1.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with one zero mutation
	cout<<"\n****Testing two sequences with one zero mutation sequence 2 :"<<endl; 
	seq2.positions[0]=30;
	seq[0].numMu=0;
	seq[1].numMu=1;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(1.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with two identical mutations
	cout<<"\n****Testing two sequences with two identical mutation sequence :"<<endl; 
	seq2.positions[0]=20;
	seq[0].numMu=2;
	seq[1].numMu=2;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(0, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 1 identical mutations 1 non identical
	cout<<"\n****Testing two sequences with 1 non-identical and 1 identical  mutation sequence :"<<endl; 
	seq2.positions[0]=21;
	seq[0].numMu=2;
	seq[1].numMu=2;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(2.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 2 non identical
	cout<<"\n****Testing two sequences with two non-identical mutation sequence :"<<endl; 
	seq2.positions[0]=21;
	seq2.positions[1]=31;
	seq[0].numMu=2;
	seq[1].numMu=2;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(4.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 2 non identical 1identical
	cout<<"\n****Testing two sequences with two non-identical and 1 identical mutation sequence :"<<endl; 
	seq2.positions[0]=21;
	seq2.positions[1]=31;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(4.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with one sequence mutation not covered on sequence of the second 
	cout<<"\n****Testing two sequences with one sequence mutation not covered on sequence of the second :"<<endl; 
	seq1.positions[0]=1;
	seq1.positions[1]=31;
	seq1.positions[2]=43;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(3.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 3 sequence mutation not covered on sequence of the second 
	cout<<"\n****Testing two sequences with 3 sequence mutation not covered on sequence of the second :"<<endl; 
	seq1.positions[0]=1;
	seq1.positions[1]=2;
	seq1.positions[2]=6;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(3.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 3 sequence mutation AND ZERO ON  the second 
	cout<<"\n****Testing two sequences with 3 sequence mutation not covered and zero on the second :"<<endl; 
	seq1.positions[0]=1;
	seq1.positions[1]=2;
	seq1.positions[2]=6;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=0;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
	CHECK_EQUAL(3.0/278, idi);
	
	//------------------------------------------------Test two sequence with 3 sequence mutations but not covered on the  second 
	cout<<"\n****Testing two sequences with 3 mutation not covered and zero on the second (switched):"<<endl; 
	seq1.positions[0]=1;
	seq1.positions[1]=2;
	seq1.positions[2]=6;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=0;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);
							
		//------------------------------------------------Test two sequence with 3 sequence mutations but two not covered on the  second
			//      and one mutation is right in the begining of the sequence 
	cout<<"\n****Testing two sequences with 3 sequence mutation two not covered and two on the begining and zero on the second :"<<endl; 
	seq1.positions[0]=1;
	seq1.positions[1]=2;
	seq1.positions[2]=7;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=0;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(1.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 3 sequence mutations but one not covered on the  second
			//      and one mutation is right in the begining of the sequence 
	cout<<"\n****Testing two sequences with one sequence one on the beginning of the second  and 4 mutation s on the second :"<<endl; 
	seq1.positions[0]=8;
	seq1.positions[1]=10;
	seq1.positions[2]=100;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=4;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(7.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 3 sequence sustitution but one not covered on the  second
			//      and one mutation is right in the begining of the sequence 
	cout<<"\n****Testing two sequences with one sequence mutation not covered and two on the begining and 4 mutatio sequence of the second :"<<endl; 
	seq1.positions[0]=6;
	seq1.positions[1]=7;
	seq1.positions[2]=8;
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(5.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;
	
	//------------------------------------------------Test two sequence with 3 sequence mutations but one not covered on the  second
			//      and one mutation is right in the begining of the sequence 
	cout<<"\n****Testing two sequences with one sequence Insertion  :"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	seq2.types[0]=muType2[0];
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(3.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;

	//------------------------------------------------Test two sequence with 3 sequence Insertion
			//      and the other sequence with substitution 
	cout<<"\n****Testing two sequences with 3 Insertion  and 3 sub on the other :"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(9.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;

	//------------------------------------------------Test two sequence with 3 identical sequence Insertion
	cout<<"\n****Testing two sequences with 3 Insertion  and 3 sub on the other :"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(0.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;	

	//------------------------------------------------Test two sequence with 3 identical sequence Insertion
	cout<<"\n****Testing two sequences with 2 identical Insertions  and one insertion not cover ed and one good  on the other :"<<endl; 
	seq1.positions[0]=6;
	seq1.positions[1]=7;
	seq1.positions[2]=8;
	muType2[0]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq2.positions[0]=7; //restore the original setting
	seq2.positions[1]=8;//restore the original setting
	seq2.positions[2]=9;
	seq[0].numMu=3;
	seq[1].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														2,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(2.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;	
	
	
	//------------------------------------------------Test 3 sequence with 3 identical sequence sub
	cout<<"\n****Testing3sequences with 3 identical sub"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Substitution";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(0.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;	
	
	//------------------------------------------------Test 3 sequence with 3 identical sequence insertion
	cout<<"\n****Testing3sequences with 3 identical insertion"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq[2].types[0]=muType2[0];
	seq[2].types[1]=muType2[0];
	seq[2].types[2]=muType2[0];
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(0.0/278, idi);
	cout<<"\n\tidi: "<<idi<<endl;	
	
	//------------------------------------------------Test 3 sequence with 2 identical sequence substitution and 1 non-identical
	cout<<"\n****Testing3sequences with 2 identical insertion and 1 non-identical"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Substitution";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq[2].types[0]=muType2[0];
	seq[2].types[1]=muType2[0];
	seq[2].types[2]=muType2[0];
	
	seq2.positions[0]=10; //restore the original setting
	seq2.positions[1]=11;//restore the original setting
	seq2.positions[2]=12;
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[0].readID<<"; seq[1].positions[0]:"<<seq[0].positions[0]<<"; seq[1].type[0]:"<<seq[0].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[2].readID<<"; seq[1].positions[0]:"<<seq[2].positions[0]<<"; seq[1].type[0]:"<<seq[2].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL(12.0/278/3, idi);
	cout<<"\n\tidi: "<<idi<<endl;	
	
		//------------------------------------------------Test 3 sequence with non-identical sequence substitution
	cout<<"\n****Testing 3 sequences with 3 non-identical"<<endl; 
	seq1.positions[0]=28;
	seq1.positions[1]=38;
	seq1.positions[2]=48;
	muType2[0]="Substitution";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq[2].types[0]=muType2[0];
	seq[2].types[1]=muType2[0];
	seq[2].types[2]=muType2[0];
	
	seq2.positions[0]=10; //restore the original setting
	seq2.positions[1]=11;//restore the original setting
	seq2.positions[2]=12;
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[0].readID<<"; seq[1].positions[0]:"<<seq[0].positions[0]<<"; seq[1].type[0]:"<<seq[0].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[2].readID<<"; seq[1].positions[0]:"<<seq[2].positions[0]<<"; seq[1].type[0]:"<<seq[2].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL((6.0/278+6.0/278+6.0/279)/3, idi);
	cout<<"\n\tidi: "<<idi<<endl;	
	
	
		//------------------------------------------------Test 3 sequence with identical sequence deletion
	cout<<"\n****Testing3sequences with  identical deletion"<<endl; 
	seq1.positions[0]=20;
	seq1.positions[1]=30;
	seq1.positions[2]=43;
	muType2[0]="Deletion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	//muType2[2]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq[2].types[0]=muType2[0];
	seq[2].types[1]=muType2[0];
	seq[2].types[2]=muType2[0];
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=43;
	
	seq[2].positions[0]=20; //restore the original setting
	seq[2].positions[1]=30;//restore the original setting
	seq[2].positions[2]=43;
	
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[0].readID<<"; seq[1].positions[0]:"<<seq[0].positions[0]<<"; seq[1].type[0]:"<<seq[0].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[2].readID<<"; seq[1].positions[0]:"<<seq[2].positions[0]<<"; seq[1].type[0]:"<<seq[2].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL((0.0/278+0.0/278+0.0/279)/3, idi);
	cout<<"\n\tidi: "<<idi<<endl;	

				//------------------------------------------------Test 3 sequence with non-identical sequence deletion
	cout<<"\n****Testing 3 sequences with  non-identical deletion"<<endl; 
	seq1.positions[0]=6;
	seq1.positions[1]=30;
	seq1.positions[2]=23;
	muType2[0]="Deletion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[2]="Substitution";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[0];
	seq2.types[2]=muType2[0];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[0];
	seq1.types[2]=muType2[0];
	
	seq[2].types[0]=muType2[0];
	seq[2].types[1]=muType2[0];
	seq[2].types[2]=muType2[0];
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=49;
	
	seq[2].positions[0]=20; //restore the original setting
	seq[2].positions[1]=30;//restore the original setting
	seq[2].positions[2]=43;
	
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[0].readID<<"; seq[1].positions[0]:"<<seq[0].positions[0]<<"; seq[1].type[0]:"<<seq[0].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[2].readID<<"; seq[1].positions[0]:"<<seq[2].positions[0]<<"; seq[1].type[0]:"<<seq[2].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL((6.0/278+8.0/279+4.0/278)/3, idi);
	cout<<"\n\tidi: "<<idi<<endl;	

				//------------------------------------------------Test 3 sequence with non-identical sequence mixed mutation
	cout<<"\n****Testing 3 sequences with  non-identical mixed mutation"<<endl; 
	seq1.positions[0]=4;
	seq1.positions[1]=30;
	seq1.positions[2]=23;
	muType2[0]="Deletion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[1]="Insertion";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	muType2[2]="Substitution";//{"Insertion","Substitution","Substitution","Substitution","Substitution"};
	
	seq2.types[0]=muType2[0];
	seq2.types[1]=muType2[1];
	seq2.types[2]=muType2[2];
	
	seq1.types[0]=muType2[0];
	seq1.types[1]=muType2[1];
	seq1.types[2]=muType2[2];
	
	seq[2].types[0]=muType2[0];
	seq[2].types[1]=muType2[1];
	seq[2].types[2]=muType2[2];
	
	seq2.positions[0]=20; //restore the original setting
	seq2.positions[1]=30;//restore the original setting
	seq2.positions[2]=49;
	
	seq[2].positions[0]=20; //restore the original setting
	seq[2].positions[1]=30;//restore the original setting
	seq[2].positions[2]=43;
	
	seq[0].numMu=3;
	seq[1].numMu=3;
	seq[2].numMu=3;
	cout<<"Seq[1].readID:"<<seq[0].readID<<"; seq[1].positions[0]:"<<seq[0].positions[0]<<"; seq[1].type[0]:"<<seq[0].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[1].readID<<"; seq[1].positions[0]:"<<seq[1].positions[0]<<"; seq[1].type[0]:"<<seq[1].types[0]<<endl;
	cout<<"Seq[1].readID:"<<seq[2].readID<<"; seq[1].positions[0]:"<<seq[2].positions[0]<<"; seq[1].type[0]:"<<seq[2].types[0]<<endl;
	idi=intraClonalDiversity( seq, 
														3,  //# of sequenecs fo 
														2.0   //penalty or score for indel
							);					
	CHECK_EQUAL((4.0/278+4.0/279+2.0/278)/3, idi);
	cout<<"\n\tidi: "<<idi<<endl;	
	
	free(seq1.positions);
	free(seq1.types);
	free(seq2.positions);
	free(seq2.types);
	free(seq[2].types);
	free(seq[2].positions);
	free(seq);
	cout<<"-----Succed!!!\n"<<endl;
}

