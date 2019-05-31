#include"Develop.h"

/********************************************************************/
//  Compute Unconstrained Weight Change and Update Prevs..

void Develop :: computeUWtChange()
{
    int i,j,k,l;
	int kk,ll;
//	cerr << "\nFreq of Sinusoid: " << F ;
//	cerr << "\nAngle of Sinusoid: " << T*180.0/M_PI ;
//	cerr << "\nWinner is : " << wx << "\t" << wy  ;
	for(i=0;i<rows;i++)
		for(j=0; j<cols; j++)
			for(k=-ab/2; k<=ab/2; k++)
				for(l=-ab/2; l<=ab/2; l++){
					kk=k+ab/2;
					ll=l+ab/2;
					wch[i][j].wd2[kk][ll].OnCh=wch[i][j].wd1[kk][ll].OnCh;
					wch[i][j].wd2[kk][ll].OfCh=wch[i][j].wd1[kk][ll].OfCh;
					wch[i][j].wd1[kk][ll].OnCh=wch[i][j].wd[kk][ll].OnCh;
					wch[i][j].wd1[kk][ll].OfCh=wch[i][j].wd[kk][ll].OfCh;
					wch[i][j].wd[kk][ll].OnCh=0.0;
					wch[i][j].wd[kk][ll].OfCh=0.0;
				}
	for(i=wx-ab/2; i<=wx+ab/2; i++)
      	for(j=wy-ab/2; j<=wy+ab/2; j++)
    		for(k=-ab/2; k<=ab/2; k++)
    			for(l=-ab/2; l<=ab/2; l++){
					uOnWtChange(i,j,k,l);
					uOfWtChange(i,j,k,l);
				}

}

/*******************************************************************/

void Develop :: adjustNeighbor(int i, int j)
{
	int ii=wrapx(i);
	int jj=wrapy(j);
	if(prcnt(ii,jj)>90) return;
	double adis;
	double ** OnW=cortex[ii][jj].OnWeights;
	double ** OfW=cortex[ii][jj].OfWeights;
	double wt;
	int k,l,kk,ll;
	wt=exp(-((wx-i)*(wx-i)+(wy-j)*(wy-j))/(sN*sN))  ;
    for(k=-ab/2; k<=ab/2; k++)
    	for(l=-ab/2; l<=ab/2; l++){
			kk=k+ab/2; ll=l+ab/2;
			adis=sqrt(k*k+l*l);
			if(OnW[kk][ll]==0 || OnW[kk][ll]==Smax*A(adis)) {
				wch[ii][jj].wd2[kk][ll].OnCh=0.0;
				wch[ii][jj].wd1[kk][ll].OnCh=0.0;
				wch[ii][jj].wd[kk][ll].OnCh=0.0;
			}
			else
	  			wch[ii][jj].wd[kk][ll].OnCh=wt*wch[wx][wy].wd[kk][ll].OnCh;
			if(OfW[kk][ll]==0 || OfW[kk][ll]==Smax*A(adis)) {
				wch[ii][jj].wd2[kk][ll].OfCh=0.0;
				wch[ii][jj].wd1[kk][ll].OfCh=0.0;
				wch[ii][jj].wd[kk][ll].OfCh=0.0;
			}
			else
				wch[ii][jj].wd[kk][ll].OfCh=wt*wch[wx][wy].wd[kk][ll].OfCh;
    	}
}

/*******************************************************************/
//	4-D Convolution for computing On wt. change..

void Develop :: uOnWtChange(int xx, int xy, int ax, int ay)
{
	int k,l;
	int ii=wrapx(xx);
	int jj=wrapy(xy);
	if(prcnt(ii,jj)>90) return;
	double ** OnW=cortex[ii][jj].OnWeights;
	double ** OfW=cortex[ii][jj].OfWeights;
	double wt=exp(-((wx-xx)*(wx-xx)+(wy-xy)*(wy-xy))/(sN*sN));
	double adis=sqrt(ax*ax+ay*ay);
	double sumOn=0.0;
	int kk=ax+ab/2;
	int ll=ay+ab/2;
	if(OnW[kk][ll]==0 || OnW[kk][ll]==Smax*A(adis)) {
		wch[ii][jj].wd2[kk][ll].OnCh=0.0;
		wch[ii][jj].wd1[kk][ll].OnCh=0.0;
		wch[ii][jj].wd[kk][ll].OnCh=0.0;
		return;
	}
	for(k=-ab/2; k<=ab/2; k++)
		for(l=-ab/2; l<=ab/2; l++)
			sumOn += Scxx(ax-k,ay-l)*OnW[k+ab/2][l+ab/2]+
					Scxy(ax-k,ay-l)*OfW[k+ab/2][l+ab/2];
	sumOn *= lamda*A(adis)*wt;
	wch[ii][jj].wd2[kk][ll].OnCh=wch[ii][jj].wd1[kk][ll].OnCh;
	wch[ii][jj].wd1[kk][ll].OnCh=wch[ii][jj].wd[kk][ll].OnCh;
	wch[ii][jj].wd[kk][ll].OnCh=sumOn;
}
					

