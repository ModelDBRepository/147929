#include"Analysis.h"

Analysis :: Analysis(): Develop ()
{
}


/*****************************************************************/

void Analysis :: LoadStatus(char * filename)
{
	char * flnms=new char[50];
	if(!filename){
		cout << "\nGive Status filename : ";
		cin >> flnms ;
	}
	else strcpy(flnms,filename);
	if(strstr(flnms,".stt")){
		Orient :: LoadStatus(flnms);
		Develop :: assignspace();
	}
	else if (strstr(flnms,".dst"))
		Develop :: LoadStatus(flnms);
	assignspace();
	delete(flnms);
}

/*****************************************************************/

void Analysis :: CalculatePhyWeights()
{
	int x,y,k,l;
	for(x=0;x<rows;x++){
		cerr << "Computing " << x << " row ....\n" ;
		for(y=0; y<cols; y++)
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++)
					phyweight(x,y,k,l);
	}
	storephy();
}

/*****************************************************************/

void Analysis :: assignspace()
{
   	int i,j,k;
    PhyWts = new CortexNeuron * [rows];
    for(i=0; i<rows; i++){
        PhyWts[i]=new CortexNeuron [cols];
        for(j=0; j<cols; j++){
            PhyWts[i][j].OnWeights=new double * [ab];
            PhyWts[i][j].OfWeights=new double * [ab];
            for(k=0; k<ab; k++){
                PhyWts[i][j].OnWeights[k]=new double [ab];
                PhyWts[i][j].OfWeights[k]=new double [ab];
            }
        }
    }
	Img = new unsigned char * [rows];
	for(i=0; i<rows; i++)
		Img[i]=new unsigned char [cols];
}

/*****************************************************************/

void Analysis :: phyweight(int xx, int xy, int ax, int ay)
{
    int i,j,k,l;
    int ii,jj,tempx,tempy;
	int xt,yt;
    double dis ;
    double sumOn=0.0;
    double sumOf=0.0;
    double nsum=0.0;
    double fsum=0.0;
    double ** OnW, ** OfW;

    double ** Msk = new double * [MSZ];

    int kk=ax+ab/2;
    int ll=ay+ab/2;

    for (i=0;i<MSZ;i++)
        Msk[i]=new double[MSZ];
    generateMask(Msk,ON);

    for(i=xx-ab/2; i<=xx+ab/2; i++)
        for(j=xy-ab/2; j<=xy+ab/2; j++){
            ii=wrapx(i);
            jj=wrapy(j);
            OnW=cortex[ii][jj].OnWeights;
            OfW=cortex[ii][jj].OfWeights;
            nsum=0.0;
            fsum=0.0;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++){
					tempx=xx+ax-(ii+k);
					tempy=xy+ay-(jj+l);
					if((abs(tempx)>MSZ/2) || (abs(tempy)>MSZ/2))
						break;
					xt=tempx+MSZ/2;
					yt=tempy+MSZ/2;
                    nsum += OnW[k+ab/2][l+ab/2]*Msk[xt][yt];
                    fsum += OfW[k+ab/2][l+ab/2]*Msk[xt][yt];
                }
			if(nsum<0) nsum=0;
			if(fsum<0) fsum=0;
            dis=sqrt((i-xx)*(i-xx)+(j-xy)*(j-xy));
			sumOn += nsum*I(dis);
			sumOf += fsum*I(dis);
        }
	if (sumOn > 0)
		PhyWts[xx][xy].OnWeights[kk][ll]=sumOn;
	else 
		PhyWts[xx][xy].OnWeights[kk][ll] =0;
	if (sumOf > 0) 
		PhyWts[xx][xy].OfWeights[kk][ll]=sumOf;
	else 
		PhyWts[xx][xy].OfWeights[kk][ll] =0;
    for (i=0;i<MSZ;i++)
		delete(Msk[i]);
	delete(Msk);
}

/*****************************************************************/


void Analysis :: storephy()
{
	int i,j,k,l;
    double ** OnW;
    double ** OfW;
	char sfn[35];
	sprintf(sfn,"%s.phy",oflnm);
	ofstream outfile(sfn);
	unsigned char pix ;
	double diff ;
	double max=Smax*A(0);	// Max value that any synapse can attain..
	unsigned char ** image=new unsigned char * [rows*ab];
	for(i=0;i<rows*ab;i++)
		image[i]=new unsigned char [cols*ab];
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
    		OnW=PhyWts[i][j].OnWeights;
    		OfW=PhyWts[i][j].OfWeights;
    		for(k=-ab/2; k<=ab/2; k++)
        		for(l=-ab/2; l<=ab/2; l++){
					diff = OnW[k+ab/2][l+ab/2]-OfW[k+ab/2][l+ab/2];
					pix = (unsigned char)((255.0*(diff+max))/(2.0*max));
					image[i*ab+k+ab/2][j*ab+l+ab/2]=pix;
			}
		}
	for(i=0;i<rows*ab;i++){
		outfile.write((const char *)image[i],cols*ab);
		delete(image[i]);
	}
	delete(image);
	outfile.close();
}

