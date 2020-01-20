#include "Decision.h"
#include "DecisionTree.h"

#include "AttributeTree.h"
#include "ForwardList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Helper aliases:
using Record 		= AttributeTree;
using Records 		= ForwardList< Record >;
using Category 		= std::pair<std::string, Records>;
using Categories 	= ForwardList< Category >;
using Attributes 	= ForwardList< std::string >;
	
/** Parsuje plik wejściowy z rekordami, korzystając z drzewa decyzyjnego.
 * @param fileContents_ zawartość pliku wejściowego
 * @param decisionTree_ drzewo decyzyjne
 * @return wygenerowana zawartość pliku wyjściowego
*/
static std::string parseInput(std::string fileContents_, DecisionTree const& decisionTree_);

/** Wybiera etykiete danemu rekordowi, korzystając z drzewa decyzyjnego.
 * @param record_ rekord
 * @param decisionTree_ drzewo decyzyjne
 * @return nazwa etykiety
*/
static std::string categorize(Record const& record_, DecisionTree const& decisionTree_);

/** Serializuje listę etykiet wraz z przyporządkowanymi rekordami do std::string-a.
 * @param categories_ lista etykiet
 * @param attributes_ lista atrybutów (wymagana przy czytaniu rekordów)
 * @return zserializowana lista etykiet wraz z rekordami
*/
static std::string serializeCategories(Categories const& categories_, Attributes const& attributes_);

/** Wczytuje plik sekwencyjnie.
 * @param filePath_ ścieżka do pliku.
 * @return cała zawartość pliku
*/
static std::string readFileSequentially(std::string const& filePath_);

/** Wyświetla pomoc programu */
static void displayHelp();

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

		DecisionTree decisionTree;
		
		{
			std::string fileContents = readFileSequentially(exec_.treeFile);
			if (fileContents.empty())
				throw std::runtime_error("tree file is empty or could not be read");

			decisionTree = readDecisionTree(fileContents.data(), fileContents.data() + fileContents.size());
		}

		std::cout << "# Reading input file: \"" << exec_.inputFile << "\"\n";

		std::string output;
		{
			std::string fileContents = readFileSequentially(exec_.inputFile);
			if (fileContents.empty())
				throw std::runtime_error("input file is empty or could not be read");

			output = parseInput(std::move(fileContents), decisionTree);
		}

		std::cout << "# Writing output file: \"" << exec_.outputFile << '\"' << std::endl;

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
	Attributes attributes;
	Categories categories;

	std::istringstream iss(fileContents_);

	std::string currentLine;
	// Read first line (with attributes):
	{
		std::getline(iss, currentLine);
		std::istringstream issF(currentLine);
		std::string attributeName;
		while(issF >> attributeName)
		{
			if (attributeName.size() >= 1 && attributeName[0] == '%')
				break;

			attributes.push(std::move(attributeName));
		}
	}

	// Read the rest of the file (line by line)
	while(std::getline(iss, currentLine))
	{
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

		// Determine category name:
		std::string catName = categorize(record, decisionTree_);
		
		// Find specified category...
		auto cat = categories.findIf(
				[&](Category const& c) {
					return c.first == catName;
				}
			);

		// ... or create new one if not found.
		if (!cat)
			cat = &(categories.push( { std::move(catName), {} } ));

		// Push record:
		auto& records = cat->value.second;

		records.push( std::move(record) );
	}

	return serializeCategories(categories, attributes);
}

/////////////////////////////////////////
std::string categorize(Record const& record_, DecisionTree const& decisionTree_)
{
	using CNode = DecisionTree::Node const;

	CNode* root = decisionTree_.root;
	while (root)
	{
		bool choice = false; // left (false) / right (true)
		if (root->cond.op == CNode::Condition::GreaterThan)
			choice = record_.get(root->cond.attributeName) > root->cond.value;
		else if (root->cond.op == CNode::Condition::LowerThan)
			choice = record_.get(root->cond.attributeName) < root->cond.value;

		CNode::Anchor const& anch = choice ? root->succeededAnchor : root->failedAnchor;
		if (anch.isLabel)
			return anch.label;
		else
			root = choice ? root->succeeded : root->failed;
	}
	throw std::runtime_error("decision leaf is null");
}

/////////////////////////////////////////
static std::string serializeCategories(Categories const& categories_, Attributes const& attributes_)
{
	std::stringstream output;
	output.precision(1);
	output << std::fixed;

	Categories::Node* cat = categories_.head;
	while (cat)
	{
		// Append category name:
		output << cat->value.first << std::endl;
		
		// Iterate through every record that belongs to category
		Records::Node* rec = cat->value.second.head;
		while (rec)
		{
			// Iterate through every attribute:
			auto at = attributes_.head;
			while(at)
			{
				// Append attribute value:
				output << rec->value.get(at->value) << ' ';
				at = at->next;
			}

			// Append new line:
			output << std::endl;
			
			rec = rec->next;
		}
		cat = cat->next;
	}

	return output.str();
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