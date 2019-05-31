#include"Vector.h"

Vector :: Vector ( ) 
{
   	xcoord = 0.0 ;
   	ycoord = 0.0 ;
}

/*************************************************************/

Vector :: Vector ( double x, double y ) 
{
	xcoord = x ;
	ycoord = y ;
}

/*************************************************************/

void Vector :: setVector (double x, double y)
{
	xcoord = x ;
	ycoord = y ;
}

/*************************************************************/

double Vector :: getxcoord ( ) 
{
	return xcoord ;
}

/*************************************************************/


double Vector :: getycoord ( ) 
{
	return ycoord  ;
}

/*************************************************************/

Vector Vector :: operator + ( Vector & f1 ) 
{
	Vector temp;
	double xcoo =  xcoord + f1. xcoord ;
	double ycoo = ycoord + f1.ycoord ;
	temp.xcoord=xcoo;
	temp.ycoord=ycoo;
	return temp ;
}

/*************************************************************/

Vector Vector :: operator - ( Vector & f1 ) 
{
	Vector temp;
	double xcoo =  xcoord - f1. xcoord ;
	double ycoo = ycoord - f1.ycoord ;
	temp.xcoord=xcoo;
	temp.ycoord=ycoo;
	return temp ;
}

/*************************************************************/

Vector Vector :: operator * ( double c  ) 
{
	Vector temp;
	double xcoo = xcoord * c ; 
	double ycoo = ycoord * c ;
	temp.xcoord=xcoo;
	temp.ycoord=ycoo;
	return temp ; 
}


/*************************************************************/

ostream & operator << (ostream & s, Vector vect ) 
{
	s << vect . xcoord << "  " << vect . ycoord << endl ;
	return s ; 
}

/*************************************************************/

istream & operator >> ( istream & s , Vector & vect ) 
{
	if(s == cin)
	    cout <<"\nGive Xcoord: " ;
	s >> vect . xcoord ;
	if(s == cin)
	    cout <<"\nGive Ycoord: " ;
	s >> vect . ycoord ;
	return s ;
}

/*************************************************************/

Vector Vector :: operator / ( double s ) 
{
	Vector temp;
	double xcoo = xcoord / s ; 
	double ycoo = ycoord / s ;
	temp.xcoord=xcoo;
	temp.ycoord=ycoo;
	return temp ; 
}

/*************************************************************/

void Vector :: operator += (Vector v)
{
  	xcoord += v.xcoord ;
	ycoord += v.ycoord ;
}

/*************************************************************/

void Vector :: operator -= (Vector v)
{
  	xcoord -= v.xcoord ;
	ycoord -= v.ycoord ;
}

/*************************************************************/

Vector  Vector :: multiplywith  ( double xmul, double ymul ) 
{
	Vector temp;
	double xcoo = xcoord * xmul ;
	double ycoo = ycoord * ymul ;
	temp.xcoord=xcoo;
	temp.ycoord=ycoo;
	return temp ;
}

/*************************************************************/

Vector Vector :: divideby ( double xdiv , double ydiv ) 
{
	Vector temp;
	double xcoo = xcoord / xdiv ;
	double ycoo = ycoord / ydiv ;
	temp.xcoord=xcoo;
	temp.ycoord=ycoo;
	return temp ;
}

/*************************************************************/

double Euclidean(Vector v1, Vector v2)
{
	return (sqrt((v1.xcoord-v2.xcoord)*(v1.xcoord-v2.xcoord)+
		     (v1.ycoord-v2.ycoord)*(v1.ycoord-v2.ycoord)));
}

/*************************************************************/

double Norm (Vector v)
{
	return(sqrt(v.xcoord*v.xcoord+v.ycoord*v.ycoord));
}

/*************************************************************/

double InnerProduct(Vector v1, Vector v2)
{
	return(v1.xcoord*v2.xcoord+v1.ycoord*v2.ycoord);
}


/*************************************************************/

