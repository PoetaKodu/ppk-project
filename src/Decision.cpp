#include "Decision.h"
#include "DecisionTree.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

static void displayHelp();
static std::string readFileSequentially(std::string const& filePath_);

/////////////////////////////////////////
void run(ExecSetup const & exec_)
{
	if (exec_.help)
	{
		displayHelp();
	}
	else
	{
		std::cout << "# Reading tree file: \"" << exec_.treeFile << "\"\n";

		DecisionTreeNode* decisionTree = nullptr;

		{
			std::string fileContents = readFileSequentially(exec_.treeFile);
			if (fileContents.empty())
				throw std::runtime_error("tree file is empty or could not be read");

			decisionTree = readTree(fileContents.data(), fileContents.data() + fileContents.size());
		}

		std::cout << "# Reading input file: \"" << exec_.inputFile << "\"\n";

		

		std::cout << "# Writing output file: \"" << exec_.outputFile << '\"' << std::endl;

		delete decisionTree;
	}
}

/////////////////////////////////////////
void displayHelp()
{
	constexpr char const* helpContents =
R"help(Decision Tree:
This program parses records using decision tree.
Usage:

	-h - displays this help

	-i <file path> - [Required] - specifies input file path with records to parse
	-t <file path> - [Required] - specifies input file path with decision tree structure
	-o <file path> - [Required] - specifies output file path to write parsed records to
)help";

	std::cout << helpContents;
}

/////////////////////////////////////////
std::string readFileSequentially(std::string const& filePath_)
{
	constexpr std::size_t bufferSize = 16 * 1024;
	constexpr std::size_t reserveSize = 512 * 1024;

	std::string result;
	result.reserve(reserveSize);

	std::ifstream file(filePath_);
	if (file.is_open())
	{
		char buffer[bufferSize];
		while(file.read(buffer, bufferSize))
			result.append(buffer, buffer + bufferSize);
		result.append(buffer, buffer + file.gcount());
	}
	
	result.shrink_to_fit();
	return result;
}