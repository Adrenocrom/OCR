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

#include "feedforward.h"

using namespace cv;
using namespace std;

struct inputExample
{
	std::vector<float> image;
	std::vector<float> k;
};

std::vector<float> setKClass(int k);
int getKClass(std::vector<float>* pK);
std::vector<float> getImageFromRect(cv::Mat& src);

vector<float> makeClean(vector<float> vIn);

bool correct(std::vector<float> in, std::vector<float> out);
void printv(std::vector<float> in);

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

int getKClass(std::vector<float>* pK) {
	int size = pK->size();
	int result = 0;

	int index = size-1;
	for(int i = 0; i < size; ++i) {
		if(pK->at(i) == 1)
			result += pow(2.0, index);
		index--;
	}

	return result;
}

std::vector<float> setKClass(int k) {
	std::vector<float> out;
	std::bitset<6> klass(k);

//	cout<<"k: "<<klass<<endl;

	for(int i = klass.size(); i >= 0; --i) {
//		cout<<klass[i]<<endl;

		if(klass[i] == 1)
			out.push_back(1.0f);
		else
			out.push_back(0.0f);
	}

	return out;
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

int getFromChar(char k) {
	if(k == '0') return 0;
	if(k == '1') return 1;
	if(k == '2') return 2;
	if(k == '3') return 3;
	if(k == '4') return 4;
	if(k == '5') return 5;
	if(k == '6') return 6;
	if(k == '7') return 7;
	if(k == '8') return 8;
	if(k == '9') return 9;
	if(k == 'a') return 11;
	if(k == 'b') return 12;
	if(k == 'c') return 13;
	if(k == 'd') return 14;
	if(k == 'e') return 15;
	if(k == 'f') return 16;
	if(k == 'g') return 17;
	if(k == 'h') return 18;
	if(k == 'i') return 19;
	if(k == 'j') return 20;
	if(k == 'k') return 21;
	if(k == 'l') return 22;
	if(k == 'm') return 23;
	if(k == 'n') return 24;
	if(k == 'o') return 25;
	if(k == 'p') return 26;
	if(k == 'q') return 27;
	if(k == 'r') return 28;
	if(k == 's') return 29;
	if(k == 't') return 30;
	if(k == 'u') return 31;
	if(k == 'v') return 32;
	if(k == 'w') return 33;
	if(k == 'x') return 34;
	if(k == 'y') return 35;
	if(k == 'z') return 36;
	if(k == 'A') return 37;
	if(k == 'B') return 38;
	if(k == 'C') return 39;
	if(k == 'D') return 40;
	if(k == 'E') return 41;
	if(k == 'F') return 42;
	if(k == 'G') return 43;
	if(k == 'H') return 44;
	if(k == 'I') return 45;
	if(k == 'J') return 46;
	if(k == 'K') return 47;
	if(k == 'L') return 48;
	if(k == 'M') return 49;
	if(k == 'N') return 50;
	if(k == 'O') return 51;
	if(k == 'P') return 52;
	if(k == 'Q') return 53;
	if(k == 'R') return 54;
	if(k == 'S') return 55;
	if(k == 'T') return 56;
	if(k == 'U') return 57;
	if(k == 'V') return 58;
	if(k == 'W') return 59;
	if(k == 'X') return 60;
	if(k == 'Y') return 61;
	if(k == 'Z') return 62;
	else return 63;
}

vector<float> makeClean(vector<float> vIn)
{
	for(int i = 0; i < vIn.size(); ++i) {
		if(vIn[i] < 0.5f) vIn[i] = 0.0f; 
		else vIn[i] = 1.0f;
	}

	return vIn;
}


bool correct(std::vector<float> in, std::vector<float> out) {
	float fSum = 0.0f;

	for(int a = 0; a < in.size(); ++a) {
		fSum += pow((in[a] - out[a]), 2);
	}

	if(fSum == 0) return true;

 	return false;
}

void printv(std::vector<float> in) {
	int size = in.size();

	for(int i = 0; i < size; ++i) {
		printf("%.0f", in[i]);
	}
	printf("\n");
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
