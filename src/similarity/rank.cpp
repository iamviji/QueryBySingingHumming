#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include "SModel.h"
#include "SUtil.h"
#include "SMelody.h"
#include "SDFuzzySearch.h"

#include "SBuildModel.h"
#include <iostream>
#include <fstream>
#include "midifile.h"
#include "similarity/rank.h"

#define PHRASE_SEGMENTATION
bool debug_print = true;
float SAMPLE_RATE =  0.2;
extern int _gFS;//VIJAY


float SCalcDTWDistance(float* Qry, int lenQry, float* Lib, int lenLib);

string GetSongName(string myStr){
	int pos=myStr.find("{");
	return myStr.substr(0,pos);
}

vector<MIDIINFO> LoadMidiFile(char* szInFile){ 
	MidiFile_t myMidi = MidiFile_load(szInFile);
	
	int nTracks=MidiFile_getNumberOfTracks(myMidi);
	MidiFileTrack_t curTrack = MidiFile_getFirstTrack(myMidi);
	int eType=0;
	int isNoteStartEvent=0;
	int nCount=0;
	vector<MIDIINFO> NoteVec;
	int CountStart=0;
	int CountEnd=0;
	while(curTrack){
		MidiFileEvent_t eEvent=MidiFileTrack_getFirstEvent(curTrack);	
		while(eEvent){
			eEvent=MidiFileEvent_getNextEventInTrack(eEvent);
			eType = MidiFileEvent_getType(eEvent);
			isNoteStartEvent=MidiFileEvent_isNoteStartEvent(eEvent);
			int isNoteEndEvent=MidiFileEvent_isNoteEndEvent(eEvent);
			if(eType==1){
				if(isNoteStartEvent==1){
					CountStart++;
					MIDIINFO tmp;
					tmp.Durationms=0;
					tmp.type=MidiFileNoteOnEvent_getNote(eEvent);
					tmp.Startms=(int)(MidiFile_getTimeFromTick(myMidi, MidiFileEvent_getTick(eEvent))*1000);
					tmp.Channel=MidiFileNoteOnEvent_getChannel(eEvent)+1;
					tmp.velocity=MidiFileNoteOnEvent_getVelocity(eEvent);
					NoteVec.push_back(tmp);
				}

				if(isNoteEndEvent==1){
					int _trackNo=MidiFileNoteOnEvent_getChannel(eEvent)+1;
					int _note=MidiFileNoteOnEvent_getNote(eEvent);
					int _startTime=(int)(MidiFile_getTimeFromTick(myMidi, MidiFileEvent_getTick(eEvent))*1000);
					int BestOffset=-1;
					int BestPos=-1;
					for(int x=0;x<NoteVec.size();x++){
						if(NoteVec[x].Channel == _trackNo && NoteVec[x].type == _note 
							&& _startTime>NoteVec[x].Startms && NoteVec[x].Durationms==0){
							NoteVec[x].Durationms=(int)(MidiFile_getTimeFromTick(myMidi, MidiFileEvent_getTick(eEvent))*1000) - NoteVec[x].Startms;
							break;
						}
					}

				}
			}
		}
		curTrack = MidiFileTrack_getNextTrack(curTrack);
	}
	
	return NoteVec;
}

void GetPoints (const char* file, float*& points, int& len)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
		if (debug_print) std::cout << "csv : opened\n";	
		int i = 0;
		while (csv.good ())
		{
			getline (csv, tk1, ',');	
			if (tk1 == "")
			{
				break;
			}
			getline (csv, tk2, ',');	
			getline (csv, tk3, ',');	
			getline (csv, tk4);	
			//std::cout << tk1 << " " << tk2 << " " << tk3 << " " << tk4 << "\n";
			i++;	
		}
		len = i-1;
		i = 0;
		int time  = 0;
		points = new float [len];
		csv.clear ();
		csv.seekg(0, std::ios::beg);
		while (csv.good ())
		{
			getline (csv, tk1, ',');	
			if (tk1 == "")
			{
				break;
			}
			getline (csv, tk2, ',');	
			getline (csv, tk3, ',');	
			getline (csv, tk4);	
			//std::cout << tk1 << " " << tk2 << " " << tk3 << " " << tk4 << "\n";
			if (i != 0)
			{
				points [i-1]= atof(tk1.c_str ());
				//std::cout << points[i-1] <<"\n";
			} else
			{
				if (debug_print) std::cout << "skiping header\n";
			}
			i++;	
		}
	}
}

