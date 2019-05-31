#include"Image.h"


/**************** Image Class Functions ***************/

Image:: Image( ) 
{
	MEMFLAG = 0 ;
}

void Image :: assignInputFile (char * filename ) 
{
    if (!filename) {	
		cout<< "\nGive Input Image file :  " ;
		cin >> InFileName ; 
    }
    else strcpy  ( InFileName , filename ) ;
}

void Image :: assignOutputFile (char * filename ) 
{
    if (!filename) {	
		cout <<"\nGive Output Image file : ";
		cin >> OutFileName;
    }
    else strcpy  ( OutFileName , filename ) ;
}
	
void Image :: LoadImage (char * filename) 
{
	if(!MEMFLAG) {
  	    cerr <<"\nImage :: Memory Allocation Not Done.\n";
 	    exit(1);
	}
	assignInputFile(filename);
	ifstream inImage;
	inImage.open( InFileName )   ; 	
	if (!inImage){
		perror (InFileName) ; 
		exit ( 1 );
	}
	Img buffer = new unsigned char * [XSize];
	for (int i=0 ; i<XSize ; i++) {
	    buffer[i]=new unsigned char [YSize];
	    inImage.read ( (char *)buffer [ i ], YSize ) ;
	    for(int j=0;j<YSize;j++)
		image[i][j]=buffer[i][j];
	}
	inImage.close ();
}

void Image :: SaveImage (char * filename) 
{	
    if(!MEMFLAG) {
        cerr <<"\nImageS :: Memory Allocation Error.\n";
        exit(1);
    }
    assignOutputFile(filename);
    ofstream outImage;
    outImage.open (OutFileName);
    double min ;
    double max ;
    FindMinMax(min,max);
    if (max == min ) max = min+1 ;
    Img ret = new unsigned char * [ XSize ];
    for (int i = 0; i < XSize; i++) {
  		ret[i]=new unsigned char[YSize];
        for (int j = 0; j < YSize; j++)
  	  		ret[i][j]= (unsigned char)(MAXGRAY*(image[i][j]- min)/(max - min));
		outImage.write ((char *)ret[i], YSize) ;
    } 	
    outImage.close ();
}

void Image :: SetImage(Img img,int xsize, int ysize)
{
  memAlloc(xsize,ysize);
  for (int i =0 ; i < xsize; i ++ )
    for ( int j = 0 ; j < ysize; j++ )
      	image[i][j]=img[i][j];
}
      
void Image :: SetImage(FImg img,int xsize, int ysize)
{
  memAlloc(xsize,ysize);
  for (int i =0 ; i < xsize; i ++ )
    for ( int j = 0 ; j < ysize; j++ )
      	image[i][j]=img[i][j];
}
		
void Image :: SetImage(DImg img,int xsize, int ysize)
{
  memAlloc(xsize,ysize);
  for (int i =0 ; i < xsize; i ++ )
    for ( int j = 0 ; j < ysize; j++ )
      	image[i][j]=img[i][j];
}

void Image :: SetImage( Imgstr & stpic)
{
  memAlloc(stpic.xs,stpic.ys);
  for (int i =0 ; i < XSize; i ++ )
    for ( int j = 0 ; j < YSize; j++ )
      	image[i][j]=stpic.ima[i][j];
}

void Image :: SetSize(int xsize, int ysize)
{
  memAlloc(xsize,ysize);
}


void Image :: GiveSize ()
{
  int height, width ;
  cout << "\nGive height of the Image : " ;
  cin >> height ;
  cout << "\nGive width of the image : " ;
  cin >> width ;
  memAlloc(height,width);
}

int Image :: Height () 
{
  return XSize ;
}

int Image :: Width ()
{
  return YSize ;
}

void Image :: memAlloc(int xsize, int ysize)
{
  if(this->MEMFLAG)
  {
    for ( int i = 0; i< xsize; i++ )
      delete(image[i]);
    delete(image);
  } 	
  XSize = xsize;
  YSize = ysize;
  image = new double * [ xsize ];
  for ( int i = 0; i < xsize; i++)
    image[i]=new double[ ysize ];
  MEMFLAG = 1;
}

Image &  Image :: operator = (Image & pic)
{
  memAlloc (pic.XSize, pic.YSize);
  for ( int i = 0; i < XSize; i++)
    for ( int j = 0; j < YSize; j++)
      image[i][j]=pic.image[i][j];
  return *this ;
}

Imgstr Image :: GetImage()
{
  Imgstr ret ;
  int i;
  ret.xs = XSize ;
  ret.ys = YSize ;
  ret.ima = new double * [ret.xs];
  for ( i = 0; i < ret.xs; i++)
    ret.ima[i]=new double [ret.ys];
  for ( i = 0; i < ret.xs; i++)
    for ( int j = 0; j < ret.ys; j++)
      ret.ima[i][j]=image[i][j];
  return ret;
}

