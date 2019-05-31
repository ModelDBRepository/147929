#include"Analysis.h"

#define ORG_MIL 1
#define SIN_MIL 2
#define SIN_DEV 3
#define ANAL	4

void printhelp(char exname[])
{
	cerr << "\n\nUsage : \n\n" 
		 << "\t" << exname 
		 << " [-help] [-org] [-sin] [-dev] [-s] [filename]\n\n"
		 << " -help \t\t displays this message\n"
		 << " -org  \t\t runs millers original algorithm\n"
		 << " -sin  \t\t runs millers algorithm with sinusoids\n"
		 << " -dev  \t\t runs develop with self--organizing scheme\n"
		 << " -anal \t\t runs analysis with the status file\n"
		 << " -s    \t\t lets program run with status file\n"
		 << " filename\t is the status or parameter file.\n\n";
	exit(1);
}

main(int argc, char ** argv)
{
	int count;
	char * fila=new char[50];
	fila[0]='\0';
	int SET=0;
	int PROC=SIN_DEV;
	Orient orient ;
	Develop develop ;
	Analysis analysis ;

	for(count=1;count<argc;count++) {
        if (!strcmp(argv[count],"-s")) SET=1;
		else if (!strcmp(argv[count],"-org")) PROC=ORG_MIL;
		else if (!strcmp(argv[count],"-sin")) PROC=SIN_MIL;
		else if (!strcmp(argv[count],"-dev")) PROC=SIN_DEV;
		else if (!strcmp(argv[count],"-anal")) PROC=ANAL;
		else if (!strcmp(argv[count],"-help")) printhelp(argv[0]);
		else strcpy(fila, argv[count]);
	}
	if(fila[0] == '-'){
		cerr <<"\nUnknown Option : " << fila << endl ;
		printhelp(argv[0]);
	}

// Filename is not there in the command line..

	if(fila[0] == '\0') fila=NULL;


// Analysis of the RF's after simulation.


	if(PROC==ANAL){			
		analysis.LoadStatus(fila);
		analysis.AnalyzeRF();
	}

/************************************************************************
*
* Original Miller's simulation.
* If SET is set, then it means the filename has to be a status file
* Otherwise it means that it is a Parameters file only.
*
************************************************************************/

	else if(PROC==ORG_MIL){
		if(SET) orient.LoadStatus(fila);
		else orient.LoadParams(fila);
		orient.Start();
	}

/***********************************************************************
*
* Handles two cases: Miller's algo with sinusoids and the self
* -organized development of RF's with status file or parameter file.
*
* If SET is set, then it means the filename has to be a status file
* Otherwise it means that it is a Parameters file only.
*
***********************************************************************/

	else{
		if(SET) develop.LoadStatus(fila);
		else develop.LoadParams(fila);
		if(PROC==SIN_MIL) develop.Start_Sin();
		else develop.Start();
	}
}
