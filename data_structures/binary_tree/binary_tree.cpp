#include <iostream>

struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* new_node(int data) {
    Node* node = new Node;
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

void print_preorder(Node* node) {
    if (not node)
        return;

    std::cout << node->data << " ";
    print_preorder(node->left);
    print_preorder(node->right);
}

void print_inorder(Node* node) {
    if (not node)
        return;

    print_inorder(node->left);
    std::cout << node->data << " ";
    print_inorder(node->right);
}

void print_postorder(Node* node) {
    if (not node)
        return;

    print_postorder(node->left);
    print_postorder(node->right);
    std::cout << node->data << " ";
}

int height(Node* node) {
    if (not node)
        return 0;

    int lheight = height(node->left);
    int rheight = height(node->right);

    if (lheight > rheight)
        return lheight + 1;
    return rheight + 1;
}

void print_givenlevel(Node* node, int level) {
    if (not node)
        return;

    if (level == 1) {
        std::cout << node->data << " ";
        return;
    }

    if (level > 1) {
        print_givenlevel(node->left, level - 1);
        print_givenlevel(node->right, level - 1);
    }
}

void print_levelorder(Node* node) {
    for (int i = 0; i <= height(node); i++)
        print_givenlevel(node, i);
}

int search(char arr[], int strt, int end, char value) {
    for (int i = strt; i <= end; i++)
        if (arr[i] == value)
            return i;
    return -1;
}

Node* build_tree(char in[], char pre[], int inStart, int inEnd) {
    static int preIndex = 0;

    if (inStart > inEnd)
        return nullptr;

    Node* node = new_node(pre[preIndex++]);

    if (inStart == inEnd)
        return node;

    int inIndex = search(in, inStart, inEnd, node->data);

    node->left = build_tree(in, pre, inStart, inIndex - 1);
    node->right = build_tree(in, pre, inIndex + 1, inEnd);

    return node;
}

int main() {
    Node* root = new_node(1);
    root->left = new_node(2);
    root->right = new_node(3);
    root->left->left = new_node(4);
    root->left->right = new_node(5);

    std::cout << "preorder traversal of binary tree: ";
    print_preorder(root);
    std::cout << std::endl;

    std::cout << "inorder traversal of binary tree: ";
    print_inorder(root);
    std::cout << std::endl;
    
    std::cout << "postorder traversal of binary tree: ";
    print_postorder(root);
    std::cout << std::endl;

    std::cout << "levelorder traversal of binary tree: ";
    print_levelorder(root);
    std::cout << std::endl;

    char in[] = {'D', 'B', 'E', 'A', 'F', 'C'};
    char pre[] = {'A', 'B', 'D', 'E', 'C', 'F'};
    int len = sizeof(in) / sizeof(in[0]);
    Node* node = build_tree(in, pre, 0, len - 1);

    std::cout << "inorder traversal of binary tree: ";
    print_inorder(node);
    std::cout << std::endl;
}
