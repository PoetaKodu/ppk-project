/** @file Główna logika programu. */

#pragma once

#include "ArgumentParsing.h"

#include "DecisionTree.h"

#include "AttributeTree.h"
#include "ForwardList.h"

// Aliasy pomocnicze
using Record 		= AttributeTree;
using Records 		= ForwardList< Record >;
using Category 		= std::pair<std::string, Records>;
using Categories 	= ForwardList< Category >;
using Attributes 	= ForwardList< std::string >;

/** Wykonuje główną logikę programu, korzystając z danych o trybie wykonania.
 * @param exec_ tryb wykonania
*/
void run(ExecSetup const & exec_);

/** Parsuje plik wejściowy z rekordami, korzystając z drzewa decyzyjnego.
 * @param fileContents_ zawartość pliku wejściowego
 * @param decisionTree_ drzewo decyzyjne
 * @return wygenerowana zawartość pliku wyjściowego
*/
std::string parseInput(std::string fileContents_, DecisionTree const& decisionTree_);

/** Wybiera etykiete danemu rekordowi, korzystając z drzewa decyzyjnego.
 * @param record_ rekord
 * @param decisionTree_ drzewo decyzyjne
 * @return nazwa etykiety
*/
std::string categorize(Record const& record_, DecisionTree const& decisionTree_);

/** Serializuje listę etykiet wraz z przyporządkowanymi rekordami do std::string-a.
 * @param categories_ lista etykiet
 * @param attributes_ lista atrybutów (wymagana przy czytaniu rekordów)
 * @return zserializowana lista etykiet wraz z rekordami
*/
std::string serializeCategories(Categories const& categories_, Attributes const& attributes_);

/** Wczytuje plik sekwencyjnie.
 * @param filePath_ ścieżka do pliku.
 * @return cała zawartość pliku
*/
std::string readFileSequentially(std::string const& filePath_);

/** Wyświetla pomoc programu */
void displayHelp();