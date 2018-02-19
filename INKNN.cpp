// INKNN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
using std::ios;
using std::ifstream;
using std::ofstream;
#include <iostream>
using std::cin;
using std::cout;
#include "IN1NN.h"


int _tmain(int argc, _TCHAR* argv[])
{

	//classes in the data set should begin from zero 0, number of data
	//means INterval_size for the trivial fin should be equal to one 1 and
	//for the NN.claculate should be larger than one 1
	const int C=2;                     //number of classes
	const int dim=4;                  //dimension 
	const int num_in=303;             // number of the data in the original file
	int Interval_size=5;            //number of vectors that a FIN contain
	double ro=10000;              // assimilation parameter                              //not necessary
	int pop=10;        //initial population of chromosome
	int t=0;         //iteration counter
	int T=100;         //total iteration
	IN1NN NN(C, dim, num_in, Interval_size, ro, pop);
	NN.set_Epsilon(.00001);
	NN.set_G(1);

	NN.Read_file("D:\\1.txt");
	NN.train();
    NN.makeFIN();
	NN.GSA();

	while(t<T)
	{
		NN.Mass();
		NN.update(t, T);
		t++;
		cout<<"t= "<<t<<"\n";
		//system("pause");
	}
	
	system("pause");
	return 0;
}

