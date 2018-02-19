#include "stdafx.h"
#include <fstream>
using std::ios;
using std::ifstream;
using std::ofstream;
#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
#include "IN1NN.h"

IN1NN::IN1NN(const int C, const int dim, const int num_in, int Interval_size, double ro, int pop)
	:Class(C), dimension(dim), num_input(num_in), 
	FINsize(Interval_size), size(0), population(pop)
{
	setRo(ro);
	w=new double**[population];
	pts=new double **[population];
	val=new double **[population];
	select=new int[num_input];
	Mean=new double*[population];
	data=new double*[num_input];
	Bestpts=new double *[num_input];
	for(int i=0;i<num_input;i++)
		Bestpts[i]=new double[dimension];

	for(int i=0;i<population;i++)
	{
		w[i]=new double*[Class*FINsize];
		pts[i]=new double *[num_input];
		val[i]=new double *[num_input];
		Mean[i]=new double[dimension-1];
	}
	
	for(int i=0;i<population;i++)
		for(int j=0;j<num_input;j++)
		{
			pts[i][j]=new double[dimension];
			val[i][j]=new double[dimension];
		}
	for(int i=0;i<population;i++)
		for(int j=0;j<num_input;j++)
			for(int k=0;k<dimension;k++)
			{
				pts[i][j][k]=0;
				val[i][j][k]=0;
			}

	for(int i=0;i<population;i++)
		for(int j=0;j<Class*FINsize;j++)
			w[i][j]=new double[dimension];
	for(int i=0;i<population;i++)
		for(int j=0;j<Class*FINsize;j++)
			for(int k=0;k<dimension;k++)
				w[i][j][k]=0;
					
	for(int i=0;i<num_input;i++)
		data[i]=new double[dimension];
	for(int i=0;i<num_input;i++)
	{
		select[i]=0;
		for(int j=0;j<dimension;j++)
			data[i][j]=0;
	}
	for(int i=0;i<population;i++)
		for(int j=0;j<dimension;j++)
			Mean[i][j]=0;
}
void IN1NN::setRo(double ro)
{
	Ro=ro;
}
void IN1NN::Read_file(char *Fileaddress)
{
	ifstream infile(Fileaddress, ios::in);
	if(!infile)
	{
		cerr<<Fileaddress<<" could not be open\n" ;
		system("pause");
		exit(1);
	}

	for(int i=0;i<num_input;i++)
		for(int j=0;j<dimension;j++)
			infile>>data[i][j];
}
void IN1NN::bubblesort1(double ***vector)
{
	for(int i=0;i<population;i++)
		for(int j=0;j<dimension-1;j++)
			for(int c=0;c<Class;c++)
				for(int l=FINsize-1;l>=0;l--)
					for(int k=0;k<l;k++)
						if (vector[i][c*FINsize+k][j]>vector[i][c*FINsize+k+1][j])
						{
							double temp=vector[i][c*FINsize+k][j];
							vector[i][c*FINsize+k][j]=vector[i][c*FINsize+k+1][j];
							vector[i][c*FINsize+k+1][j]=temp;
						}

}

