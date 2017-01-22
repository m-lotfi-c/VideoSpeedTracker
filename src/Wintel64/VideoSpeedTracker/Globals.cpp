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



bool Globals::readConfig(){
	ifstream configIn("VST.cfg");
	if (!configIn.good()){
		cout << "Can't open VST.cfg." << endl;
		return false;
	}

	string::size_type place, place2;
	string inLine;
	while (getline(configIn, inLine)) {
		if (!configIn){
			cout << "Unexpected error in config file." << endl;
			return false;
		}
		if ((place = inLine.find('#')) != string::npos)
			inLine = inLine.substr(0, place);
		inLine = trim(inLine);
		while (inLine.back() == '\r')
			inLine.pop_back();
		if (inLine.empty())
			continue;   // blank line
		string lhs, rhs;
		getSides(inLine, &lhs, &rhs);
		if (lhs == "dataPathPrefix") {
			dataPathPrefix = rhs;
			cout << "dataPathPrefix = " << dataPathPrefix << endl;
		}
		else if (lhs == "L2RDirection") {
			L2RDirection = rhs;
			cout << "L2RDirection = " << L2RDirection << endl;
		}
		else if (lhs == "R2LDirection") {
			R2LDirection = rhs;
			cout << "R2LDirection = " << R2LDirection << endl;
		}
		else if (lhs == "obstruction") {	// If no foreground obstructions in scene, just make first value >= second value
			place = rhs.find(',');
			place2 = rhs.find(']');
			obstruction[0] = stoi(rhs.substr(1, place - 1));
			obstruction[1] = stoi(rhs.substr(place + 1, (place2 - place) - 1));
			cout << "obstruction[0] = " << obstruction[0] << endl;
			cout << "obstruction[1] = " << obstruction[1] << endl;
		}
		else if (lhs == "AnalysisBoxLeft") {	// must be >= 0 and <= 1279
			AnalysisBoxLeft = stoi(rhs);
			cout << "AnalysisBoxLeft = " << AnalysisBoxLeft << endl;
		}
		else if (lhs == "AnalysisBoxTop") {	// must be >= 0 and <= 719
			AnalysisBoxTop = stoi(rhs);
			cout << "AnalysisBoxTop = " << AnalysisBoxTop << endl;
		}
		else if (lhs == "AnalysisBoxWidth") {	// must be >= 0 and <= (1279 - AnalysisBoxLeft)
			AnalysisBoxWidth = stoi(rhs);
			cout << "AnalysisBoxWidth = " << AnalysisBoxWidth << endl;
		}
		else if (lhs == "AnalysisBoxHeight") {	// must be >= 0 and <= (719 - AnalysisBoxTop)
			AnalysisBoxHeight = stoi(rhs);
			cout << "AnalysisBoxHeight = " << AnalysisBoxHeight << endl;
		}
		else if (lhs == "speedLineLeft") {	// relative to AnalysisBoxLeft.  Must be >= 0 and <= (719 - AnalysisBoxWidth)
			speedLineLeft = stoi(rhs);
			cout << "speedLineLeft = " << speedLineLeft << endl;
		}
		else if (lhs == "speedLineRight") {	// relative to AnalysisBoxLeft.  Must be > speedLineLeft and <= (719 - AnalysisBoxWidth)
			speedLineRight = stoi(rhs);
			cout << "speedLineRight = " << speedLineRight << endl;
		}
		else if (lhs == "maxL2RDistOnEntry") {
			maxL2RDistOnEntry = stoi(rhs);
			cout << "maxL2RDistOnEntry = " << maxL2RDistOnEntry << endl;
		}
		else if (lhs == "maxR2LDistOnEntry") {
			maxR2LDistOnEntry = stoi(rhs);
			cout << "maxR2LDistOnEntry = " << maxR2LDistOnEntry << endl;
		}
		else if (lhs == "entryLookBack") {
			entryLookBack = stoi(rhs);
			cout << "entryLookBack = " << entryLookBack << endl;
		}
		else if (lhs == "obstruction_extent") {
			obstruction_extent = stoi(rhs);
			cout << "obstruction_extent = " << obstruction_extent << endl;
		}
		else if (lhs == "largeVehicleArea") {
			largeVehicleArea = stoi(rhs);
			cout << "largeVehicleArea = " << largeVehicleArea << endl;
		}
		else if (lhs == "CalibrationFramesL2R") {	// How many frames does it take a L2R vehicle to pass thru speed zone at speed limit?
			CalibrationFramesL2R = stoi(rhs);
			cout << "CalibrationFramesL2R = " << CalibrationFramesL2R << endl;
		}
		else if (lhs == "CalibrationFramesR2L") {	// How many frames does it take a R2L vehicle to pass thru speed zone at speed limit?
			CalibrationFramesR2L = stoi(rhs);
			cout << "CalibrationFramesR2L = " << CalibrationFramesR2L << endl;
		}
		else if (lhs == "SENSITIVITY_VALUE") {
			SENSITIVITY_VALUE = stoi(rhs);
			cout << "SENSITIVITY_VALUE = " << SENSITIVITY_VALUE << endl;
		}
		else if (lhs == "BLUR_SIZE") {
			BLUR_SIZE = stoi(rhs);
			cout << "BLUR_SIZE = " << BLUR_SIZE << endl;
		}
		else if (lhs == "SLOP") {
			SLOP = stoi(rhs);
			cout << "SLOP = " << SLOP << endl;
		}
		else if (lhs == "R2LStreetY") {	// R2L hubcap line
			R2LStreetY = stoi(rhs);
			cout << "R2LStreetY = " << R2LStreetY << endl;
		}
		else if (lhs == "L2RStreetY") {	// L2R hubcap line
			L2RStreetY = stoi(rhs);
			cout << "L2RStreetY = " << L2RStreetY << endl;
		}
		else if (lhs == "nextHeight") {
			nextHeight = stoi(rhs);
			cout << "nextHeight = " << nextHeight << endl;
		}
		else {
			cout << "Unknown key in config file: <" << lhs << ">.  Abortiing." << endl;
			return false;
		}
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



