#include "Decision.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
	ExecSetup execSetup;

	try {
		parseArguments(execSetup, argc, argv);

		run(execSetup);
	}
	catch(std::exception &exc) {
		std::cerr << "Error, details: " << exc.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Error, no details available." << std::endl;
	}
}