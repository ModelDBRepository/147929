#ifndef __VECTORH
#define __VECTORH

#include<fstream.h>
#include<iostream.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>




class Vector 

{
protected:

	double xcoord ; 

	double ycoord ; 
public: 

	Vector ( ) ; 

	Vector ( double , double ) ;//constructor to enable overloading

	void setVector (double , double );

	double getxcoord ( ) ;//To get x and y coords.

	double getycoord ( ) ;

	Vector operator + ( Vector & v1 ) ;

	Vector operator - ( Vector & v1 ) ;

	Vector operator * ( double c ) ; //Scalar multiplication 

	Vector  operator / ( double ) ;//Scalar division

	void operator += (Vector v1) ;
	
	void operator -= (Vector v1) ;

//*********overloaded extraction operators.*************/
	
	friend ostream & operator << ( ostream& , Vector ) ;

	friend istream & operator >> ( istream & , Vector & ) ;

	Vector multiplywith ( double , double  ) ;
	
	Vector divideby ( double , double  ) ;

//******** Distance Functions ***********************/

	friend double Euclidean (Vector v1, Vector v2);

	friend double Norm      (Vector v);

	friend double InnerProduct     (Vector v1, Vector v2);
};


#endif
