#ifndef __IMG_DIS_H__
#define __IMG_DIS_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include<fstream.h>
#include<iostream.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>

#define COLORMAPSIZE	256
#define TITLE		"Image Display Window"
#define BORDERWIDTH	5		/* border width = 5 pixels */

#ifndef __IMAGETY
#define __IMAGETY

typedef unsigned char ** Img ;
typedef double ** DImg ;
typedef float ** FImg ;

struct Imgstr
{
	DImg ima ;
	int xs;
	int ys;
};
#endif



class Imgdis
{
protected:

	Display         *display;		/* display and others */
	int		screen;
	Window		window;
	Visual		*visual;
	GC       	imageGC;		/* graphic contexts */
	unsigned long 	pixels[COLORMAPSIZE];	/* pixel values */
	int		cmapsize;		/* colormap size */
	XEvent		event;
	
	char 		*Title ;
	int 		width ;
	int 		height ;
	int 		wheight ;
	int		wwidth;

public :

	Imgdis ();

	void close(void);

	XImage * createXimage(Imgstr, unsigned int magnify=1);

	void displayImage(Imgstr,unsigned int magnify =1, 
		int xoffset =0, int yoffset = 0); 

	void drawImg(XImage *ximg, int xoffset=0, int yoffset=0);

	void setTitle(char *title);

	void setupXWindow(); 

	void setupGC();

	void setWindowsize(int heigh, int widt);

	void setImagesize(int,int);

	void OffsetInit(int,int);

	void OffsetDisplay(Imgstr,unsigned int = 1, int= 0, int = 0 );

};

#endif
