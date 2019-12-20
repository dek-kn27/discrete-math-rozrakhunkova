#include <bits/stdc++.h>

using namespace std;

const short int EXCEPTION_GRAPH_VERTEXOUTOFRANGE = 1;
const short int PARSE_ERROR = 2;

typedef struct {
    unsigned long int a;
    unsigned long int b;
    long int w;
} neograph_edge;
typedef neograph_edge* neograph_matrix_elem;
typedef neograph_matrix_elem* neograph_matrix_row;
typedef neograph_matrix_row* neograph_matrix;
typedef struct {
    neograph_matrix matrix;
    unsigned long int vertex_count;
    neograph_edge** edges;
    unsigned long int edge_count;
} neograph;
typedef struct {
    unsigned long int a;
    unsigned long int b;
    long int w;
} parsedinp;

neograph_edge* NeographEdge(unsigned long int a, unsigned long int b, long int w) {
    neograph_edge* new_edge = (neograph_edge*)malloc(sizeof(neograph_edge));
    new_edge->a = a;
    new_edge->b = b;
    new_edge->w = w;
    return new_edge;
};

neograph* Neograph(unsigned long int n_vertices) {
    //printf("Neograph debug");
    neograph_matrix new_matrix = (neograph_matrix)calloc(n_vertices, sizeof(neograph_matrix_row));
    unsigned long j;
    for (unsigned long int i = 0; i < n_vertices; i++) {
        new_matrix[i] = (neograph_matrix_row)calloc(n_vertices, sizeof(neograph_matrix_elem));
        for (j = 0; j < n_vertices; j++) {
            new_matrix[i][j] = NULL;
        }
    };
    neograph* new_neograph = (neograph*)malloc(sizeof(neograph));
    new_neograph->matrix = new_matrix;
    new_neograph->vertex_count = n_vertices;
    new_neograph->edges = (neograph_edge**)calloc(255, sizeof(neograph_edge*));
    new_neograph->edge_count = 0;
    return new_neograph;
};

void remove_neograph(neograph* ng) {
    free(ng);
};

void print_edge(neograph_edge* e) {
    cout << "(a=" << e->a + 1 << ", b=" << e->b + 1 << ", weight=" << e->w << ")";
};

void print_edge_array(neograph* ng) {
    printf("Edge array: [");
    for (unsigned long i = 0; i < ng->edge_count; i++) {
        print_edge(ng->edges[i]);
        printf(", ");
    };
    printf("]\n");
};

bool add_edge(neograph* p_gr, neograph_edge* e) {
    if ((e->a < p_gr->vertex_count) && (e->b < p_gr->vertex_count)) {
        if (p_gr->matrix[e->a][e->b] == NULL) {
            p_gr->edge_count++;
            p_gr->edges[p_gr->edge_count - 1] = e;
            p_gr->matrix[e->a][e->b] = e;
            p_gr->matrix[e->b][e->a] = e;
            return true;
        } else {
            return false;
        };
    }
    else {
        throw EXCEPTION_GRAPH_VERTEXOUTOFRANGE;
    };
};

parsedinp parse_input(char* s) {
    parsedinp result;
    if (sscanf(s, "%u %u %li", &result.a, &result.b, &result.w) == 3) {
        return result;
    }
    else {
        throw PARSE_ERROR;
    };
};

neograph* read_neograph(unsigned long int n_vertices, FILE* f) {
    neograph* new_neograph = Neograph(n_vertices);
    char* temp = (char*)calloc(255, sizeof(char));
    parsedinp parsed;
    neograph_edge* debug_t;
    while (true) {
        try {
            parsed = parse_input(fgets(temp, 255, f));
            debug_t = NeographEdge(parsed.a - 1, parsed.b - 1, parsed.w);
            add_edge(new_neograph, debug_t);
        } catch (const short int e) {
            if (e == PARSE_ERROR) {
                break;
            }
            else {
                printf("[Invalid vertex number]\n");
            };
        };
    };
    return new_neograph;
};

neograph* fileinput_neograph(const char* filename) {
    FILE* f = fopen(filename, "r");
    unsigned long int n_vertices;
    fscanf(f, "%u\n", &n_vertices);
    neograph* new_neograph = read_neograph(n_vertices, f);
    fclose(f);
    return new_neograph;
};

void print_neograph_matrix(neograph* ng) {
    printf("Neograph:\n");
    unsigned long int i, j;
    printf("    | ");
    for (i = 0; i < ng->vertex_count; i++) {
        printf("%3u  ", i + 1);
    };
    printf("\n----+-");
    for (i = 0; i < ng->vertex_count; i++) {
        printf("-----");
    };
    printf("\n");
    for (i = 0; i < ng->vertex_count; i++) {
        printf("%3u |", i + 1);
        for (j = 0; j < ng->vertex_count; j++) {
            printf("  ");
            if (ng->matrix[i][j] != NULL) {
                printf("1");
            }
            else {
                printf("0");
            };
            printf("  ");
        };
        printf("\n");
    };
};

void swap(neograph_edge** xp, neograph_edge** yp) {
    neograph_edge* temp = *xp;
    *xp = *yp;
    *yp = temp;
};

void sort_array_by_weight(neograph_edge** arr, unsigned long int n) {
    // bubblesort from https://www.geeksforgeeks.org/bubble-sort/
    unsigned long int i, j;
    for (i = 0; i < n-1; i++) {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++) {
            if (((arr[j])->w) > ((arr[j+1])->w)) {
                swap(arr + j, arr + (j+1));
            };
        };
    };
};

void print_stage_info(unsigned long int stage_number, neograph* ng) {
    printf("After stage %u: {\n", stage_number);
    print_neograph_matrix(ng);
    printf("}\n");
};

int main() {
    printf("[ Algorytm Kraskala ]\n");
    // import neograph
    char* filename = new char[256];
    printf("Enter filename (don't use spaces):\n");
    cin >> filename;
    neograph* a = fileinput_neograph(filename);

    print_neograph_matrix(a);

    print_edge_array(a);

    sort_array_by_weight(a->edges, a->edge_count);

    print_edge_array(a);

    neograph* b = Neograph(a->vertex_count);

    print_stage_info(0, b);

    unsigned long int j, old_id, new_id;
    unsigned long int* vertex_tree_ids = (unsigned long int*)calloc(a->vertex_count, sizeof(unsigned long int));
    for (unsigned long int i = 0; i < a->vertex_count; i++) {
        vertex_tree_ids[i] = i;
    };
    for (unsigned long int i = 0; i < a->edge_count; i++) {
        printf("Edge ");
        print_edge(a->edges[i]);
        if (vertex_tree_ids[a->edges[i]->a] != vertex_tree_ids[a->edges[i]->b]) { // add
            add_edge(b, a->edges[i]);
            printf(" added\n");
            old_id = vertex_tree_ids[a->edges[i]->b];
            new_id = vertex_tree_ids[a->edges[i]->a];
            for (j = 0; j < a->vertex_count; j++) {
                if (vertex_tree_ids[j] == old_id) {
                    vertex_tree_ids[j] = new_id;
                };
            };
        }
        else { // skip
            printf(" skipped\n");
        }
        print_stage_info(i + 1, b);
    };
    print_edge_array(b);

    remove_neograph(a);
    remove_neograph(b);
    free(vertex_tree_ids);

    return 0;
};