/*****************************************************************/

void Analysis :: CalculateOrientations()
{
	int k,l;
	double f,t,s;
	double MAX,fx,tx,sx;
	double prod;
	double minf=20,maxf=-1;
	double mint=360,maxt=-1;
	int h;
	int hist[82];

	for(h=0;h<82;h++)
		hist[h]=0;
	

	for(k=0; k<rows; k++)
		for(l=0;l<cols;l++){
		MAX=0.0;
		for(s=0;s<2*M_PI;s+=0.5)
			for(f=0.0;f<=0.8;f+=0.1){
				for(t=0;t<=180;t+=10){
					F=f; 
					T=t*M_PI/180.0; 
					phi=s;
					generateImage();
					computemasked();
					findExcitations();
					prod=findOutput(k,l);
					if(prod>MAX){
						MAX=prod;
						fx=f;
						tx=t;
						sx=s;
					}
				}
		}	
			cerr << "\n" << k << ", " << l << "   =   " << fx << ", "
			 	<< tx << ", " << sx << "\t\t" << MAX  ;
			Img[k][l]=(unsigned char)(60.0*floor(tx/45.0));
			hist[(int)(fx*100)]++;
			if(fx<minf) minf=fx;
			if(fx>maxf) maxf=fx;

			if(tx<mint) mint=tx;
			if(tx>maxt) maxt=tx;
		}
	ofstream outfile("tst.img");
	for(k=0; k<rows; k++)
		outfile.write((const char *)Img[k],cols);
	
	cerr << "\n\n Min F = " << minf << "\n Max F = " << maxf ;
	cerr << "\n\n Min T = " << mint << "\n Max T = " << maxt ;
	cerr << endl << endl ;

	for(h=0; h<82; h++)
		cerr << h*0.01 << "    " << hist[h] << endl ;
	cerr << endl << endl ;
}

/*****************************************************************/

void Analysis :: CalculateSelectivity()
{
	int k,l;
	double f,t,s;
	double t1,tm,tp;
	double MAX,fx,tx,sx;
	double prod;
	double minf=20,maxf=-1;
	double mint=360,maxt=-1;
	double rMax[20];
	Vector vectors[20];
	Vector v_R(0,0);
	double numer,denom;

	for(k=0; k<rows; k++)
		for(l=0;l<cols;l++){
		MAX=0.0;
		for(t=0;t<180;t+=10){
			rMax[(int)(t/10)]=-1;
			for(f=0.5;f<=0.8;f+=0.1){
				for(s=0;s<2*M_PI;s+=0.2){
					F=f; 
					T=t*M_PI/180.0; 
					phi=s;
					generateImage();
					computemasked();
					findExcitations();
					prod=findOutput(k,l);
					if(prod>rMax[(int)(t/10)])
						rMax[(int)(t/10)]=prod;
					if(prod>MAX){
						MAX=prod;
						fx=f;
						tx=t;
						sx=s;
					}
				}
			}
//			cerr << t << "\t->\t" << rMax[(int)(t/10)] << endl ;
		}	
		v_R.setVector(0.0,0.0);	
		denom=0.0;
		for(t1=0;t1<180;t1+=10){
			tm=rMax[(int)(t1/10)];
			tp=(2*t1*M_PI)/180.0;
			vectors[(int)(t1/10)].setVector(tm*cos(tp), tm*sin(tp));
			v_R+=vectors[(int)(t1/10)];
			denom+=tm*tm;
		}
		numer=Norm(v_R)/18.0;
		denom/=18.0;
		denom=sqrt(denom);
//		cerr <<"\nSelectivity= " << numer/denom << endl ;

			
			cerr << "\n" << k << "     " << l << "      " 
				 << fx << "     " << tx << "     " 
				 << sx << "     " << numer/denom  ;
			Img[k][l]=(unsigned char)(60.0*floor(tx/45.0));
			if(fx<minf) minf=fx;
			if(fx>maxf) maxf=fx;

			if(tx<mint) mint=tx;
			if(tx>maxt) maxt=tx;
	}
	ofstream outfile("tst.img");
	for(k=0; k<rows; k++)
		outfile.write((const char *)Img[k],cols);
	
	cerr << "\n\n Min F = " << minf << "\n Max F = " << maxf ;
	cerr << "\n\n Min T = " << mint << "\n Max T = " << maxt ;
	cerr << endl << endl ;
}

