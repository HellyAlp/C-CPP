#include "RBTree.h"
#include <stdlib.h>
#define ACTION_SUCCEEDED 1
#define ACTION_NOT_SUCCEEDED 0

void fixDeletion(RBTree *tree, Color nodeColor, int isRight, Node* nodeParent);
/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{

    RBTree *rbTree = malloc(sizeof(RBTree));
    if(rbTree == NULL)
    {
        return NULL;
    }
    rbTree->root = NULL;
    rbTree->size = 0;
    rbTree->compFunc = compFunc;
    rbTree->freeFunc = freeFunc;
    return rbTree;
}

/**
 * method that gets the smallest node in the tree
 * @param root the root of the tree
 * @return  the smallest node
 */
Node* getMin(Node * root)
{
    Node * currentNode = root;
    while (currentNode->left != NULL)
    {
        currentNode = currentNode->left;
    }
    return currentNode;
}

/**
 * a method that gets a successor of a given node
 * @param tree tha given tree
 * @param node the wanted node
 * @return A successor node
 */
Node* getSuccessor(const RBTree *tree, Node* node)
{
    Node* successor = NULL;
    if(node->right != NULL)
    {
        return getMin(node->right);
    }
    Node* currentNode = tree->root;
    while (currentNode != NULL)
    {
        if(tree->compFunc(node->data, currentNode->data) < 0)
        {
            successor = currentNode;
            currentNode = currentNode->left;
        }
        else if (tree->compFunc(node->data, currentNode->data) > 0)
        {
            currentNode = currentNode->right;
        }
        else
        {
            break;
        }
    }
    return successor;
}

/**
 * A method that get node containing given data
 * @param tree
 * @param data
 * @return the node
 */
