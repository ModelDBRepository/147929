#ifndef __ANAL__H_
#define __ANAL__H_


#include"Develop.h"
#include"Vector.h"


class Analysis : public Develop
{
protected :

    CortexNeuron ** PhyWts ;        // Physiological Weights...
	unsigned char ** Img ;

public :

	Analysis();
	void LoadStatus(char * filename=NULL);
	void CalculatePhyWeights();
	void CalculateOrientations();
	void CalculateSelectivity();
	void AnalyzeRF();

protected :

	void storephy();
	void phyweight(int xx, int xy, int ax, int ay);
	void assignspace();
};

#endif