void IN1NN::bubblesort2(double ***vector)
{
	int data_count=size/Class;
	for(int i=0;i<population;i++)
		for(int j=0;j<dimension-1;j++)
			for(int c=0;c<Class;c++)
				for(int l=data_count-1;l>=0;l--)
					for(int k=0;k<l;k++)
						if(vector[i][c*data_count+k][j]>vector[i][c*data_count+k+1][j])
						{
							double temp=vector[i][c*data_count+k][j];
							vector[i][c*data_count+k][j]=vector[i][c*data_count+k+1][j];
							vector[i][c*data_count+k+1][j]=temp;
						}
}
void IN1NN::train()
{
	
	ofstream outfile("d:\\population.txt", ios::app);
	if(!outfile)
	{
		cerr<<"d:\\population.txt"<<" could not be open\n" ;
		system("pause");
		exit(1);
	}

	for(int p=0;p<population;p++)
	{
		int k=0;
		for(int c=0;c<Class;c++)
		{
			k=c*FINsize;
			int count=0,i=0;
			while( (i<num_input) && (count<FINsize) )
			{
				if(select[i]==0)
					if(data[i][dimension-1]==c)
					{
						for(int j=0;j<dimension;j++)
						{w[p][k][j]=data[i][j];outfile<<w[p][k][j]<<"\t";}
						outfile<<"\n";
						++count;
						select[i]=1;
						k++;
					}
				i++;
			}
		}
	}
}
void IN1NN::calFIN(int low, int high)
{
	if( (high-low)<1)  
		return;
	for(int i=0;i<population;i++)
		for(int l=0;l<dimension-1;l++)
			pts[i][size][l]=(w[i][(high-low)/2+low][l]+w[i][low+(high-low+1)/2][l])/2;
	size++;
	calFIN(low,low+(high-low-1)/2);
	calFIN(low+(high-low)/2+1,high);
}
void IN1NN::val_assigning()
{
	//vector val is initialised
	double *sum=new double[Class]; 
	double *step=new double[Class];
	for(int i=0;i<Class;i++)
	{sum[i]=0;step[i]=0;}
	int data_count=size/Class;

	for(int i=0;i<population;i++)
		for(int c=0;c<Class;c++)
			for(int m=0;m<dimension-1;m++)
			{
				sum[c]=0;
				step[c]=2.0/(data_count+1);
				for(int l=0;l<data_count;l++)			
				{
					sum[c]+=step[c];
					if(sum[c]>=1)
					{
						val[i][c*data_count+l][m]=1;
						step[c]=-step[c];
					}
					else
						val[i][c*data_count+l][m]=sum[c]; 
				}
			}
			delete [] sum;
			delete [] step;
}
void IN1NN::makeFIN()
{
	bubblesort1(w);
	for(int c=0;c<Class;c++)
	{
		//size=0;
		calFIN(c*FINsize,(c+1)*FINsize-1);
	}
	bubblesort2(pts);                       
	val_assigning();
}
double IN1NN::Max(double a, double b)
{
	if(a>=b)
		return a;
	else
		return b;
}
double IN1NN::Min(double a, double b)
{
	if(a<=b)
		return a;
	else
		return b;
}
double IN1NN::v(double x, double mu)
{
	return 1/(1+exp(-(x-mu)));
}
double IN1NN::theta(double mu, double x)
{
	return 2*mu-x;
}
double IN1NN::v(double x)
{
	return x;
}
double IN1NN::theta(double x)
{
	return 1-x;
}
/*int IN1NN::trivialcalculate(int p)
{
	for(int p=0;p<population;p++)
		for(int i=0;i<dimension-1;i++)
			for(int j=0;j<size;j++)
				Mean[p][i]+=pts[p][j][i];
	for(int p=0;p<population;p++)
		for(int i=0;i<dimension-1;i++)
			Mean[p][i]/=size;

	int correct=0;
	for(int i=0;i<num_input;i++)
		if(select[i]==1)
		{
			int winner;
			double min=10000;
			for(int c=0;c<Class;c++)
			{
				double distance=0;
				for(int k=0;k<dimension-1;k++)
					distance+=+v(Max(data[i][k], w[p][c][k]), Mean[p][k])-v(Min(data[i][k], w[p][c][k]), Mean[p][k]);
				distance/=dimension;
				if(distance<min)
				{
					min=distance;
					winner=c;
				}
			}
			if(min<=Ro)
				if(data[i][dimension-1]==w[p][winner][dimension-1])
				correct++;
		}
			return correct;
}*/	

int IN1NN::calculate(int pop)
{ 
	int correct=0;
	const int L=33; // L can equal to 16 an 32 for cutting FINs
	double **distance=new double*[L];
	for(int i=0;i<L;i++)
		distance[i]=new double[dimension-1];
	int data_count=size/Class;
	
	/*for(int p=0;p<population;p++)
		for(int i=0;i<dimension-1;i++)
			for(int j=0;j<size;j++)
				Mean[p][i]+=pts[p][j][i];
	for(int p=0;p<population;p++)
		for(int i=0;i<dimension-1;i++)
			Mean[p][i]/=size;*/

	for(int i=0;i<num_input;i++)
	{
		if(select[i]==1)
		{
			int winner=-1;
	
			for(int i=0;i<L;i++)
				for(int j=0;j<dimension-1;j++)
					distance[i][j]=0;
			double min=1000000;
			for(int c=0;c<Class;c++)
			{
				double totaldistance=0;
				for(int k=0;k<dimension-1;k++)
				{
					double min1=0, max1=0;
					for(int h=1;h<L;h++)						
					{
						for(int l=0;l<data_count;l++)
							if( (val[pop][c*data_count+l][k]>=(double)h/(L-1)))
							{
								min1=pts[pop][c*data_count+l][k];
								break;
							}
				    	for(int l=data_count-1;l>0;l--)
					    	if( (val[pop][c*data_count+l][k]>=(double)h/(L-1)) )
						    {
							    max1=pts[pop][c*data_count+l][k];
							    break;
					     	}
						//distance[h][k]=v(theta(Min(data[i][k], min1), Mean[pop][k]), Mean[pop][k])-v(theta(Max(data[i][k], min1), Mean[pop][k]), Mean[pop][k])+v(Max(data[i][k], max1), Mean[pop][k])-v(Min(data[i][k], max1), Mean[pop][k]);
							distance[h][k]=v(theta(Min(data[i][k], min1)))-v(theta(Max(data[i][k], min1)))+v(Max(data[i][k], max1))-v(Min(data[i][k], max1));
					//	cout<<min1<<"\t"<<max1<<"\t"<<distance[h][k]<<"\n";
					}
				}
				for(int h=1;h<L;h++)
					for(int k=0;k<dimension-1;k++)
						totaldistance+=pow((double)distance[h][k], 2.0);
				totaldistance=sqrt(totaldistance)/dimension;
				if(totaldistance<min)
				{
					min=totaldistance;
					winner=c;
				}
				
			}
			//if(min<=Ro)
				if(data[i][dimension-1]==winner)
					correct++;
		}
	}
	//size=0;            
	delete [] distance;
	return correct;
}
	
