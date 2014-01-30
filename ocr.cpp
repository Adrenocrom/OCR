#include "ocr.h"

COCR::COCR() {


}

COCR::~COCR() {

}

std::vector<float> COCR::setKClass(int k) {
	std::vector<float> out;
	std::bitset<6> klass(k);

	for(int i = klass.size(); i >= 0; --i) {
		if(klass[i] == 1)
			out.push_back(1.0f);
		else
			out.push_back(0.0f);
	}

	return out;

}

int COCR::getKClass(std::vector<float>* pK) {
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

int OCR::getFromChar(char k) {
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

std::vector<float> OCR::makeClean(std::vector<float> vIn)
{
	for(int i = 0; i < vIn.size(); ++i) {
		if(vIn[i] < 0.5f) vIn[i] = 0.0f; 
		else vIn[i] = 1.0f;
	}

	return vIn;
}

bool OCR::correct(std::vector<float> in, std::vector<float> out) {
	float fSum = 0.0f;

	for(int a = 0; a < in.size(); ++a) {
		fSum += pow((in[a] - out[a]), 2);
	}

	if(fSum == 0) return true;

 	return false;
}

void OCR::printv(std::vector<float> in) {
	int size = in.size();

	for(int i = 0; i < size; ++i) {
		printf("%.0f", in[i]);
	}
	printf("\n");
}

void OCR::pushInputExample(SInputExample example) {
	vExamples.push_back(example);
}

void OCR::popInputExample() {
	if(vExamples.size() > 0)
		vExamples.pop_back();
}

void OCR::save() {

}

void OCR::load() {

}

void OCR::train(float fLearningRate, float fMomentum) {

}
