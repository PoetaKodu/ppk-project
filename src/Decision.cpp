#include "Decision.h"
#include "DecisionTree.h"

#include "AttributeTree.h"
#include "ForwardList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

static void displayHelp();
static std::string readFileSequentially(std::string const& filePath_);
static std::string parseInput(std::string fileContents, DecisionTreeNode const& decisionTree_);
static std::string categorize(AttributeTreeNode const& record, DecisionTreeNode const& decisionTree_);

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
	struct CategorizedRecords
	{
		~CategorizedRecords()
		{
			if (recordsHead) delete recordsHead;
			if (next) delete next;
		}

		struct Records
		{
			Records(AttributeTreeNode* rec = nullptr)
				: record(rec)
			{
			}
			~Records() {
				if (next) delete next;
			}
			AttributeTreeNode* record;
			Records* next = nullptr;
		};

		std::string categoryName;
		Records* recordsHead = nullptr;
		Records* recordsTail = nullptr;

		CategorizedRecords* next = nullptr;
	};


	ForwardList<std::string> attributes;
	// First line with attribs:

	std::istringstream iss(fileContents);

	std::string currentLine;

	// Read first line.
	{
		std::getline(iss, currentLine);
		std::istringstream issF(currentLine);
		std::string attributeName;
		while(issF >> attributeName)
		{
			if (attributeName.size() >= 1 && attributeName[0] == '%')
				break;

			attributes.push(attributeName);
		}
	}

	CategorizedRecords* categories = nullptr;
	{
		while(std::getline(iss, currentLine))
		{
			auto at = attributes.head;
			std::istringstream issF(currentLine);

			AttributeTreeNode *record = nullptr;
			while(at)
			{
				double val;
				if (!(issF >> val))
					throw std::runtime_error("failed to read parameter \"" + at->value + "\"");
				// TODO: /\ delete record

				setAttribute(record, at->value, val);

				at = at->next;
			}

			std::string category = categorize(*record, decisionTree_);
			std::cout << category << std::endl;
			
			CategorizedRecords* cat = nullptr;
			if (!categories)
			{
				cat = categories = new CategorizedRecords;
				categories->categoryName = category;
			}
			else
			{
				CategorizedRecords** processedCat = &categories;
				while (*processedCat)
				{
					if ((*processedCat)->categoryName == category)
						break;
					else
						processedCat = &((*processedCat)->next);
				}
				if (*processedCat == nullptr)
				{
					*processedCat = new CategorizedRecords;
					(*processedCat)->categoryName = category;
				}
				cat = *processedCat;
			}
				
			auto rec = new CategorizedRecords::Records{ record };
			if (!cat->recordsHead)
				cat->recordsHead = cat->recordsTail = rec;
			else
			{
				cat->recordsTail->next = rec;
				cat->recordsTail = rec;
			}
		}
	}

	std::string outputStr;
	{
		std::stringstream output;

		CategorizedRecords *cat = categories;
		while (cat)
		{
			output << cat->categoryName << std::endl;
			auto rec = cat->recordsHead;
			while (rec)
			{
				auto at = attributes.head;
				while(at)
				{
					output << getAttributeValue(rec->record, at->value) << ' ';
					at = at->next;
				}

				output << std::endl;
				rec = rec->next;
			}

			cat = cat->next;
		}

		outputStr = output.str();
	}


	// TODO: delete on throw!
	delete categories;

	return outputStr;
}

/////////////////////////////////////////
std::string categorize(AttributeTreeNode const& record, DecisionTreeNode const& decisionTree_)
{
	using CNode = DecisionTreeNode const;

	CNode* root = &decisionTree_;
	while (root)
	{
		bool choice = false; // left (false) / right (true)
		if (root->cond.op == CNode::Condition::GreaterThan)
			choice = getAttributeValue(&record, root->cond.attributeName) > root->cond.value;
		else if (root->cond.op == CNode::Condition::GreaterThan)
			choice = getAttributeValue(&record, root->cond.attributeName) < root->cond.value;

		CNode::Anchor const& anch = choice ? root->succeededAnchor : root->failedAnchor;
		if (anch.isLabel)
			return anch.label;
		else
			root = choice ? root->succeeded : root->failed;
	}
	throw std::runtime_error("decision leaf is null");
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