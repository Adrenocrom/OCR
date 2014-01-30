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

	if(pOCR) {
		delete pOCR;
		pOCR = NULL;
	}
	return 0;
}
