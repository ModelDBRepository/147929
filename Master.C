#include"Develop.h"

					
/*******************************************************************/
//  Final Algorithm..

void Develop :: Start()
{
	char temp[35];
	sprintf(temp,"%s.prc",oflnm);
	ofstream outfile(temp);
    it=0;
	double initprc ;
	sN=sNi;
	initprc=Orient::perc();
    do{
        storediff();
		saveStatus();
		F=Fmin+drand48()*(Fmax-Fmin);
		T=drand48()*M_PI;
		phi=drand48()*2*M_PI; // Random phase shift.
//		phi=0.0;
		generateImage();
		computemasked();
		findwinner();
        computeUWtChange();
        computeCWtChange();
		if(Orient::perc()>(initprc+10)){
			sN/=2.0;
			if(sN<sNf) sN=sNf;
			initprc+=10;
		}
        adjustsynapses();
		it++;
		outfile << it << "\t" << Orient::perc() << "\t" 
				<< perc() << "\t" << sN << endl ;
//        cerr << "\nAt iteration number " << it
//             << " percentage of synapses saturated is " << Orient::perc() 
//			 << ", " << perc() << endl ;
    }
    while(perc()<90);
    storediff();
	saveStatus();
}

/*******************************************************************/

void Develop :: Start_Sin()
{
	char temp[35];
    it=0;
	T=M_PI/4.0;
	sprintf(temp,"%s.prc",oflnm);
	ofstream outfile(temp);
    do{
        storediff();
		saveStatus();
		F=Fmin+drand48()*(Fmax-Fmin);
		T=drand48()*M_PI;
		phi=drand48()*2*M_PI; 
        computeUWtChange_Sin();
        computeCWtChange();
        adjustsynapse();
        outfile << it++ << "\t" << F << "\t" << T << "\t" 
				<< Orient::perc() << endl ;
    }
    while(Orient::perc()<90);
    storediff();
	saveStatus();
}

/*****************************************************************/

void Develop :: saveStatus()
{
	int i,j,k,l;
    double ** OnW;
    double ** OfW;
    char sfn[35];
	sprintf(sfn,"%s.dst",oflnm);
	ofstream outfile(sfn);
	outfile << rows  << endl << cols  << endl;
	outfile << ab    << endl << 79342 << endl;
	outfile << rC    << endl << rI    << endl;
	outfile << Smax  << endl << lamda << endl;
	outfile << sNi   << endl << sNf   << endl;
	outfile << oflnm << endl << Fmax  << endl;
	outfile << Fmin  << endl << endl  << endl ;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
            OnW=cortex[i][j].OnWeights;
            OfW=cortex[i][j].OfWeights;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++)
					outfile << OnW[k+ab/2][l+ab/2] << "   " ;
			outfile << endl << endl ;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++)
					outfile << OfW[k+ab/2][l+ab/2] << "   " ; 
			outfile << endl << endl ;
		}
	outfile << endl << endl ;
	outfile.close();
}

/******************************************************************/

void Develop :: LoadStatus(char * filename)
{
    char name[35];
    if(!filename){
        cout << "\nGive Status Filename[.dst] : ";
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
	ParamFile >> sNi ;
	ParamFile >> sNf ;
    ParamFile >> oflnm ;

	ParamFile >> Fmax ;
	ParamFile >> Fmin ;
	assignspace();
	Orient::assignspace();
	
	int i,j,k,l;
	double ** OnW, ** OfW;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
            OnW=cortex[i][j].OnWeights;
            OfW=cortex[i][j].OfWeights;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++)
					ParamFile >> OnW[k+ab/2][l+ab/2] ;
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++)
					ParamFile >> OfW[k+ab/2][l+ab/2] ;
		}
    ParamFile.close();
    srand48(seed);
}

/*****************************************************************/

void Develop :: computeUWtChange_Sin()
{
    int i,j,k,l;
    for(i=0; i<rows; i++){
        //cerr << "\nIn computeUWtChange "  << i << " .." ;
        for(j=0; j<cols; j++)
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++){
                    uOnWtChange_Sin(i,j,k,l);
                    uOfWtChange_Sin(i,j,k,l);
                }
    }
}

/*******************************************************************/

void Develop :: uOnWtChange_Sin(int xx, int xy, int ax, int ay)
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
					lsum += Scxx(xx+ax-(i+k),xy+ay-(j+l))*OnW[k+ab/2][l+ab/2]+
							Scxy(xx+ax-(i+k),xy+ay-(j+l))*OfW[k+ab/2][l+ab/2];
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

void Develop :: uOfWtChange_Sin(int xx, int xy, int ax, int ay)
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
					lsum += Scxx(xx+ax-(i+k),xy+ay-(j+l))*OfW[k+ab/2][l+ab/2]+
							Scxy(xx+ax-(i+k),xy+ay-(j+l))*OnW[k+ab/2][l+ab/2];
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
