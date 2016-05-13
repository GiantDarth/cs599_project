//
// Created by chaos on 4/14/2016.
//

#ifndef CS599_PROJECT_NODE_H
#define CS599_PROJECT_NODE_H

#include <vector>


class Node
{
    public:
        // Constructor
        Node()
        {
            content = ' ';
            index = 0;
        }

        // Deconstructor
        ~Node() { nodeChildren.clear();}

        // Return base
        char getContent() { return content; }

        // Set base
        void setContent(char base) { content = base; }

        // index if query ends at current node, 0 otherwise
        int getIndexMarker() { return index; }

        // Set index if end query, naturally 0
        void setIndexMarker(int newIndex) { index = newIndex; }

        // Add a child to the current node's vector
        void appendChild(Node* child) { nodeChildren.push_back(child); } // push_back == append

        // Prototype
        Node* findChild(char base);

        // Returns vector of children if found
        std::vector<Node*> children() { return nodeChildren; }

    private:
        // Single base
        char content;

        // index of query
        int index;
        
        // Dynamic-size array that holds all children
        std::vector<Node*> nodeChildren;
};


#endif //CS599_PROJECT_NODE_H
