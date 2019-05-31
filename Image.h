#ifndef __IMAGEH
#define __IMAGEH


#include<fstream.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<iostream.h>
#include<errno.h>
#include<stdio.h>
#include"Imgdis.h"

#define MAXGRAY 255.0
#define MINGRAY 0.0


#ifndef __IMAGETY
#define __IMAGETY

typedef unsigned char ** Img ;
typedef double ** DImg ;
typedef float ** FImg ;

struct Imgstr
{
	DImg ima ;
	int xs;
	int ys;
};
#endif


class Image
{
protected :
	
	DImg image ;

	int XSize ;

	int YSize ;
	
	int MEMFLAG ;

	char InFileName[35];

	char OutFileName[35];
public :

	Image();

/******************* BASIC I/O OPERATIONS ******************************/

	void SetImage(Img, int=128 , int=128 );
	void SetImage(DImg , int=128 , int=128 );
	void SetImage(FImg , int=128 , int=128 );
	void SetImage(Imgstr &);
	void LoadImage (char * = NULL);
	void SaveImage (char * = NULL);
	void ReadRas   (char * = NULL);
	void WriteRas  (char * = NULL);
	void SetSize(int=128,int=128);
	void GiveSize ();

/****************** OTHER OPERATIONS **********************************/

   	Image & operator = ( Image & );
	Imgstr GetImage();
	void GetImage(unsigned char **);
	int Height ();
	int Width ();
	Image ScaleRange ();
	void FindMinMax (double &, double&);
	Imgstr Threshold (double x);
	double GetPixel (int x, int y);
	void PutPixel(int x, int y, double val);
	void Initialize(double initval=0);
	void DisplayImage();
	friend Image operator + ( Image&, Image& );
	friend Image operator - ( Image&, Image& );
	friend Image operator * ( Image&, Image& ) ;
	friend Image operator | ( Image&, Image& ) ;
	friend Image operator & ( Image&, Image& );


protected :
	
	void assignInputFile (char * = NULL );
	void assignOutputFile (char * = NULL );
	void memAlloc(int=128,int=128);
};
	
#endif
