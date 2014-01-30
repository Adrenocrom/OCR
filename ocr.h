#ifndef OCR_H
#define OCR_H

#include <iostream>
#include <vector>
#include <bitset>
#include <math.h>

using namespace std;

struct SInputExample {
	std::vector<float> image;
	std::vector<float> k;
};

class COCR
{
private:
	std::vector<SInputExample> vExamples;
	
public:
	COCR();
	~COCR();

	static std::vector<float> setKClass(int k);
	static int getKClass(std::vector<float>* pK);

	std::vector<SInputExample>* getExamples() {return &vExamples;}

	static int getFromChar(char k);
	static std::vector<float> makeClean(std::vector<float> vIn);
	static bool correct(std::vector<float> in, std::vector<float> out);
	static void printv(std::vector<float> in);


	void train(float fLearningRate, float fMomentum);

	void save();
	void load();

	void pushInputExample(SInputExample example);
	void popInputExample();
};

#endif
