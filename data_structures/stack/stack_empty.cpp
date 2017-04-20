#include <iostream>

struct Stack {
    int top;
    int capacity;
    int* arr;
};

void push(Stack* stack, int item);
int pop(Stack* stack);

Stack* create_stack(int capacity) {
    Stack* stack = new Stack;
    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = new int[capacity];
    return stack;
}

Stack* create_stack_for_test(int capacity) {
    Stack* stack = create_stack(capacity);
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);
    push(stack, 40);
    return stack;
}

bool is_empty(Stack* stack) {
    return stack->top == -1;
}

bool is_full(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

void push(Stack* stack, int item) {
    if (is_full(stack))
        throw std::runtime_error("stack overflow");
    stack->arr[++stack->top] = item;
}

int pop(Stack* stack) {
    if (is_empty(stack))
        throw std::runtime_error("stack underflow");
    return stack->arr[stack->top--];
}

int top(Stack* stack) {
    if (is_empty(stack))
        throw std::runtime_error("stack underflow");
    return stack->arr[stack->top];
}

void print_stack(Stack* stack) {
    while (not is_empty(stack))
        std::cout << pop(stack) << " ";
    std::cout << std::endl;
}

void reverse(std::string& str) {
    Stack* stack = create_stack(100);
    for (int i = 0; i < str.size(); i++)
        push(stack, str[i]);
    for (int i = 0; i < str.size(); i++)
        str[i] = pop(stack);
}

void reverse_without_stack(std::string& str) {
    for (int i = 0; i < str.size() / 2; i++)
        std::swap(str[i], str[str.size() - i - 1]);
}

void sorted_insert_recursive(Stack* stack, int item) {
    if (is_empty(stack) or top(stack) <= item) {
        push(stack, item);
        return;
    }

    int temp = pop(stack);
    sorted_insert_recursive(stack, item);
    push(stack, temp);
}

void sorted_insert_iterative(Stack* stack, int item) {
    Stack* temp_stack = create_stack(100);

    while (not is_empty(stack) and top(stack) > item)
        push(temp_stack, pop(stack));
    push(stack, item);
    while (not is_empty(temp_stack))
        push(stack, pop(temp_stack));
}

int main() {
    {
        Stack* stack = create_stack_for_test(100);
        print_stack(stack);
    }

    {
        std::string str = "hello, world!";
        std::cout << "str = " << str << ", reversed str = ";
        reverse(str);
        std::cout << str << std::endl;
    }

    {
        std::string str = "hello, world!";
        std::cout << "str = " << str << ", reversed str = ";
        reverse_without_stack(str);
        std::cout << str << std::endl;
    }

    {
        Stack* stack = create_stack_for_test(100);
        sorted_insert_recursive(stack, 15);
        print_stack(stack);
    }

    {
        Stack* stack = create_stack_for_test(100);
        sorted_insert_iterative(stack, 15);
        print_stack(stack);
    }
}
