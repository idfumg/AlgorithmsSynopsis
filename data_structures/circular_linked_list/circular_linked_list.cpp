#include <iostream>

struct Node {
    int data;
    Node* next;
};

Node* create_node(int data) {
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

void push(Node** head, int data) {
    Node* ptr1 = create_node(data);
    ptr1->next = *head;
    Node* temp = *head;
    
    if (*head) {
        while (temp->next != *head)
            temp = temp->next;
        temp->next = ptr1;
    }
    else {
        ptr1->next = ptr1;
    }
    
    *head = ptr1;
}

void print_list(Node* head) {
    if (not head)
        return;
    
    Node* current = head;
    
    do {
        std::cout << current->data << " ";
        current = current->next;
    } while (current != head);
    
    std::cout << std::endl;
}

int main() {
    Node* head = nullptr;
    push(&head, 12);
    push(&head, 123);
    push(&head, 124);
    push(&head, 43);
    push(&head, 77);

    print_list(head);
}