/*******************************************************************/
//	4-D Convolution for computing Off wt. change..

void Develop :: uOfWtChange(int xx, int xy, int ax, int ay)
{
    int k,l;
    int ii=wrapx(xx);
    int jj=wrapy(xy);
    if(prcnt(ii,jj)>90) return;
    double ** OnW=cortex[ii][jj].OnWeights;
    double ** OfW=cortex[ii][jj].OfWeights;
    double wt=exp(-((wx-xx)*(wx-xx)+(wy-xy)*(wy-xy))/(sN*sN));
    double adis=sqrt(ax*ax+ay*ay);
    double sumOf=0.0;
    int kk=ax+ab/2;
    int ll=ay+ab/2;
    if(OfW[kk][ll]==0 || OfW[kk][ll]==Smax*A(adis)) {
        wch[ii][jj].wd2[kk][ll].OfCh=0.0;
        wch[ii][jj].wd1[kk][ll].OfCh=0.0;
        wch[ii][jj].wd[kk][ll].OfCh=0.0;
        return;
    }
    for(k=-ab/2; k<=ab/2; k++)
        for(l=-ab/2; l<=ab/2; l++)
            sumOf += Scxx(ax-k,ay-l)*OfW[k+ab/2][l+ab/2]+
                    Scxy(ax-k,ay-l)*OnW[k+ab/2][l+ab/2];
    sumOf *= lamda*A(adis)*wt;
	wch[ii][jj].wd2[kk][ll].OfCh=wch[ii][jj].wd1[kk][ll].OfCh;
	wch[ii][jj].wd1[kk][ll].OfCh=wch[ii][jj].wd[kk][ll].OfCh;
	wch[ii][jj].wd[kk][ll].OfCh=sumOf;
}

/*******************************************************************/
void Develop :: adjustsynapses()
{
    int i,j,k,l;
    int ii,jj,kk,ll;
    double ** OnW;
    double ** OfW;
    WtChange ** wc;
    int SET = 0;
    for(ii=wx-ab/2; ii<=wx+ab/2; ii++)
        for(jj=wy-ab/2; jj<=wy+ab/2; jj++){
			i=wrapx(ii);
			j=wrapy(jj);
            OnW=cortex[i][j].OnWeights;
            OfW=cortex[i][j].OfWeights;
            wc=wch[i][j].wd;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++){
    				kk=k+ab/2;
    				ll=l+ab/2;
        			OnW[kk][ll]+=wc[kk][ll].OnCh;
					OfW[kk][ll]+=wc[kk][ll].OfCh;
                    SET+=cutoff(i,j,k,l);
                }
            if(SET){    //  Renormalize only when something has been cutoff..
                renormalize(i,j);
                SET=0;
            }
        }
}


					
/*******************************************************************/

double Develop :: prcnt(int i, int j)
{
    int k,l,kk,ll;
    double ** OnW;
    double ** OfW;
    double dis,percent =0;
    OnW=cortex[i][j].OnWeights;
    OfW=cortex[i][j].OfWeights;
    for(k=-ab/2; k<=ab/2; k++)
    	for(l=-ab/2; l<=ab/2; l++){
            kk=k+ab/2; ll=l+ab/2;
            dis=sqrt(k*k+l*l);
            if(OnW[kk][ll]==0 || OnW[kk][ll]==Smax*A(dis))
                percent ++ ;
            if(OfW[kk][ll]==0 || OfW[kk][ll]==Smax*A(dis))
                percent ++ ;
        }
    percent *= 100.0 ;
    percent /= (ab*ab*2.0);
    return percent ;
}

/*******************************************************************/

double Develop :: perc()
{
    int i,j;
    double percent =0;
	double min=100;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
			if((percent=prcnt(i,j))<min) min=percent;
    return min ;
}

/*******************************************************************/

double Develop :: Scxx(int x, int y)
{
	double fx=F*cos(T);
	double fy=F*sin(T);
	return(cos(fx*x+fy*y));
}

/*******************************************************************/

double Develop :: Scxy(int x, int y)
{
	double fx=F*cos(T);
	double fy=F*sin(T);
	return(-cos(fx*x+fy*y));
}

/*******************************************************************/

