//
// Created by milutin on 8/31/25.
//

#ifndef TREENODE_H
#define TREENODE_H



class TreeNode{
public:
    TreeNode *levi,*desni;
    char value;
    int frekvencija;
    TreeNode(char value,int frekvencija);
    TreeNode(TreeNode *left,TreeNode *right);

    bool operator<(const TreeNode &rhs) const;

    bool operator>(const TreeNode &rhs) const;

    bool operator<=(const TreeNode &rhs) const;

    bool operator>=(const TreeNode &rhs) const;

    virtual ~TreeNode();

    bool isLeaf();

};



#endif //TREENODE_H
