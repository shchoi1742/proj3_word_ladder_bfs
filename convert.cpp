#include "convert.h"
#include "Wordset.h"
#include <iostream>
#include <set>
#include <sstream>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <vector>
#include <iterator>

using namespace std;

// You should not need to change this function.
void loadWordsIntoTable(WordSet& words, std::istream& in)
{
	std::string line, word;
	std::stringstream ss;

	while (getline(in, line))
	{
		ss.clear();
		ss << line;
		while (ss >> word)
		{
			words.insert(word);
		}
	}

}

bool checkDuplicate(vector<string> v, string s)
{
	for (unsigned i = 0; i < v.size(); i++) {
		if (v[i] == s) {
			return true;
		}
	}
	return false;
}

// You probably want to change this function.
std::vector< std::string > convert(const std::string& s1, const std::string& s2, const WordSet& words)
{

	std::vector< std::string > ret;
	unordered_map<string, string> map;
	queue<string> tempList;
	string temp = "";
	if (s1.size() != s2.size()) {
		return ret;
	}
	if (!words.contains(s1) || !words.contains(s2)) {
		return ret;
	}
	if (s1 == s2) {
		return ret;
	}
	tempList.push(s1);
	while (!tempList.empty()) {
		for (unsigned i = 0; i < s1.size(); i++) {
			temp = tempList.front();
			for (char ch = 'a'; ch <= 'z'; ch++) {
				temp[i] = ch;
				if (temp != s1 && words.contains(temp) && map.find(temp) == map.end()) {
					tempList.push(temp);
					map.emplace(temp, tempList.front());
					if (temp == s2) {
						break;
					}
				}
			}
			if (temp == s2) {
				break;
			}
		}
		tempList.pop();
		if (temp == s2) {
			break;
		}
	}
	unordered_map<string, string>::iterator it = map.begin();
	string key = s2;
	while (it != map.end()) {
		if (map.find(key) != map.end()) {
			string value = map[key];
			if (!checkDuplicate(ret, value)) {
				ret.insert(ret.begin(), 1, value);
				key = value;
			}
		}
		it++;
	}
	if (!ret.empty()) {
		ret.push_back(s2);
	}


	return ret;  // stub obviously 
}
