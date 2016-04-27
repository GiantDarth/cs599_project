//
// Created by chaos on 4/14/2016.
//

#include "node.h"

Node *Node::appendChild(char base)
{
    // If the map does not find the value.
    if(children.find(base) == children.end())
    {
        children[base] = new Node();

        return children[base];
    }

    return nullptr;
}

/* Input: char (base)
 * Function: checks if a certain base exists for the current node
 * Output: pointer to the node found (child)
 */
Node* Node::findChild(char base)
{
    if(children.find(base) == children.end())
    {
        return nullptr;
    }
    else
    {
        return children[base];
    }
}