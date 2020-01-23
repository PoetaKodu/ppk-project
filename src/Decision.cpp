#include "Decision.h"

#include "TextHelper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

/////////////////////////////////////////
void run(ExecSetup const & exec_)
{
	if (exec_.help)
	{
		displayHelp();
	}
	else
	{
		DecisionTree decisionTree;
		
		{
			std::string fileContents;
			try { 
				fileContents = readFileSequentially(exec_.treeFile);
			}
			catch(...) {
				throw std::runtime_error("tree file could not be read");
			}

			if (fileContents.empty())
				throw std::runtime_error("tree file cannot be empty");

			decisionTree = readDecisionTree(fileContents.data(), fileContents.data() + fileContents.size());
		}

		std::string output;
		{
			std::string fileContents;
			try { 
				fileContents = readFileSequentially(exec_.inputFile);
			}
			catch(...) {
				throw std::runtime_error("input file could not be read");
			}

			output = parseInput(std::move(fileContents), decisionTree);
		}

		std::ofstream outputFile(exec_.outputFile);
		if (outputFile.is_open())
			outputFile << output;
		else
			throw std::runtime_error("could not open output file for writing");
	}
}

/////////////////////////////////////////
std::string parseInput(std::string fileContents_, DecisionTree const& decisionTree_)
{
	
	constexpr std::size_t baseReserveSize = 64 * 1024;
	constexpr std::size_t bigFileReserveSize = 5 * 1024 * 1024;
	constexpr std::size_t bigFileThreshold = 10 * 1024 * 1024;

	std::size_t reserveSize = baseReserveSize;
	if (fileContents_.length() > bigFileThreshold)
		reserveSize = bigFileReserveSize;

	ForwardList< std::string > attributes;
	Labels labels;

	std::istringstream iss(fileContents_);

	std::string currentLine;
	
	// Read first line (with attributes):
	while(std::getline(iss, currentLine))
	{
		currentLine = trimAndRemComment(currentLine);
		if (currentLine.empty())
			continue;

		std::istringstream issF(currentLine);
		std::string attributeName;
		while(issF >> attributeName)
		{
			attributes.push(std::move(attributeName));
		}

		break;
	}

	// Read the rest of the file (line by line)
	while(std::getline(iss, currentLine))
	{
		// Trim string and cut comments
		currentLine = trimAndRemComment(currentLine);
		if (currentLine.empty())
			continue;

		std::istringstream issF(currentLine);

		// Setup record (read every attribute one by one and insert to tree)
		Record record;
		auto at = attributes.head;
		while(at)
		{
			double val;
			if (!(issF >> val))
				throw std::runtime_error("failed to read attribute \"" + at->value + "\"");

			record.set(&at->value, val);

			at = at->next;
		}

		// Determine label name:
		std::string labelName = determineLabel(record, decisionTree_);
		
		// Find specified label...
		std::string* cat = labels.tryGet(labelName);

		// ... or create new one if not found.
		if (!cat)
		{
			cat = &(labels.set( std::move(labelName), {} ));
			cat->reserve(reserveSize);
		}

		cat->append(currentLine);
		cat->push_back('\n');
	}

	return serializeLabels(labels);
}

/////////////////////////////////////////
std::string determineLabel(Record const& record_, DecisionTree const& decisionTree_)
{
	using CNode = DecisionTree::Node const;

	CNode* root = decisionTree_.root;
	while (root)
	{
		bool choice = false; // left (false) / right (true)
		if (root->cond.op == CNode::Condition::GreaterThan)
			choice = record_.get(&root->cond.attributeName) > root->cond.value;
		else if (root->cond.op == CNode::Condition::LowerThan)
			choice = record_.get(&root->cond.attributeName) < root->cond.value;

		CNode::Anchor const& anch = choice ? root->succeededAnchor : root->failedAnchor;
		if (anch.isLabel)
			return anch.label;
		else
			root = choice ? root->succeeded : root->failed;
	}
	throw std::runtime_error("decision leaf is null");
}

/////////////////////////////////////////
std::string serializeLabels(Labels const& labels_)
{
	constexpr std::size_t initialReserveSize = 1 * 1024 * 1024;
	std::string output;
	output.reserve(initialReserveSize);

	labels_.forEach(
		[&](std::string const& labelName, std::string const& cat)
		{
			output += labelName;
			output += '\n';
			output += cat;
		});

	return output;
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
	else
		throw std::runtime_error("could not open file \"" + filePath_ + "\"");
	
	result.shrink_to_fit();
	return result;
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