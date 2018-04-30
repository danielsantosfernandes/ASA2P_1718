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
    bool isBackGround = false;
} Vertix;

typedef struct edge {
public:
    Vertix *a, *b;
    int weight;
    int flow = 0;
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
        target->vertix->adj = new Edge*[M*N];  //DESNECESSARIO
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

    /*scan de M e N*/
	if(1 != scanf("%d", &(graph->M))) return 1;
	if(1 != scanf("%d", &(graph->N))) return 1;    
    
    n = graph->start;

    graph->initializeVertices();

    /*scan de valores de primeiro plano*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = graph->start->vertix->adj[(i-1)*graph->N + (j-1)] = new Edge();
            auxEdge->a = graph->start->vertix;
            auxEdge->b = n->vertix;
            if(1 != scanf("%d", &(auxEdge->weight))) return 1;
        }
    }

    n = graph->start;

    /*scan de valores de cenario*/
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = n->vertix->adj[0] = new Edge();
            auxEdge->a = n->vertix;
            auxEdge->b = graph->target->vertix;
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
                n->vertix->adj[1] = new Edge();
                n->vertix->adj[1]->a = n->vertix;
                n->vertix->adj[1]->b = above->vertix;
                above->vertix->adj[3] = n->vertix->adj[1];
            } /*else {
                n->vertix->adj[1] = NULL;
            }

            if (i = graph->M) {
                n->vertix->adj[3] = NULL;
            }*/

            if (j < graph->N) {
                n->vertix->adj[2] = new Edge();
                n->vertix->adj[2]->a = n->vertix;
                n->vertix->adj[2]->b = n->next->vertix;
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

    n = graph->start; std::cout << "oi\n";

    printf("%d\n\n", edmondsKarp()); std::cout << "oioi\n";
    
    BFS(); std::cout << "oioioi\n";

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

    while (true) { ////std::cout << "236---------------\n";
        bool stop = false;
        std::queue<Vertix*> queue;
        for (n = graph->start; n != NULL ; n = n->next) {
            n->vertix->pi = NULL;
        }
        s->capacity = MAX_INT;
        s->pi = s;
        queue.push(s);
        while (!queue.empty()) { ////std::cout << "\t239\n";
            stop = false;
            u = queue.front();
            queue.pop();
            if (0 == u->c) {
                adjN = MN;
            } else {
                adjN = 5;
            }
            for (i = 0; i < adjN && !stop; i++) { //std::cout << "\t\t247 " << u->l << " " << u->c << std::endl;
                e = u->adj[i];
                if (NULL == e) continue;
                v = ( e->a == u ? e->b : e->a ); //std::cout << "\t\t "<< v->l << " " << v->c << std::endl;
                // There is available capacity,
                // and v is not seen before in search
                if (e->weight - e->flow > 0 && v->pi == NULL) {
                    v->pi = u; 
                    //std::cout << u->capacity << " - (" << e->weight << " - " << e->flow << ") > 0 ? " << e->weight << " - " << e->flow << " : " << u->capacity << std::endl;
                    v->capacity = (u->capacity - (e->weight - e->flow) > 0 ? e->weight - e->flow : u->capacity);
                    //std::cout << v->capacity << std::endl;
                    if (v != t)
                        queue.push(v);
                    else {
                        // Backtrack search, and write flow
                        while (v->pi != v) { //std::cout << "\t\t\t259 ";
                            u = v->pi;
                            if (0 == v->l) { //std::cout << "\t\tto target\n";
                                e = u->adj[0];
                            } else if (0 == u->l) { //std::cout << "\t\tfrom start\n";
                                e = u->adj[(v->l - 1) * graph->N + (v->c - 1)];
                            } else if (u->l > v->l) { //std::cout << "\t\tto north\n";
                                e = u->adj[1];
                            } else if (u->l < v->l) { //std::cout << "\t\tto south\n";
                                e = u->adj[3];
                            } else if (u->c > v->c) { //std::cout << "\t\tto west\n";
                                e = u->adj[4];
                            } else if (u->c < v->c) { //std::cout << "\t\tto east\n";
                                e = u->adj[2];
                            } else printf("WHAT THE FUCK");
                            v = ( e->a == u ? e->b : e->a );
                            e->flow += t->capacity;  //std::cout << "\t\t\t273 " << t->capacity << std::endl;
                            /*F[v][u] -= M[t]; let's ignore this line bECAUSE I CAN*/
                            v = u;
                        }
                        stop = true;
                    }
                }
            }
        }
        if (t->pi == NULL) { // We did not find a path to t
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
            if (e->weight - e->flow > 0 && v->pi == NULL) {
                v->pi = u;
                v->isBackGround = true;
                if (v != t)
                    queue.push(v);
            }
        }
    }


}