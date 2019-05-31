#include"Image.h"



void Image :: ReadRas(char * filename)
{
	char InFileName[50];
    if (!filename) {	
		cout<< "\nGive Input Raster file :  " ;
		cin >> InFileName ; 
    }
    else strcpy (InFileName, filename) ;
	int header[8];
	FILE * InRas = fopen(InFileName,"rb");
	if(!InRas){
		cerr << "\nFile " << InFileName << " doesnot exist.\n";
		exit(1);
	}
	fread (header,sizeof(int),8,InRas);
	if(header[0]!=0x59a66a95){
		cerr << "\nFile not a Sun Raster Image...\n";
		exit(1);
	}
	fseek(InRas,header[7],SEEK_CUR);
	memAlloc(header[2],header[1]);
	unsigned char * buffer = new unsigned char  [YSize];
	char odd = YSize%2;
	for (int i=0; i<XSize; i++) {
        fread (buffer,1,YSize,InRas);
	    for(int j=0;j<YSize;j++)
			image[i][j]=buffer[j];
		if(odd) fread(buffer,1,1,InRas); 
	}
	delete(buffer);
	fclose(InRas);
}

void Image :: WriteRas(char * filename)
{
    if(!MEMFLAG){
        cerr <<"\nImage :: Memory Allocation Error.\n";
        exit(1);
    }
	char OutFileName[50];
    if (!filename){	
		cout <<"\nGive Output Raster file : ";
		cin >> OutFileName;
    }
    else strcpy(OutFileName, filename) ;
	FILE * OutRas ;
	OutRas = fopen(OutFileName,"wb");
	int header[8];
	char odd = YSize%2;
	header[0]=0x59a66a95; //Magic Number.
	header[1]=YSize;
	header[2]=XSize;
	header[3]=8;
	header[4]=YSize*XSize;
	header[5]=1;
	header[6]=0;
	header[7]=0;
	fwrite(header,sizeof(int),8,OutRas);
	double min, max ;
	FindMinMax(min,max);
    if (max == min ) max = min+1 ;
    unsigned char * ret = new unsigned char [YSize];
    for (int i = 0; i < XSize; i++) {
        for (int j = 0; j < YSize; j++)
  	  		ret[j]= (unsigned char)(255.0*(image[i][j]- min)/(max - min));
	    fwrite(ret,sizeof(unsigned char),YSize,OutRas);
	    if(odd) fputc(0,OutRas);
    } 	
	fclose(OutRas);
}

void Image :: DisplayImage()
{
	int i;
	if(!MEMFLAG){
  	    cerr <<"\nImage :: Memory Allocation Not Done.\n";
 	    exit(1);
	}
	Imgstr temp ;
	temp.xs=XSize;
	temp.ys=YSize;
	temp.ima=new double * [XSize];
	for(i=0; i<XSize; i++){
		temp.ima[i]=new double [YSize];
		for(int j=0;j<YSize;j++)
			temp.ima[i][j]=image[i][j];
	}
	Imgdis imgdis ;
	imgdis.displayImage(temp);
	for(i=0; i<XSize; i++)
		delete(temp.ima[i]);
	delete(temp.ima);
}


void Image :: GetImage(unsigned char **  img)
{
	if(!MEMFLAG){
  	    cerr <<"\nImage :: Memory Allocation Not Done.\n";
 	    exit(1);
	}
	double max,min;
	FindMinMax(min,max);
	if(min==max) max=min+1;
    for (int i = 0; i < XSize; i++) 
        for (int j = 0; j < YSize; j++)
  	  		img[i][j]=(unsigned char)(255.0*(image[i][j]-min)/(max-min));
}

