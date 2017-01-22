//                  Copyright Paul Reynolds, Locust Avenue, Charlottesville, Va,  2016
//                                     All rights reserved.

//                                     License Agreement
//                                For VideoSpeedTracker (VST)
//                                 (3 - clause BSD License)

// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
// conditions are met :

// 1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
//      in the documentation and / or other materials provided with the distribution.
// 3) Neither the name of the copyright holder nor the names of the contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.

// This software is provided by the copyright holder and contributors “as is” and any express or implied warranties, including,
// but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed.In no event
// shall copyright holders or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential
// damages(including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business
// interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort(including negligence
// or otherwise) arising in any way out ofthe use of this software, even if advised of the possibility of such damage.


#include "Globals.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../common/portability.h"
// #include <string>

using namespace std;

string trim(const string &toBeTrimmed){
	string::size_type first = 0;
	while (first < toBeTrimmed.size() && toBeTrimmed[first] == ' ') first++;
	int last = toBeTrimmed.length() - 1;
	while (toBeTrimmed[last] == ' ') last--;
	return toBeTrimmed.substr(first, (last-first)+1);
}

void getSides(const string &inLine, string *lhs, string *rhs) {
	string::size_type p;
	p = inLine.find('=');
	*lhs = trim(inLine.substr(0, p));
	*rhs = trim(inLine.substr(p+1));
}

bool Globals::applyConfig(const string &lhs, const string &rhs) {
	string::size_type place, place2;

	enum { STRING, INT, INT_ARR2 };
	struct { const char *name; void *dest; int type; } configKeys[] = {
		{ "L2RDirection",         &L2RDirection,         STRING },
		{ "R2LDirection",         &R2LDirection,         STRING },
		{ "obstruction",          obstruction,           INT_ARR2 },
		{ "AnalysisBoxLeft",      &AnalysisBoxLeft,      INT },
		{ "AnalysisBoxTop",       &AnalysisBoxTop,       INT },
		{ "AnalysisBoxWidth",     &AnalysisBoxWidth,     INT },
		{ "AnalysisBoxHeight",    &AnalysisBoxHeight,    INT },
		{ "speedLineLeft",        &speedLineLeft,        INT },
		{ "speedLineRight",       &speedLineRight,       INT },
		{ "maxL2RDistOnEntry",    &maxL2RDistOnEntry,    INT },
		{ "maxR2LDistOnEntry",    &maxR2LDistOnEntry,    INT },
		{ "entryLookBack",        &entryLookBack,        INT },
		{ "obstruction_extent",   &obstruction_extent,   INT },
		{ "largeVehicleArea",     &largeVehicleArea,     INT },
		{ "CalibrationFramesL2R", &CalibrationFramesL2R, INT },
		{ "CalibrationFramesR2L", &CalibrationFramesR2L, INT },
		{ "SENSITIVITY_VALUE",    &SENSITIVITY_VALUE,    INT },
		{ "BLUR_SIZE",            &BLUR_SIZE,            INT },
		{ "SLOP",                 &SLOP,                 INT },
		{ "R2LStreetY",           &R2LStreetY,           INT },
		{ "L2RStreetY",           &L2RStreetY,           INT },
		{ "nextHeight",           &nextHeight,           INT },
	};

	for (size_t i=0; i<sizeof(configKeys)/sizeof(configKeys[0]); i++) {
		if (lhs == configKeys[i].name) {
			switch (configKeys[i].type) {
				case STRING:
					*(string*)configKeys[i].dest = rhs;
					break;
				case INT:
					*(int*)configKeys[i].dest = stoi(rhs);
					break;
				case INT_ARR2:
					place = rhs.find(',');
					place2 = rhs.find(']');
					((int*)configKeys[i].dest)[0] = stoi(rhs.substr(1, place - 1));
					((int*)configKeys[i].dest)[1] = stoi(rhs.substr(place + 1, (place2 - place) - 1));
					break;
			}
			return true;
		}
	}

	cout << "Unknown key in config file: <" << lhs << ">.  Aborting." << endl;
	return false;
}

bool Globals::readConfig(){
	ifstream configIn("VST.cfg");
	if (!configIn.good()){
		const char *home = getenv("HOME");
		if (home) {
			string rcfile = string(home) + DIR_SEP + ".vstrc";
			configIn.open("/home/patrick/.vstrc");
		}
		if (!configIn.good()){
			cout << "Can't open VST.cfg." << endl;
			return false;
		}
	}

	string inLine;
	while (getline(configIn, inLine)) {
		if (!configIn){
			cout << "Unexpected error in config file." << endl;
			return false;
		}
		string::size_type place;
		if ((place = inLine.find('#')) != string::npos)
			inLine = inLine.substr(0, place);
		inLine = trim(inLine);
		while (inLine.back() == '\r')
			inLine.pop_back();
		if (inLine.empty())
			continue;   // blank line
		string lhs, rhs;
		getSides(inLine, &lhs, &rhs);
		if (!applyConfig(lhs, rhs))
			return false;
	}  // while not eof

	configIn.close();

	if ((AnalysisBoxLeft + AnalysisBoxWidth) > 1279){
		cout << "Analysis box too wide.  Must be <= 1279. Check config file.  Aborting." << endl;
		return false;
	}
	else if ((AnalysisBoxTop + AnalysisBoxHeight) > 719){
		cout << "Analysis box too high.  Must be <= 719.  Check config file.  Aborting." << endl;
		return false;
	}
	return true;
};



