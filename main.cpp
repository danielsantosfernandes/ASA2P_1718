/*-------------------------------*
  2º projeto ASA - 2017/28
  Daniel Santos Fernandes - 86400
 *-------------------------------*/

#include <stdio.h>

typedef struct node {
	Vertix *vertix;
	struct node *next, *prev;
} node;

typedef node *Node;

class Vertix {
public:
    int l, c;
    int bg, fg;
    Vertix *north, *south, *east, *west;
};

class Edge {
public:
    int id;
    int weight;
};

class Graph {  /* l=0,c=0 - start ; l=0,c=1 - target*/
public:
    int M, N;
    Node start;
    Node target;
    Graph() {
        start = new node();
        target = new node();

        /*inicializar start*/
        start->vertix = new Vertix();
        start->vertix->l = 0;
        start->vertix->c = 0;
        start->vertix->bg = start->vertix->fg = 0;

        /*inicializar target*/
        target->vertix = new Vertix();
        target->vertix->l = 0;
        target->vertix->c = 1;
        target->vertix->bg = target->vertix->fg = 0;

        start->next = target;
        target->prev = start;

    }

    void initializeGraph() {
        Node aux;
        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; i++) {
                aux = target->prev;
                target->prev = new node();
                target->prev->next = target;
                target->prev->prev = aux;
                aux->next = target->prev;

                target->prev->vertix = new Vertix();
                target->prev->vertix->l = i;
                target->prev->vertix->c = j;
            }
        }
    }
};

Graph *graph;



//guardar so os pesos, coordenadas - se calhar nao eh bom
//se calhar é melhor fazer grafo mesmo

int main() {

    int i, j;
    Node n, above, aux;

    graph = new Graph();

	if(1 != scanf("%d", &(graph->M))) return 1;
	if(1 != scanf("%d", &(graph->N))) return 1;    //IGNORAR \n ?????

    n = graph->start;

    graph->initializeGraph();

    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; i++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->fg))) return 1;
        }
    }

    n = graph->start;

    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; i++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->bg))) return 1;
        }
    }

    n = graph->start;

    for (i = 1; i <= graph->M; i++) {
        if (i > 1) {
            above = aux;
        }
        aux = n->next;
        for (j = 1; j <= graph->N; i++) {
            n = n->next;
            if (i > 1) {
                n->vertix->north = above->vertix;
                above->vertix->south = n->vertix;
            }
            if (j > 1) {
                n->vertix->east = n->next->vertix;
            }
            if (j < graph->N) {
                n->vertix->west = n->prev->vertix;
            }

        }
    }

    n = graph->start;

    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; i++) {
            printf("%d", n->vertix->c);
        }
    }




}