double IN1NN::test(int p)
{
	int correct=0;
	const int L=33; // L can equal to 16 an 32 for cutting FINs
	double **distance=new double*[L];
	for(int i=0;i<L;i++)
		distance[i]=new double[dimension-1];
	int data_count=size/Class;
		
	
	for(int i=0;i<num_input;i++)
	{
		if(select[i]==0)
		{
			int winner=0;
			for(int i=0;i<L;i++)
				for(int j=0;j<dimension-1;j++)
					distance[i][j]=0;
			double min=1000000;
			for(int c=0;c<Class;c++)
			{
				double totaldistance=0;
				for(int k=0;k<dimension-1;k++)
				{
					double min1=0, max1=0;
					for(int h=1;h<L;h++)						
					{
						for(int l=0;l<data_count;l++)
							if( (val[p][c*data_count+l][k]>=(double)h/(L-1)))
							{
								min1=pts[p][c*data_count+l][k];
								break;
							}
				    	for(int l=data_count-1;l>0;l--)
					    	if( (val[p][c*data_count+l][k]>=(double)h/(L-1)) )
						    {
							    max1=pts[p][c*data_count+l][k];
							    break;
					     	}
						//distance[h][k]=v(theta(Min(data[i][k], min1), Mean[p][k]), Mean[p][k])-v(theta(Max(data[i][k], min1), Mean[p][k]), Mean[p][k])+v(Max(data[i][k], max1), Mean[p][k])-v(Min(data[i][k], max1), Mean[p][k]);
						distance[h][k]=v(theta(Min(data[i][k], min1)))-v(theta(Max(data[i][k], min1)))+v(Max(data[i][k], max1))-v(Min(data[i][k], max1));
						//cout<<min1<<"\t"<<max1<<"\t"<<distance[h][k]<<"\n";
					}
					
				}
				for(int h=1;h<L;h++)
					for(int k=0;k<dimension-1;k++)
						totaldistance+=pow((double)distance[h][k], 2.0);
				totaldistance=sqrt(totaldistance)/dimension;
				if(totaldistance<min)
				{
					min=totaldistance;
					winner=c;
				}

			}
			//if(min<=Ro)
				if(data[i][dimension-1]==winner)
					correct++;
		}
	}
	delete [] distance;
	return correct;
	
}
void IN1NN::GSA()
{
	idx=0;
	M=new double[population];
	fit=new double[population];
	A=new double**[population];
	V=new double**[population];
	F=new double**[population];
	
	for(int i=0;i<population;i++)
	{
		M[i]=0; fit[i]=0;
		A[i]=new double*[size];
		V[i]=new double*[size];
		F[i]=new double*[size];
	}
	for(int i=0;i<population;i++)
		for(int j=0;j<size;j++)
		{
			A[i][j]=new double[dimension];
			V[i][j]=new double[dimension];
			F[i][j]=new double[dimension];
		}
		for(int i=0;i<population;i++)
			for(int j=0;j<size;j++)
				for(int k=0;k<dimension;k++)
				{
					A[i][j][k]=0;
					V[i][j][k]=0;
					F[i][j][k]=0;
				}
}
void IN1NN::set_Epsilon(double epsilon)
{
	Epsilon=epsilon;
}
void IN1NN::set_G(double G0)
{
	G=G0;
}
void IN1NN::Mass()
{
	////////// Prints Results in a File ///////////

	ofstream outfile("d:\\INKNN_Results.txt", ios::app);
	if(!outfile)
	{
		cerr<<"d:\\INNN_Results.txt"<<" could not be open\n" ;
		system("pause");
		exit(1);
	}



	double worst=1000000;
	double sum=0;
	for(int i=0;i<population;i++)
	{
		fit[i]=calculate(i);     //fit[i]=trivialcalculate(i) or test(i);
		if(fit[i]<worst)
			worst=fit[i];
	}
	
	//Sort
	double **temp2=new double*[size];
	for(int i=0;i<size;i++)
		temp2[i]=new double[dimension];
	
	for(int l=0;l<population-1;l++)
		for(int k=0;k<population-1;k++)
			if(fit[k]>fit[k+1])
			{
				double temp1=fit[k];
				fit[k]=fit[k+1];
				fit[k+1]=temp1;

				for(int j=0;j<size;j++)
					for(int i=0;i<dimension;i++)
					{
						temp2[j][i]=pts[k][j][i];
						pts[k][j][i]=pts[k+1][j][i];
						pts[k+1][j][i]=temp2[j][i];
					}
			}
	static int iteration=0;
	
	if(iteration==0)
	{
		for(int j=0;j<size;j++)
			for(int i=0;i<dimension;i++)
				Bestpts[j][i]=pts[population-1][j][i];
		Bestfit=fit[population-1];
	}
	else if(fit[population-1]<Bestfit)
	{
		for(int j=0;j<size;j++)
			for(int i=0;i<dimension;i++)
				pts[population-1][j][i]=Bestpts[j][i];
		fit[population-1]=Bestfit;
	}
	else if(fit[population-1]>Bestfit)
	{
		for(int j=0;j<size;j++)
			for(int i=0;i<dimension;i++)
				Bestpts[j][i]=pts[population-1][j][i];
		Bestfit=fit[population-1];
	}


	++iteration;
	if((iteration%20)==0)
		idx++;
	
	
	//////////// print in a file and on the screen ////////
	if(iteration==1)
	{
		cout<<"Pop"<<"\t"<<"fit"<<"\t"<<"test"<<"\n";              
		outfile<<"Pop"<<"\t"<<"fit"<<"\t"<<"test"<<"\n"; 
	}
	outfile<<"Iteration="<<iteration<<"\n";
	for(int i=0;i<population;i++)
	{
		cout<<i+1<<"\t"<<fit[i]<<"\t"<<test(i)<<"\n";                             //fit=correct classification on training data
	    outfile<<i+1<<"\t"<<fit[i]<<"\t"<<test(i)<<"\n";                          //test=correct classification on testing data
		//system("pause");
	}
			
	for(int i=0;i<population;i++)
		sum+=fit[i]-worst;
	for(int i=0;i<population;i++)
		M[i]=(fit[i]-worst)/sum;
	delete [] temp2;
}
double IN1NN::dis(int index1, int index2)
{
	double distance=0;
	for(int i=0;i<dimension-1;i++)
		for(int j=0;j<size;j++)
			distance+=pow((double)(pts[index1][j][i]-pts[index2][j][i]),2.0);  
		       //distance+=+v(Max(w[index1][j][i], w[index2][j][i]), (Mean[index1][i]+Mean[index2][i])/2.0 )-v(Min(w[index1][j][i], w[index2][j][i]), (Mean[index1][i]+Mean[index2][i])/2.0);
	return sqrt(distance);      //        /(dimension);
}
void IN1NN::update(int t, int T)
{
	//G=G*(1-(double)t/T);
	G=G*exp(-10*(double)(t/T));

	for(int i=0;i<population;i++)
			for(int j=0;j<size;j++)
				for(int k=0;k<dimension;k++)
					F[i][j][k]=0;

	for(int i=0;i<population;i++)
		for(int j=0;j<size;j++)
			for(int d=0;d<dimension-1;d++)
				for(int p=0;p<population;p++)
					if(p>=idx)
					{
						if(i!=p)
						{
							double r=(rand()%1000)/1000.0;
							F[i][j][d]+=r*G*M[i]*M[p]*(pts[p][j][d]-pts[i][j][d])/(dis(i,p)+Epsilon);
						}
					}
	for(int i=0;i<population;i++)
		for(int j=0;j<size;j++)
			for(int d=0;d<dimension-1;d++)
				A[i][j][d]=F[i][j][d]/(M[i]+.000001);
	for(int i=0;i<population;i++)
		for(int j=0;j<size;j++)
		{
			double r=(rand()%1000)/1000.0;
			for(int d=0;d<dimension-1;d++)
				V[i][j][d]=r*V[i][j][d]+A[i][j][d];
		}

	for(int i=0;i<population;i++)
		for(int j=0;j<size;j++)
			for(int d=0;d<dimension-1;d++)
				pts[i][j][d]=pts[i][j][d]+V[i][j][d];
	
	//bubblesort2(pts);
}
