#include "Decision.h"
#include "DecisionTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

static void displayHelp();
static std::string readFileSequentially(std::string const& filePath_);
static std::string parseInput(std::string fileContents, DecisionTreeNode const& decisionTree_);

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

		std::string output;
		{
			std::string fileContents = readFileSequentially(exec_.inputFile);
			if (fileContents.empty())
				throw std::runtime_error("input file is empty or could not be read");

			output = parseInput(std::move(fileContents), *decisionTree);
		}

		delete decisionTree;

		std::cout << "# Writing output file: \"" << exec_.outputFile << '\"' << std::endl;

		std::ofstream outputFile(exec_.outputFile);
		if (outputFile.is_open())
			outputFile << output;
		else
			throw std::runtime_error("could not open output file for writing");
	}
}

/////////////////////////////////////////
std::string parseInput(std::string fileContents, DecisionTreeNode const& decisionTree_)
{
	struct Attribs {
		std::string attributeName;
		~Attribs() {
			if (next) delete next;
		}
		Attribs* next = nullptr;
	};

	Attribs* attribsHead = nullptr;
	Attribs** attribsTail = &attribsHead;

	// First line with attribs:

	std::istringstream iss(fileContents);

	std::string firstLine;
	// Read first line.
	{
		std::getline(iss, firstLine);
		std::istringstream issF(firstLine);
		std::string attributeName;
		while(issF >> attributeName)
		{
			if (attributeName.size() >= 1 && attributeName[0] == '%')
				break;

			(*attribsTail) = new Attribs();
			(*attribsTail)->attributeName = std::move(attributeName);
			attribsTail = &(*attribsTail)->next;
		}
	}

	// TODO: delete on throw!
	delete attribsHead;

	return "";
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