Node* getNode(const RBTree *tree, const void *data)
{
    Node *currentNode = tree->root;
    while(currentNode != NULL)
    {
        int compResult = tree->compFunc(data, currentNode->data);
        if (compResult == 0)
        {
            return currentNode;
        }
        else if (compResult < 0)
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
    }
    return NULL;
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
    if(data == NULL || tree == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    Node* getNodeResult = getNode(tree, data);
    if (getNodeResult == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    else
    {
        return ACTION_SUCCEEDED;
    }
}

/**
 * A method that frees a single node
 * @param rbTree
 * @param node
 */
void freeNode(const RBTree *rbTree, Node* node)
{
    if(node != NULL)
    {
        rbTree->freeFunc(node->data);
        free(node);
        node = NULL;
    }

}

/**
 * A helper method to free the allocated memory: frees all nodes in the tree
 * @param rbTree the tree
 * @param node  the node to free
 */
void freeAllNodes(const RBTree *rbTree, Node* node)
{
    if(node != NULL)
    {
        freeAllNodes(rbTree, node->left);
        freeAllNodes(rbTree, node->right);
        freeNode(rbTree, node);
    }
}



/**
 * rotation that rotate left.
 */
void leftRotation(RBTree *tree,  Node* node)
{
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if(node->right != NULL)
    {
        node->right->parent = node;
    }
    rightChild->parent = node->parent;
    if(node->parent == NULL)
    {
        tree->root = rightChild;
    }
    else if( node == node->parent->left)
    {
        node->parent->left = rightChild;
    }
    else
    {
        node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
    node->color = RED;
    node->parent->color = BLACK;



}

/**
 * rotation that rotates right.
 */
void rightRotation(RBTree* tree, Node* node)
{
    Node* leftChild = node->left;
    node->left = leftChild->right;
    if(node->left != NULL)
    {
        node->left->parent = node;
    }
    leftChild->parent = node->parent;
    if(node->parent == NULL)
    {
        tree->root = leftChild;
    }
    else if( node == node->parent->left)
    {
        node->parent->left = leftChild;
    }
    else
    {
        node->parent->right = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
    node->color = RED;
    node->parent->color = BLACK;
}


/**
 * A method that fixes the red black tree after insertion.
* @param tree the given tree
* @param node the node that was added
*/
void fixInsert( RBTree *tree, Node *node)
{
    Node* uncle ;
    while (node != tree->root)
    {
        if(node->parent->color == BLACK)
        {
            return;
        }
        if(node->parent->parent->left == node->parent)
        {
            uncle = node->parent->parent->right;
        }
        else
        {
            uncle = node->parent->parent->left;
        }
        if(uncle != NULL && uncle->color == RED)
        {
            uncle->color = BLACK;
            node->parent->color = BLACK;
            node->parent->parent->color = RED;
            node = node->parent->parent;
        }
        else
        {
             if (node->parent->parent->left == node->parent && node->parent->right == node)
             {
                leftRotation(tree, node->parent);
                node = node->left;
             }
             else if(node->parent->parent->right == node->parent && node->parent->left == node)
             {
                rightRotation(tree, node->parent);
                node = node->right;
             }
             if(node == node->parent->left)
             {
                 rightRotation(tree, node->parent->parent);
             }
             else
             {
                 leftRotation(tree, node->parent->parent);
             }
        }
    }
    tree->root->color = BLACK;
}
/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree( RBTree *tree, void *data)
{
    Node *newNode = NULL;
    newNode = (Node*)malloc(sizeof(Node));
    if(data == NULL || tree == NULL || newNode == NULL)
    {
        if(data == NULL || tree == NULL)
        {
            free(newNode);
        }
        return ACTION_NOT_SUCCEEDED;
    }
    if(RBTreeContains(tree, data))
    {
        free(newNode);
        return ACTION_NOT_SUCCEEDED;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    if(tree->root == NULL)
    {
        tree->root = newNode;
        newNode->color = BLACK;
        tree->size++;
        return ACTION_SUCCEEDED;
    }
    Node *currentNode = tree->root;
    while(currentNode != NULL)
    {
        int compResult = tree->compFunc(newNode->data, currentNode->data);
        if(compResult == 0)
        {
            return ACTION_NOT_SUCCEEDED;
        }
        else if(compResult < 0)
        {
            newNode->parent = currentNode;
            currentNode = currentNode->left;
        }
        else
        {
            newNode->parent = currentNode;
            currentNode = currentNode->right;
        }
    }
    int compRes =  tree->compFunc(newNode->data, newNode->parent->data);
    if( compRes < 0)
    {
        newNode->parent->left = newNode;
    }
    else
    {
        newNode->parent->right = newNode;
    }
    newNode->color = RED;
    fixInsert(tree, newNode);
    tree->size++;
    return ACTION_SUCCEEDED;
}


/**
 * A method that checks if a given node is a right son or not
 * @param node
 * @return  0 if no, 1 if yes
 */
int isRightSon(Node* node)
{
    if(node == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    if(node->parent == NULL)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    else
    {
        if(node->parent->left == node)
        {
            return ACTION_NOT_SUCCEEDED;
        }
        else
        {
            return ACTION_SUCCEEDED;
        }
    }
}

/**
 * A method that returns the sibling of a given node.
 * @param node
 * @return node's sibling
 */
Node* getSibling(Node* node)
{
    if(node == NULL || node->parent == NULL )
    {
        return NULL;
    }
    else
    {
        if(node->parent->left == node)
        {
            return node->parent->right;
        }
        return node->parent->left;
    }
}

/**
 * A method that check is a given node color is black
 * @param node
 * @return  0 if no, 1 if yes
 */
int isBlack(Node* node)
{
    if(node == NULL || node->color == BLACK)
    {
        return ACTION_SUCCEEDED;
    }
    return ACTION_NOT_SUCCEEDED;

}

/**
 * A method that handles the DB case
 * @param tree
 * @param parentNode
 * @param childNode
 */
void doubleBlackCase(RBTree *tree, Node* parentNode, Node* childNode)
{
    if(tree->root == childNode)
    {
        tree->root = NULL;
        return;
    }
    Node* sibling = getSibling(childNode);
    if(childNode == NULL)
    {
        if(parentNode->left != NULL)
        {
            sibling = parentNode->left;
        }
        else
        {
            sibling = parentNode->right;
        }
    }
    //case 3.2: if S is Black and its 2 sons is black
    if(sibling != NULL && sibling->color == BLACK && isBlack(sibling->right) && isBlack(sibling->left))
    {
        if(parentNode->color == RED)
        {
            parentNode->color = BLACK;
            sibling->color = RED;
            return;
        }
        else
        {
            sibling->color = RED;
            return;
        }
        //case 3.3: S is red
    }
    else if (childNode != NULL && sibling != NULL && sibling->color == RED)
    {
        if(isRightSon(sibling))
        {

            leftRotation(tree, childNode->parent);
            sibling->color = BLACK;
            parentNode->color = RED;
        }
        else
        {
            rightRotation(tree, childNode->parent);
            sibling->color = BLACK;
            parentNode->color = RED;
        }
        doubleBlackCase(tree, parentNode , childNode);
        return;
    }
    //case 3.4: S black ans S-child close is black and S-child far is red
    Node* sClose = NULL;
    Node* sFar = NULL;
    if(sibling != NULL && isRightSon(sibling))
    {
        sClose = sibling->left;
        sFar = sibling->right;
    }
    else if(sibling != NULL && !isRightSon(sibling))
    {
        sFar = sibling->left;
        sClose = sibling->right;
    }
    if(isBlack(sibling) && isBlack(sClose) && sFar != NULL && sFar->color == RED)
    {
        Color temp = parentNode->color;
        Color temp2 = sibling->color;
        if(isRightSon(sibling))
        {
            leftRotation(tree, sibling->parent);
        }
        else
        {
            rightRotation(tree, sibling->parent);
        }
        sFar->color = BLACK;
        parentNode->color = temp2;
        sibling->color = temp;
    }
    else if(isBlack(sibling) &&  isBlack(sFar) && sClose != NULL && sClose->color == RED)
    {

        if(isRightSon(sClose))
        {
            leftRotation(tree, sClose->parent);
        }
        else
        {
            rightRotation(tree, sClose->parent);
        }
        sibling->color = RED;
        sClose->color = BLACK;
        doubleBlackCase(tree, sClose->parent, getSibling(sClose));
    }


}

void fixDeletion(RBTree *tree, Color nodeColor, int isRight, Node* nodeParent)
{
    Node* c;
    if(nodeColor == RED)
    {
        return;
    }
    else
    {
        if(nodeParent == NULL)
        {
            c = tree->root;
        }
        else if(isRight == ACTION_SUCCEEDED)
        {
           c = nodeParent->right;
        }
        else
        {
            c = nodeParent->left;
        }
        if(c != NULL && c->color == RED)
        {
            c->color = BLACK;
        }
        else if (c == NULL ||  c->color == BLACK)
        {
            doubleBlackCase(tree, nodeParent, c);
        }
    }

}


/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    if(data == NULL ||  tree == NULL || RBTreeContains(tree, data) == ACTION_NOT_SUCCEEDED)
    {
        return ACTION_NOT_SUCCEEDED;
    }
    Node* toDelete = getNode(tree, data);
    Node* node;
    //case 2.3: node has 2 childes
    if(toDelete->right != NULL && toDelete->left != NULL)
    {
        node = getSuccessor(tree, toDelete);
        toDelete->data = node->data;
        node->data = data;
    }
    else
    {
        node = toDelete;
    }
    toDelete = NULL;
    Node* nodeParent = node->parent;
    int isRight = isRightSon(node);
    //case 2.1: node has no childes
    if(node->left == NULL && node->right == NULL)
    {
        if(node->parent == NULL)
        {
            tree->root = NULL;
            tree->size--;
            tree->freeFunc(node->data);
            free(node);
            node = NULL;
            return ACTION_SUCCEEDED;
        }
        if(node->parent->right == node)
        {
            nodeParent->right = NULL;
        }
        else
        {
            nodeParent->left = NULL;
        }
    }
    //case 2.2: node has 1 child
    else if(node->left != NULL && node->right == NULL)
    {
        if(node == tree->root)
        {
            tree->root = node->left;
            tree->root->parent = NULL;
        }
        else if(isRight)
        {
            nodeParent->right = node->left;
            node->left->parent = nodeParent;
        }
        else
        {
            nodeParent->left = node->left;
            node->left->parent = nodeParent;
        }
    }
    else if (node->left == NULL && node->right != NULL)
    {
        if(node == tree->root)
        {
            tree->root = node->right;
            tree->root->parent = NULL;
        }
        else if(isRight)
        {
            nodeParent->right = node->right;
            node->right->parent = nodeParent;
        }
        else
        {
            nodeParent->left = node->right;
            node->right->parent = nodeParent;
        }
    }
    tree->size--;
    Color nodeColor = node->color;
    tree->freeFunc(node->data);
    free(node);
    node = NULL;
    fixDeletion(tree, nodeColor, isRight, nodeParent);
    return ACTION_SUCCEEDED;
}


/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if(tree == NULL )
    {
        return ACTION_NOT_SUCCEEDED;
    }
    if(tree->root == NULL)
    {
        return ACTION_SUCCEEDED;
    }

    Node* node = getMin(tree->root);
    while(node != NULL)
    {
        if(func(node->data, args) == ACTION_NOT_SUCCEEDED )
        {
            return ACTION_NOT_SUCCEEDED;
        }
        node = getSuccessor(tree, node);
    }
    return ACTION_SUCCEEDED;
}


/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    RBTree *rbTree = *tree;
    freeAllNodes(rbTree, rbTree->root);
    free(rbTree);
    rbTree = NULL;
}
