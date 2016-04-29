//
// Created by chaos on 4/14/2016.
//

#ifndef CS599_PROJECT_NODE_H
#define CS599_PROJECT_NODE_H

#include <unordered_map>

class Node
{
    public:
        // Constructor
        Node() {}

        // Copy Constructor
        Node(const Node &obj)
        {
            // Invokes the unordered_map's copy constructor, which in turn invokes the nodes' copy constructors.
            this->children = obj.children;
        }

        // Deconstructor
        ~Node()
        {
            this->children.clear();
        }

        // Add a child to the current node's vector
        Node* appendChild(char base);

        // Prototype
        Node* findChild(char base);

    private:
        // Dynamic-size dictionary that holds all children where the key is the base.
        std::unordered_map<char, Node*> children;
};


#endif //CS599_PROJECT_NODE_H