/*****************************************************************/

void Analysis :: AnalyzeRF ()
{
	char filename[50];
	sprintf(filename,"%s.all",oflnm);
	ofstream outf(filename);
	sprintf(filename,"%s.pow",oflnm);
	ofstream powfile(filename);

	double * rMax=new double[(int)(180/10.0+1)];
    Vector * vectors=new Vector[(int)(180/10.0+1)];
    Vector v_R(0,0);
    double numer,denom;
	double tp,tm,t1;

	int i,j,k,l;
	double ** OnW;
	double ** OfW;
	double ** Sig = new double * [ab];
	for(i=0;i<ab;i++)
		Sig[i]=new double [ab];
	
	int h;
	double f,t,s;
	double MAX,fx,tx,sx;
	double prod,wx,wy;
	double minf=20,maxf=-1;
	double mint=360,maxt=-1;
	double sel;
	double minsel=2.0, maxsel=-1.0, meansel=0.0;

	int hist[82];
	for(h=0;h<82;h++)
		hist[h]=0;

	for(k=0; k<rows; k++)
		for(l=0;l<cols;l++){
			OnW=cortex[k][l].OnWeights;
			OfW=cortex[k][l].OfWeights;
			for(i=0; i<ab; i++)
				for(j=0; j<ab; j++)
					Sig[i][j]=OnW[i][j]-OfW[i][j];
			MAX=0.0;
			for(t=0;t<180;t+=10.0){
				rMax[(int)(t/10.0)]=-10000.0;
				for(f=0.1;f<=0.8;f+=0.01)
					for(s=0;s<2*M_PI;s+=0.2){
						prod=0.0;
						wx=f*cos(t*M_PI/180.0);
						wy=f*sin(t*M_PI/180.0);
						for(i=0;i<ab;i++)
							for(j=0; j<ab; j++)
								prod += Sig[i][j]*sin((i-ab/2)*wx+
													  (j-ab/2)*wy+s);
						if(prod>rMax[(int)(t/10.0)])
                        	rMax[(int)(t/10.0)]=prod;
						if(prod>MAX){
							MAX=prod;
							fx=f;
							tx=t;
							sx=s;
						}
					}
				//cerr << t << "\t->\t" << rMax[(int)(t/10.0)] << endl ;
			}

		v_R.setVector(0.0,0.0);	
		denom=0.0;
        for(t1=0;t1<180;t1+=10.0){
            tm=rMax[(int)(t1/10.0)];
            tp=(2*t1*M_PI)/180.0;
            vectors[(int)(t1/10.0)].setVector(tm*cos(tp), tm*sin(tp));
            v_R+=vectors[(int)(t1/10.0)];
            denom+=tm*tm;
        }
        numer=Norm(v_R)/(180.0/10.0);
        denom/=(180.0/10.0);
        denom=sqrt(denom);
		sel=numer/denom;
		powfile << v_R.getxcoord() << "\t" << v_R.getycoord() << endl ;
        //cerr <<"\nSelectivity = " << numer/denom << endl ;

		meansel+=sel;
		if (sel > maxsel) maxsel=sel;
		if (sel < minsel) minsel=sel;
		outf << "\n" << k << "      " << l << "      " << fx << "      "
		 	<< tx << "      " << sx  << "      " << sel;

		Img[k][l]=(unsigned char)(60.0*floor(tx/45.0));
		hist[(int)(fx*100)]++;
		if(fx<minf) minf=fx;
		if(fx>maxf) maxf=fx;

		if(tx<mint) mint=tx;
		if(tx>maxt) maxt=tx;
	}
	meansel/=(double)(rows*cols) ;
	sprintf(filename,"%s.img",oflnm);
	ofstream outfile(filename);
	for(k=0; k<rows; k++)
		outfile.write((const char *)Img[k],cols);
					
	outf << "\n\n Min F = " << minf << "\n Max F = " << maxf ;
	outf << "\n\n Min T = " << mint << "\n Max T = " << maxt ;
	outf << "\n\n Min Sel = " << minsel << "\n Max Sel = " << maxsel ;
	outf << "\n\n Mean Sel = " << meansel ;
	outf << endl << endl ;

	for(h=0; h<82; h++)
		outf << h*0.01 << "    " << hist[h] << endl ;
	outf << endl << endl ;
	
	powfile.close();
	outfile.close();
	outf.close();
}
