OBJS=Image.o Imageaux.o Mask.o Imgdis.o Orient1.o Orient2.o Develop.o \
		Dori.o Analysis.o Master.o MasterMain.o Vector.o
CC=g++ -g -Wno-deprecated
LIBS=-lX11 -lm

Master : $(OBJS)
	$(CC) -o Master $(OBJS) $(LIBS)

$(OBJS) : Image.h Imgdis.h Mask.h Orient.h Develop.h Analysis.h Vector.h

.C.o :
	$(CC) -c $*.C

clean :
	rm *.o
