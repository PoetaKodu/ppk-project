/** @file Zawiera funkcję główną programu */

#include "Decision.h"

#include <iostream>
#include <stdexcept>

/** Funkcja główna programu.
 * @param argc ilość argumentów
 * @param argv tablica argumentów
 * @return kod błędu lub 0
*/
int main(int argc_, char* argv_[])
{
	ExecSetup execSetup;

	try {
		parseArguments(execSetup, argc_, argv_);

		run(execSetup);
	}
	catch(std::exception &exc) {
		std::cerr << "Error, details: " << exc.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Error, no details available." << std::endl;
	}
}