#include"Orient.h"

Orient :: Orient ()
{
    it=0;
}

/********************************************************************/

void Orient :: LoadParams(char * filename)
{
	char name[35];
	if(!filename){
		cout << "\nGive Parameter Filename[.ort] : ";
		cin >> name ;
	}
	else strcpy(name,filename);
	ifstream ParamFile (name);
	if(!ParamFile){
		cerr << "\nFile " << name << " doesnot exist.\n";
		exit(1);
	}
	int seed ;
	ParamFile >> rows ;
	ParamFile >> cols ;
	ParamFile >> ab ;
	ParamFile >> seed ;
	ParamFile >> rC ;
	ParamFile >> rI ;
	ParamFile >> Smax ;
	ParamFile >> lamda ;
	ParamFile >> oflnm ;
	ParamFile.close();
	srand48(seed);
	assignspace();
    assignInitialWeights();
}

/********************************************************************/

void Orient :: assignspace()
{
	int i,j,k;
	cortex = new CortexNeuron * [rows];
	wch = new WeightChange * [rows];
	for(i=0; i<rows; i++){
		cortex[i]=new CortexNeuron [cols];
		wch[i] = new WeightChange  [cols];
		for(j=0; j<cols; j++){
			cortex[i][j].OnWeights=new double * [ab];
			cortex[i][j].OfWeights=new double * [ab];
			wch[i][j].wd=new WtChange * [ab];
			wch[i][j].wd1=new WtChange * [ab];
			wch[i][j].wd2=new WtChange * [ab];
			for(k=0; k<ab; k++){
				cortex[i][j].OnWeights[k]=new double [ab];
				cortex[i][j].OfWeights[k]=new double [ab];
				wch[i][j].wd[k]=new WtChange [ab];
				wch[i][j].wd1[k]=new WtChange [ab];
				wch[i][j].wd2[k]=new WtChange [ab];
			}
		}
	}
}

/********************************************************************/
//	Assign initial weights  from (1+Snoise)*A to (1-Snoise)*A

void Orient :: assignInitialWeights()
{
	int i,j,k,l,kk,ll;
	double Snoise = 0.2;
	double dis;
	for(i=0; i<rows; i++)
		for(j=0; j<cols; j++){
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					kk=k+ab/2; ll=l+ab/2;
					dis=sqrt(k*k+l*l);
					cortex[i][j].OnWeights[kk][ll]=
						A(dis)*(2.0*drand48()*Snoise+1-Snoise);
					cortex[i][j].OfWeights[kk][ll]=
						A(dis)*(2.0*drand48()*Snoise+1-Snoise);
					wch[i][j].wd[kk][ll].OnCh=0.0;
					wch[i][j].wd1[kk][ll].OnCh=0.0;
					wch[i][j].wd2[kk][ll].OnCh=0.0;
					wch[i][j].wd[kk][ll].OfCh=0.0;
					wch[i][j].wd1[kk][ll].OfCh=0.0;
					wch[i][j].wd2[kk][ll].OfCh=0.0;
				}
				renormalize(i,j);
		}
}

/********************************************************************/
// 	Arbor Function: describes the neighborhood interaction.

double Orient :: A (double distance)
{
	double rA = (ab-1)/2;
	double cA = 0.5 ;
	if(distance > rA)
		return 0;
	else if(distance < (rA*(1-cA)))
		return 1;
	else
		return((rA-distance)/(rA*cA));
}

/********************************************************************/
//	Intra-cortical Interaction function..		   
// 	Chosen to be excitatory-inhibitory in nature.. 

double Orient :: I (double distance)
{
	double a=1.0;
	if(distance) a=0.5;
	double gI=3.0;
//	double ret=a*(G(distance,6.5*rI)-G(distance,6.5*rI*gI)/(gI*gI)) ;
	double ret=a*(G(distance,(ab*rI)/2.0)-G(distance,(ab*rI*gI)/2.0)/(gI*gI)) ;
	return ret ;
}

/********************************************************************/
// 	Gaussian Function..

