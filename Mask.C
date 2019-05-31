#include"Mask.h"

CMask :: CMask ():Image()
{
    MaskSize=7;
}

CMask :: ~CMask()
{
	int i;
	if(MEMFLAG){
		 for(i=0; i<XSize; i++)
			 delete(image[i]);
		 delete(image);
	}
	if(mask){
		for(i=0;i<MaskSize;i++)
			delete(mask[i]);
		delete(mask);
	}
	if(FinFlo){
		for(i=0;i<XSize;i++)
			delete(FinFlo[i]);
		delete(FinFlo);
	}
}
		

void CMask :: OperateMaskonImage ()
{
    FinFlo = new double * [XSize];
	int i;
    for ( i = 0; i < XSize; i++)
    {
	FinFlo [ i ] = new double [ YSize ];
	for ( int j = 0; j < YSize; j++)
	    FinFlo[i][j]=0.0;
    }
    for ( i = MaskSize/2; i < XSize-MaskSize/2; i++ )
     	for ( int j = MaskSize/2; j< YSize-MaskSize/2;j++)
	    FinFlo [ i ] [ j ]=convolute(i,j);
}

double CMask :: convolute (int i, int j)
{
    double returnvalue = 0.0 ;
    for ( int x = 0; x < MaskSize; x++ )
	for ( int y = 0; y< MaskSize;y++ )
	    returnvalue += mask[x][y] * image[x+i-MaskSize/2][y+j-MaskSize/2];
    return returnvalue ;
}

void CMask :: StoreImage()
{
    Image Final ;
    Final.SetImage(FinFlo,XSize,YSize);
    Final.SaveImage();
}

void CMask :: SetMask (DImg array, int masksize)
{
	int i;
    MaskSize = masksize ;
    mask = new double * [MaskSize];
    for ( i = 0; i < MaskSize; i++ )
		mask[i] = new double [ MaskSize ];
    for ( i = 0; i < MaskSize; i++ )
		for ( int j = 0; j < MaskSize; j++ )
	    	mask[i][j]=array[i][j];
}

int CMask :: GetMask(DImg array)
{
    for ( int i = 0; i< MaskSize; i++ )
	for ( int j = 0; j < MaskSize; j++ )
	    array[i][j]=mask[i][j];
    return MaskSize ;
}

void CMask :: GetMaskedResult (DImg result)
{
    for ( int i = 0;i < XSize; i++ )
	for ( int j = 0; j < YSize; j++ )
	    result [ i ] [ j ] = FinFlo [ i ] [ j ] ;
}

Imgstr CMask :: GetMaskedResult()
{
    Imgstr result ;
    result.xs = XSize;
    result.ys = YSize;
    result.ima=new double * [XSize];
    for ( int i = 0; i < XSize; i++)
    {
	result.ima[i]=new double[YSize];
	for ( int j = 0; j < YSize; j++)
	    result.ima[i][j]=FinFlo[i][j];
    }
    return result;
}
