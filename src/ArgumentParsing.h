/** @file Parsowanie argumentów */
#pragma once

#include <string>

/** Informacje o trybie uruchomienia. */
struct ExecSetup
{
	std::string inputFile; 	///< Ścieżka do pliku wejściowego rekordów
	std::string treeFile; 	///< Ścieżka do pliku wejściowego drzewa
	std::string outputFile; ///< Ścieżka do pliku wyjściowego
	bool help = false; 		///< Czy wyświetlić pomoc?
};

/** Parsuje argumenty i wypełnia informacje o trybie uruchomienia.
 * @param[out] execSetup_ tryb uruchomienia
 * @param argc_ ilość argumentów
 * @param argv_ tablica argumentów
*/
void parseArguments(ExecSetup & execSetup_, int argc_, char* argv_[]);