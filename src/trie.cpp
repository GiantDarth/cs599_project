//
// Created by chaos on 4/14/2016.
//

#include "trie.h"
#include <iostream>
#include <queue>
#include <bitset>
#include <stack>

// Constructor --> create an empty root
Trie::Trie()
{
    root = new Node();
    numberOfQuerys = 0;
    bestMismatch = 99999999;
    bestIndex = -1;
    numberOfNodes =0;
}

// Deconstructor --> free memory
Trie::~Trie() {}

/* Input: std::string

 * Function: adds a query to the trie
 * Output: void
 */
void Trie::addQuery(std::string query)
{
    // Start at root
    Node* current = root;

    // Go until end of query
    for ( unsigned int i = 0; i < query.length(); i++ )
    {
        // Check if there's already a child (base) to go to
        Node* child = current->findChild(query[i]);

        // Child was found, go to it
        if (child != nullptr)
        {
            current = child;
        }
        // No child was found, create one
        else
        {
            current = current->appendChild(query[i]);
            numberOfNodes++;
        }
    }
    
}

void Trie::exhaustiveSearch(std::string subject, unsigned short limit)
{
    // A queue of tuples such that (current_path, current_mismatch_score, current_node_ptr)
    std::stack<std::tuple<std::string, unsigned short, Node*>> stack;
    // Bit array for marking off resultant indices under subject
    std::bitset<subject.length()> marked_array;
    const unsigned short QUERY_LEN = 50;

    Node *current, *next;
    for(unsigned int l = 0; l < limit; l++)
    {
        for(unsigned int s = 0; s < subject.length() - QUERY_LEN + 1; s++)
        {
            // If index is already previous marked, skip it.
            if(marked_array[s / 8] & (0x1 << (s % 8)))
            {
                continue;
            }

            current = root;
            unsigned short mismatch = 0;
            unsigned int q = 0;
            while(current != nullptr || !stack.empty())
            {
                if(current == nullptr && !stack.empty())
                {

                }
                // A mismatch has occurred.
                if((next = current->findChild(subject[s + q])) == nullptr)
                {
                    for(char base : "ACGT")
                    {
                        if(subject[s + q] == base)
                        {
                            continue;
                        }

                        if(stack.empty())
                        {
                            stack.push(std::make_tuple(subject.substr(s, s + q), 1, next));
                        }
                        else
                        {
                            stack.push(std::make_tuple(std::get<0>(stack.top()) + base,
                                                       std::get<1>(stack.top()) + 1, next));
                        }
                    }
                    stack.push(std::make_tuple());
                }
            }
//            for (unsigned int q = 0; q < QUERY_LEN; q++)
//            {
//                for (char base : BASES)
//                {
//                    if((current = current->findChild(subject[s + q])) == nullptr)
//                    {
//                        if(++mismatch <= l)
//                        {
//                            stack.push(std::make_tuple(path, ++mismatch, current));
//                        }
//                    }
//                }
//            }

            if(mismatch <= l)
            {
                marked_array[s / 8] |= 0x1 << (s % 8);
            }
        }
    }
}

///* Input: std::string
//  (subject)
// * Function: attempts to fuzzy match subject to all queries up to X mismatches
// * Output: best query and score
// */
//int Trie::searchTrie(std::string subject)
//{
//    // Start at root
//    Node* current = root;
//
//    // Go until end of trie
//    while ( current != NULL )
//    {
//
//        // Go until end of subject length
//        for ( unsigned int i = 0; i < subject.length(); i++ )
//        {
//
//            // Try to find child with current base of subject
//            Node* tmp = current->findChild(subject[i]);
//
//            // Child is dead end for matching
//            if ( tmp->getIndexMarker() == -1 ) { return false; };
//
//            // Move to child
//            current = tmp;
//        }
//
//        // Found everything up until end of word --> success
//        if ( current->getIndexMarker() != 0 ) { return current->getIndexMarker(); }
//
//        // Dead end
//        else { return -1; }
//    }
//
//    // Failure
//    return false;
//}
///*
// * This function will recursively search the prefix tree for the given subject,
// * Inputs:
// *      Node* current       : A pointer to the root Node
// *      std::st *     subject      : The subject we are searching for
// *      int limit           : The maximum allowable number of mismatches (inclusive)
// *      int* bestMatch      : A pointer to our least amount of mismatches for our
// *                            best query. Initialized to our limit + 1
// *      int* bestIndex      : A pointer to the index of the best query so far.
// *                            Initialized to any value (preferably 0)
// *      int currentMismatch : Default value of the current number of mismatches.
// *      int subjectIndex    : Default value of what base in the subject we are
// *                            trying to match.
// * Outputs:
// *      Updates values of:
// *          int* bestMismatch
// *          int* bestIndex
// */
//void Trie::searchTrieRecursively(Node* current, std::string subject, int limit, int currentMismatch, int subjectIndex)
//{
//    // Check if the content of this Node matches our subject as long as current Node
//    // is not the root Node
//    // - if not then increment the mismatch counter
//    if(current->getContent() != subject[subjectIndex] && current->getIndexMarker() != -1)
//    {
//        currentMismatch++;
//    }
//
//    // Base case 1: Number of current mismatches is greater then limit or
//    // current mismatches is not better then our best
//    // - So we just end this path by returning
//    if(currentMismatch > limit || currentMismatch >= this->bestMismatch)
//    {
//        return;
//    }
//
//    // Base case 2: End of the Query
//    // If we have gotten to this point we know that this is better then our current best
//    // - So update values in the memory addresses given by pointers
//    if(current->getIndexMarker() != 0 && current->getIndexMarker() != -1)
//    {
//        this->bestMismatch = currentMismatch;
//        this->bestIndex = current->getIndexMarker();
//        return;
//    }
//
//    // If this Node is not the root, then increment the subjectIndex by one, as
//    // we are going to check the next base in the subject.
//    if(current->getIndexMarker() != -1)
//    {
//        subjectIndex++;
//    }
//
//    // Then, for each child of the current Node, call this method
//    // on the child. After, decrement the subject index
//    for(unsigned int i=0; i<(current->children().size()); i++)
//    {
//        searchTrieRecursively(current->children().at(i), subject, limit, currentMismatch, subjectIndex);
//    }
//    return;
//}
//// a working search function to make sure our trie is being built correctly
////this is neede for debugging
//bool Trie::strictSearch(std::string subject) {
//
//    return false;
//}