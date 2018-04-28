/*-------------------------------*
  2º projeto ASA - 2017/28
  Daniel Santos Fernandes - 86400
 *-------------------------------*/

#include <iostream>
#include <stdio.h>

class Vertix;
class Edge;

typedef struct node {
	Vertix *vertix;
	struct node *next, *prev;
} node;

typedef node *Node;

class Vertix {
public:
    int l, c;
    int bg, fg;
    Edge *north, *south, *east, *west;
};

class Edge {
public:
    int id;
    Vertix *a, *b;
    int weight;
    Edge(Vertix *v1, Vertix *v2) {
        a = v1;
        b = v2;
    }
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

    void initializeVertices() {
        Node aux;
        for (int i = 1; i <= M; i++) {
            for (int j = 1; j <= N; j++) {
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


int main() {

    int i, j;
    Node n, above, aux;

    graph = new Graph(); std::cout << "93\n";

    /*scan de M e N*/
	if(1 != scanf("%d", &(graph->M))) return 1;
	if(1 != scanf("%d", &(graph->N))) return 1;    
    
    //TODO IGNORAR \n ????? 
    std::cout << "96\n";

    n = graph->start;

    graph->initializeVertices(); std::cout << "103\n";

    /*scan de valores de primeiro plano*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->fg))) return 1;
        }
    }

    n = graph->start;

    /*scan de valores de cenario*/ std::cout << "115\n";
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->bg))) return 1;
        }
    }

    n = graph->start;

    /*inicializar arestas*/ std::cout << "125\n";
    for (i = 1; i <= graph->M; i++) {
        if (i > 1) {
            above = aux->prev;
        }
        aux = n->next;
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if (i > 1) {
                above = above->next;
                n->vertix->north = new Edge(n->vertix, above->vertix);
                above->vertix->south = n->vertix->north;
                //std::cout << above->vertix->l << " " << above->vertix->c << " " << above->vertix->south << std::endl;
            }
            if (j < graph->N) {
                n->vertix->east = new Edge(n->vertix, n->next->vertix);
            }
            if (j > 1) {
                n->vertix->west = n->prev->vertix->east;
            } 
        }
    }

    n = graph->start; std::cout << "146\n";

    /*scan de pesos horizontais*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N - 1; j++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->east->weight))) return 1;
        }
        n = n->next;
    }

    n = graph->start;

    /*scan de pesos verticais*/   std::cout << "157\n";
    for (i = 1; i <= graph->M - 1; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next; //std::cout << i << " " << j << " " << n->vertix->south << std::endl;
            if(1 != scanf("%d", &(n->vertix->south->weight))) return 1;
        }
    }

    n = graph->start; std::cout << "164\n";

/*  printa o grafo com os pesos dos vertices
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            printf("%d,%d: %d %d  ", n->vertix->l, n->vertix->c, n->vertix->fg, n->vertix->bg);
        }
        printf("\n");
    } */

/*  printa o grafo com os pesos das arestas
    for (i = 1; i <= graph->M; i++) {
        aux = n;
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if (j < graph->N)
                printf("(%d,%d) - %d - ", n->vertix->l, n->vertix->c, n->vertix->east->weight);
            else
                printf("(%d,%d)", n->vertix->l, n->vertix->c);
        }
        printf("\n");
        for (j = 1; j <= graph->N; j++) {
            aux = aux->next;
            if (i < graph->M)
                printf("  %d         ", aux->vertix->south->weight);
        }
        printf("\n");
    }
    */

}
