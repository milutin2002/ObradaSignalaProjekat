//
// Created by milutin on 8/31/25.
//

#include "TreeNode.h"

TreeNode::~TreeNode() {
    delete levi;
    delete desni;
}

TreeNode::TreeNode(char value, int frekvencija) {
    this->value=value;
    this->frekvencija=frekvencija;
    levi=desni= nullptr;
}

TreeNode::TreeNode(TreeNode *left, TreeNode *right) {
    this->value='v';
    this->frekvencija=left->frekvencija+right->frekvencija;
    this->levi=left;
    this->desni=right;
}

bool TreeNode::operator<(const TreeNode &rhs) const {
    return frekvencija < rhs.frekvencija;
}

bool TreeNode::operator>(const TreeNode &rhs) const {
    return rhs < *this;
}

bool TreeNode::operator<=(const TreeNode &rhs) const {
    return !(rhs < *this);
}

bool TreeNode::operator>=(const TreeNode &rhs) const {
    return !(*this < rhs);
}

bool TreeNode::isLeaf() {
    return desni== nullptr && levi== nullptr;
}