Image Image :: ScaleRange()
{
  double min ;
  double max ;
  Image ret ;
  FindMinMax(min,max);
  if (max == min ) max = min+1 ;
  if ((max > MAXGRAY)||(min<MINGRAY))
  {
     ret.memAlloc(XSize,YSize);
     for (int i = 0; i < XSize; i++)
	for (int j = 0; j < YSize; j++)
	   ret.image[i][j]= (unsigned char)(MAXGRAY*((double)(image[i][j])-
			min)/(max - min));
  }
  else ret = *this;
  return ret ;
}

void Image :: FindMinMax (double & min, double & max)
{
   double small = 1000000.0 ;
   double big = -1000000.0 ;
   for ( int i= 0; i < XSize; i++)
     	for ( int j = 0; j < YSize; j++) {
			if ( image[i][j]<small) small = image[i][j];
			if ( image[i][j]>big) big = image[i][j];
       	} 
   min = small; 
   max = big ; 
}

Imgstr Image :: Threshold (double x)
{
    Imgstr gir ;
    gir.xs=XSize ;
    gir.ys=YSize ;
    gir.ima=new double * [gir.xs];
    for (int i = 0; i < XSize; i++) {
		gir.ima[i]=new double [gir.ys];
		for (int j = 0; j < YSize; j++) {
	    	if (image[i][j] > x)
	    		gir.ima[i][j]=255 ;
            	else gir.ima[i][j] = 0 ;
       	}
    }
    return gir ;
}

double Image :: GetPixel(int x, int y)
{
    if(!MEMFLAG) {
        cerr <<"\nImage :: Memory Allocation Not Done.\n";
        exit(1);
    }
    return (image[x][y]);
}

void Image :: PutPixel(int x, int y, double val)
{
    if(!MEMFLAG) {
        cerr <<"\nImage :: Memory Allocation Not Done.\n";
        exit(1);
    }
    image[x][y]=val ;
}

void Image :: Initialize(double initval)
{
    if(!MEMFLAG) {
        cerr <<"\nImage :: Memory Allocation Not Done.\n";
        exit(1);
    }
    for(int i=0;i<XSize;i++)
	for(int j=0;j<YSize;j++)
	    image[i][j]=initval;
}

Image operator + (Image & img1, Image & img2)
{
	if((img1.XSize != img2.XSize) || (img1.YSize != img2.YSize)){
		cerr << "\nUnequal images cannot be added..\n" ;
		exit(1);
	}
	Image temp ;
	int i,j;
	temp.SetSize(img1.XSize,img1.YSize);
    for(i=0;i<img1.XSize;i++)
		for(j=0;j<img1.YSize;j++)
			temp.image[i][j]=img1.image[i][j]+img2.image[i][j];
	return(temp);
}

Image operator - (Image & img1, Image & img2)
{
	if((img1.XSize != img2.XSize) || (img1.YSize != img2.YSize)){
		cerr << "\nUnequal images cannot be subtracted..\n" ;
		exit(1);
	}
	Image temp ;
	int i,j;
	temp.SetSize(img1.XSize,img1.YSize);
    for(i=0;i<img1.XSize;i++)
		for(j=0;j<img1.YSize;j++)
			temp.image[i][j]=img1.image[i][j]-img2.image[i][j];
	return(temp);
}

Image operator * (Image & img1, Image & img2)
{
	if((img1.XSize != img2.XSize) || (img1.YSize != img2.YSize)){
		cerr << "\nUnequal images cannot be dotted..\n" ;
		exit(1);
	}
	Image temp ;
	int i,j;
	temp.SetSize(img1.XSize,img1.YSize);
    for(i=0;i<img1.XSize;i++)
		for(j=0;j<img1.YSize;j++)
			temp.image[i][j]=img1.image[i][j]*img2.image[i][j];
	return(temp);
}

Image operator | (Image & img1, Image & img2) // Max Operator.
{
	if((img1.XSize != img2.XSize) || (img1.YSize != img2.YSize)){
		cerr << "\nUnequal images cannot be Maxed..\n" ;
		exit(1);
	}
	Image temp ;
	int i,j;
	temp.SetSize(img1.XSize,img1.YSize);
    for(i=0;i<img1.XSize;i++)
		for(j=0;j<img1.YSize;j++)
			temp.image[i][j]=(img1.image[i][j]>img2.image[i][j])?
					img1.image[i][j]:img2.image[i][j];
	return(temp);
}

Image operator & (Image & img1, Image & img2) // Min Operator.
{
	if((img1.XSize != img2.XSize) || (img1.YSize != img2.YSize)){
		cerr << "\nUnequal images cannot be Mined..\n" ;
		exit(1);
	}
	Image temp ;
	int i,j;
	temp.SetSize(img1.XSize,img1.YSize);
    for(i=0;i<img1.XSize;i++)
		for(j=0;j<img1.YSize;j++)
			temp.image[i][j]=(img1.image[i][j]<img2.image[i][j])?
					img1.image[i][j]:img2.image[i][j];
	return(temp);
}