void GetPitchCurvePoints (const char* file, float*& points, int& len)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
		if (debug_print) std::cout << "csv : opened\n";	
		int i = 0;
		float time  = 0;
		while (csv.good ())
		{
			getline (csv, tk1, ',');	
			if (tk1 == "")
			{
				break;
			}
			getline (csv, tk2, ',');	
			getline (csv, tk3, ',');	
			getline (csv, tk4);	
			if ( i == 1)
			{
				time = atof(tk2.c_str ());
			}
			//std::cout << tk1 << " " << tk2 << " " << tk3 << " " << tk4 << "\n";
			i++;	
		}
		i = 0;
		points = new float [10000];
		csv.clear ();
		csv.seekg(0, std::ios::beg);
		while (csv.good ())
		{
			getline (csv, tk1, ',');	
			if (tk1 == "")
			{
				break;
			}
			getline (csv, tk2, ',');	
			getline (csv, tk3, ',');	
			getline (csv, tk4);	
			//std::cout << tk1 << " " << tk2 << " " << tk3 << " " << tk4 << "\n";
			if (i != 0)
			{
				float onsettime = atof(tk2.c_str ());
				//time  = time +  SAMPLE_RATE;
				if (debug_print) std::cout <<"------\n";
				while (onsettime >= time)
				{
					points [i-1] = atof(tk1.c_str ());
					if (debug_print) std::cout << time << ","<<onsettime<<","<< points[i-1] <<"\n";
					++i;
					time  = time +  SAMPLE_RATE;
				} 
				//std::cout << points[i-1] <<"\n";
			} else
			{
				++i;
				if (debug_print) std::cout << "skiping header\n";
			}
		}
		len = i-1;
	}
}

/* both frame-based and note-based melody feature are extracted */
int SMelodyFeatureExtraction(char* filename, float*& pFeaBuf,int &nFeaLen, SNote *&Query, int &nNoteLen){
	/* 0. pitch extraction */
	SPitchContourExtraction(filename,pFeaBuf,nFeaLen);	

	/* 1. five point median filtering */
	SMedianFilter(pFeaBuf,nFeaLen);
	if(nFeaLen<20){
		printf("Too short! please try again[%d]\n", nFeaLen);
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ERROR_CODE_TOO_SHORT_INPUT;
	}

	/* 2. post-processing the pitch sequence and re-sampling the pitch sequence */
	SProcessQuery(pFeaBuf,nFeaLen);
	if(nFeaLen<20){
		printf("Too short! please try again\n");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ERROR_CODE_TOO_SHORT_INPUT;
	}
	
	/* 3. note transcription */
	STranscribeQueryNote(pFeaBuf,nFeaLen, Query, nNoteLen);
	if(nFeaLen<20){
		printf("Too short! please try again\n");
		if(NULL!=pFeaBuf){
			delete[] pFeaBuf;
			pFeaBuf=NULL;
		}
		return ERROR_CODE_TOO_SHORT_INPUT;
	}
#if 0 
	std::cout << "Wave File Notes:"<<nNoteLen<<"\n";
	for (int k = 0; k < nNoteLen; k++)
	{
		std::cout << Query[k].fNoteValue << "," << Query[k].fNoteDuration << "\n";
	}
#endif

	return 0;
}

