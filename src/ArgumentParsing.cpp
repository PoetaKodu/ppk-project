#include "ArgumentParsing.h"

#include <stdexcept>
#include <cstring>

/////////////////////////////////////////////////
void parseArguments(ExecSetup & execSetup_, int argc_, char* argv_[])
{
	constexpr char const* inputFileTrigger = "-i";
	constexpr char const* treeFileTrigger = "-t";
	constexpr char const* outputFileTrigger = "-o";
	constexpr char const* helpTrigger = "-h";

	for(int i = 1; i < argc_; ++i)
	{
		if (strcmp(argv_[i], inputFileTrigger) == 0)
		{
			if (i + 1 == argc_)
				throw std::runtime_error("could not parse input file name argument");
			else
				execSetup_.inputFile = argv_[i + 1];
		}
		else if (strcmp(argv_[i], treeFileTrigger) == 0)
		{
			if (i + 1 == argc_)
				throw std::runtime_error("could not parse tree file name argument");
			else
				execSetup_.treeFile = argv_[i + 1];
		}
		else if (strcmp(argv_[i], outputFileTrigger) == 0)
		{
			if (i + 1 == argc_)
				throw std::runtime_error("could not parse output file name argument");
			else
				execSetup_.outputFile = argv_[i + 1];
		}
		else if (strcmp(argv_[i], helpTrigger) == 0)
		{
			execSetup_.help = true;
		}
	}

	if (!execSetup_.help)
	{
		if (execSetup_.inputFile.empty() ||
			execSetup_.treeFile.empty() ||
			execSetup_.outputFile.empty())
		{
			throw std::runtime_error("missing required parameters, use -h for help");
		}
	}	
}