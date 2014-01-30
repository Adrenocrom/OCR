#include <cv.h>
#include <highgui.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ocr.h"
#include "feedforward.h"

using namespace cv;
using namespace std;

std::vector<float> getImageFromRect(cv::Mat& src);

int g_threshold = 150;

int getFromChar(char k);

void saveExamples(std::vector<inputExample> e);
std::vector<inputExample> loadExamples();

int main(int argc, char *argv[])
{
	if(strcmp(argv[1], "create") == 0) {
		std::vector<inputExample> examples;
		Mat thr,gray,con;
		Mat src=imread(argv[2], 1);

		std::istringstream iss( argv[3] ); iss >> g_threshold;
		cout<<g_threshold<<endl;

		cv::resize(src, src, cv::Size(), 2.0, 2.0);
		cvtColor(src,gray,CV_BGR2GRAY);
		threshold(gray,thr, g_threshold, 255,THRESH_BINARY_INV); //Threshold to find contour
		thr.copyTo(con);
		imshow("thr", thr);
	
		vector< vector <Point> > contours; // Vector for storing contour
		vector< Vec4i > hierarchy;
		Mat sample;
		Mat response_array;  
		findContours( con, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); 
		
		cout<<"image: "<<src.cols<<"x"<<src.rows<<endl;
		for( unsigned int i = 0; i < contours.size(); i = hierarchy[i][0] ) {
			Rect r= boundingRect(contours[i]); //Find bounding rect for each contour
	  
			cv::Mat temp = src(cv::Rect(r));
			cv::Mat tem;
			cv::resize(temp, tem, cv::Size(32, 32), 0, 0, INTER_CUBIC);		

			inputExample e;
			e.image = getImageFromRect(tem);

			//examples.push_back(e);
			//rectangle(src,Point(r.x,r.y), Point(r.x+r.width,r.y+r.height), Scalar(0,255,0),2,8,0);
	   	imshow("src",tem);

			printf("Klasse: ");
			char k = waitKey();
			cout<<k<<endl;

			e.k = setKClass(getFromChar(k));

			examples.push_back(e);
		}
   	waitKey();

		saveExamples(examples);
	}
	else if(strcmp(argv[1], "train") == 0) {
		cout<<"load examples from file"<<endl;
		std::vector<inputExample> examples = loadExamples(); 
		cout<<"create knn"<<endl;
		feedForward knn(1024, 6, "100");
		bool trained = false;
		int numExamples = examples.size();


		cout<<"start training"<<endl;
		int cnt = 0;
		do {
			trained = true;
			cnt++;

			for(int i = 0; i < numExamples; ++i) {
				vector<float> result = makeClean(knn.calcOutput(examples[i].image));
				if(!correct(result, examples[i].k)) {
					trained = false;
	
					knn.learnNetwork(examples[i].image,
										  examples[i].k,
										  0.3f, 0.0f);
				} else {
					cout<<"correct!"<<endl;
				}

				printf("Step: %d\n", cnt);
				printf("in : ");
				printv(examples[i].k);
				printf("out: ");
				printv(result);
			}
		} while(!trained && cnt < 20000);
		cout<<"trained"<<endl;

		knn.saveWeights("net.txt");
	}

	return 0;
}

std::vector<float> getImageFromRect(cv::Mat& src) {
	std::vector<float> out;
	unsigned char *input = (unsigned char*)(src.data);

	threshold(src, src, g_threshold, 255,THRESH_BINARY_INV);

	float p;
	for(int y = 0; y < src.rows; ++y) {
		for(int x = 0;  x < src.cols; ++x) {
			p = input[src.step * x + y] / 255;
			out.push_back(p);
		}
	}

	return out;
}

void saveExamples(std::vector<inputExample> e) {
	FILE* pFile = NULL;

	if((pFile = fopen("examples.txt", "w")) == NULL) {
		fclose(pFile);
		pFile = NULL;
		return;
	}

	fprintf(pFile, "numExamples= %d\n\n", (int)e.size());

	for(int i = 0; i < e.size(); ++i) {
		for(int a = 0; a < e[i].k.size()-1; ++a)
			fprintf(pFile, "%.0f ", e[i].k[a]);
		fprintf(pFile, "%.0f\n", e[i].k[e[i].k.size()-1]);

		for(int b = 0; b < e[i].image.size()-1; ++b)
			fprintf(pFile, "%.0f ", e[i].image[b]);
		fprintf(pFile, "%.0f\n", e[i].image[e[i].image.size()-1]);
	}

	fclose(pFile);
	pFile = NULL;
}

std::vector<inputExample> loadExamples() {
	FILE* pFile = NULL;
	std::vector<inputExample> result;
	char cBuffer[200];
	int numExamples;
	float temp;

	if((pFile = fopen("examples.txt", "r")) == NULL) {
		fclose(pFile);
		pFile = NULL;
		return result;
	}

	fscanf(pFile, "%s", cBuffer);
	fscanf(pFile, "%d", &numExamples);

	for(int i = 0; i < numExamples; ++i) {
		cout<<"read example "<<i+1<<endl;
		inputExample example;
		for(int k = 0; k < 6; ++k) {
			fscanf(pFile, "%f", &temp);
			example.k.push_back(temp);
		}

		example.k = makeClean(example.k);

		for(int l = 0; l < 1024; ++l) {
			fscanf(pFile, "%f", &temp);
			example.image.push_back(temp);
		}

		result.push_back(example);
	}

	fclose(pFile);
	pFile = NULL;
	return result;
}
