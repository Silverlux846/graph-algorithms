#include <stdio.h>
#include <string.h>

#define MAX 1005

typedef struct {
    int l;
    int c;
} Posicao;

char mapa[MAX][MAX];
int visitado[MAX][MAX];

int paiL[MAX][MAX];
int paiC[MAX][MAX];

char mov[MAX][MAX];

Posicao fila[MAX * MAX];

int dl[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

char letra[4] = {'C', 'B', 'E', 'D'};

int main() {

    int N, M;

    scanf("%d %d", &N, &M);

    int iniL, iniC;
    int fimL, fimC;

    for(int i = 0; i < N; i++) {

        for(int j = 0; j < M; j++) {

            scanf(" %c", &mapa[i][j]);

            if(mapa[i][j] == 'A') {
                iniL = i;
                iniC = j;
            }

            if(mapa[i][j] == 'R') {
                fimL = i;
                fimC = j;
            }
        }
    }

    int inicio = 0;
    int fim = 0;

    fila[fim++] = (Posicao){iniL, iniC};

    visitado[iniL][iniC] = 1;

    while(inicio < fim) {

        Posicao atual = fila[inicio++];

        if(atual.l == fimL && atual.c == fimC)
            break;

        for(int k = 0; k < 4; k++) {

            int nl = atual.l + dl[k];
            int nc = atual.c + dc[k];

            if(nl < 0 || nl >= N)
                continue;

            if(nc < 0 || nc >= M)
                continue;

            if(mapa[nl][nc] == '@')
                continue;

            if(visitado[nl][nc])
                continue;

            visitado[nl][nc] = 1;

            paiL[nl][nc] = atual.l;
            paiC[nl][nc] = atual.c;

            mov[nl][nc] = letra[k];

            fila[fim++] = (Posicao){nl, nc};
        }
    }

    if(!visitado[fimL][fimC]) {

        printf("VOU MORRER DE FOME\n");

        return 0;
    }

    char caminho[MAX * MAX];

    int tam = 0;

    int l = fimL;
    int c = fimC;

    while(!(l == iniL && c == iniC)) {

        caminho[tam++] = mov[l][c];

        int pl = paiL[l][c];
        int pc = paiC[l][c];

        l = pl;
        c = pc;
    }

    printf("AMEM\n");

    for(int i = tam - 1; i >= 0; i--)
        printf("%c", caminho[i]);

    printf("\n");

    printf("%d\n", tam);

    return 0;
}