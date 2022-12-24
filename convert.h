#ifndef ___PROJECT_3_CONVERT_HPP
#define ___PROJECT_3_CONVERT_HPP

#include <string>
#include <vector>
#include "Wordset.h"

void loadWordsIntoTable(WordSet& words, std::istream& in);
bool checkDuplicate(std::vector<std::string> v, std::string s);

std::vector< std::string > convert(const std::string& s1, const std::string& s2, const WordSet& words);



#endif
#pragma once