SModel* LoadModel (char* refMelody, RefType refType, float*& pFeaBuf, int& nFeaLen)
{
	SModel *SQBHModels = NULL;
	if (refType == RefType_Csv)
	{
		std::ifstream csv(refMelody);
		std::string tk1,tk2,tk3,tk4;
		if (csv.is_open ())
		{
			SQBHModels = new SModel;
			int nNotes = 0;
			std::cout << "csv : opened\n";	
			int i = 0;
			while (csv.good ())
			{
				getline (csv, tk1, ',');	
				if (tk1 == "")
				{
					break;
				}
				getline (csv, tk2, ',');	
				getline (csv, tk3, ',');	
				getline (csv, tk4);	
				//std::cout << tk1 << " " << tk2 << " " << tk3 << " " << tk4 << "\n";
				i++;	
			}
			nNotes = i;
			i = 0;
			SQBHModels->sNotes = new SNote [nNotes];
			SQBHModels->nNoteNum = nNotes;
			SQBHModels->nSongID = 0;
			SQBHModels->PhrasePos = new int [1];
			SQBHModels->PhrasePos[0] = 0;
			SQBHModels->nPhraseNum = 1;
			if (debug_print) std::cout << "number of notes: " << nNotes << "\n";
			csv.clear ();
			csv.seekg(0, std::ios::beg);
			while (csv.good ())
			{
				getline (csv, tk1, ',');	
				if (tk1 == "")
				{
					break;
				}
				getline (csv, tk2, ',');	
				getline (csv, tk3, ',');	
				getline (csv, tk4);	
				//std::cout << tk1 << " " << tk2 << " " << tk3 << " " << tk4 << "\n";
				if (i != 0)
				{
					SQBHModels->sNotes[i-1].fNoteValue = atof(tk1.c_str ());
					SQBHModels->sNotes[i-1].fNoteDuration = atof(tk3.c_str ()) * 100;
					if (debug_print) std::cout << SQBHModels->sNotes[i-1].fNoteValue << "," << SQBHModels->sNotes[i-1].fNoteDuration << "\n";
				} else
				{
					if(debug_print) std::cout << "skiping header\n";
				}
				i++;	
			}
		} else
		{
			if (debug_print) std::cout << "csv : not opened\n";	
		}
	} 
	else if (refType == RefType_Wav) 
	{
		//float *pFeaBuf = NULL;
		//int nFeaLen = 0;
		SNote *Notes= NULL;
		int nNoteLen=0;
		SMelodyFeatureExtraction(refMelody,pFeaBuf,nFeaLen,Notes,nNoteLen);	
		SQBHModels = new SModel;
		SQBHModels->sNotes = Notes;
		SQBHModels->nNoteNum = nNoteLen;
		SQBHModels->nSongID = 0;
		SQBHModels->PhrasePos = new int [1];
		SQBHModels->PhrasePos[0] = 0;
		SQBHModels->nPhraseNum = 1;
	}
	else if (refType == RefType_Midi)
	{
		vector<MIDIINFO> myNotes=LoadMidiFile(refMelody);
		if(myNotes.size()==0){
			printf("file cannot be opened: %s, please check\n",refMelody);
			return NULL;
		}
		int i=0;
		int ValidNoteCount=0;
		int TotalNoteNumber=myNotes.size();
		NoteStructure *pMidiNoteStruct=new NoteStructure[TotalNoteNumber];
		
		for(i=0;i<TotalNoteNumber;i++){
			//if(myNotes[i].Durationms!=0)//VIJAYA
			{
				pMidiNoteStruct[ValidNoteCount].iNoteStartTime=myNotes[i].Startms;
				pMidiNoteStruct[ValidNoteCount].iDurationms=myNotes[i].Durationms;
				pMidiNoteStruct[ValidNoteCount].iNote=myNotes[i].type;
				pMidiNoteStruct[ValidNoteCount].itrackNo=myNotes[i].Channel;
				if(myNotes[i].velocity==5)
					pMidiNoteStruct[ValidNoteCount].iPhraseTag=1;
				else
					pMidiNoteStruct[ValidNoteCount].iPhraseTag=0;
				ValidNoteCount++;
			}
		}

		ModelStru myQBHModel;
		TotalNoteNumber=ValidNoteCount;
		for(i=0;i<TotalNoteNumber-1;i++){
			pMidiNoteStruct[i].iDurationWithRest=pMidiNoteStruct[i+1].iNoteStartTime-pMidiNoteStruct[i].iNoteStartTime;
				NOTE_TEMPLATE Note;
				Note.Pitch=pMidiNoteStruct[i].iNote;
				Note.Duration=pMidiNoteStruct[i].iDurationms/10.0f;
				Note.Duration=(int)((Note.Duration/5)+0.5);
				
				Note.DurationWithRest=pMidiNoteStruct[i].iDurationWithRest/10.0f;
				Note.DurationWithRest=(int)((Note.DurationWithRest/5)+0.5);
				myQBHModel.notes.push_back(Note);
				if(pMidiNoteStruct[i].iPhraseTag==1)
					myQBHModel.PhraseOffsetVector.push_back(i);
		}
		pMidiNoteStruct[TotalNoteNumber-2].iDurationWithRest=pMidiNoteStruct[TotalNoteNumber-2].iDurationms;
	

		delete[] pMidiNoteStruct;

		SModel* SQBHModels = new SModel;
		SQBHModels->nNoteNum = myQBHModel.notes.size ();
		SNote *Notes= new SNote[SQBHModels->nNoteNum];
		SQBHModels->sNotes = Notes;
		SQBHModels->nSongID = 0;

		for(i=0;i<myQBHModel.notes.size();i++)
		{
			Notes[i].fNoteValue = myQBHModel.notes[i].Pitch;
			Notes[i].fNoteDuration =myQBHModel.notes[i].Duration;
			if (debug_print) std::cout << SQBHModels->sNotes[i].fNoteValue << "," << SQBHModels->sNotes[i].fNoteDuration << "\n";
		}
			
#ifdef PHRASE_SEGMENTATION
		int pno = myQBHModel.PhraseOffsetVector.size();
		SQBHModels->nPhraseNum = pno;
		SQBHModels->PhrasePos = new int [pno];
		for(i=0; i < myQBHModel.PhraseOffsetVector.size(); i++)
		{
			SQBHModels->PhrasePos[i] = myQBHModel.PhraseOffsetVector[i];
		} 
#else
		SQBHModels->nPhraseNum = 0;
		SQBHModels->PhrasePos = NULL;
#endif
	}
	return SQBHModels;
}

