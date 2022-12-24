#include "Wordset.h"
#include <string>
using namespace std;

// returns s, as a number in the given base, mod the given modulus
unsigned powerFunction(unsigned base, unsigned power)
{
	unsigned product = 1;
	unsigned count = 0;
	while (count != power) {
		product = product * base;
		count++;
	}
	return product;

}
unsigned polynomialHashFunction(const std::string& s, unsigned base, unsigned mod)
{
	unsigned power = s.size() - 1;
	unsigned sum = 0;
	for (unsigned i = 0; i < s.size(); i++) {
		sum = sum + ((s[i] - 'a' + 1) * powerFunction(base, power - i));
	}
	return sum % mod;
}

WordSet::WordSet(unsigned initialCapacity, unsigned evictionThreshold)
{
	arr1 = new Node * [initialCapacity];
	arr2 = new Node * [initialCapacity];
	head = nullptr;
	tail = nullptr;
	currsize = initialCapacity;
	thresholds = evictionThreshold;
	for (unsigned i = 0; i < initialCapacity; i++) {
		arr1[i] = new Node;
		arr2[i] = new Node;
	}

}

WordSet::~WordSet()
{
	for (unsigned j = 0; j < currsize; j++) {
		delete* (arr1 + j);
		delete* (arr2 + j);
	}
	delete[]arr1;
	delete[]arr2;
	while (head != nullptr) {
		LinkedList* trash = head;
		head = head->next;
		delete trash;
	}
}
void WordSet::insert(const std::string& s)
{
	if (head == nullptr) {
		head = new LinkedList(s);
		tail = head;
	}
	else {
		tail->next = new LinkedList(s);
		tail = tail->next;
	}
	bool checker = insertInHash(s);
	while (!checker) {                 //resize until the insertions is done
		reSize();
		checker = reHash();
	}
}

bool WordSet::insertInHash(std::string s)
{
	unsigned count = 1;
	unsigned temp1 = polynomialHashFunction(s,BASE_H1,currsize);
	unsigned temp2 = polynomialHashFunction(s,BASE_H2,currsize);
	Node* ptr1 = *(arr1 + temp1);
	Node* ptr2 = *(arr2 + temp2);
	while (count < thresholds) {
		if (ptr1->val.empty()) {
			ptr1->val = s;
			return true;
		}
		if (!ptr1->val.empty() && ptr2->val.empty()) {
			ptr2->val = s;
			return true;
		}
		if (!ptr1->val.empty() && !ptr2->val.empty()) {
			string tempString1 = ptr1->val;
			string tempString2 = ptr2->val;
			ptr1->val = s;
			ptr2->val = tempString1;
			s = tempString2;
			temp1 = polynomialHashFunction(s, BASE_H1, currsize);
			temp2 = polynomialHashFunction(s, BASE_H2, currsize);
			ptr1 = *(arr1 + temp1);
			ptr2 = *(arr2 + temp2);
			count++;
		}
	}
	if (count == thresholds) {
		return false;
	}
	return false;
}
void WordSet::reSize()
{
	unsigned newSize = currsize * 2;
	//check the doubled size is prime number or not
	while (!primeNumberCheck(newSize)) {
		newSize++;
	}
	//create new arrays with new size
	Node** newArr1 = new Node * [newSize];
	Node** newArr2 = new Node * [newSize];
	for (unsigned x = 0; x < newSize; x++) {
		newArr1[x] = new Node;
		newArr2[x] = new Node;
	}
	//delete previous arrays
	for (unsigned y = 0; y < currsize; y++) {
		delete *(arr1 + y);
		delete* (arr2 + y);
	}
	delete []arr1;
	delete []arr2;
	//set new arrays in original arrays
	arr1 = newArr1;
	arr2 = newArr2;
	currsize = newSize;
}
bool WordSet::primeNumberCheck(unsigned n)
{
	for (unsigned i = 2; i < n; i++) {    //loop start with 2 because prime number can divide by 1 and themselves
		if (n % i == 0) {                 //if modulus is 0, return false becuase it is not prime number
			return false;
		}
	}
	return true;
}
bool WordSet::reHash()
{
	//reinsert all elements into the new arrays
	LinkedList* list = head;
	while (list!=nullptr) {
		bool result = insertInHash(list->word);     //insert in new arrays
		if (!result) {                                    //if insert is failed again, return false
			return false;
		}
		list = list->next;
	}
	return true;
}

bool WordSet::contains(const std::string& s) const
{
	Node* ptr1 = *(arr1 + polynomialHashFunction(s, BASE_H1,currsize));
	Node* ptr2 = *(arr2 + polynomialHashFunction(s, BASE_H2,currsize));
	if (ptr1->val != "" && ptr1->val == s) {
		return true;
	}
	if (ptr2->val != "" && ptr2->val == s) {
		return true;
	}
	return false; 
}

// return how many distinct strings are in the set
unsigned WordSet::getCount() const
{
	unsigned numStrings = 0;
	for (unsigned i = 0; i < currsize; i++) {
		if (!arr1[i]->val.empty()) {
			numStrings++;
		}
		if (!arr2[i]->val.empty()) {
			numStrings++;
		}
	}
	return numStrings;  
}

// return how large the underlying array is.
unsigned WordSet::getCapacity() const
{
	return currsize;
}