double Orient :: G(double distance, double sigma)
{
	return(exp(-(distance*distance)/(sigma*sigma)));
}

/********************************************************************/
// 	On-On, Off-Off Correlations..

double Orient :: Cx_x (double distance)
{
	double gC=3.0;
	double ret=G(distance,(rC*ab)/2.0)-
			   G(distance,(gC*rC*ab)/2.0)/(gC*gC);
	return ret;
}

/********************************************************************/
// 	On-Off, Off-On Correlations.. 

double Orient :: Cx_y (double distance)
{
	return (-0.5*Cx_x(distance));
}

/********************************************************************/
// 	Compute Unconstrained Weight Change and Update Prevs..

void Orient :: computeUWtChange()
{
	int i,j,k,l;
	for(i=0; i<rows; i++){
		cerr << "\nIn computeUWtChange "  << i << " .." ;
		for(j=0; j<cols; j++)
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					uOnWtChange(i,j,k,l);
					uOfWtChange(i,j,k,l);
				}
	}
}

/*******************************************************************/
// 	Distance calculated for wrapped around cortex..

double Orient :: wrapdis(int xx, int xy, int yx, int yy)
{
	int x_d,y_d;
	int xt=abs(xx-yx);
	int yt=abs(xy-yy);
    if(xt>rows/2) x_d = rows-xt;
    else x_d = xt;
    if(yt>cols/2) y_d = cols-yt;
    else y_d = yt;
	return(sqrt(x_d*x_d+y_d*y_d));
}

/*******************************************************************/
// 	Distance calculated for Correlation computation.

double Orient :: cordis(int iax, int iay, int ik, int il)
{
	int x_d,y_d;
	int xt,yt;
	xt=abs(iax-ik)%rows;
	yt=abs(iay-il)%cols;
    if(xt>rows/2) x_d = rows-xt;
    else x_d = xt;
    if(yt>cols/2) y_d = cols-yt;
    else y_d = yt;
	return(sqrt(x_d*x_d+y_d*y_d));
}

/*******************************************************************/
// 	Compute Constrained Weight Change.. Subtractive normalization..

void Orient :: computeCWtChange()
{
	int i,j,k,l,kk,ll;
	double ** OnW,** OfW;
	double aonsum=0.0,aofsum=0.0;
	double dis,Sum,epsilon ;
	WtChange ** wc ;
	for(i=0; i<rows; i++)
		for(j=0; j<cols; j++){
			OnW=cortex[i][j].OnWeights;
			OfW=cortex[i][j].OfWeights;
			Sum=0.0;
			aonsum=0.0; aofsum=0.0;
			wc=wch[i][j].wd;
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					dis=sqrt(k*k+l*l);	
					kk=k+ab/2;ll=l+ab/2;
					if(OnW[kk][ll]!=0 && OnW[kk][ll]!=Smax*A(dis))
						aonsum+=A(dis);
					if(OfW[kk][ll]!=0 && OfW[kk][ll]!=Smax*A(dis))
						aofsum+=A(dis);
					Sum+=wc[kk][ll].OnCh+wc[kk][ll].OfCh;
				}
			epsilon=Sum/(aonsum+aofsum);
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					dis=sqrt(k*k+l*l);	
					kk=k+ab/2;ll=l+ab/2;
					if(OnW[kk][ll]!=0 && OnW[kk][ll]!=Smax*A(dis))
						wc[kk][ll].OnCh -= epsilon*A(dis);
					if(OfW[kk][ll]!=0 && OfW[kk][ll]!=Smax*A(dis))
						wc[kk][ll].OfCh -= epsilon*A(dis);
				}

/*			
			Sum=0.0;
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					kk=k+ab/2;ll=l+ab/2;
					Sum+=wc[kk][ll].OnCh+wc[kk][ll].OfCh;
				}
			cerr << "For " << i << "\t" << j << " is : " 
				 << Sum << endl ;
*/
		}
}

/*******************************************************************/
//	4-D Convolution for computing On wt. change..

