#include <stdio.h>
#include <stdlib.h>

#include "ocr.h"

int main(int argc, char *argv[])
{
	COCR* pOCR = NULL;
	pOCR = new COCR();

	if(strcmp(argv[1], "createSamples") == 0) {
		pOCR->clearExamples();
		pOCR->addExampleSet(pOCR->createExampleSetFromImage(argv[2]));
		pOCR->saveExamples();
	} 
	else if(strcmp(argv[1], "addSamples") == 0) {
		pOCR->loadExamples();
		pOCR->addExampleSet(pOCR->createExampleSetFromImage(argv[2]));
		pOCR->saveExamples();
	} 
	else if(strcmp(argv[1], "train") == 0) {
		if(strcmp(argv[2], "-n") == 0)
			pOCR->loadExamples();
		if(strcmp(argv[2], "-o") == 0)
			pOCR->load();
		pOCR->train(10000, 0.4f, 0.0f);
		pOCR->save();
	}
	else if(strcmp(argv[1], "detect") == 0) {
		pOCR->load();
		pOCR->clearExamples();
		pOCR->addExampleSet(pOCR->createWorkingSetFromImage(argv[2]));
		pOCR->detectWorkingSet();
	}

	if(pOCR) {
		delete pOCR;
		pOCR = NULL;
	}
	return 0;
}
