#include <iostream>

struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* create_node(int data) {
    Node* node = new Node;
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

Node* new_node_for_test() {
    Node* node = create_node(1);
    node->left = create_node(2);
    node->right = create_node(3);
    node->left->left = create_node(4);
    node->left->right = create_node(5);
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

    return std::max(1 + height(node->left), 1 + height(node->right));
}

void print_givenlevel(Node* node, int level) {
    if (not node or level == 0)
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

int main() {
    Node* node = new_node_for_test();

    print_preorder(node);
    std::cout << std::endl;

    print_inorder(node);
    std::cout << std::endl;

    print_postorder(node);
    std::cout << std::endl;

    print_levelorder(node);
    std::cout << std::endl;
}
