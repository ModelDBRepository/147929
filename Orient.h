#ifndef __ORIENT_H__
#define __ORIENT_H__

#include<fstream.h>
#include<iostream.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<assert.h>
#include<string.h>

struct CortexNeuron{
	double ** OnWeights ;
	double ** OfWeights ;
};

struct WtChange{
	double OnCh; 
	double OfCh;
};

struct WeightChange{
	WtChange ** wd;
	WtChange ** wd1;
	WtChange ** wd2;
};


class Orient
{

protected :

	CortexNeuron ** cortex ;		// Cortex Neurons..
	WeightChange ** wch ;			// Weight Changes..
	int rows, cols ; 				// No of LGN & cortex rows and cols..
	int ab ;						// Arbor Diameter..
	double rC ;			// Parameter controlling spread of correlation fn.
	double rI ;			// Parameter controlling spread of Intracortical fn.
	double Smax ;		// Upper Bound on Synapse Values..
	double lamda ;		// Growth Constant..
	int it;				// Current Iteration number;
	char outfilename[35];
	char oflnm[50];

public :
	
	Orient();
	void LoadParams(char * = NULL);
	void LoadStatus(char * = NULL);
	void Start();

protected :

	void assignspace ();
	void assignInitialWeights();
	double A (double distance);
	double I (double distance);
	double G(double distance, double sigma);
	double Cx_x (double distance);
	double Cx_y (double distance);
	void computeUWtChange();
	void uOnWtChange(int xx, int xy, int ax, int ay);
	void uOfWtChange(int xx, int xy, int ax, int ay);
	void computeCWtChange();
	void adjustsynapse();
	void threestep(int i, int j, int k, int l);
	int cutoff (int i, int j, int k, int l);
	void renormalize(int i, int j);
	double perc();
	double wrapdis(int xx, int xy, int yx, int yy);
	double cordis(int iax, int iay, int ik, int il);
	void store();
	void storediff();
	void saveStatus();
    int wrapx(int);
    int wrapy(int);
};

#endif
