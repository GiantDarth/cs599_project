//
// Created by chaos on 4/14/2016.
//

#include "node.h"
#include <stdlib.h> 

/* Input: char (base)
 * Function: checks if a certain base exists for the current node
 * Output: pointer to the node found (child)
 */
Node* Node::findChild(char base)
{
    Node* null = new Node();
    null->setIndexMarker(-1);
    
    if(nodeChildren.empty()) { return null; };
    
    // Goes through every child
    
            
    for (unsigned int i = 0; i < nodeChildren.size(); i++ )
    {
        // Current node is the child
        Node* tempChild = &nodeChildren.at(i);

        // Child matched the content
        if ( tempChild->getContent() == base )
        {
            // Return child
            return tempChild;
        }
    }
    
    return null;
}
Node::~Node() {
    if (nodeChildren.size() >= 1) {nodeChildren.clear();}
    
    
}