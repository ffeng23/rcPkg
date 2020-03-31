#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"
#include <string.h>
#include <iostream>
using namespace std;
/** Test main as usual */
int main(int ac, char** av)
{
	char  x[]= "hello world!";
	char y[]="hello xS!";
	
	int c=strcmp(x,x);
	cout<<"string comp results:"<<c<<"; w:"<<(int)'w'<<"v:"<<(int)'v'<<endl;
	cout<<x<<"; "<<y<<"y"<<endl;
    return RUN_ALL_TESTS(ac, av);
}