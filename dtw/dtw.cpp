
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include "dtw.h"


float SAMPLE_RATE =  0.2;

void getPointsOrg (const char* file, vector<double>& points)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
		uint32_t i = 0;
		int time  = 0;
		// csv.clear ();
		// csv.seekg(0, std::ios::beg);
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
				points.push_back(atof(tk1.c_str ()));
				//std::cout << points[i-1] <<"\n";
			} else
			{
				std::cout << "skiping header\n";
			}
			i++;	
		}
	}
}
void getPoints (const char* file, vector<double>& points)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
		std::cout << "csv : opened\n";	
		int i = 0;
		float time  = 0;
		// csv.clear ();
		// csv.seekg(0, std::ios::beg);
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
			if (i != 0)
			{
				float onsettime = atof(tk2.c_str ());
				//time  = time +  SAMPLE_RATE;
				//std::cout <<"------\n";
				while (onsettime >= time)
				{
					points.push_back (atof(tk1.c_str ()));
					std::cout << time << ","<<onsettime<<","<< points[i-1] <<"\n";
					++i;
					time  = time +  SAMPLE_RATE;
				} 
				//std::cout << points[i-1] <<"\n";
			} else
			{
				++i;
				std::cout << "skiping header\n";
			}
		}
	}
}


std::vector <uint32_t> split(const std::string& s, char delimiter)
{
	std::vector<uint32_t> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(atoi(token.c_str ()));
		std::cout << atoi(token.c_str ()) << "\n";
	}
	return tokens;
}

vector<uint32_t> def_beats = {1,1,1,1};



int main(int argc, char *argv[])
{
    vector <double>  points1V, points2V;
    vector <uint32_t> beats;
    int type = 0;
    SAMPLE_RATE = atof (argv[3]);	
    if (argc >= 5)  beats = split(argv[4], ','); 
    else beats = def_beats;
    if (argc >= 6) type = atoi (argv[5]);	

    if (type == 0) getPoints (argv[1], points1V);
    else getPointsOrg (argv[1], points1V);
    std::cout << "file1\n";
    for (uint32_t i = 0; i < points1V.size (); i++)
    {
   	std::cout << points1V [i] << "\n"; 
    }
    std::cout << "file2\n";
    if (type == 0) getPoints (argv[2], points2V);
    else getPointsOrg (argv[2], points2V);
    for (uint32_t i = 0; i < points2V.size (); i++)
    {
   	std::cout << points2V [i] << "\n"; 
    }
    vector <DTWDetail> distPerSample;
    vector <BeatScoreInfo> scorePerBeats;
    dtw d (points1V.size (), points2V.size (), 100000); 
    double r = d.fastdynamicDetail(points1V, points2V,beats, distPerSample, scorePerBeats);
	uint32_t size = distPerSample.size ();
	float sum = 0;
	//float *diffCuve = new float [size+1];
	//diffCuve [0] = points1 [0];
	int points = 1;
	int k =0;
	int gap = 100;
	//printf ("%d,%f,%f,%f\n", k,points1[k],points2[k], diffCuve[k]);
	//k =1;
	int cord = 0;
	int cord2 = 0;
	double gp1;
	double gp2;
	bool flip = false;
	float sumx=0;
	for (uint32_t i = 0; i < distPerSample.size (); i++)
	{
		DTWDetail dtwDetail = distPerSample [i];

		sum = sum + dtwDetail.dist;
		if (i < size-1)
		{
			DTWDetail dtwDetailN = distPerSample [i+1];
			if (points == 0)
			{
				gp1=points2V [dtwDetail.testX]+gap;
				gp2=points1V [dtwDetail.refX];
				cord = dtwDetail.testX;
				cord2= dtwDetail.refX;
				if ((dtwDetail.refX-dtwDetailN.refX) == 0)
				{
				} else
				{
					points = 1;
					flip = true;
				}
			} else
			{
				gp1=points1V [dtwDetail.refX];
				gp2=points2V [dtwDetail.testX]+gap;
				cord = dtwDetail.refX;
				cord2= dtwDetail.testX;
				if ((dtwDetail.testX-dtwDetailN.testX) == 0)
				{
				} else
				{
					points = 0;
					flip = true;
				}
			}
			printf ("%d,%d,%d,%f,%f,%f,%d,%f\n",
					k+1,dtwDetail.refX, dtwDetail.testX,
					dtwDetail.dist, 
					points1V[dtwDetail.refX],
					points2V[dtwDetail.testX],
					cord+1,
					gp1);
			//sumx = sumx + fabs(points1V[dtwDetail.refX]-points2V[dtwDetail.testX]);
			if (flip == true)
			{
				//printf ("%d,%d,%d,%f,%f,%f,%d,%f\n", k+1,dtwDetail.refX, dtwDetail.testX, dtwDetail.dist, points1[dtwDetail.refX], points2[dtwDetail.testX], cord2+1, gp2);
				flip = false;
			
			}
			flip = false;
			k++;
		} 
		sumx = sumx + fabs(points1V[dtwDetail.refX]-points2V[dtwDetail.testX]);
		//printf ("%3d,%3d,%05.3f\n",dtwDetail.refX, dtwDetail.testX, dtwDetail.dist);	
	}
	printf("sum: %f \n",sum);
	printf("sumx: %f \n",sumx);
	double sumAnother = 0;;
	printf ("------------------------------------------\n");
	int scorePerBeatsSize = scorePerBeats.size ();
	for (uint32_t i = 0; i < scorePerBeatsSize; i++)
	{
		printf ("BeatNo=%d,BeatIndex=%d,noOfNoteInBeats=%d,[", i, scorePerBeats[i].beatIndex, scorePerBeats[i].noOfNoteInBeats);
		int subBeatSize = scorePerBeats[i].dtwDetails.size ();
		for (int j = 0; j < subBeatSize; j++)
		{
			printf ("(%d,%d,%f) ", scorePerBeats[i].dtwDetails[j].refX, scorePerBeats[i].dtwDetails[j].testX,scorePerBeats[i].dtwDetails[j].dist);
		}
		printf ("],Dist=%f\n", scorePerBeats[i].dist);
		sumAnother += scorePerBeats[i].dist;
	}
    cout << "RESULT:"<<r<<"\n";
    cout << "RESULTANother:"<<sumAnother<<"\n";
    return 0;
}
