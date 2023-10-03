#include <iostream>
#include "node.h"
#include <string>
#include <ctype.h>
#include "timer.h"
#include <fstream>
using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::ios;
using namespace std;

#ifndef MYLIST_
#define MYLIST_
#include "node.h"
template <class T>
class myList {
private:
    node<T>* head;
    int size;
    long long int compares;
public:
    myList()
    {
        head = nullptr; size = 0;
    }
    ~myList();
    void insert(T item);
    void remove();
    T front();
    void print();
    bool find(T item);
    bool isEmpty() {
        return size == 0;
    }
    int getSize() { return size; }
    long long int getCompares() { return compares; }
	void writeToFile(string fileName);
};
#endif

template <class T>
myList<T>::~myList()
{
	node<T>* current = head;
	while (current != 0) {
		node<T>* next = current->next;
		delete current;
		current = next;
	}
	head = nullptr;
}

template <class T>
void myList<T>::insert(T item)
{
	node<T>* tmp = new node<T>(item);
	tmp->next = head;
	head = tmp;
	size++;
}

template <class T>
void myList<T>::remove()
{
	node<T>* tmp;
	if (head != nullptr)
	{
		tmp = head;
		head = tmp->next;
		delete tmp;
		size--;
	}
}

template <class T>
T myList<T>::front()
{
	if (head != nullptr)
	{
		return head->data;
	}
	else {
		return T(0);
	}
}

template <class T>
void myList<T>::print()
{
	for (node<T>* tmp = head; tmp != nullptr; tmp - tmp->next)
	{
		cout << tmp->data;
		if (tmp->next != nullptr)
			cout << " -> ";
		else
			cout << "/\n";
	}
}

template <class T>
void myList<T>::writeToFile(string fileName)
{
	ofstream file;
	file.open(fileName);
	for (node<T>* tmp = head; tmp != nullptr; tmp - tmp->next)
	{
		file << tmp->data;
		if (tmp->next != nullptr)
			file << " -> ";
		else
			file << "/\n";
	}
}

#include <string>
using namespace std;

template <class T>
bool myList<T>::find(T item)
{
	compares = 0;
	node<T>* tmp;
	for (tmp = head; tmp != nullptr; tmp = tmp->next)
	{
		compares++;
		if (tmp->data.compare(item))
			return true;
	}
	return false;
}



string cleanWord(string str) {
	string ret = "";
	char c;
	for (int i = 0; i < str.length(); i++)
	{
		c = str[i];
		if (isalnum(c))
		{
			if (isdigit(c))
				ret += c;
			else
				ret += tolower(c);
		}
		else if (c == '\'')
			ret += c;
	}
	return ret;
}

int main() {
	Timer tim;
	fstream readDict;
	readDict.open("dict.txt", ios::in);
	if (!readDict) {
		cout << "File dict.txt not found" << endl;
	}
	myList<string>dictionary;
	string word;
	getline(readDict, word);
	while (!readDict.eof())
	{
		dictionary.insert(word);
		getline(readDict, word);
	}

	tim.Start();
	fstream readBook;
	readBook.open("book.txt", ios::in);
	if (!readBook)
		cout << "File book.txt not found";
	long long int totalWords = 0;
	long long int numWordsFound = 0;
	long long int numComparesFound = 0;
	long long int numComparesNotFound = 0;
	long long int numWordsNotFound = 0;
	long long int numWordsNotChecked = 0;
	myList<string>misspelled;
	string rawWord;

	while (!readBook.eof()) {
		readBook >> rawWord;
		word = cleanWord(rawWord);
		if (word.empty())
			;
		else if (!isalpha(word[0]))
			numWordsNotChecked++;
		else
		{
			if (dictionary.find(word))
			{
				numComparesFound += dictionary.getCompares();
				numWordsFound++;
			}
			else
			{
				numComparesNotFound += dictionary.getCompares();
				numWordsNotFound++;
				misspelled.insert(word);
			}

		}

	}
	tim.Stop();

	misspelled.writeToFile("misspelled.txt");

	cout << "Dictionary size " << dictionary.getSize() << endl;
	cout << "Done checking and these are the results" << endl;
	cout << "Finished in time: " << tim.Time() << endl;
	cout << "There are " << numWordsFound << " words found in the dictionary " << numComparesFound << " compares. Average: " << (numComparesFound / numWordsFound) << endl;
	cout << "There are " << numWordsNotFound << " words NOT found in the dictionary " << numComparesNotFound << " compares. Average: " << (numComparesNotFound / numWordsNotFound)
		<< endl;
	cout << "There are " << numWordsNotChecked << " not checked" << endl;




	return 0;
}

