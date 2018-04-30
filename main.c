/*-------------------------------*
  2º projeto ASA - 2017/18
  Daniel Santos Fernandes - 86400
 *-------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_INT 2147483647

struct node;
struct vertix;
struct edge;
int edmondsKarp();
void BFS();
struct vertix *pop();
void push(struct vertix *vertix);

typedef struct node {
	struct vertix *vertix;
	struct node *next, *prev;
} node;

typedef node *Node;

typedef struct vertix {
    int l, c;
    int capacity;
    struct edge **adj;
    struct vertix *pi;
    int isBackGround;
} Vertix;

typedef struct edge {
    Vertix *a, *b;
    int weight;
    int flow;
} Edge;

typedef struct graph {  /* l=0,c=0 - start ; l=0,c=1 - target*/
    int M, N;
    Node start;
    Node target;
} Graph;

Graph *graph;
Node Qhead, Qtail;

void initializeVertices() {
    int i, j;
    Node aux, start = graph->start, target = graph->target;
    start->vertix->adj = (Edge**)malloc(sizeof(Edge*) * graph->M * graph->N);
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            aux = target->prev;
            target->prev = (Node)malloc(sizeof(node));
            target->prev->next = target;
            target->prev->prev = aux;
            aux->next = target->prev;

            target->prev->vertix = (Vertix*)malloc(sizeof(Vertix));
            target->prev->vertix->l = i;
            target->prev->vertix->c = j;
            target->prev->vertix->isBackGround = 0;
            target->prev->vertix->adj = (Edge**)malloc(sizeof(Edge*) * 5); /*0-target; 1-north; 2-east; 3-south; 4-west*/
        }
    }
}


int main() {

    int i, j;
    Node n, above = NULL, aux;
    Edge *auxEdge;

    graph = (Graph*)malloc(sizeof(graph));

    graph->start = (Node)malloc(sizeof(node));
    graph->target = (Node)malloc(sizeof(node));

    /*inicializar start*/
    graph->start->vertix = (Vertix*)malloc(sizeof(Vertix));
    graph->start->vertix->l = 0;
    graph->start->vertix->c = 0;
    graph->start->vertix->pi = graph->start->vertix;

    /*inicializar target*/
    graph->target->vertix = (Vertix*)malloc(sizeof(Vertix));
    graph->target->vertix->l = 0;
    graph->target->vertix->c = 1;

    graph->start->next = graph->target;
    graph->target->prev = graph->start;

    /*scan de M e N*/
	if(1 != scanf("%d", &(graph->M))) return 1;
	if(1 != scanf("%d", &(graph->N))) return 1;    
    
    n = graph->start;

    initializeVertices();

    /*scan de valores de primeiro plano*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = graph->start->vertix->adj[(i-1)*graph->N + (j-1)] = (Edge*)malloc(sizeof(Edge));
            auxEdge->a = graph->start->vertix;
            auxEdge->b = n->vertix;
            auxEdge->flow = 0;
            if(1 != scanf("%d", &(auxEdge->weight))) return 1;
        }
    }

    n = graph->start;

    /*scan de valores de cenario*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = n->vertix->adj[0] = (Edge*)malloc(sizeof(Edge));
            auxEdge->a = n->vertix;
            auxEdge->b = graph->target->vertix;
            auxEdge->flow = 0;
            if(1 != scanf("%d", &(auxEdge->weight))) return 1;
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
                n->vertix->adj[1] = (Edge*)malloc(sizeof(Edge));
                n->vertix->adj[1]->a = n->vertix;
                n->vertix->adj[1]->b = above->vertix;
                n->vertix->adj[1]->flow = 0;
                above->vertix->adj[3] = n->vertix->adj[1];
            } /*else {
                n->vertix->adj[1] = NULL;
            }

            if (i = graph->M) {
                n->vertix->adj[3] = NULL;
            }*/

            if (j < graph->N) {
                n->vertix->adj[2] = (Edge*)malloc(sizeof(Edge));
                n->vertix->adj[2]->a = n->vertix;
                n->vertix->adj[2]->b = n->next->vertix;
                n->vertix->adj[2]->flow = 0;
            }/* else {
                n->vertix->adj[2] = NULL;
            }*/

            if (j > 1) {
                n->vertix->adj[4] = n->prev->vertix->adj[2];
            }/* else  {
                n->vertix->adj[4] = NULL;
            }*/
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

    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if (n->vertix->isBackGround) {
                printf("C ");
            } else {
                printf("P ");
            }
        }
        printf("\n");
    }


   return 0;
}

