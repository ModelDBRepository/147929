#include"Develop.h"

Develop :: Develop () : Orient()
{
}

/*******************************************************************/

void Develop :: LoadParams (char * filename)
{
	char name [35];
	if (!filename){
		cerr << "\nGive Parameter File[.dev] : ";
		cin >> name ;
	}
	else strcpy (name,filename);
	ifstream infile (name);
	if(!infile){
		cerr << "\nFile " << name << " doesnot exist.\n";
		exit(1);
	}
    int seed ;
    infile >> rows ;		// Parameters corresponding to network..
    infile >> cols ;
    infile >> ab ;
    infile >> seed ;
    infile >> rC ;
    infile >> rI ;
    infile >> Smax ;
    infile >> lamda ;
	infile >> sNi ;
	infile >> sNf ;
    infile >> oflnm ;
	infile >> Fmax ;
	infile >> Fmin ;
    srand48(seed);
	infile.close();
	assignspace();
	Orient :: assignspace();
	assignInitialWeights();
}

/*******************************************************************/

void Develop :: assignspace ()
{
	int i;
	image=new unsigned char * [ab+MSZ];
	E=new double * [rows];
	for(i=0; i<rows; i++)
		E[i]=new double [cols];
	dimage=new double * [ab+MSZ];
	onimage=new double * [ab+MSZ];
	ofimage=new double * [ab+MSZ];
	for(i=0; i<ab+MSZ; i++){
		image[i]=new unsigned char [ab+MSZ];
		dimage[i]=new double[ab+MSZ];
		onimage[i]=new double[ab+MSZ];
		ofimage[i]=new double[ab+MSZ];
	}
/*	connects=new Connect * [rows];
	for(i=0; i<rows; i++){
		connects[i]=new Connect[cols];
		for(int j=0; j<cols; j++){
			connects[i][j].connect=new char * [ab];
			for(int k=0;k<ab; k++)
				connects[i][j].connect[k]=new char[ab];
		}
	}*/
}

/*******************************************************************/

void Develop :: generateMask(double ** Msk,int MODE)
{
	int i,j;
	double gI=1.3;
	double dist ;
	double temp1,temp2;
	for(i=0; i<MSZ; i++){
		for(j=0; j<MSZ; j++){
			dist=sqrt((i-MSZ/2)*(i-MSZ/2)+(j-MSZ/2)*(j-MSZ/2));
			temp1=exp(-(dist*dist)/(PROD*PROD/4.0));
			temp2=exp(-(dist*dist)/(PROD*PROD*gI*gI/4.0));
			if(MODE==ON)
				Msk[i][j]=temp1-temp2/(gI*gI);
			else if(MODE==OF)
				Msk[i][j]=-temp1+temp2/(gI*gI);
		}
	}
}

/*******************************************************************/

void Develop :: operateonImage(double ** Msk, int MODE,int k)
{
	CMask DoG ;
	DoG.SetMask(Msk,MSZ);
    DoG.SetImage(dimage,ab+MSZ,ab+MSZ);
    DoG.OperateMaskonImage();
	int i,j;
	if(MODE==ON){
		DoG.GetMaskedResult(onimage);
    	for(i=MSZ/2;i<ab+MSZ-MSZ/2;i++)
        	for(j=MSZ/2;j<ab+MSZ-MSZ/2;j++){
            	if(onimage[i][j]<0)onimage[i][j]=0.0;
				onimage[i][j] *= onimage[i][j];
			}
	}
	else if(MODE==OF){
		DoG.GetMaskedResult(ofimage);
    	for(i=MSZ/2;i<ab+MSZ-MSZ/2;i++)
        	for(j=MSZ/2;j<ab+MSZ-MSZ/2;j++){
            	if(ofimage[i][j]<0)ofimage[i][j]=0.0;
				ofimage[i][j] *= ofimage[i][j];
			}
	}
	
}

/*******************************************************************/

void Develop :: computemasked ()
{
	int i;
	double ** Msk = new double * [MSZ];
	for (i=0;i<MSZ;i++)
		Msk[i]=new double[MSZ];
	generateMask(Msk,ON);
	operateonImage(Msk,ON,i);
	generateMask(Msk,OF);
	operateonImage(Msk,OF,i);
	for (i=0;i<MSZ;i++)
		delete(Msk[i]);
	delete(Msk);
}

/*******************************************************************/

void Develop :: findWinner()
{
	double max=-100000.0;
	double temp;
	for(int i=0; i<rows; i++)	
		for(int j=0; j<cols; j++)
			if(prcnt(i,j)<90){
				temp=productat(i,j);
				if(temp>max){
					max=temp;
					wx=i;
					wy=j;
				}
			}
}

/********************************************************************/

double Develop :: productat (int x, int y)
{
	double sumon = 0.0,sumof=0.0;
	double ** OnW, ** OfW;
	int cx=(ab+MSZ)/2;
	int cy=(ab+MSZ)/2;
	OnW=cortex[x][y].OnWeights;
	OfW=cortex[x][y].OfWeights;
	for(int i=-ab/2; i<=ab/2; i++)
		for(int j=-ab/2; j<=ab/2; j++){
			sumon += OnW[i+ab/2][j+ab/2]*onimage[cx+i][cy+j] ;
			sumof += OfW[i+ab/2][j+ab/2]*ofimage[cx+i][cy+j] ;
		}
	sumon /= (double)(ab*ab);
	sumof /= (double)(ab*ab);
//	sumon=fabs(sumon);
//	sumof=fabs(sumof);
//  cerr << x << "\t" << y << "\t" << sumon << "\t"  << sumof  << endl ;
//		 << "\t" << sumon-sumof << "\t" << sumon+sumof <<endl ;
//	return ((sumon>sumof)?sumon:sumof) ;
	return (sumon+sumof);
}

