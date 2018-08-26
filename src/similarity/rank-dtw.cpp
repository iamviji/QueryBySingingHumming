#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include <iostream>
#include <fstream>

#include "similarity/rank.h"
#include "SFrameMatch.h"

extern float SAMPLE_RATE;
extern bool debug_print;

int main(int argc, char* argv[])
{
	int typeGetPoints = 1;
	debug_print = false;
	if (argc < 3 || argc > 4)
	{ 
		printf("usage: rank-dtw.exe ref.csv test.csv [pitch_curve_sample_rate{=0.1,0.2..}] \n"); 
		return 0; 
	}
        if (argc == 4)
	{
		SAMPLE_RATE = atof (argv[3]);	
		typeGetPoints = 0;
		printf ("SampleRate=%f", SAMPLE_RATE);
	}	


	float* points1;
	int len1;
	float* points2;
	int len2;

	if (typeGetPoints == 0)
	{
	    GetPitchCurvePoints (argv[1], points1, len1);
	} else
	{
	    GetPoints (argv[1], points1, len1);
	}
	std::cout << "file1\n";
#if 0 
	for (int i = 0; i < len1; i++)
	{
	    std::cout << points1 [i] << "\n"; 
	}
#endif
	std::cout << "file2\n";
	if (typeGetPoints == 0)
	{
	    GetPitchCurvePoints (argv[2], points2, len2);
	} else
	{
	    GetPoints (argv[2], points2, len2);
	}
#if 0 
	for (int i = 0; i < len2; i++)
	{
	    std::cout << points2 [i] << "\n"; 
	}
#endif

	std::cout<<"result:";
	std::cout<<SCalcDTWDistance(points1, len1, points2, len2)<<"\n";
	printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
	return 0;
}

 

