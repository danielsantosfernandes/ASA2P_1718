/*-------------------------------*
  2º projeto ASA - 2017/18
  Daniel Santos Fernandes - 86400
 *-------------------------------*/

#include <iostream>
#include <stdio.h>
#include <queue>

#define MAX_INT 2147483647

struct vertix;
struct edge;
int edmondsKarp();
void BFS();

typedef struct node {
	struct vertix *vertix;
	struct node *next, *prev;
} node;

typedef node *Node;

typedef struct vertix {
public:
    int l, c;
    int capacity;
    struct edge **adj;
    struct vertix *pi = NULL;
    struct edge *piE = NULL;
    bool isBackGround = false;
} Vertix;

typedef struct edge {
public:
    Vertix *a, *b;
    int weight;
    int flowAB = 0;
    int flowBA = 0;
} Edge;

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
        start->vertix->pi = start->vertix;

        /*inicializar target*/
        target->vertix = new Vertix();
        target->vertix->l = 0;
        target->vertix->c = 1;

        start->next = target;
        target->prev = start;

    }

    void initializeVertices() {
        Node aux;
        start->vertix->adj = new Edge*[M*N];
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
                target->prev->vertix->adj = new Edge*[5](); /*0-target; 1-north; 2-east; 3-south; 4-west*/
            }
        }
    }
};

Graph *graph;


int main() {

    int i, j;
    Node n, above = NULL, aux;
    Edge *auxEdge;

    graph = new Graph();

    Vertix *s = graph->start->vertix;

    /*scan de M e N*/
	if(1 != scanf("%d", &(graph->M))) return 1;
	if(1 != scanf("%d", &(graph->N))) return 1;    
    
    n = graph->start;

    graph->initializeVertices();

    /*scan de valores de primeiro plano*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = s->adj[(i-1)*graph->N + (j-1)] = new Edge();
            auxEdge->a = s;
            auxEdge->b = n->vertix;
            if(1 != scanf("%d", &(auxEdge->weight))) return 1;
        }
    }

    n = graph->start;
    Edge *e1;

    /*scan de valores de cenario*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = n->vertix->adj[0] = new Edge();
            auxEdge->a = n->vertix;
            auxEdge->b = graph->target->vertix;
            if(1 != scanf("%d", &(auxEdge->weight))) return 1;

            e1 = s->adj[(i-1)*graph->N + (j-1)];
            int capacity = ((e1->weight - auxEdge->weight) > 0 ? auxEdge->weight : e1->weight);
            e1->flowAB = auxEdge->flowAB = capacity;
            e1->flowBA = auxEdge->flowBA = -capacity;

        }
    }

    n = graph->start;

    /*inicializar arestas*/
    /*0-target; 1-north; 2-east; 3-south; 4-west*/ 
    for (i = 1; i <= graph->M; i++) {
        if (i > 1) {
            above = aux->prev;
        }
        aux = n->next;
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if (i > 1) {
                above = above->next;
                n->vertix->adj[1] = new Edge();
                n->vertix->adj[1]->a = n->vertix;
                n->vertix->adj[1]->b = above->vertix;
                above->vertix->adj[3] = n->vertix->adj[1];
            }
            if (j < graph->N) {
                n->vertix->adj[2] = new Edge();
                n->vertix->adj[2]->a = n->vertix;
                n->vertix->adj[2]->b = n->next->vertix;
            }
            if (j > 1) {
                n->vertix->adj[4] = n->prev->vertix->adj[2];
            }
        }
    }

    n = graph->start;

    /*scan de pesos horizontais*/
    /*0-target; 1-north; 2-east; 3-south; 4-west*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N - 1; j++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->adj[2]->weight))) return 1;
        }
        n = n->next;
    }

    n = graph->start;

    /*scan de pesos verticais*/
    for (i = 1; i <= graph->M - 1; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->adj[3]->weight))) return 1;
        }
    }

    n = graph->start;

    printf("%d\n\n", edmondsKarp());

    BFS();

    std::string str = "";

    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if (n->vertix->isBackGround) {
                str += "C ";
            } else {
                str += "P ";
            }
        }
        str += "\n";
    }
    

    printf("%s", str.c_str());


    return 0;
}

int edmondsKarp() {

    Node n;
    Vertix *u, *v;
    Vertix *s = graph->start->vertix;
    Vertix *t = graph->target->vertix;
    Edge *e;
    int i, spare, adjN, MN = graph->M * graph->N;

    while (true) {
        bool stop = false;
        std::queue<Vertix*> queue;
        for (n = graph->start; n != NULL ; n = n->next) {
            n->vertix->piE = NULL;
            n->vertix->capacity = 0;
        }
        s->capacity = MAX_INT;
        queue.push(s);
        while (!queue.empty() && !stop) {
            stop = false;
            u = queue.front();
            queue.pop();
            if (0 == u->c) {
                adjN = MN;
            } else {
                adjN = 5;
            }
            for (i = 0; i < adjN && !stop; i++) { 
                e = u->adj[i];
                if (NULL == e) continue;
                v = ( e->a == u ? e->b : e->a );
                if (u == e->a) {
                    spare = e->weight - e->flowAB;
                } else {
                    spare = e->weight - e->flowBA;
                }
                if (spare > 0 && v->piE == NULL) {
                    v->piE = e; 
                    int uCap = u->capacity;
                    v->capacity = (uCap - spare > 0 ? spare : uCap);
                    if (v != t)
                        queue.push(v);
                    else {
                        while (v != s) {
                            e = v->piE;
                            u = ( e->a == v ? e->b : e->a );
                            if (u == e->a) {
                                e->flowAB += t->capacity;
                                e->flowBA -= t->capacity;
                            } else {
                                e->flowBA += t->capacity;
                                e->flowAB -= t->capacity;
                            }
                            v = u;
                        }
                        stop = true;
                    }
                }
            }
        }
        if (t->piE == NULL) {
            int sum = 0;
            for (i = 0; i < graph->M * graph->N; i++) {
                sum += s->adj[i]->flowAB;
            }
            return sum;
        }
    }
}


void BFS() {
    Node n;
    Vertix *u, *v;
    Vertix *s = graph->start->vertix;
    Vertix *t = graph->target->vertix;
    Edge *e;
    int i, adjN;

    std::queue<Vertix*> queue;

    for (n = graph->start; n != NULL ; n = n->next) {
        n->vertix->pi = NULL;
    }
    s->pi = s;

    queue.push(s);
    while (!queue.empty()) {
        u = queue.front();
        queue.pop();
        if (0 == u->c) {
            adjN = graph->M * graph->N;
        } else {
            adjN = 5;
        }
        for (i = 0; i < adjN; i++) {
            e = u->adj[i];
            if (NULL == e) continue;
            v = ( e->a == u ? e->b : e->a );
            if (e->weight - ( u == e->a ? e->flowAB : e->flowBA) > 0 && v->pi == NULL) {
                v->pi = u;
                v->isBackGround = true;
                if (v != t)
                    queue.push(v);
            }
        }
    }
}
