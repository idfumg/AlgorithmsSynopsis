/*
  In this file a linked list properties and problems in simple c/c++ language only.
 */

#include <iostream>

struct Node {
    int data;
    Node* next;
};

void delete_list(Node** head);
void print_list(Node* head);

Node* new_node(int data) {
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

void push(Node** head, int data) {
    Node* node = new_node(data);
    node->next = *head;
    *head = node;
}

Node* create_new_list() {
    Node* head = nullptr;
    push(&head, 1);
    push(&head, 1);
    push(&head, 2);
    push(&head, 3);
    push(&head, 4);
    push(&head, 2);
    push(&head, 5);
    push(&head, 6);
    push(&head, 6);
    push(&head, 6);
    return head;
}

template <typename Fn, typename ...Args>
void test_function_(Fn fn, const std::string& action, const std::string& params, Args... args) {
    Node* head = create_new_list();
    print_list(head);
    std::cout << " -> " + action + "(" + params + ")" + " -> ";

    fn(&head, std::forward<Args>(args)...);

    print_list(head);
    std::cout << std::endl;
    delete_list(&head);
}

#define test_function(fn_arg) test_function_(fn_arg, #fn_arg, "")
#define test_function_with_params(fn_arg, ...) test_function_(fn_arg, #fn_arg, #__VA_ARGS__, __VA_ARGS__)

/*******************************************************************************
 * Functions.
 *******************************************************************************/

void print_list(Node* head) {
    if (not head) {
        std::cout << "(nullptr)";
        return;
    }

    Node* current = head;

    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
}

void delete_list(Node** head) {
    Node* current = *head;
    Node* prev = nullptr;

    while (current) {
        prev = current;
        current = current->next;
        delete prev;
    }

    *head = nullptr;
}

void append(Node** head, int new_data) {
    Node* current = *head;

    while (current->next)
        current = current->next;

    current->next = new_node(new_data);
}

void delete_node_with_key(Node** head, int key) {
    Node* current = *head;
    Node* prev = nullptr;

    while (current and current->data != key) {
        prev = current;
        current = current->next;
    }

    if (current) {
        if (prev)
            prev->next = current->next;
        delete current;
    }
}

void delete_node_in_position(Node** head, int pos) {
    Node* current = *head;
    Node* prev = nullptr;
    int count = 0;

    while (current and count != pos) {
        prev = current;
        current = current->next;
        count++;
    }

    if (current) {
        if (prev)
            prev->next = current->next;
        delete current;
    }
}

void find_list_length_iterative(Node** head) {
    Node* current = *head;
    int length = 0;

    while (current) {
        current = current->next;
        length++;
    }

    std::cout << "[" << length << "] -> ";
}

int find_list_length_recursive_(Node** head) {
    if (not *head)
        return 0;

    return 1 + find_list_length_recursive_(&(*head)->next);
}

void find_list_length_recursive(Node** head) {
    std::cout << "[" << find_list_length_recursive_(head) << "] -> ";
}

void swap_pair(Node** head, int first_data, int second_data) {
    Node* prev_first = nullptr;
    Node* first = *head;

    while (first and first->data != first_data) {
        prev_first = first;
        first = first->next;
    }

    if (not first)
        return;

    Node* prev_second = nullptr;
    Node* second = *head;

    while (second and second->data != second_data) {
        prev_second = second;
        second = second->next;
    }

    if (not second)
        return;

    if (prev_first)
        prev_first->next = second;

    if (prev_second)
        prev_second->next = first;

    Node* second_next = second->next;

    second->next = first->next;
    first->next = second_next;
}

Node* get_nth_list_node(Node** head, int pos) {
    Node* current = *head;
    int count = 0;

    while (current and count != pos) {
        current = current->next;
        count++;
    }

    return current;
}

Node* find_list_node_by_key(Node** head, int key) {
    Node* current = *head;

    while (current and current->data != key)
        current = current->next;

    return current;
}

void count_key_count(Node** head, int key) {
    Node* current = *head;
    int count = 0;

    while (current) {
        if (current->data == key)
            count++;
        current = current->next;
    }

    std::cout << "[" << count << " times] -> ";
}

void print_middle_node(Node** head) {
    Node* slow = *head;
    Node* fast = *head;

    while (slow and fast and fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    std::cout << "[" << slow->data << "] -> ";
}

void reverse_list(Node** head) {
    Node* prev = nullptr;
    Node* current = *head;
    Node* next = nullptr;

    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head = prev;
}

void remove_loop(Node** head, Node* loop_node);

Node* enable_loop(Node** head) {
    Node* current = *head;
    while (current->next)
        current = current->next;
    current->next = *head;
    return current;
}

void disable_loop(Node** head, Node* loop_node) {
    loop_node->next = nullptr;
}

void find_duplicates(Node** head) {
    Node* current = *head;

    while (current) {
        Node* node = current->next;

        while (node) {
            if (node->data == current->data)
                std::cout << " found " << node->data << " -> ";
            node = node->next;
        }

        current = current->next;
    }
}

void remove_duplicates(Node** head) {
    Node* current = *head;

    while (current) {
        Node* node = current->next;
        Node* prev = current;

        while (node) {
            if (node->data == current->data) {
                prev->next = node->next;
                Node* temp = node;
                node = node->next;
                delete temp;
            }
            else {
                prev = node;
                node = node->next;
            }
        }

        current = current->next;
    }
}

void remove_loop(Node** head, Node* loop_node) {
    Node* ptr = loop_node->next;

    while (ptr->next != loop_node)
        ptr = ptr->next;

    ptr->next = nullptr;
}

void detect_and_remove_loop(Node** head) {
    Node* slow = *head;
    Node* fast = *head;

    enable_loop(head);

    while (slow and fast and fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            remove_loop(head, slow);
            return;
        }
    }
}

int main() {
    test_function(delete_list);
    test_function_with_params(append, 5);
    test_function_with_params(delete_node_with_key, 2);
    test_function_with_params(delete_node_in_position, 1);
    test_function(find_list_length_iterative);
    test_function(find_list_length_recursive);
    test_function_with_params(swap_pair, 5, 1);
    test_function_with_params(get_nth_list_node, 3);
    test_function_with_params(find_list_node_by_key, 3);
    test_function_with_params(count_key_count, 2);
    test_function(print_middle_node);
    test_function(reverse_list);
    test_function(find_duplicates);
    test_function(remove_duplicates);
    test_function(detect_and_remove_loop);

    return 0;
}
