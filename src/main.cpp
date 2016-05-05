#include <iostream>
#include <fstream>
#include <string>
#include "trie.h"

int runTest(int toleranceLimit) {
    
    // Declaration of Trie
    Trie* trie = new Trie;
    
    // File IO for Genome and Reads
        // Genome
	std::ifstream subjectFile("Genome.txt");
	std::string subject;
        // Because fasta files start with header, read that line to skip it
	std::getline(subjectFile, subject);
        // Then get the actual line we want
        std::getline(subjectFile, subject);

	// Reads
	std::ifstream file("Reads.txt");
	std::string str;
        //Because fasta files start with header, read that line to skip it
        std::getline(file, str);

	// Output file
	std::ofstream out("output.txt");
        
    // Number of Reads per Trie
    int sizeTrie = 1000;
    // Testing purposes
    int segment = 1;
    int reads = 1;
    // While there are more reads to search
    while (std::getline(file, str))
    {
        // Because reads all have headers, skip any line that starts with >
        if (str[0] == '>') {
            continue;
        }
        if(reads < sizeTrie){ 
            // Add to trie
            trie->addQuery(str);
            reads++;
        }
        else {
            // Break the subject into 50 mers and search for each
            for (int i = 0; i <= subject.size() - 50; i++) {
		std::string subjectSnip = subject.substr(i, i + 50);
		//Search the trie for the subject snippet
		std::vector<Trie::map> solutions = trie->searchTrieStack(subjectSnip, toleranceLimit);
                //printToFile(solutions, out, i);
            }
            // After we did all searches, we delete the trie to make room for
            // the next trie
            trie->~Trie();
            trie = new Trie;
            reads = 0;
            
            //reads.clear();
            if (segment % 100 == 0) {
                std::cout << "Finished search segment of " << sizeTrie << " reads. " << segment << " number of segments finished. " << std::endl;
            }
            segment++;
            // Even though we searched, we still took in a read from the file 
            //so we must add that to the new trie and reads 
            trie->addQuery(str);
            reads++;
        }
    }
    return 0;
}
void printToFile(std::vector<Trie::map> solutions, std::ofstream out, int i) {
    // For each match found
    for (Trie::map sol : solutions) {
            // Count total number of mismatches
            int size = 0;
            for (int n : sol.mismatches) {
                   size += n;
            }
            // Print to file solution
            out << "Found subject from index " << i << " to " << i + 50 << std::endl;
            out << "The read matched, index number " << sol.index << " with: " << size << " mismatches." << std::endl;
            // If there were mismatches, print out their locations
            if (size != 0) {
                   out << "Mismatches at locations: < ";
                   for (int j = 0; j < 50; j++) {
                           if (sol.mismatches.at(j) != 0) {
                                out << j + i << " ";
                            }
                    }
                    out << "> " << std::endl;
            }
    }
}
int main()
{
    runTest(1);
    // Success
    return 0;
}