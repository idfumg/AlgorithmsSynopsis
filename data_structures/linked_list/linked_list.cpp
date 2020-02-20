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

Node* create_new_node(int data) {
    Node* node = new Node;
    node->data = data;
    node->next = nullptr;
    return node;
}

void push(Node** head, int data) {
    Node* node = create_new_node(data);
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
    for (Node* node = head; node != nullptr; node = node->next)
        std::cout << node->data << " ";
}

void delete_list(Node** head) {
    if (not *head)
        return;

    Node* current = *head;
    Node* next = nullptr;

    while (current) {
        next = current->next;
        delete current;
        current = next;
    }

    *head = nullptr;
}

void append(Node** head, int new_data) {
    Node* new_node = create_new_node(new_data);

    if (not *head) {
        *head = new_node;
        return;
    }

    Node* current = *head;
    while (current->next)
        current = current->next;

    current->next = new_node;
}

void delete_node_with_key(Node** head, int key) {
    if (head == nullptr)
        return;

    Node* current = *head;
    Node* prev = nullptr;

    while (current) {
        if (current->data == key) {
            prev->next = current->next;
            delete current;
            break;
        }
        else {
            prev = current;
            current = current->next;
        }
    }
}

void delete_node_in_position(Node** head, int pos) {
    if (head == nullptr)
        return;

    Node* current = *head;
    Node* prev = nullptr;
    int count = 0;

    while (current and count != pos) {
        prev = current;
        current = current->next;
        count++;
    }

    prev->next = current->next;
    delete current;
}

void find_list_length_iterative(Node** head) {
    int length = 0;

    for (Node* current = *head; current; current = current->next)
        length++;

    std::cout << "[" << length << "] -> ";
}

int find_list_length_recursive_(Node** head) {
    static int length = 0;

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

    if (not first) {
        std::cout << first_data << " not found! -> ";
        return;
    }

    Node* prev_second = nullptr;
    Node* second = *head;

    while (second and second->data != second_data) {
        prev_second = second;
        second = second->next;
    }

    if (not second) {
        std::cout << second_data << " not found! -> ";
        return;
    }

    Node* second_next = second->next;

    if (not prev_first)
        *head = second;
    else
        prev_first->next = second;

    second->next = first->next;

    prev_second->next = first;
    first->next = second_next;
}

Node* get_nth_list_node(Node** head, int pos) {
    if (not *head)
        return nullptr;

    Node* current = *head;
    int count = 0;

    while (current and count != pos) {
        current = current->next;
        count++;
    }

    if (current) {
        std::cout << "[" << current->data << "] -> ";
        return current;
    }

    return nullptr;
}

Node* find_list_node_by_key(Node** head, int key) {
    if (not *head)
        return nullptr;

    Node* current = *head;
    while (current and current->data != key)
        current = current->next;

    if (current)
        std::cout << "[" << current << "] -> ";

    return current;
}

void count_key_count(Node** head, int key) {
    if (not *head)
        return;

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
    if (not *head)
        return;

    Node* fast = *head;
    Node* slow = *head;

    while (fast and fast->next) {
        fast = fast->next->next;
        slow = slow->next;
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

void detect_loop(Node** head) {
    Node* fast = *head;
    Node* slow = *head;

    Node* loop_node = enable_loop(head);

    while (slow and fast and fast->next) {
        fast = fast->next->next;
        slow = slow->next;

        if (fast == slow) {
            std::cout << "[loop in element " << slow->data
                      << " to element " << slow->next->data << "] -> ";
            remove_loop(head, slow);
            break;
        }
    }
}

void find_duplicates(Node** head) {
    /*best dicision - using hash*/
    Node* current = *head;
    while (current) {
        Node* node = current->next;
        while (node) {
            if (node->data == current->data)
                std::cout << "found " << node->data << "! -> ";

            node = node->next;
        }

        current = current->next;
    }
}

void remove_duplicates(Node** head) {
    Node* current = *head;
    Node* prev = nullptr;

    while (current) {
        Node* node = current->next;
        prev = current;

        while (node) {
            if (node->data == current->data) {
                prev->next = node->next;

                Node* temp = node;
                node = node->next;
                delete temp;
            }
            else {
                node = node->next;
                prev = prev->next;
            }
        }

        current = current->next;
    }
}

void remove_loop(Node** head, Node* loop_node) {
    Node* ptr1 = loop_node;
    Node* ptr2 = loop_node;

    unsigned int k = 1;
    while (ptr2->next != ptr1) {
        ptr2 = ptr2->next;
        k++;
    }

    ptr1 = *head;
    ptr2 = *head;

    for (int i = 0; i < k; i++)
        ptr2 = ptr2->next;

    while (ptr1 != ptr2) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    ptr2 = ptr2->next;
    while (ptr2->next != ptr1)
        ptr2 = ptr2->next;
    ptr2->next = nullptr;
}

void detect_and_remove_loop(Node** head) {
    Node* slow = *head;
    Node* fast = *head;

    while (slow and fast and fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            remove_loop(head, slow);
            break;
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
    test_function(detect_loop);
    test_function(find_duplicates);
    test_function(remove_duplicates);
    test_function(detect_and_remove_loop);

    return 0;
}
