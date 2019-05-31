#ifndef __DEVELOP_H_
#define __DEVELOP_H_

#include"Mask.h"
#include"Orient.h"

#define SGMP 0.269
#define MSZ  7
#define PROD SGMP*MSZ

#define ON 0
#define OF 1

struct Connect{
	char ** connect ;
};

class Develop  : public Orient
{

protected :
	
	double ** dimage;		//Actual Image
	unsigned char ** image;		//Actual Image
	double ** onimage; 
	double ** ofimage;
	Connect ** connects ;
	double **E ;
	int wx, wy;			// Winner coords.
	double sN;			// Neighborhood parameter..
	double sNi,sNf;			// Neighborhood parameter i & f..
	double T ;			// Theta value of current sinusoid
	double F ;			// Frequency for Sinusoid
	double phi ;		// Phase of Sinusoid
	double Fmin, Fmax ;

public :
	
	Develop();
	void LoadParams (char * = NULL);
	void LoadStatus (char * = NULL);
	void Start();	// Normal Development.
	void Start_Sin(); // Miller's with Sinusoidal correlation.


protected :

	void assignspace();
	void generateMask(double ** Msk,int MODE);
	double scxx(int,int);
	double scxy(int,int);
	double Scxx(int,int);
	double Scxy(int,int);
	void generateImage();
	void operateonImage(double **, int, int);
	void computemasked ();
	void findWinner();
	void findwinner();
	void findExcitations();
	double findOutput(int x, int y);
	double productat (int x, int y);
	void computeUWtChange();
	void uOnWtChange(int xx, int xy, int ax, int ay);
	void uOfWtChange(int xx, int xy, int ax, int ay);
	void computeUWtChange_Sin();
	void uOnWtChange_Sin(int xx, int xy, int ax, int ay);
	void uOfWtChange_Sin(int xx, int xy, int ax, int ay);
	void adjustNeighbor(int, int);
	double prcnt(int,int);
	void storerf();
	double comprod(int,int);
	void adjustsynapses();
	double perc();
	void saveStatus();
};

#endif
