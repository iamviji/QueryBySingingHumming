#ifndef __RANK_H__
#define __RANK_H__

enum RefType
{
	RefType_Wav,
	RefType_Midi,
	RefType_Csv,
	RefType_None
};



void GetPoints (const char* file, float*& points, int& len);
void GetPitchCurvePoints (const char* file, float*& points, int& len);
int GetRank(char* refMelody,char* szWav);

#endif
