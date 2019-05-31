#ifndef __MASKH
#define __MASKH


#include"Image.h"
#include<iomanip.h>

class CMask : public Image
{
protected :
	
	int MaskSize ;

	DImg mask;

	DImg FinFlo ;

public:
	CMask() ;

	~CMask();

	int GetMask(DImg); 

	void SetMask(DImg, int);

	void StoreImage();

	void OperateMaskonImage();

	void GetMaskedResult (DImg);

	Imgstr GetMaskedResult ();

protected :

	double convolute(int ,int);
};

#endif

