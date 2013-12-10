#pragma once

class GlobalData
{

};

class GlobalHelper
{
	static GlobalData* theData;
public:
	static GlobalData* getData();


};