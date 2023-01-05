/*

DSA 2: Section 2 - Sep. 20
Program 1 - Hash Tables (Spellchecker)
Andy Kim

*/

#include <iostream>
#include <fstream>
#include <time.h>
#include "hash.h"

using namespace std;

// set size of hash table
const int table_size = 50000;

// function declarations
hashTable load_dictionary(const string& dict_file);
void spell_check(hashTable &hash, const string& input_file, const string& output_file);

// main function
int main()
{
    clock_t start, end;                             // used for hash table load time
    string dict_file, input_file, output_file;      // used for filestream

    // user input for dictionary file name
    cout << "Enter name of dictionary: ";
    cin >> dict_file;

    start = clock();
    hashTable d = load_dictionary(dict_file);       // load dictionary file into hashTable d
    end = clock() - start;
    cout << "Total time (in seconds) to load dictionary: " << ((double)end)/CLOCKS_PER_SEC << endl;     // output dictionary load time

    // user input for input/output file names
    cout << "Enter name of input file: ";
    cin >> input_file;
    cout << "Enter name of output file: ";
    cin >> output_file;

    start = clock();
    spell_check(d, input_file, output_file);        // spell check input file with dictionary hashTable d and output results
    end = clock() - start;

    cout << "Total time (in seconds) to check document: " << ((double)end)/CLOCKS_PER_SEC << endl;      // output document spell check time

    return 0;
}

// load_dictionary: takes input dictionary file, checks each line if its a valid word or not, sends it to a hash table
hashTable load_dictionary(const string& dict_file) {
    string line;
    ifstream dictionary;

    hashTable hash(table_size);         // create hashTable hash

    dictionary.open(dict_file);
    if(!dictionary){
        cout << "File failed to open." << endl;
        exit(EXIT_FAILURE);
    }
    else{
        // check each line if it is a valid word, if so, insert into hash
        while (getline(dictionary, line)) {
            for(int i = 0; i < line.size(); i++) {
                if(!(isalnum(line[i]) || line.size() <= 20 || line[i] == '-' || line[i] == '\'')) {
                    line.clear();
                }
            }
            transform(line.begin(), line.end(), line.begin(), ::tolower);       // convert line to lowercase
            hash.insert(line);
        }
    }
    dictionary.close();
    return hash;    // return hash table to main() to be used in spell_check
}

// spell_check: checks each word of input file for valid words, checks it with the hash table, and outputs to a separate output file
void spell_check(hashTable &hash, const string& input_file, const string& output_file) {
    string line, word;      // for input file stream
    int lineNum = 0;        // line number counter
    int isNum = 0;          // set to 1 if char is a number in the word

    // open input file, check for failure
    ifstream input;
    input.open(input_file);
    if(!input) {
        cout << "Input file failed to open." << endl;
        exit(EXIT_FAILURE);
    }

    // open output file, check for failure
    ofstream output;
    output.open(output_file);
    if(!output) {
        cout << "Output file failed to open." << endl;
        exit(EXIT_FAILURE);
    }

    // for each line of the input file, convert to lowercase, check if it is a valid char,
    // add it to string word, then check if word is too long/is in hash table
    while(getline(input, line)) {
        lineNum++;
        word.clear();
        transform(line.begin(), line.end(), line.begin(), ::tolower);           // convert line to lowercase

        for(int i = 0; i <= line.size(); i++) {
            if(isalnum(line[i]) || line[i] == '-' || line[i] == '\'') {
                word += line[i];                                // add char to the word string
                if(isdigit(line[i])) {isNum = 1;}               // if the char is a digit, set isNum = 1
            }
            else {
                // check if word is empty and if a number was detected
                if(!word.empty() && !isNum) {
                    if(word.size() > 20) {
                        // valid word, but too long
                        output << "Long word at line " << lineNum << ", starts: " << word.substr(0,20) << endl;
                        word.clear();
                        isNum = 0;
                    }
                    else if(!hash.contains(word)) {
                        // valid word, but not contained in hash table dictionary
                        output << "Unknown word at line " << lineNum << ": " << word << endl;
                        word.clear();
                        isNum = 0;
                    }
                    else {
                        // valid word found in hash table dictionary, move onto next word/line
                        word.clear();
                        isNum = 0;
                    }
                }
                // the word was either empty, or a number was detected, move onto next word/line
                else {
                    word.clear();
                    isNum = 0;
                }
            }

        }
        line.clear();
    }
    input.close();      // close filestream files
    output.close();
}
