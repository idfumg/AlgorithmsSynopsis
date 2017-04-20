#include <iostream>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* insert_recursive(Node* node, int key);

Node* new_node(int item) {
    Node* node = new Node;
    node->data = item;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

Node* new_node_for_test() {
    Node* node = new_node(10);

    insert_recursive(node, 60);
    insert_recursive(node, 70);
    insert_recursive(node, 50);
    insert_recursive(node, 30);
    insert_recursive(node, 40);
    insert_recursive(node, 2);
    insert_recursive(node, -4);
    insert_recursive(node, 3);
}

Node* insert_recursive(Node* node, int key) {
    if (not node)
        return new_node(key);

    if (node->data < key) {
        if (not node->right)
            node->right = insert_recursive(node->right, key);
        else
            insert_recursive(node->right, key);
    }
    else if (node->data > key) {
        if (not node->left)
            node->left = insert_recursive(node->left, key);
        else
            insert_recursive(node->left, key);
    }
    return node;
}

void print_inorder(Node* root) {
    if (not root)
        return;

    print_inorder(root->left);
    std::cout << root->data << " ";
    print_inorder(root->right);
}

void print_inorder_iterative(Node* root) {
    std::stack<Node*> stack;
    Node* current = root;

    while (current or not stack.empty()) {
        if (current) {
            stack.push(current);
            current = current->left;
        }
        else {
            current = stack.top(); stack.pop();
            std::cout << current->data << " ";
            current = current->right;
        }
    }
}

Node* min_value_node(Node* node) {
    Node* current = node;
    while (current->left)
        current = current->left;
    return current;
}

Node* max_value_node(Node* node) {
    Node* current = node;
    while (current->right)
        current = current->right;
    return current;
}

bool is_bst_recursive(Node* root) {
    static Node* prev = nullptr;

    if (not root)
        return true;

    if (not is_bst_recursive(root->left))
        return false;

    if (prev and prev->data >= root->data)
        return false;

    prev = root;

    return is_bst_recursive(root->right);
}

bool is_bst_iterative(Node* root) {
    std::stack<Node*> stack;
    Node* current = root;

    while (current or not stack.empty()) {
        if (current) {
            stack.push(current);
            current = current->left;
        }
        else {
            current = stack.top(); stack.pop();
            if (current->data >= stack.top()->data)
                return false;
            current = current->right;
        }
    }
}

void broke_bst(Node* root) {
    Node* current = root;
    while (current->left)
        current = current->left;
    current->left = new_node(10000);
}

Node* delete_node(Node* root, int key) {
    if (not root) {
        return root;
    }
    else if (root->data < key) {
        root->right = delete_node(root->right, key);
    }
    else if (root->data > key) {
        root->left = delete_node(root->left, key);
    }
    else if (root->data == key) {
        if (not root->left) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else if (not root->right) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else {
            Node* temp = min_value_node(root->right);
            root->data = temp->data;
            root->right = delete_node(root->right, temp->data);
        }
    }
    return root;
}

int main() {
    {
        Node* root = new_node_for_test();

        std::cout << "print_inorder = ";
        print_inorder(root);
        std::cout << std::endl;

        std::cout << "print_inorder_iterative = ";
        print_inorder_iterative(root);
        std::cout << std::endl;

        std::cout << "min value = " << min_value_node(root)->data << std::endl;
        std::cout << "max value = " << max_value_node(root)->data << std::endl;
        std::cout << "is_bst_recursive = " << is_bst_recursive(root) << std::endl;
    }

    {
        Node* root = new_node_for_test();
        broke_bst(root);
        std::cout << "print_inorder_iterative = ";
        print_inorder_iterative(root);
        std::cout << std::endl;
        std::cout << "is_bst_iterative = " << is_bst_iterative(root) << std::endl;
    }

    {
        Node* root = new_node_for_test();
        delete_node(root, 10);
        std::cout << "delete 10 and print_inorder_iterative = ";
        print_inorder_iterative(root);
        std::cout << std::endl;
    }
}
