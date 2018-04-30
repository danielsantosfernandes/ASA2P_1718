/*-------------------------------*
  2º projeto ASA - 2017/18
  Daniel Santos Fernandes - 86400
 *-------------------------------*/

#include <iostream>
#include <stdio.h>
#include <queue>

class Vertix;
class Edge;
int edmondsKarp();

typedef struct node {
	Vertix *vertix;
	struct node *next, *prev;
} node;

typedef node *Node;

class Vertix {
public:
    int l, c;
    int capacity;
    Edge **adj;
    Vertix *pi = NULL;
};

class Edge {
public:
    Vertix *a, *b;
    int weight;
    int flow;
    Edge(Vertix *v1, Vertix *v2) {
        a = v1;
        b = v2;
        flow = 0;
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
    Node n, above, aux;
    Edge *auxEdge;

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
            auxEdge = graph->start->vertix->adj[(i-1)*graph->N + (j-1)] = new Edge(graph->start->vertix, n->vertix);
            if(1 != scanf("%d", &(auxEdge->weight))) return 1;
        }
    }

    n = graph->start;

    /*scan de valores de cenario*/ std::cout << "115\n";
    for (i = 1; i <= graph->M; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            auxEdge = n->vertix->adj[0] = new Edge(n->vertix, graph->target->vertix);
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
                n->vertix->adj[1] = new Edge(n->vertix, above->vertix);
                above->vertix->adj[3] = n->vertix->adj[1];
                //std::cout << above->vertix->l << " " << above->vertix->c << " " << above->vertix->south << std::endl;
            } /*else {
                n->vertix->adj[1] = NULL;
            }

            if (i = graph->M) {
                n->vertix->adj[3] = NULL;
            }*/

            if (j < graph->N) {
                n->vertix->adj[2] = new Edge(n->vertix, n->next->vertix);
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

    n = graph->start; std::cout << "146\n";

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

    /*scan de pesos verticais*/   std::cout << "157\n";
    for (i = 1; i <= graph->M - 1; i++) {
        for (j = 1; j <= graph->N; j++) {
            n = n->next;
            if(1 != scanf("%d", &(n->vertix->adj[3]->weight))) return 1;
        }
    }

    n = graph->start; std::cout << "164\n";

    printf("%d\n", edmondsKarp());




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

int edmondsKarp() {

    Node n;
    Vertix *u, *v;
    Vertix *s = graph->start->vertix;
    Vertix *t = graph->target->vertix;
    Edge *e;
    int i, adjN;

    while (true) { std::cout << "236---------------\n";
        bool stop = false;
        std::queue<Vertix*> queue;
        for (n = graph->start; n != NULL ; n = n->next) {
            n->vertix->pi = NULL;
        }
        s->capacity = 2147483647;
        s->pi = s;
        queue.push(s);
        while (!queue.empty() && !stop) { std::cout << "239\n";
            u = queue.front();
            queue.pop();
            if (0 == u->c) {
                adjN = graph->M * graph->N;
            } else {
                adjN = 5;
            }
            for (i = 0; i < adjN && !stop; i++) { std::cout << "247 " << i << std::endl;
                e = u->adj[i];
                if (NULL == e) break;
                v = ( e->a == u ? e->b : e->a );
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
                        while (v->pi != v) { std::cout << "259 !\n";
                            u = v->pi; std::cout << "260\n";
                            if (0 == v->l) { std::cout << "261\n";
                                e = u->adj[0];
                            } else if (0 == u->l) { std::cout << "262\n";
                                e = u->adj[(v->l - 1) * graph->N + (v->c - 1)];
                            } else if (u->l > v->l) { std::cout << "263\n";
                                e = u->adj[1];
                            } else if (u->l < v->l) {std::cout << "265\n";
                                e = u->adj[3];
                            } else if (u->c > v->c) {std::cout << "267\n";
                                e = u->adj[4];
                            } else if (u->c < v->c) {std::cout << "269\n";
                                e = u->adj[2];
                            } else printf("WHAT THE FUCK"); std::cout << "271\n";
                            v = ( e->a == u ? e->b : e->a ); std::cout << "272\n";
                            e->flow += t->capacity; std::cout << "273 " << t->capacity << std::endl;
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