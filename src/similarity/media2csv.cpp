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

extern int _gFS;//VIJAY

SModel* LoadModel (char* refMelody, RefType refType, float*& pFeaBuf, int& nFeaLen);

extern float SAMPLE_RATE;
extern bool debug_print;
int main(int argc, char* argv[])
{
	RefType refType = RefType_None;
	if (argc != 3)
	{ 
		printf("usage: media2csv.exe media[wav|mid] sample_rate=[11025,16000,22050,24000,32000,44100]\n"); return 0; 
		return 0; 
	}
	_gFS = atoi (argv[2]);	


	std::string f1(argv[1]);
	if(f1.substr(f1.find_last_of(".") + 1) == "wav") 
	{
		refType = RefType_Wav;
	} else if(f1.substr(f1.find_last_of(".") + 1) == "mid") 
	{
		refType = RefType_Midi;
	} else
	{
		printf ("type must be midi or wav");
	}

	float *pFeaBufT = NULL;
	int nFeaLenT = 0;	

	debug_print = false;
	SModel *SQBHModels= LoadModel (argv[1], refType, pFeaBufT, nFeaLenT);
	if (SQBHModels == NULL)
	{
		std::cout << "model is null\n";
		return -1;	
	}
	float onset_time = 0;
	std::cout <<"pitch,onset_time,duration,volume\n";
	for(int i=0;i<SQBHModels->nNoteNum;i++)
	{
		float time = SQBHModels->sNotes[i].fNoteDuration/100;
		std::cout << SQBHModels->sNotes[i].fNoteValue <<","<<onset_time << ","<< time << ",0\n";
		onset_time = onset_time + time;
	} 

	return 0;
}

 

