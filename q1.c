#include <stdio.h>
#include <stdlib.h>

#define MAXN 1005

typedef struct Node {
    int v;
    struct Node *next;
} Node;

Node *adj[MAXN];
char name[MAXN][51];
int indeg[MAXN];
int q[MAXN];

void addEdge(int u, int v) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->v = v;
    n->next = adj[u];
    adj[u] = n;
}

int main() {
    int N, M;

    if (scanf("%d %d", &N, &M) != 2)
        return 0;

    for (int i = 0; i < MAXN; i++) {
        adj[i] = NULL;
        indeg[i] = 0;
    }

    for (int i = 0; i < N; i++) {
        int id;

        scanf("%d", &id);
        scanf("%50s", name[id]);
    }

    for (int i = 0; i < M; i++) {
        int u, v;

        scanf("%d %d", &u, &v);

        addEdge(u, v);

        indeg[v]++;
    }

    int front = 0;
    int rear = 0;

    for (int i = 1; i <= N; i++) {
        if (indeg[i] == 0)
            q[rear++] = i;
    }

    int processed = 0;

    while (front < rear) {

        int u = q[front++];

        printf("%s\n", name[u]);

        processed++;

        for (Node *p = adj[u]; p != NULL; p = p->next) {

            indeg[p->v]--;

            if (indeg[p->v] == 0)
                q[rear++] = p->v;
        }
    }

    if (processed != N)
        printf("IMPOSSIVEL\n");

    return 0;
}