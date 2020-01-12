#include "Decision.h"

#include <iostream>

static void displayHelp();

//////////////////////////////////
void run(ExecSetup const & exec_)
{
	if (exec_.help)
	{
		displayHelp();
	}
	else
	{
		std::cout << "# Reading input file: \"" << exec_.inputFile << "\"\n";

		std::cout << "# Reading tree file: \"" << exec_.treeFile << "\"\n";

		std::cout << "# Writing output file: \"" << exec_.outputFile << '\"' << std::endl;
	}
}

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