/* rank query */
int GetRank(char* refMelody, char* szWav)
{
	RefType refType = RefType_None;
	bool isTestWav = true;

	std::string f1(refMelody);
	std::string f2(szWav);
	if(f1.substr(f1.find_last_of(".") + 1) == "wav") 
	{
		refType = RefType_Wav;
	} 
	if(f1.substr(f1.find_last_of(".") + 1) == "mid") 
	{
		refType = RefType_Midi;
	} 
	if(f1.substr(f1.find_last_of(".") + 1) == "csv") 
	{
		refType = RefType_Csv;
	} 


	int i=0;
	float *pFeaBufT = NULL;
	int nFeaLenT = 0;	
	//0, Read from sonic csv here 
	SModel *SQBHModelsR= LoadModel (refMelody, refType, pFeaBufT, nFeaLenT);
	if (SQBHModelsR == NULL) return -1;
	char **szModelInfoStrs=NULL; 
	int nModels=1;
	int nTotalModel=1;
	int nTotalSongs=1;

	//1, Feature Extraction
	float *pFeaBuf = NULL;
	int nFeaLen = 0;
	SNote *QueryNotes= NULL;
	int nNoteLen=0;
	SMelodyFeatureExtraction(szWav,pFeaBuf,nFeaLen,QueryNotes,nNoteLen);	


	SModel *SQBHModelsT = new SModel;
	SQBHModelsT->sNotes = QueryNotes;
	SQBHModelsT->nNoteNum = nNoteLen;
	SQBHModelsT->nSongID = 0;
	SQBHModelsT->PhrasePos = new int [1];
	SQBHModelsT->PhrasePos[0] = 0;
	SQBHModelsT->nPhraseNum = 1;

	SModel *SQBHModels = NULL;
//	if (isRefWav == isTestWav && isRefWav == true)
	{
		if (SQBHModelsR->nNoteNum >= SQBHModelsT->nNoteNum)
		{
			SQBHModels = SQBHModelsR;
		} else
		{
			SQBHModels = SQBHModelsT;
			QueryNotes = SQBHModelsR->sNotes;
			nNoteLen   = SQBHModelsR->nNoteNum; 
			pFeaBuf    = pFeaBufT;
			nFeaLen    = nFeaLenT;
		}
	}	
	//2, Melody Search
	NoteBasedResStru *myEMDResStru=new NoteBasedResStru[nTotalModel];
	SNoteBasedMatch(SQBHModels, nModels, QueryNotes, nNoteLen,myEMDResStru,nFeaLen);
	std::cout <<"NoteBased.SimilarityScore=" << myEMDResStru->fScore << ",SimilarityScale=" << myEMDResStru->fScale << ",Mid="<<myEMDResStru->nModelID << ",PID="<< myEMDResStru->nPhraseID<<"\n";

	FrameBasedResStru *myDTWResStru=new FrameBasedResStru[20];
	myEMDResStru[0].nModelID = 0;    /* song id */
#if 1
	//if (refType != RefType_Csv)
	{
		SFrameBasedMatch(SQBHModels, nModels, pFeaBuf, nFeaLen, myEMDResStru, 1, myDTWResStru);
		printf ("final\n");
		//3, Finalize and print the result
	//	FILE *OutFile=fopen(szOut,"a+t");
	//	fprintf(OutFile,"%s ",szWav);
		for(i=0;i<1;i++){
			//fprintf(OutFile,"%d: %s, %f;\n",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
			//printf("%d: %s, %f\n",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
			printf("FrameBasedMatch.SimilarityScore=%f\n", myDTWResStru[i].fScore);
		}
		//fprintf(OutFile,"\n");
	//	fclose(OutFile);
	}
#endif
	for(i=0;i<nTotalSongs;i++){
		if(NULL!=SQBHModels[i].PhrasePos){
			delete[] SQBHModels[i].PhrasePos; 
			SQBHModels[i].PhrasePos=NULL;
		}
		if(NULL!=SQBHModels[i].sNotes){
			delete[] SQBHModels[i].sNotes;
			SQBHModels[i].sNotes=NULL;
		}
	}

	for(i=0;i<nTotalSongs;i++){
		if (NULL!=szModelInfoStrs) {
			if(NULL != szModelInfoStrs[i]){
				delete[] szModelInfoStrs[i];
				szModelInfoStrs[i]=NULL;
			}
		}
	}
	if(NULL!=myEMDResStru){
		delete[] myEMDResStru;
		myEMDResStru=NULL;
	}
	if(NULL!=myDTWResStru){
		delete[] myDTWResStru;
		myDTWResStru=NULL;
	}
	if(NULL!=SQBHModels){
		delete[] SQBHModels;
		SQBHModels=NULL;
	}
	if(NULL!=szModelInfoStrs){
		delete[] szModelInfoStrs;
		szModelInfoStrs=NULL;
	}
	if(NULL!=QueryNotes){
		delete[] QueryNotes;
		QueryNotes=NULL;
	}
	if(NULL!=pFeaBuf){
		delete[] pFeaBuf;
		pFeaBuf=NULL;
	}
	return 0;
}

