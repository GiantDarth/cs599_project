//
// Created by chaos on 4/14/2016.
//

#ifndef CS599_PROJECT_TRIE_H
#define CS599_PROJECT_TRIE_H

#include <string>
#include "node.h"


/* TODO: exhaustive search
 * Return the query and number of mismatches
 */
class Trie
{
    public:
        // Constructor/Deconstructor
        Trie();
        ~Trie();

        // Prototypes
        void addQuery(std::string query);
        int searchTrie(std::string subject);
        void searchTrieRecursively(Node* current, std::string subject, int limit, int currentMismatch = 0, int subjectIndex = 0);
        bool strictSearch(std::string subject);
        void exhaustiveSearch(std::string subject, unsigned short limit);
        int getMismatch() { return this->bestMismatch; };
        int getIndex() { return this->bestIndex; };
        Node* getRoot() { return this->root; };
        int getSize() {return this->numberOfNodes;}
        
    private:
        Node* root;
        int bestIndex;
        int bestMismatch;
        int numberOfQuerys;
        int numberOfNodes;      //keep track
};



#endif //CS599_PROJECT_TRIE_H
