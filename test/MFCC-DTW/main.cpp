
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>

#include"WaveFunction.h"

float SAMPLE_RATE =  0.2;

void getPointsOrg (const char* file, float*& points, int& len)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
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
				std::cout << "skiping header\n";
			}
			i++;	
		}
	}
}
void getPoints (const char* file, float*& points, int& len)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
		std::cout << "csv : opened\n";	
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
		points = new float [5000];
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
				//std::cout <<"------\n";
				while (onsettime >= time)
				{
					points [i-1] = atof(tk1.c_str ());
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
		len = i-1;
	}
}

#if 0
void getPoints (const char* file, float*& points, int& len)
{
	std::ifstream csv(file);
	std::string tk1,tk2,tk3,tk4;
	if (csv.is_open ())
	{
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
		len = i-1;
		i = 0;
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
				std::cout << "skiping header\n";
			}
			i++;	
		}
	}
}
#endif
int main(int argc, char *argv[])
{
#if 0
    WaveFunction* a=new WaveFunction(128,13);//每帧多少个采样点，MFCC参数的维数
    
    //vector<vector<float> > mfccs1 = a->getMFCCs("D:\\1.wav");//提取mfcc参数
    //vector<vector<float> > mfccs2 = a->getMFCCs("D:\\2.wav");
    vector<vector<float> > mfccs1 = a->getMFCCs(argv[1]);//提取mfcc参数
    vector<vector<float> > mfccs2 = a->getMFCCs(argv[2]);

    //mfccs1=a->addFirstOrderDifference(mfccs1);//增加一阶差分系数,此时mfcc参数变为13+13维

    mfccs1=a->addOrderDifference(mfccs1);//增加一阶差分和二阶差分系数，此时mfcc参数变为13*2+13维
    mfccs2=a->addOrderDifference(mfccs2);

    
    cout<<a->ComputeDTW(mfccs1,mfccs2);//利用动态时间规整算法，计算两个语音的相似度，越小相似度越大
#else
    float* points1;
    int len1;
    float* points2;
    int len2;
    int type = 0;
    SAMPLE_RATE = atof (argv[3]);	
    if (argc == 5) type = atoi (argv[4]);	

    if (type == 0) getPoints (argv[1], points1, len1);
    else getPointsOrg (argv[1], points1, len1);
    std::cout << "file1\n";
    for (int i = 0; i < len1; i++)
    {
   	std::cout << points1 [i] << "\n"; 
    }
    std::cout << "file2\n";
    if (type == 0) getPoints (argv[2], points2, len2);
    else getPointsOrg (argv[2], points2, len2);
    for (int i = 0; i < len2; i++)
    {
   	std::cout << points2 [i] << "\n"; 
    }

    WaveFunction* a=new WaveFunction(128,13);
    float r = a->ComputeDTW(points1,points2, len1, len2);
    cout << "RESULT:"<<r<<"\n";
#endif
    
    return 0;
}
