#include"Orient.h"

/*******************************************************************/
// 	Adjust synapses, cutoff and renormalize ...

void Orient :: adjustsynapse()
{
    int i,j,k,l;
	int SET = 0;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
            for(k=-ab/2; k<=ab/2; k++)
                for(l=-ab/2; l<=ab/2; l++){
					threestep(i,j,k,l);
					SET+=cutoff(i,j,k,l);
				}
			if(SET){	// 	Renormalize only when something has been cutoff..
				renormalize(i,j);
				SET=0;
			}
		}
}

/*******************************************************************/
// 	Three step method for computing total change in synapse..

void Orient :: threestep(int i, int j, int k, int l)
{
    double ** OnW=cortex[i][j].OnWeights;
    double ** OfW=cortex[i][j].OfWeights;
    WtChange ** wc=wch[i][j].wd;
    WtChange ** wc1=wch[i][j].wd1;
    WtChange ** wc2=wch[i][j].wd2;
	int kk=k+ab/2; 
	int ll=l+ab/2;
	if(it==0){
		OnW[kk][ll]+=wc[kk][ll].OnCh;
		OfW[kk][ll]+=wc[kk][ll].OfCh;
	}
	else if(it==1){
		OnW[kk][ll]+=(2*wc[kk][ll].OnCh-wc1[kk][ll].OnCh);
		OfW[kk][ll]+=(2*wc[kk][ll].OfCh-wc1[kk][ll].OfCh);
	}
	else{
		OnW[kk][ll]+=(23*wc[kk][ll].OnCh-16*wc1[kk][ll].OnCh+
					5*wc2[kk][ll].OnCh)/6.0;
		OfW[kk][ll]+=(23*wc[kk][ll].OfCh-16*wc1[kk][ll].OfCh+
					5*wc2[kk][ll].OfCh)/6.0;
	}
}

/*******************************************************************/
// Cut off value to min and max values of synapses..

int Orient :: cutoff(int i, int j, int k, int l)
{
	int SET = 0;
	int kk=k+ab/2; 
	int ll=l+ab/2;
    double ** OnW=cortex[i][j].OnWeights;
    double ** OfW=cortex[i][j].OfWeights;
	if(OnW[kk][ll]<0) {
		OnW[kk][ll]=0;
		SET = 1;
	}
	if(OfW[kk][ll]<0) {
		OfW[kk][ll]=0;
		SET = 1;
	}
	double dis=sqrt(k*k+l*l);
	double max = Smax*A(dis);
	if(OnW[kk][ll]>max) {
		OnW[kk][ll]=max;
		SET = 1;
	}
	if(OfW[kk][ll]>max) {
		OfW[kk][ll]=max;
		SET = 1;
	}
	return SET ;
}

/*******************************************************************/
// 	Renormalize according to cutoff of synapses..

void Orient :: renormalize (int i, int j)
{
	int k,l,kk,ll;
	double dis ;
	double gamma,ASum=0.0,FSum=0.0; 
    double ** OnW=cortex[i][j].OnWeights;
    double ** OfW=cortex[i][j].OfWeights;
	double asum=0.0;
    for(k=-ab/2; k<=ab/2; k++)
        for(l=-ab/2; l<=ab/2; l++){
			kk=k+ab/2; ll=l+ab/2;
            dis=sqrt(k*k+l*l);
            asum+=A(dis);
			if(OnW[kk][ll]==0 || OnW[kk][ll]==Smax*A(dis))
				FSum += OnW[kk][ll];
			else ASum += OnW[kk][ll];
			if(OfW[kk][ll]==0 || OfW[kk][ll]==Smax*A(dis))
				FSum += OfW[kk][ll];
			else ASum += OfW[kk][ll];
        }
	gamma =	(2.0*asum-FSum)/ASum;
	gamma = (gamma<0.8)?0.8:gamma ;
	gamma = (gamma>1.2)?1.2:gamma ;
    for(k=-ab/2; k<=ab/2; k++)
    	for(l=-ab/2; l<=ab/2; l++){
            dis=sqrt(k*k+l*l);
			kk=k+ab/2; ll=l+ab/2;
			if(OnW[kk][ll]!=0 && OnW[kk][ll]!=Smax*A(dis))
				OnW[kk][ll] *= gamma ;
			if(OfW[kk][ll]!=0 && OfW[kk][ll]!=Smax*A(dis))
				OfW[kk][ll] *= gamma ;
		}
}