/********************************************************************/

double Develop :: comprod (int x, int y)
{
	double sum = 0.0;
	double ** OnW, ** OfW;
	int cx=(ab+MSZ)/2;
	int cy=(ab+MSZ)/2;
	OnW=cortex[x][y].OnWeights;
	OfW=cortex[x][y].OfWeights;
	double tempn, tempf;
	for(int i=-ab/2; i<=ab/2; i++)
		for(int j=-ab/2; j<=ab/2; j++){
			tempn=OnW[i+ab/2][j+ab/2]*onimage[cx+i][cy+j];
			tempf=OfW[i+ab/2][j+ab/2]*ofimage[cx+i][cy+j];
			if(fabs(tempn)>fabs(tempf)){
				connects[x][y].connect[i+ab/2][j+ab/2]=1;
				sum += tempn;
			}
			else{ 
				connects[x][y].connect[i+ab/2][j+ab/2]=0;
				sum += tempf;
			}
		}
	return (fabs(sum));
}

/********************************************************************/

void Develop :: storerf()
{
    int i,j,k,l;
    double ** OnW;
    double ** OfW;
    ofstream outfile("test.rf");
//    ofstream outfilen("test.on");
//    ofstream outfilef("test.of");
    unsigned char pix ;
    double diff ;
    double max=Smax*A(0);   // Max value that any synapse can attain..
//  double max,dis;
    unsigned char ** image=new unsigned char * [rows*ab];
    unsigned char ** imagen=new unsigned char * [rows*ab];
    unsigned char ** imagef=new unsigned char * [rows*ab];
    for(i=0;i<rows*ab;i++){
        image[i]=new unsigned char [cols*ab];
        imagen[i]=new unsigned char [cols*ab];
        imagef[i]=new unsigned char [cols*ab];
    }
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
            OnW=cortex[i][j].OnWeights;
            OfW=cortex[i][j].OfWeights;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++){
                    diff = (OnW[k+ab/2][l+ab/2]>OfW[k+ab/2][l+ab/2])?OnW[k+ab/2][l+ab/2]:-OfW[k+ab/2][l+ab/2];
                    pix = (unsigned char)((255.0*(diff+max))/(2.0*max));
                    image[i*ab+k+ab/2][j*ab+l+ab/2]=pix;

                    diff = OnW[k+ab/2][l+ab/2];
                    pix = (unsigned char)((255.0*diff)/max);
                    imagen[i*ab+k+ab/2][j*ab+l+ab/2]=pix;

                    diff = OfW[k+ab/2][l+ab/2];
                    pix = (unsigned char)((255.0*diff)/max);
                    imagef[i*ab+k+ab/2][j*ab+l+ab/2]=pix;
            }
        }
    for(i=0;i<rows*ab;i++){
        outfile.write((const char *)image[i],cols*ab);
 //       outfilen.write(imagen[i],cols*ab);
 //       outfilef.write(imagef[i],cols*ab);
        delete(image[i]);
        delete(imagen[i]);
        delete(imagef[i]);
    }
    delete(image);
    delete(imagen);
    delete(imagef);
    outfile.close();
//    outfilen.close();
//    outfilef.close();
}

/********************************************************************/

void Develop :: generateImage()
{
	int i,j;
    double fx=F*cos(T);
    double fy=F*sin(T);
	for(i=0; i<ab+MSZ; i++)
		for(j=0; j<ab+MSZ; j++)
			dimage[i][j]=sin(fx*i+fy*j+phi);
}

/*****************************************************************/

void Develop :: findExcitations()
{
    double sumon, sumof;
    double synon,synof;
    double ** OnW, ** OfW;
    int cx=(ab+MSZ)/2;
    int cy=(ab+MSZ)/2;
    for(int x=0; x<rows; x++)
        for(int y=0; y<cols; y++){
            OnW=cortex[x][y].OnWeights;
            OfW=cortex[x][y].OfWeights;
            synon=synof=sumon=sumof=0.0;
            for(int i=-ab/2; i<=ab/2; i++)
                for(int j=-ab/2; j<=ab/2; j++){
                    sumon += OnW[i+ab/2][j+ab/2]*onimage[cx+i][cy+j] ;
                    sumof += OfW[i+ab/2][j+ab/2]*ofimage[cx+i][cy+j] ;
					synon += OnW[i+ab/2][j+ab/2]*OnW[i+ab/2][j+ab/2];
					synof += OfW[i+ab/2][j+ab/2]*OfW[i+ab/2][j+ab/2];
                }
            sumon /= (double)(ab*ab*sqrt(synon));
			sumof /= (double)(ab*ab*sqrt(synof));
            E[x][y]=(sumon+sumof);
        }
}


/*****************************************************************/

double Develop :: findOutput(int x, int y)
{
	int i,j;
	double temp;
	double sum=0.0;
	for(i=0; i<rows; i++)
		for(j=0; j<cols; j++){
        	if((i==x)&&(j==y))
            	sum += E[i][j];
            else{
				temp=E[i][j];
				if(temp < 0.0) temp=0.0;
				sum += temp*temp*I(wrapdis(x,y,i,j));
			}
		}
	sum /= (double)(rows*cols);
	if(sum<0.0) sum=0.0;
	sum *= sum;
	return sum ;
}

/*****************************************************************/

void Develop :: findwinner()
{
	int x,y;
	double out ;
	double max=-100000.0;
	findExcitations();
	for(x=0; x<rows; x++)
		for(y=0; y<cols; y++){
            if(prcnt(x,y)<90){
				out=findOutput(x,y);
				if(out>max){
					max=out;
					wx=x;
					wy=y;
				}
			}															
		}
}

/*****************************************************************/

