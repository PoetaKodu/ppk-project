#pragma once

#include <cctype>
#include <algorithm>
#include <string>

/** Sprawdza czy znak c_ jest spacją.
 * @param c_ znak
 * @return true jeśli znak jest spacją, jeśli nie false
 * */
bool isWhitespace(char c_);

/** Zwraca wskaźnik na pierwszy znak z zakresu, który nie jest spacją.
 * @param beg_ początek zakresu
 * @param end_ koniec zakresu
 * @return wskaźnik na pierwszy znak z zakresu, który nie jest spacją
 * */
char const* findFirstNonSpace(char const* beg_, char const* end_);

/** Zwraca wskaźnik na pierwszy znak z zakresu, który jest spacją.
 * @param beg_ początek zakresu
 * @param end_ koniec zakresu
 * @return wskaźnik na pierwszy znak z zakresu, który jest spacją
 * */
char const* findFirstSpace(char const* beg_, char const* end_);

/** Usuwa komentarz z linii oraz ucina spacje na początku.
 * @param line_ linia tekstu
 * @return linia po usunięciu komentarza oraz spacji z początku.
 * */
std::string trimAndRemComment(std::string line_);