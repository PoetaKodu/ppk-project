#pragma once

#include <string>

struct ExecSetup
{
	std::string inputFile;
	std::string treeFile;
	std::string outputFile;
	bool help = false;
};

void parseArguments(ExecSetup & execSetup_, int argc_, char* argv_[]);