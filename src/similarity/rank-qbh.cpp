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

extern float SAMPLE_RATE;
extern int _gFS;//VIJAY
extern bool debug_print;

int main(int argc, char* argv[]){
	//debug_print = false;
	if (argc !=  4) 
	{ 
		printf("usage: rank-qbh.exe ref.[csv|wav|mid] test.wav sample_rate=[11025,16000,22050,24000,32000,44100]\n"); return 0; 
	} 
	_gFS = atoi (argv[3]);	
	printf ("FS=%d", _gFS);
	GetRank(argv[1], argv[2]);
	return 0;
}

 

