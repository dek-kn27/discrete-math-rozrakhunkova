#include <bits/stdc++.h>

const int EMPTY = 1;

using namespace std;

void push(unsigned long int* stack, unsigned long int n_items, unsigned long int item) {
    n_items++;
    realloc(stack, n_items);
    stack[n_items - 1] = item;
};

unsigned long int pop(unsigned long int* stack, unsigned long int n_items) {
    if (n_items) {
        unsigned long int item = stack[n_items - 1];
        n_items--;
        realloc(stack, n_items);
        return item;
    }
    else {
        throw EMPTY;
    };
};

unsigned long int top(unsigned long int* stack, unsigned long int n_items) {
    if (n_items) {
        return stack[n_items - 1];
    }
    else {
        throw EMPTY;
    };
};

unsigned long int* Stack() {
    return (unsigned long int*)calloc(0, sizeof(unsigned long int));
};

struct my_queue {
    unsigned long int* q;
    unsigned long int start;
    unsigned long int stop;
};

void add_to_tail(my_queue* q, unsigned long int item) {
    q->stop++;
    q->q[q->stop - 1] = item;
};

unsigned long int pop_from_head(my_queue* q) {
    unsigned long int item = q->q[q->start];
    q->start++;
    return item;
};

unsigned long int head(my_queue* q) {
    return q->q[q->start];
};

bool not_empty(my_queue* q) {
    return q->stop - q->start;
};

my_queue* MyQueue(unsigned long int n) {
    my_queue* new_queue = (my_queue*)malloc(sizeof(my_queue));
    new_queue->q = (unsigned long int*)calloc(n, sizeof(unsigned long int));
    new_queue->start = 0;
    new_queue->stop = 0;
    return new_queue;
};

int main() {
    // вводимо граф
    cout << "Enter vertex count=";
    unsigned long int n, m, a, b;
    cin >> n;
    bool** graph = (bool**)calloc(n, sizeof(bool*));
    unsigned long int i, j;
    for (i=0; i<n; i++) {
        graph[i] = (bool*)calloc(n, sizeof(bool));
        for (j=0; j<n; j++) {
            graph[i][j] = false;
        };
    };
    cout << "Enter number of edges=";
    cin >> m;
    cout << "Enter edges:" << endl;
    for (unsigned long int i=0; i<m; i++) {
        cin >> a >> b;
        graph[a - 1][b - 1] = true;
        graph[b - 1][a - 1] = true;
    };
    // вглиб
    cout << "Vglyb:" << endl;
    unsigned long int* s = Stack();
    unsigned long int stack_length = 0;
    bool* marked = (bool*)calloc(n, sizeof(bool));
    for (i = 0; i < n; i++) {
        marked[i] = false;
    };
    marked[0] = true;
    push(s, stack_length, 0);
    stack_length++;
    cout << "1 ";
    while (stack_length) {
        for (i=0; i<n; i++) {
            if (graph[i][top(s, stack_length)] && (!marked[i])) {
                // visit
                marked[i] = true;
                push(s, stack_length, i);
                stack_length++;
                cout << (i + 1) << " ";
                break;
            };
        };
        // else
        if (i>=n) {
            pop(s, stack_length);
            stack_length--;
        };
    };
    cout << endl;
    // вшир
    cout << "Vshyr:" << endl;
    my_queue* q = MyQueue(n);
    for (i = 0; i < n; i++) {
        marked[i] = false;
    };
    marked[0] = true;
    add_to_tail(q, 0);
    while (not_empty(q)) {
        for (i = 0; i < n; i++) {
            if (graph[i][head(q)] && (!marked[i])) {
                marked[i] = true;
                add_to_tail(q, i);
            };
        };
        cout << (head(q) + 1) << " ";
        pop_from_head(q);
    };
    cout << endl;

    free(graph);
    free(s);
    free(marked);
    free(q);

    return 0;
};