int edmondsKarp() {

    Node n;
    Vertix *u, *v;
    Vertix *s = graph->start->vertix;
    Vertix *t = graph->target->vertix;
    Edge *e;
    int i, adjN, MN = graph->M * graph->N;

    while (1) {
        int stop = 0;
        Qhead = Qtail = NULL;
        for (n = graph->start; n != NULL ; n = n->next) {
            n->vertix->pi = NULL;
        }
        s->capacity = MAX_INT;
        s->pi = s;
        push(s);
        while (NULL != Qhead) { /*printf("bom meio dia");*/
            stop = 0;
            u = pop();
            if (0 == u->c) {
                adjN = MN;
            } else {
                adjN = 5;
            }
            for (i = 0; i < adjN && !stop; i++) { /*printf("boa tarde");*/
                e = u->adj[i];
                if (NULL == e) continue;
                v = ( e->a == u ? e->b : e->a );
                if (e->weight - e->flow > 0 && v->pi == NULL) {
                    v->pi = u; 
                    v->capacity = (u->capacity - (e->weight - e->flow) > 0 ? e->weight - e->flow : u->capacity);
                    if (v != t)
                        push(v);
                    else {
                        while (v->pi != v) { 
                            u = v->pi;
                            if (0 == v->l) {
                                e = u->adj[0];
                            } else if (0 == u->l) { 
                                e = u->adj[(v->l - 1) * graph->N + (v->c - 1)];
                            } else if (u->l > v->l) { 
                                e = u->adj[1];
                            } else if (u->l < v->l) { 
                                e = u->adj[3];
                            } else if (u->c > v->c) { 
                                e = u->adj[4];
                            } else if (u->c < v->c) { 
                                e = u->adj[2];
                            } else printf("WHAT THE FUCK");
                            v = ( e->a == u ? e->b : e->a );
                            e->flow += t->capacity;
                            /*F[v][u] -= M[t]; let's ignore this line bECAUSE I CAN*/
                            v = u;
                        }
                        stop = 1;
                    }
                }
            }
        }
        if (t->pi == NULL) {
            int sum = 0;
            for (i = 0; i < graph->M * graph->N; i++) {
                sum += s->adj[i]->flow;
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

    Qhead = Qtail = NULL;

    for (n = graph->start; n != NULL ; n = n->next) {
        n->vertix->pi = NULL;
    }
    s->pi = s;

    push(s);
    while (NULL != Qhead) {
        u = pop(Qhead, Qtail);
        if (0 == u->c) {
            adjN = graph->M * graph->N;
        } else {
            adjN = 5;
        }
        for (i = 0; i < adjN; i++) {
            e = u->adj[i];
            if (NULL == e) continue;
            v = ( e->a == u ? e->b : e->a );
            if (e->weight - e->flow > 0 && v->pi == NULL) {
                v->pi = u;
                v->isBackGround = 1;
                if (v != t)
                    push(v);
            }
        }
    }


}

void push(Vertix *vertix) {
	Node n = (Node)malloc(sizeof(node));
	n->vertix = vertix;
	n->next = NULL;
	if (Qhead == NULL){
		Qhead = n;
        Qtail = n;
	} else {
		Qtail->next = n;
        Qtail = Qtail->next;
	}
}

Vertix *pop() {
	Vertix *popped;
	if (Qhead == NULL){
		return NULL;
	} else {
		popped = Qhead->vertix;
		Node next = Qhead->next;
		free(Qhead);
		Qhead = next;
		if (Qhead == NULL)
			Qtail = NULL;
	}
	return popped;
}
