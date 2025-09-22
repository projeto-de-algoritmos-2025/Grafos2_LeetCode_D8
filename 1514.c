//medium 

#include <stdio.h>
#include <stdlib.h>

// Estrutura para os nós da fila de prioridade (heap)
typedef struct {
    double prob;
    int node;
} Node;

// Estrutura para o heap de prioridade (Max-Heap)
typedef struct {
    Node* heap;
    int size;
    int capacity;
} PriorityQueue;

// Funções para o heap de prioridade
void init_pq(PriorityQueue* pq, int capacity) {
    pq->heap = (Node*)malloc(capacity * sizeof(Node));
    pq->size = 0;
    pq->capacity = capacity;
}

void destroy_pq(PriorityQueue* pq) {
    if (pq && pq->heap) {
        free(pq->heap);
    }
}

void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue* pq, int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && pq->heap[index].prob > pq->heap[parent].prob) {
        swap(&pq->heap[index], &pq->heap[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void heapify_down(PriorityQueue* pq, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->heap[left].prob > pq->heap[largest].prob) {
        largest = left;
    }

    if (right < pq->size && pq->heap[right].prob > pq->heap[largest].prob) {
        largest = right;
    }

    if (largest != index) {
        swap(&pq->heap[index], &pq->heap[largest]);
        heapify_down(pq, largest);
    }
}

void push(PriorityQueue* pq, Node node) {
    if (pq->size >= pq->capacity) {
        // Redimensiona o heap se estiver cheio
        pq->capacity *= 2;
        pq->heap = (Node*)realloc(pq->heap, pq->capacity * sizeof(Node));
    }
    pq->heap[pq->size] = node;
    heapify_up(pq, pq->size);
    pq->size++;
}

Node pop(PriorityQueue* pq) {
    Node max_node = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);
    return max_node;
}

// Estrutura para os nós da lista de adjacências
typedef struct GraphNode {
    int dest;
    double prob;
    struct GraphNode* next;
} GraphNode;

// Funções para a lista de adjacências
void add_edge(GraphNode* adj_list[], int u, int v, double prob) {
    GraphNode* new_node1 = (GraphNode*)malloc(sizeof(GraphNode));
    new_node1->dest = v;
    new_node1->prob = prob;
    new_node1->next = adj_list[u];
    adj_list[u] = new_node1;

    GraphNode* new_node2 = (GraphNode*)malloc(sizeof(GraphNode));
    new_node2->dest = u;
    new_node2->prob = prob;
    new_node2->next = adj_list[v];
    adj_list[v] = new_node2;
}

// Função principal do problema
double maxProbability(int n, int** edges, int edgesSize, int* edgesColSize, double* succProb, int succProbSize, int start_node, int end_node) {
    // Inicialização do grafo e probabilidades
    GraphNode** adj_list = (GraphNode**)calloc(n, sizeof(GraphNode*));
    double* maxProb = (double*)calloc(n, sizeof(double));
    maxProb[start_node] = 1.0;

    for (int i = 0; i < edgesSize; i++) {
        add_edge(adj_list, edges[i][0], edges[i][1], succProb[i]);
    }

    // Inicialização da fila de prioridade
    PriorityQueue pq;
    init_pq(&pq, n);
    Node start = {1.0, start_node};
    push(&pq, start);

    while (pq.size > 0) {
        Node current = pop(&pq);
        double current_prob = current.prob;
        int current_node = current.node;

        // Se a probabilidade atual é menor do que a já encontrada
        if (current_prob < maxProb[current_node]) {
            continue;
        }

        // Se alcançamos o nó final
        if (current_node == end_node) {
            // Libera a memória antes de retornar
            for (int i = 0; i < n; i++) {
                GraphNode* temp = adj_list[i];
                while (temp != NULL) {
                    GraphNode* to_free = temp;
                    temp = temp->next;
                    free(to_free);
                }
            }
            free(adj_list);
            free(maxProb);
            destroy_pq(&pq);
            return current_prob;
        }

        // Percorre os vizinhos
        GraphNode* temp = adj_list[current_node];
        while (temp != NULL) {
            double new_prob = current_prob * temp->prob;
            if (new_prob > maxProb[temp->dest]) {
                maxProb[temp->dest] = new_prob;
                Node next_node = {new_prob, temp->dest};
                push(&pq, next_node);
            }
            temp = temp->next;
        }
    }

    // Libera a memória alocada para o grafo
    for (int i = 0; i < n; i++) {
        GraphNode* temp = adj_list[i];
        while (temp != NULL) {
            GraphNode* to_free = temp;
            temp = temp->next;
            free(to_free);
        }
    }
    free(adj_list);
    free(maxProb);
    destroy_pq(&pq);

    return 0.0;
}