/*******************************************************************/
// Return percent of synapses that have reached boundary values.. 

double Orient :: perc()
{
	int i,j,k,l,kk,ll;
    double ** OnW;
    double ** OfW;
	double dis,percent =0;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
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
		}
	percent *= 100 ;
	percent /= (ab*ab*rows*cols*2.0);
	return percent ;
}

/*******************************************************************/
// 	Store synapses as double ..

void Orient :: store()
{
	int i,j,k,l;
    double ** OnW;
    double ** OfW;
	ofstream outfile(outfilename);
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++){
    		OnW=cortex[i][j].OnWeights;
    		OfW=cortex[i][j].OfWeights;
    		for(k=-ab/2; k<=ab/2; k++){
        		for(l=-ab/2; l<=ab/2; l++)
					outfile << OnW[k+ab/2][l+ab/2] << " " ;
				outfile << "   " ;
        		for(l=-ab/2; l<=ab/2; l++)
					outfile << OfW[k+ab/2][l+ab/2] << " " ;
				outfile << endl ;
			}
			outfile << endl << endl ;
		}
	outfile.close();
}

/*******************************************************************/
// 	Store synapse diff. as image..

void Orient :: storediff()
{
	int i,j,k,l;
    double ** OnW;
    double ** OfW;
	char sfn[35];
	sprintf(sfn,"%s.syn",oflnm);
	ofstream outfile(sfn);
//	ofstream outfilen("test.on");
//	ofstream outfilef("test.of");
	unsigned char pix ;
	double diff ;
	double max=Smax*A(0);	// Max value that any synapse can attain..
//	double max,dis;
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
//					dis=sqrt(k*k+l*l);
//					max=Smax*A(dis);
/*					if(!max){
						image[i*ab+k+ab/2][j*ab+l+ab/2]=127;
						imagen[i*ab+k+ab/2][j*ab+l+ab/2]=127;
						imagef[i*ab+k+ab/2][j*ab+l+ab/2]=127;
					}
					else{
*/
						diff = OnW[k+ab/2][l+ab/2]-OfW[k+ab/2][l+ab/2];
						pix = (unsigned char)((255.0*(diff+max))/(2.0*max));
						image[i*ab+k+ab/2][j*ab+l+ab/2]=pix;

						diff = OnW[k+ab/2][l+ab/2];
						pix = (unsigned char)((255.0*diff)/max);
						imagen[i*ab+k+ab/2][j*ab+l+ab/2]=pix;

						diff = OfW[k+ab/2][l+ab/2];
						pix = (unsigned char)((255.0*diff)/max);
						imagef[i*ab+k+ab/2][j*ab+l+ab/2]=pix;
//					}
			}
		}
	for(i=0;i<rows*ab;i++){
		outfile.write((const char *)image[i],cols*ab);
//		outfilen.write(imagen[i],cols*ab);
//		outfilef.write(imagef[i],cols*ab);
		delete(image[i]);
		delete(imagen[i]);
		delete(imagef[i]);
	}
	delete(image);
	delete(imagen);
	delete(imagef);
	outfile.close();
}

/*******************************************************************/
// 	Final Algorithm..

void Orient :: Start()
{
    do{
        storediff();
        sprintf(outfilename,"%s%d.stt",oflnm,it);
		saveStatus();
        cerr << "\nStarted Computation..";
        computeUWtChange();
        cerr << "\nComputed UWtChange..";
        computeCWtChange();
        cerr << "\nComputed CWtChange..";
        adjustsynapse();
        it++;
        cerr << "\nAdjusted Synapses..";
        cerr << "\nAt iteration number " << it
             << " Percentage of synapses saturated is "
             << perc() << endl ;
    }
    while(perc()<90);
    storediff();
    sprintf(outfilename,"%s%d.stt",oflnm,it);
	saveStatus();
}

/*****************************************************************/

void Orient :: saveStatus()
{
	int i,j,k,l;
    double ** OnW;
    double ** OfW;
	ofstream outfile(outfilename);
	outfile << rows  << endl << cols  << endl;
	outfile << ab    << endl << 79342 << endl;
	outfile << rC    << endl << rI    << endl;
	outfile << Smax  << endl << lamda << endl;
	outfile << oflnm << endl << it    << endl;
	outfile << endl  << endl ;
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

void Orient :: LoadStatus(char * filename)
{
    char name[35];
    if(!filename){
        cout << "\nGive Status Filename[.stt] : ";
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

	ParamFile >> it ;
	assignspace();
	
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