#if 0
int main(int argc, char* argv[]){
	//printf ("long=%d,short=%d,int=%d\n", sizeof(long), sizeof(short), sizeof (int));
	int typeGetPoints = 0;
	RefType refType = RefType_None;
	bool isTestWav = false;
	if (argc < 3){ 
		printf("usage: rank.exe sonic.csv test.wav \n"); 
		return 0; 
	}
        if (argc >= 4)
	{
		SAMPLE_RATE = atof (argv[3]);	
		printf ("SampleRate=%f", SAMPLE_RATE);
	}	
        if (argc >= 5)
	{
		typeGetPoints = atoi (argv[4]);	
		printf ("typeGetPoints=%d\n", typeGetPoints);
	}	

	std::string f1(argv[1]);
	std::string f2(argv[2]);
	if(f1.substr(f1.find_last_of(".") + 1) == "wav") 
	{
		refType = RefType_Wav;
	} 
	if(f1.substr(f1.find_last_of(".") + 1) == "mid") 
	{
		refType = RefType_Midi;
	} 
	if(f1.substr(f1.find_last_of(".") + 1) == "csv") 
	{
		refType = RefType_Csv;
	} 
	if(f2.substr(f2.find_last_of(".") + 1) == "wav") 
	{
		isTestWav = true;
	} 

#if 0
        if (argc >= 4)
	{
		_gFS = atoi (argv[3]);	
		printf ("FS=%d", _gFS);
	}	
	GetRank(argv[1],refType, argv[2], isTestWav);
#else
	float* points1;
	int len1;
	float* points2;
	int len2;

	if (typeGetPoints == 0)
	{
	    GetPoints (argv[1], points1, len1);
	} else
	{
	    GetPointsOrg (argv[1], points1, len1);
	}
	    std::cout << "file1\n";
	    for (int i = 0; i < len1; i++)
	    {
		std::cout << points1 [i] << "\n"; 
	    }
	    std::cout << "file2\n";
	if (typeGetPoints == 0)
	{
	    GetPoints (argv[2], points2, len2);
	} else
	{
	    GetPointsOrg (argv[2], points2, len2);
	}
	    for (int i = 0; i < len2; i++)
	    {
		std::cout << points2 [i] << "\n"; 
	    }

	std::cout<<"result:";
	std::cout<<SCalcDTWDistance(points1, len1, points2, len2)<<"\n";
	printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
#endif
	return 0;
}
#endif
 