void Orient :: uOnWtChange(int xx, int xy, int ax, int ay)
{
	int i,j,k,l;
	int ii,jj;
	double dis ;
	double adis=sqrt(ax*ax+ay*ay);
	double sumOn=0.0;
	double lsum=0.0;
	double ** OnW, ** OfW;
	int kk=ax+ab/2;
	int ll=ay+ab/2;
	OnW=cortex[xx][xy].OnWeights;
	if(OnW[kk][ll]==0 || OnW[kk][ll]==Smax*A(adis)) {
		wch[xx][xy].wd2[kk][ll].OnCh=0.0;
		wch[xx][xy].wd1[kk][ll].OnCh=0.0;
		wch[xx][xy].wd[kk][ll].OnCh=0.0;
		return;
	}
	for(ii=xx-ab/2; ii<xx+ab/2; ii++){
		for(jj=xy-ab/2; jj<xy+ab/2; jj++){
			i=wrapx(ii);
			j=wrapy(jj);
			OnW=cortex[i][j].OnWeights;
			OfW=cortex[i][j].OfWeights;
			lsum=0.0;
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					dis=cordis(xx+ax,xy+ay,i+k,j+l);
					lsum += Cx_x(dis)*OnW[k+ab/2][l+ab/2]+
								Cx_y(dis)*OfW[k+ab/2][l+ab/2];
				}
			dis=sqrt((xx-ii)*(xx-ii)+(xy-jj)*(xy-jj));
			sumOn += lsum*I(dis) ;
		}
	}
	sumOn *= lamda*A(adis) ;
	wch[xx][xy].wd2[kk][ll].OnCh=wch[xx][xy].wd1[kk][ll].OnCh;
	wch[xx][xy].wd1[kk][ll].OnCh=wch[xx][xy].wd[kk][ll].OnCh;
	wch[xx][xy].wd[kk][ll].OnCh=sumOn;
}
					

/*******************************************************************/
//	4-D Convolution for computing Off wt. change..

void Orient :: uOfWtChange(int xx, int xy, int ax, int ay)
{
	int i,j,k,l;
	int ii,jj;
	double dis ;
	double adis=sqrt(ax*ax+ay*ay);
	double sumOf=0.0;
	double lsum=0.0;
	double ** OnW, ** OfW;
	int kk=ax+ab/2;
	int ll=ay+ab/2;
	OfW=cortex[xx][xy].OfWeights;
	if(OfW[kk][ll]==0 || OfW[kk][ll]==Smax*A(adis)) {
		wch[xx][xy].wd2[kk][ll].OfCh=0.0;
		wch[xx][xy].wd1[kk][ll].OfCh=0.0;
		wch[xx][xy].wd[kk][ll].OfCh=0.0;
		return;
	}
	for(ii=xx-ab/2; ii<xx+ab/2; ii++){
		for(jj=xy-ab/2; jj<xy+ab/2; jj++){
			i=wrapx(ii);
			j=wrapy(jj);
			OnW=cortex[i][j].OnWeights;
			OfW=cortex[i][j].OfWeights;
			lsum=0.0;
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					dis=cordis(xx+ax,xy+ay,i+k,j+l);
					lsum += Cx_x(dis)*OfW[k+ab/2][l+ab/2]+
								Cx_y(dis)*OnW[k+ab/2][l+ab/2];
				}
			dis=sqrt((xx-ii)*(xx-ii)+(xy-jj)*(xy-jj));
			sumOf += lsum*I(dis) ;
		}
	}
	sumOf *= lamda*A(adis) ;
	wch[xx][xy].wd2[kk][ll].OfCh=wch[xx][xy].wd1[kk][ll].OfCh;
	wch[xx][xy].wd1[kk][ll].OfCh=wch[xx][xy].wd[kk][ll].OfCh;
	wch[xx][xy].wd[kk][ll].OfCh=sumOf;
}
					

/*******************************************************************/
int Orient :: wrapx(int i)
{
    return((i<0)?(rows+i):((i>(rows-1))?(i-rows):i));
}

/********************************************************************/

int Orient :: wrapy(int i)
{
    return((i<0)?(cols+i):((i>(cols-1))?(i-cols):i));
}

/********************************************************************/

