#include <iostream>
#include <fstream>
#include "trie.h"

using namespace std;

// Test program
int main()
{
	// Create trie
    Trie* trie = new Trie;
    
    // Add queries
    trie->addQuery("ATTGGTACACATTGGTACACATTGGTACACATTGGTACACATTGGTACAC");
    trie->addQuery("ACGGTGCGGCTTATTGGTACACATTGGTACACATTGGTACACATTGGTAC");
    trie->addQuery("ATAACCCATATTATTGGTACACATTGGTACACATTGGTACACATTGGTAC");
    trie->addQuery("CAAACCCCTTTTATTGGTACACATTGGTACACATTGGTACACATTGGTAC");
	trie->addQuery("ACGGTGCGGCTTATTGGTACACATTGGTACACACTGGTACACATTGGTAC");
	
	// Search for subject
    string subject = "ACGGTGCGGCTTATTGGTACACATTGGTACACACTGGTACACATTGGTAC";
	std::vector<Trie::map> solutions = trie->searchTrieStack(subject, 1);
    
	int subjectIndex = 0;
	for (auto sol : solutions) {
		int size = 0;
		for (auto n : sol.mismatches)
			size += n;
		cout << "Found " << subject << " at index: " << sol.index << " with: " << size << " mismatches." << endl;
		cout << "Mismatches at locations: ";
		if (size != 0) {
			for (int i = 0; i < 50; i++) {
				if (sol.mismatches.at(i) != 0) {
					cout << i + subjectIndex << " ";
				}
			}
			cout << endl;
		}
	}

    // Free memory because we are good people
    delete trie;

    ////input from file testing and tree building
    //int* listSize = new int[5] {100,1000,10000,50000,100000};
    //std::string fileName = "dengV2_";
    //int listIndex = 0;

    ////create new tree
    //Trie* listTest = new Trie;

    ////prefix of name of the test files
    //std::string tmpFileName = fileName;

    ////append the length of the list to the name of the file
    //tmpFileName += to_string(listSize[listIndex]);

    ////add mime type because cxx hates you
    //tmpFileName += ".txt";

    ////open the file
    //ifstream file(tmpFileName);

    ////debug
    ////ofstream wfile("dengV2_index11.txt");

    ////temp string to store references
    //std::string* strTemp = new std::string[5];

    //if (file.is_open()) {

    //    printf("\nfile was opened\n");

    //    int count = 0;

    //    for (string line; getline(file, line);) {

    //        //if(count %10 == 0)
    //        //   printf("Str count %d : %s\n",(int) line.length(), line.c_str());
    //        listTest->addQuery(line);

    //        if(count<5){

    //            strTemp[count++] = line;
    //        }

    //    }
    //}else {
    //    printf("failed to open file");
    //    return 0;
    //}

    ////display tree info
    //printf("\n%s %6d %s %6d %s\n","Trie was created with list of size ", listSize[listIndex],
    //       " the size of the trie is ", listTest->getSize(),"# of nodes");

    ////test to see if known strings are in tree
    //for(int j = 0; j < 5; j++) {

    //    listTest->searchTrieRecursively(strTemp[j], 1);

    //    if(listTest->getMismatch() != 99999999) {
    //        printf("%s %s %s %d %s %d\n", "found the word", strTemp[j].c_str(),
    //               "with #", listTest->getMismatch(), " mismatches at index ",
    //               listTest->getIndex());
    //    }
    //    }
  

    //delete listTest;
    //file.close();



    // Success
    return 0;
}
