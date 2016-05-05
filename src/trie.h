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
        struct map {
            int index;
            std::vector<int> mismatches;
        };

        struct nodeMismatch {
            Node node;
            std::vector<int> mismatches;
        };
        void addQuery(std::string query);
        int searchTrie(std::string subject);
        std::vector<map> searchTrieStack(std::string subject, int limit);
        Node* getRoot() { return this->root; };
        int getSize() {return this->numberOfNodes;}
        void clear(Node* node);
        
    private:
        Node* root;
        int numberOfQuerys;
        int numberOfNodes;      
};



#endif //CS599_PROJECT_TRIE_H
