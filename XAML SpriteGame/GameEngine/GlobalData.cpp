#include "pch.h"
#include "GlobalData.h"


GlobalData*	GlobalHelper::theData;

GlobalData* GlobalHelper::getData(){

	if (theData == NULL)
	{
		theData = new GlobalData();
	}

	return theData;
}


