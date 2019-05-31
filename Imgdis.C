#include"Imgdis.h"

Imgdis :: Imgdis ()
{
	display=NULL;
	Title=strdup(TITLE);
	wheight=wwidth=0;
}

void Imgdis :: setTitle(char *title)
{
	Title = strdup(title);
}

void Imgdis :: close(void)
{
	if(display) XCloseDisplay(display) ;
}

void Imgdis :: setWindowsize(int heigh, int widt)
{
	wheight=heigh ;
	wwidth=widt;
}

void Imgdis :: setImagesize(int heigh, int widt)
{
	height = heigh;
	width=widt;
}

void Imgdis :: setupXWindow()
{
	Window rootWindow ;	/* the root window */
	XSizeHints Hint ;	/* hints to X Server on window raising */
	unsigned long Foregrnd, Backgrnd ;
	
	display = XOpenDisplay(NULL) ;
	if( display == NULL ) 
	{ 
      	    fprintf(stderr, "\nERROR: Cannot connect to X server\n" );
	    exit(1);
   	}
   	screen = DefaultScreen( display);
  	rootWindow = RootWindow( display, screen);
   	Backgrnd = WhitePixel( display, screen);
   	Foregrnd = BlackPixel( display, screen);
   	
   	visual = DefaultVisual(display, screen) ;

	if((wwidth==0) ||(wheight==0))
	{
	    fprintf(stderr,"\nWindow Height and width not set.\n");
	    exit(1);
	}
	Hint.width=wwidth;
	Hint.height=wheight;
	Hint.flags = PPosition | PSize;

	window = XCreateSimpleWindow( display, rootWindow, 
			Hint.x, Hint.y, Hint.width, Hint.height,
			(unsigned int) BORDERWIDTH, Foregrnd, Backgrnd);


	XSetWMNormalHints(display, window, &Hint) ;
   	XSelectInput( display, window, 
		ButtonPressMask | KeyPressMask | ExposureMask |
		ButtonReleaseMask | Button1MotionMask | PointerMotionMask );
	
  	XMapRaised(display, window);
	XStoreName(display, window, Title);
	XFlush(display) ;
}

void Imgdis :: setupGC( )
{
   	XGCValues gcvalues;
   	unsigned long mask;
   	XColor xcolor ;
   	int i ;
	Colormap colormap;
	
	cmapsize = 0 ;
	colormap = DefaultColormap(display, screen) ;
		
      	while (XAllocColorCells(display, colormap, False, NULL, 0,
	     &(pixels[cmapsize]), 1))
		(cmapsize)++;
		
      	xcolor.flags = DoRed | DoGreen | DoBlue;
      	for (i = 0; i < cmapsize; ++i)
      	{
		xcolor.pixel = pixels[i];
	 	xcolor.red = (unsigned short) (i << 8);
	 	xcolor.green = (unsigned short) (i << 8); 
	 	xcolor.blue = (unsigned short) (i << 8); 
	 	XStoreColor(display, colormap, &xcolor);
      	}

      	mask = GCForeground | GCBackground | GCLineWidth | GCFunction;
      	gcvalues.function = GXcopy;
      	gcvalues.foreground = pixels[cmapsize - 1];
      	gcvalues.background = pixels[0];
      	gcvalues.line_width = 2 ;
      	imageGC = XCreateGC(display, window, mask, &gcvalues);
		XFreeColors(display,colormap,pixels,cmapsize,0);
}

XImage * Imgdis :: createXimage(Imgstr ims, unsigned int magnify)		
{
	register short i, j, m, n ;
	unsigned char *box ;
	double maxmag=0, minmag=0;
	double range ;
	int index ;

	
	box = (unsigned char *) malloc(sizeof(unsigned char) * 
			ims.xs * ims.ys * magnify * magnify) ;
	for(i=0; i<ims.xs; i++)
	    for(j=0;j<ims.ys;j++)
	    {
		if(ims.ima[i][j]<minmag) minmag = ims.ima[i][j];
		if(ims.ima[i][j]>maxmag) maxmag = ims.ima[i][j];
	    }
        if(minmag==maxmag)maxmag = minmag+1;

	range = (maxmag - minmag)/0.99 ;	/* use 99% of the range */
	
	for (i = 0; i < ims.xs; ++i)
            for (j = 0; j < ims.ys; ++j)  {
            	
		index = (int)(cmapsize * (ims.ima[i][j]-minmag)/range);
		
	    	for(m=0; m<magnify; m++)
	    	  for(n=0; n<magnify; n++) 
	    	  	box[(i*magnify+m)*magnify*ims.ys + 
	    	  	     j*magnify + n] = (unsigned char)pixels[index] ;
	}

	return XCreateImage(display, DefaultVisual(display, screen), 
			    (unsigned int) 8, ZPixmap, 0, (char *) box, 
			    ims.ys*magnify, magnify*ims.xs, 8,  
						    ims.ys*magnify);   
}

void Imgdis :: drawImg(XImage *ximg, int xoffset, int yoffset)
{
	XPutImage(display, window, imageGC, ximg, 
		0, 0, xoffset, yoffset, ximg->width, ximg->height) ;
	XFlush(display) ; 

}

void Imgdis :: displayImage (Imgstr ims, unsigned int magnify, int xoffset, int yoffset)
{
    XImage * ximage ;
    int prx, pry;
    setWindowsize(ims.xs*magnify+yoffset,ims.ys*magnify+xoffset);
    setupXWindow();
    setupGC();	
    ximage = createXimage (ims,magnify);
    int tempx,tempy;
    if(fork()==0)
    {
    while(1)
    {
	XNextEvent(display,&event);
	switch(event.type)
	{
	case MotionNotify:
	    prx=event.xmotion.x;
	    pry=event.xmotion.y;
	    break;
	case Expose :
	    drawImg(ximage,xoffset,yoffset);
	    break;
	case ButtonRelease :
	    switch(event.xbutton.button)
	    {
	      case Button1:
		if((pry<yoffset)||(prx<xoffset)) ;
		else
		{
	          tempx = (pry-yoffset)/magnify;
		  tempy = (prx-xoffset)/magnify;
		  system("clear");
   		  cerr << "\rXval = " << tempx << "  Yval = " <<  tempy 
		       << "  Value = " << ims.ima[tempx][tempy]<< "\r";
		}
		break;
	      case Button2:
		close();
		exit(1);
	      default : ;	
	    }
	    break;
	default : ;
	}
    }
    }
}


void Imgdis :: OffsetInit(int xsize, int ysize)
{
    setWindowsize(xsize,ysize);
    setupXWindow();
    setupGC();
}
    

void Imgdis :: OffsetDisplay(Imgstr pic, unsigned int magnify,int xoffset, 
				int yoffset)

{
    XImage * img1 ;
    img1=createXimage(pic,magnify);
    drawImg(img1,xoffset,yoffset);
}
