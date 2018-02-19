class IN1NN
{
public:
	IN1NN(const int, const int, const int, const int, double, int);
	void Read_file(char *);
	void bubblesort1(double ***);    //for w vector
	void bubblesort2(double ***);    // for pts vector
	void calFIN(int, int);
	void val_assigning();
	void makeFIN();
	void train();                  // it initialize w
	double Max(double, double);
	double Min(double, double); 
	int trivialcalculate(int);   //train the network with trivial FINs
	int calculate(int);                //train the network b calculating FINs
	void setRo(double);
	double v(double, double);
	double theta(double, double);
	double v(double);
	double theta(double);
	//////////////////////////
	//////GSA function///////
	////////////////////////
	void GSA();
	void set_Epsilon(double);
	void set_G(double);
	void Mass();
	double dis(int, int);
	void update(int , int);
	void calculate(double **, int);
	double test(int);

private:
	const int Class;          //number of classes
	const int dimension;      // dimension of the input data
	const int num_input;      // number of input data
	int FINsize;              //number  of data to make a FIN
	double **data;             //input data
	double ***w;                // weight of FINs
	double ***pts;
	double ***val;
	int *select;             // 1 means a the corresponding vector in the original data set has been selected an 0 means not selected
	int size;
	double Ro;
	double *KA;
	double *landa;
	double **Mean;            //mean of the selected data to train the net for each dimensional is calculated seprately
	////////////////////
	////GSA Parameter///
	///////////////////
	int population;             //initial population
	int T;
	double G;               // Gravitational constant
	double Epsilon;          // a small constant
	double *M;              // math
	double *fit;            // fitnes off each math
	double ***F;             //Force
	double ***A;              //Acceleration
	double ***V;               //Velocity
	int idx;
	double Bestfit;
	double **Bestpts;
};
