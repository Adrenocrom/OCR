#include "ocr.h"

COCR::COCR() {
	pFeedForward = NULL;
	pFeedForward = new feedForward(__IMAGE_SIZE__,
											 __VECTOR_SITE__,
											 "100");
}

COCR::~COCR() {
	if(pFeedForward) {
		delete pFeedForward;
		pFeedForward = NULL;
	}
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

int COCR::getFromChar(char k) {
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

std::vector<float> COCR::makeClean(std::vector<float> vIn)
{
	for(unsigned int i = 0; i < vIn.size(); ++i) {
		if(vIn[i] < 0.5f) vIn[i] = 0.0f; 
		else vIn[i] = 1.0f;
	}

	return vIn;
}

bool COCR::correct(std::vector<float> in, std::vector<float> out) {
	float fSum = 0.0f;

	for(int a = 0; a < in.size(); ++a) {
		fSum += pow((in[a] - out[a]), 2);
	}

	if(fSum == 0) return true;

 	return false;
}

void COCR::printv(std::vector<float> in) {
	int size = in.size();

	for(int i = 0; i < size; ++i) {
		printf("%.0f", in[i]);
	}
	printf("\n");
}

void COCR::pushInputExample(SInputExample example) {
	vExamples.push_back(example);
}

void COCR::popInputExample() {
	if(vExamples.size() > 0)
		vExamples.pop_back();
}

void COCR::addExampleSet(std::vector<SInputExample> examples) {
	int size = examples.size();

	for(int i = 0; i < size; ++i)
		vExamples.push_back(examples[i]);
}

void COCR::clearExamples() {
	vExamples.clear();
}

std::vector<float> COCR::getImageFromRect(cv::Mat& src) {
	std::vector<float> out;
	unsigned char *input = (unsigned char*)(src.data);

	threshold(src, src, __THRESHOLD__, 255, cv::THRESH_BINARY_INV);

	float p;
	for(int y = 0; y < src.rows; ++y) {
		for(int x = 0;  x < src.cols; ++x) {
			p = input[src.step * x + y] / 255;
			out.push_back(p);
		}
	}

	return out;
}

void COCR::saveExamples(const char* pcFilename) {
	FILE* pFile = NULL;

	if((pFile = fopen(pcFilename, "w")) == NULL) {
		fclose(pFile);
		pFile = NULL;
		return;
	}

	fprintf(pFile, "numExamples= %d\n\n", (int)vExamples.size());

	for(int i = 0; i < vExamples.size(); ++i) {
		for(int a = 0; a < vExamples[i].k.size()-1; ++a)
			fprintf(pFile, "%.0f ", vExamples[i].k[a]);
		fprintf(pFile, "%.0f\n", vExamples[i].k[vExamples[i].k.size()-1]);

		for(int b = 0; b < vExamples[i].image.size()-1; ++b)
			fprintf(pFile, "%.0f ", vExamples[i].image[b]);
		fprintf(pFile, "%.0f\n", vExamples[i].image[vExamples[i].image.size()-1]);
	}

	fclose(pFile);
	pFile = NULL;
}

void COCR::loadExamples(const char* pcFilename) {
	FILE* pFile = NULL;
	char cBuffer[200];
	int numExamples;
	float temp;

	if((pFile = fopen(pcFilename, "r")) == NULL) {
		fclose(pFile);
		pFile = NULL;
		return;
	}

	vExamples.clear();

	fscanf(pFile, "%s", cBuffer);
	fscanf(pFile, "%d", &numExamples);

	for(int i = 0; i < numExamples; ++i) {
		SInputExample example;
		for(int k = 0; k < 6; ++k) {
			fscanf(pFile, "%f", &temp);
			example.k.push_back(temp);
		}

		example.k = makeClean(example.k);
		for(int l = 0; l < 1024; ++l) {
			fscanf(pFile, "%f", &temp);
			example.image.push_back(temp);
		}

		vExamples.push_back(example);
	}

	fclose(pFile);
	pFile = NULL;
}

void COCR::save() {
	printf("Save Examples\n");
	saveExamples("examples.txt");
	printf("Save Network\n");
	pFeedForward->saveWeights("net.txt");
}

void COCR::load() {
	printf("Load Examples\n");
	loadExamples("examples.txt");
	printf("Load Network\n");
	pFeedForward->loadWeights("net.txt");
}

void COCR::train(int iMaxSteps,
					 float fLearningRate, 
					 float fMomentum) {
	bool trained = false;
	int numExamples = vExamples.size();

	printf("start learning... \n");

	int steps = 0;
	do {
		trained = true;
		steps++;

		for(int i = 0; i < numExamples; ++i) {
			std::vector<float> result = makeClean(pFeedForward->calcOutput(vExamples[i].image));

			if(!correct(result, vExamples[i].k)) {
				trained = false;

				pFeedForward->learnNetwork(vExamples[i].image,
									 				vExamples[i].k,
													fLearningRate,
													fMomentum);
			}
		}
	} while(!trained && steps < iMaxSteps);

	pFeedForward->saveWeights("net.txt");
	printf("has learned from %d examples in %d steps. \n", numExamples, steps);
}

std::vector<SInputExample> COCR::createExampleSetFromImage(const char* pcFilename) {
	std::vector<SInputExample> examples;

	cv::Mat src = cv::imread(pcFilename, 1);
	cv::Mat thr;
	cv::Mat con;

	cv::cvtColor(src, src, CV_BGR2GRAY);
	cv::threshold(src, thr, __THRESHOLD__, 255, cv::THRESH_BINARY_INV);
	cv::imshow("thr", thr);

	thr.copyTo(con);

	std::vector<std::vector<cv::Point> > contours;
	std::vector< cv::Vec4i > 				 hierarchy;

	cv::findContours(con, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	for(unsigned int i = 0; i < contours.size(); i = hierarchy[i][0]) {
		cv::Rect r = cv::boundingRect(contours[i]);
		cv::Mat tmp = src(cv::Rect(r));
		cv::Mat small;
		cv::resize(tmp, small, cv::Size(32, 32), 0, 0, cv::INTER_CUBIC);

		SInputExample example;
		example.image = getImageFromRect(small);

		printf("Klasse: ");
		cv::imshow("small", small);
		char k = cv::waitKey();
		printf("%s\n", &k);

		example.k = setKClass(getFromChar(k));
		examples.push_back(example);
	}

	return examples;
}
