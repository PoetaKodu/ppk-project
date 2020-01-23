/** Główna logika programu. */

#pragma once

#include "ArgumentParsing.h"

#include "DecisionTree.h"

#include "AttributeTree.h"
#include "ForwardList.h"

// Aliasy pomocnicze
using Record 		= AttributeTree;
using Labels 		= BinarySearchTree< std::string, std::string >;

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
std::string determineLabel(Record const& record_, DecisionTree const& decisionTree_);

/** Serializuje drzewo etykiet wraz z przyporządkowanymi rekordami do std::string-a.
 * @param labels_ drzewo etykiet
 * @return zserializowana lista etykiet wraz z rekordami
*/
std::string serializeLabels(Labels const& labels_);

/** Wczytuje plik sekwencyjnie.
 * @param filePath_ ścieżka do pliku.
 * @return cała zawartość pliku
*/
std::string readFileSequentially(std::string const& filePath_);

/** Wyświetla pomoc programu */
void displayHelp();