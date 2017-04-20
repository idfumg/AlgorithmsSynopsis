#include <iostream>

struct Stack {
    int capacity;
    int top;
    int* array;
};

void push(Stack* stack, int data);

Stack* create_stack(int capacity) {
    Stack* stack = new Stack;
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = new int[capacity];
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

bool is_full(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

bool is_empty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, int data) {
    if (is_full(stack))
        throw "stackoverflow";

    stack->array[++stack->top] = data;
}

int pop(Stack* stack) {
    if (is_empty(stack))
        throw "stackundeflow";

    return stack->array[stack->top--];
}

int top(Stack* stack) {
    if (is_empty(stack))
        throw "stackunderflow";

    return stack->array[stack->top];
}

void print_stack(Stack* stack) {
    while (not is_empty(stack))
        std::cout << pop(stack) << " ";
    std::cout << std::endl;
}

void reverse(std::string& str) {
    Stack* stack = create_stack(str.size());

    for (int i = 0; i < str.size(); i++)
        push(stack, str[i]);
    for (int i = 0; i < str.size(); i++)
        str[i] = pop(stack);
}

void reverse_without_stack(std::string& str) {
    for (int i = 0; i < str.size() / 2; i++)
        std::swap(str[i], str[str.size() - i - 1]);
}

void sorted_insert_recursive(Stack* stack, int x) {
    if (is_empty(stack) or top(stack) <= x) {
        push(stack, x);
        return;
    }

    int temp = pop(stack);
    sorted_insert_recursive(stack, x);
    push(stack, temp);
}

void sorted_insert_iterative(Stack* stack, int x) {
    Stack* temp_stack = create_stack(100);
    while (not is_empty(stack) and top(stack) >= x)
        push(temp_stack, pop(stack));
    push(stack, x);
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
