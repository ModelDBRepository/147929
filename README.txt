Development of orientation-selective receptive fields
*****************************************************

Author: Rishikesh Narayanan (rishi.n@gmail.com)
-----------------------------------------------

The files in this directory constitute an implementation of the
following paper:

N. Rishikesh and Y. V. Venkatesh, "A computational model for the
development of simple-cell receptive fields spanning the regimes
before and after eye-opening," Neurocomputing, 50, 125-158, January
2003.

The implementation of this requires the implementation of the
following paper by K. D. Miller. The programs in this directory can
be used for implementing only the Miller paper, or for obtaining
results pertaining to the paper above. See "Usage" section below
for how to use the program for running Miller's algorithm vs. for
obtaining results mentioned in the paper above.

K.D. Miller, "A Model for the Development of Simple Cell
Receptive Field and  the Ordered Arrangment of Orientation Columns
through Activity- dependent Competition between On- and OFF- Center
Inputs," The Journal of Neuroscience, January 1994, 14(1), 409-441.

1. Description
**************

This directory contains a experience dependent self-organizing
development scheme for the orientation selective neurons in the
primary visual cortex.  The normal course as implemented corresponds
to adjusting synapses with a Kohonen type neighborhood with a
correlation learning rule. The angle of the sinusoid keeps changing
and the winner learns the corresponding sinusoidal pattern. The
implementation in this directory employs subtractive normalization.

Start() is for this normal development course. Start_M() is for running
Miller's algorithm with sinusoidal correlation rather than DoG. The
theta value of the function may be fixed in the Start_M() function
of the Master.C file. In this case (Start_M), all the neurons in the
network are updated with a correlational rule.


2. Input File Format (.dev)
***************************

1.     No of LGN & cortex rows. 								
2.     No of LGN & cortex cols. 								
3.     Arbor Diameter.											
4.     Seed Value for random number generator.
5.     Parameter controlling spread of correlation fn.			
6.     Parameter controlling spread of Intracortical fn.		
7.     Upper Bound on Synapse Values.							
8.     Value of the Growth Constant.
9.     Neighborhood Parameter.
10.    Output filename.
11.    Frequency of the sinusoidal gratings.


3. Pre- and Post-natal development
**********************************

The above is the method for the case where the entire run of the
algorithm is dependent upon the self-organizing scheme. This has to be run
as "Master -dev filename.dev".

However, as the model is designed to model both the pre-natal and
post-natal phase of the development, a status file corresponding to the
simulation of the Miller's algorithm upto a certain percentage is fed
as the input to this phase of self-organizing weight update. To this
end, the input file format varies and is as follows:


Parameter File Name[.dst]

1.     No of LGN & cortex rows. 								[32]
2.     No of LGN & cortex cols. 								[32]
3.     Arbor Diameter.											[13]
4.     Seed Value for random number generator.					
5.     Parameter controlling spread of correlation fn.			[0.25]
6.     Parameter controlling spread of Intracortical fn.		[0.3]
7.     Upper Bound on Synapse Values.							[4]
8.     Value of the Growth Constant.							[0.001]
9.     Initial Value of Neighborhood Parameter.					[6]
10.    Final Value of Neighborhood Parameter.					[1]
11.    Output filename.	
12.    Upper bound Frequency of the input sinusoidal gratings.	[0.8]
13.    Lower bound Frequency of the input sinusoidal gratings.	[0.4]
14.    Array of the ON and OFF synapses in the following order:

	(0,0) ON-synapses (It will be an array of arbor diameter X arbor
	diameter)

	(0,0) OFF-synapses (It will be an array of arbor diameter X arbor
    diameter)

	(0,1) ON .... (0,1) OFF ......

	.....
	
	The values within [] represent the default of the typical values of the
parameters. Parameter 5 doesnot play any significant role in the
postnatal phase. Just kept for compatibility.

The usage is : "Master -s filename.dst"

4. Usage 
********

        Master [-help] [-org] [-sin] [-dev] [-s] [filename]

 -help           displays this message
 -org            runs millers original algorithm
 -sin            runs millers algorithm with sinusoids
 -dev            runs develop with self--organizing scheme
 -anal           runs analysis with the status file
 -s              lets program run with status file
 filename        is the status or parameter file.


5. Miller's Algorithm
*********************

	"Master -org filename.ort" is implementation of the paper of
miller on Development of Simple Cell Receptive Field and Orientation
Columns.  cf. K.D. Miller, "A Model for the Development of Simple Cell
Receptive Field and  the Ordered Arrangment of Orientation Columns
through Activity- dependent Competition between On- and OFF- Center
Inputs," The Journal of Neuroscience, January 1994, 14(1), 409-441.
This will store status at each iteration end into a .stt file.

6. Input File Format (.ort)
***************************

1.     No of LGN & cortex rows. 								[32]
2.     No of LGN & cortex cols. 								[32]
3.     Arbor Diameter.											[13]
4.     Seed Value for random number generator.
5.     Parameter controlling spread of correlation fn.			[0.28]
6.     Parameter controlling spread of Intracortical fn.		[0.3]
7.     Upper Bound on Synapse Values.							[4]
8.     Value of the Growth Constant.
9.     Output filename.

7. Sinusoidal Gratings on Miller's algorithm
********************************************

Miller's algorithm run with sinusoids rather than DoG correlations may
be obtained with "Master -sin filename.dev". The file format for .dev
files is already given.

8. Analysis
***********

"Master -anal status_file" yields an analysis of 

1. Preferred orientation of the model cells.
2. Preferred spatial phase of model cells.
3. Preferred frequency of model cells.
4. Orientation selectivity degree of model cells.
5. Distribution of the frequency of model cells.
6. Max, min and mean of degree of orientation selectivity of model cells.
7. An image showing the preference orientation of the entire array.

The first 6 will be stored in a file with a .all extension and the last
will be a .img extension file.

The status_file can be either of .dst format or of .stt format. Whereas
the former is given earlier, the latter is similar to the .ort file
followed by the synapses stored as in the .dst format. The status file
will be read appropriately depending upon the input file's extension.

9. Files Required
*****************


C++ Files:
---------

Image.C Imageaux.C Mask.C Imgdis.C Orient1.C Orient2.C Develop.C 
Dori.C Analysis.C Master.C MasterMain.C Vector.C

Header Files:
------------

Analysis.h Develop.h Image.h Imgdis.h Mask.h Orient.h Vector.h

Make sure that the path for X11 headers and libraries are specified
appropriately in the makefile.

*****************************************************************
