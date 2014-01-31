#ifndef OCR_H
#define OCR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <bitset>
#include <list>
#include <math.h>

// opencv
#include <cv.h>
#include <highgui.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// feedforward
#include "feedforward.h"

#define __IMAGE_SIZE__  1024
#define __VECTOR_SITE__ 6
#define __THRESHOLD__	220

using namespace std;

struct SInputExample {
	std::vector<float> image;
	std::vector<float> k;
	int x;
	int y;
};

bool compare_width (SInputExample& first, SInputExample& second) {
	if(first.x < second.x)
		return true;
	false;
}

bool compare_height (SInputExample& first, SInputExample& second) {
	if(first.y < second.y)
		return true;
	false;
}

class COCR
{
private:
	std::vector<SInputExample> vExamples;

	void saveExamples(const char* pcFilename);
	void loadExamples(const char* pcFilename);

	feedForward* pFeedForward;
	
	std::vector<float> getImageFromRect(cv::Mat& src);

	float	m_fAvgHeight;
	float m_fAvgWidth;

public:
	COCR();
	~COCR();

	static std::vector<float> setKClass(int k);
	static int getKClass(std::vector<float>* pK);

	std::vector<SInputExample>* getExamples() {return &vExamples;}

	static int getFromChar(char k);
	static char getFromInt(int k);
	static std::vector<float> makeClean(std::vector<float> vIn);
	static bool correct(std::vector<float> in, std::vector<float> out);
	static void printv(std::vector<float> in);

	void train(int iMaxSteps, 
				  float fLearningRate, 
				  float fMomentum);

	void saveExamples() {saveExamples("examples.txt");}
	void loadExamples() {loadExamples("examples.txt");}

	void save();
	void load();

	void pushInputExample(SInputExample example);
	void addExampleSet(std::vector<SInputExample> examples);
	void popInputExample();
	void clearExamples();

	std::vector<SInputExample> createExampleSetFromImage(const char* pcFilename);
	std::vector<SInputExample> createWorkingSetFromImage(const char* pcFilename);

	char detectFromExample(SInputExample example);

	void detectWorkingSet();
};

#endif
