#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX_PALAVRAS 20
#define MAX_TAM_PALAVRA 20
#define MAX_LETRAS 20

// =====================
// GLOBAL VARIABLES
// =====================
FILE *fpEntrada;
FILE *fpRanking;

char nickname[20];
char letrasFase[MAX_LETRAS];
char palavras[MAX_PALAVRAS][MAX_TAM_PALAVRA];
int palavrasCorretas[MAX_PALAVRAS];
int qtdPalavras;
int pontuacao = 0;

// =====================
// STRUCT FOR RANKING
// =====================
typedef struct {
    char nickname[20];
    int pontuacao;
} dadosjogador;

// =====================
// FUNCTION PROTOTYPES
// =====================
void menuPrincipal();
void jogar();
void instrucoes();
void configuracoes();
void rankingMostrar();
void zerarRanking();
void salvarRanking();

void carregarFase();
int verificarPalavra(char palavra[]);
void mostrarPainel();
void mostrarStatusFase();

// =====================
// MAIN
// =====================
int main() {
    system(CLEAR);

    printf("Bem vindo(a) ao jogo WORD MAGIC!!!!\n");
    printf("Informe seu nickname: ");
    scanf("%s", nickname);

    menuPrincipal();
    return 0;
}

// =====================
// MENU PRINCIPAL
// =====================
void menuPrincipal() {
    int opc;

    do {
        system(CLEAR);
        printf("*** JOGO WORD MAGIC ***\n\n");
        printf("1 - Jogar\n");
        printf("2 - Configuracoes\n");
        printf("3 - Instrucoes\n");
        printf("4 - Ranking\n");
        printf("5 - Sair\n");
        printf("\nDigite a opcao desejada: ");
        scanf("%d", &opc);

        switch(opc) {
            case 1: jogar(); break;
            case 2: configuracoes(); break;
            case 3: instrucoes(); break;
            case 4: rankingMostrar(); break;
            case 5: exit(0);
            default: break;
        }

    } while(1);
}
void jogar() {
    system(CLEAR);

    fpEntrada = fopen("entrada.txt", "r");
    if (!fpEntrada) {
        printf("Erro ao abrir arquivo de fases.\n");
        return;
    }

    pontuacao = 0;

    // -------- JOGAR TODAS AS FASES --------
    while (!feof(fpEntrada)) {

        carregarFase();
    
        system(CLEAR);
        printf("Quantidade de palavras: %d\n", qtdPalavras);

        mostrarPainel();   

        int palavrasEncontradas = 0;

        while (palavrasEncontradas < qtdPalavras) {

            printf("\nEncontre %d palavras:\n", qtdPalavras);
            for (int i = 0; i < qtdPalavras; i++) {
                if (palavrasCorretas[i] == 1)
                    printf("%s\n", palavras[i]);
                else {
                    for (int j = 0; j < strlen(palavras[i]); j++)
                        printf("_");
                    printf("\n");
                }
            }

            char tentativa[50];
            printf("\nInforme uma palavra: ");
            scanf("%s", tentativa);

            if (strcmp(tentativa, "sair") == 0 || strcmp(tentativa, "SAIR") == 0) {
                printf("Saindo da fase...\n");
                break;
            }

            if (!verificarPalavra(tentativa)) {
                printf("A palavra usa letras invalidas! -10 pontos\n");
                pontuacao -= 10;
                continue;
            }

            int achou = -1;
            for (int i = 0; i < qtdPalavras; i++) {
                if (strcmp(tentativa, palavras[i]) == 0) {
                    achou = i;
                    break;
                }
            }

            if (achou == -1) {
                printf("Palavra inexistente! -10 pontos\n");
                pontuacao -= 10;
            } 
            else {
                if (palavrasCorretas[achou] == 1) {
                    printf("Esta palavra já foi encontrada! -10 pontos\n");
                    pontuacao -= 10;
                } else {
                    printf("Parabéns! Você acertou uma palavra! +100 pontos\n");
                    palavrasCorretas[achou] = 1;
                    pontuacao += 100;
                    palavrasEncontradas++;
                }
            }

        } 

        printf("\nParabéns! Você completou a fase! +25 pontos\n");
        pontuacao += 25;

        printf("Pontuação atual: %d\n", pontuacao);

        printf("\nDeseja continuar para a próxima fase? (s/n): ");
        char opc;
        scanf(" %c", &opc);
        if (opc == 'n' || opc == 'N') break;
    }

    fclose(fpEntrada);

    salvarRanking();

    printf("\nJogo encerrado. Pontuação final: %d\n", pontuacao);
    printf("Pressione ENTER para voltar ao menu...");
    getchar(); getchar();
}

void configuracoes() {
    int opc;
    printf("1 - Zerar ranking\n");
    printf("2 - Voltar\n");
    scanf("%d", &opc);

    if (opc == 1) zerarRanking();
}

void instrucoes() {
    system(CLEAR);

    printf("=========== INSTRUCOES DO JOGO ===========\n\n");
    printf("Objetivo:\n");
    printf("  Formar todas as palavras escondidas utilizando as letras fornecidas.\n\n");

    printf("Como jogar:\n");
    printf("  - A cada fase, voce recebera um conjunto de letras.\n");
    printf("  - Um painel mostrara essas letras em um grid.\n");
    printf("  - Abaixo, palavras ocultas aparecem como '____'.\n");
    printf("  - Digite palavras que possam ser formadas usando as letras dadas.\n");
    printf("  - Para cada palavra correta voce ganha +100 pontos.\n");
    printf("  - Palavras repetidas ou inexistentes geram -10 pontos.\n");
    printf("  - Ao completar todas as palavras da fase, voce ganha +25 pontos.\n\n");

    printf("Comandos:\n");
    printf("  - Digite 'sair' durante a fase para sair antecipadamente.\n");
    printf("  - Digite apenas letras minusculas ou maiusculas (sem acentos).\n\n");

    printf("Pontuacao:\n");
    printf("  - Palavra correta: +100 pontos\n");
    printf("  - Palavra repetida: -10 pontos\n");
    printf("  - Palavra inexistente: -10 pontos\n");
    printf("  - Bonus por completar a fase: +25 pontos\n\n");

    printf("Ranking:\n");
    printf("  - Sua pontuacao final sera salva no arquivo 'ranking.bin'.\n");
    printf("  - O ranking mostra jogadores ordenados pela maior pontuacao.\n\n");

    printf("===========================================\n");
    printf("Pressione ENTER para voltar ao menu...\n");

    getchar(); getchar();
}

void rankingMostrar() {
    system(CLEAR);

    fpRanking = fopen("ranking.bin", "rb");
    if (!fpRanking) {
        printf("Nenhum ranking encontrado.\n");
        printf("Pressione ENTER para voltar...");
        getchar(); getchar();
        return;
    }

    dadosjogador lista[100];
    int count = 0;

    while (fread(&lista[count], sizeof(dadosjogador), 1, fpRanking) == 1) {
        count++;
    }
    fclose(fpRanking);

    if (count == 0) {
        printf("Ranking vazio.\n");
        printf("Pressione ENTER para voltar...");
        getchar(); getchar();
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (lista[j].pontuacao < lista[j + 1].pontuacao) {
                dadosjogador temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    printf("===== RANKING =====\n\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s — %d pontos\n", i + 1, lista[i].nickname, lista[i].pontuacao);
    }

    printf("\nPressione ENTER para voltar...");
    getchar(); getchar();
}
void zerarRanking() {
    fpRanking = fopen("ranking.bin", "wb");
    if (fpRanking) {
        fclose(fpRanking);
        printf("Ranking zerado com sucesso!\n");
    } else {
        printf("Erro ao zerar ranking!\n");
    }
    getchar(); getchar();
}
void carregarFase() {

    memset(letrasFase, 0, sizeof(letrasFase));
    memset(palavras, 0, sizeof(palavras));
    memset(palavrasCorretas, 0, sizeof(palavrasCorretas));

    fscanf(fpEntrada, "%s", letrasFase);

    fscanf(fpEntrada, "%d", &qtdPalavras);

    for (int i = 0; i < qtdPalavras; i++) {
        fscanf(fpEntrada, "%s", palavras[i]);
        palavrasCorretas[i] = 0;
    }

    char separator[5];
    fscanf(fpEntrada, "%s", separator);
}
void mostrarPainel() {
    printf("\nPainel de Letras:\n\n");

    int tam = strlen(letrasFase);

    int grid;
    if (tam <= 4) grid = 2;
    else if (tam <= 9) grid = 3;
    else if (tam <= 16) grid = 4;
    else grid = 5;

    int index = 0;

    for (int i = 0; i < grid; i++) {
        for (int j = 0; j < grid; j++) {
            if (index < tam)
                printf("%c   ", letrasFase[index++]);
            else
                printf("_   "); 
        }
        printf("\n");
    }

    printf("\n");
}
void salvarRanking() {
    fpRanking = fopen("ranking.bin", "ab");
    if (!fpRanking) {
        printf("Erro ao abrir ranking.bin\n");
        return;
    }

    dadosjogador player;
    strcpy(player.nickname, nickname);
    player.pontuacao = pontuacao;

    fwrite(&player, sizeof(dadosjogador), 1, fpRanking);
    fclose(fpRanking);
}
int verificarPalavra(char tentativa[]) {
    int freqLetras[256] = {0};
    int freqTentativa[256] = {0};

    for (int i = 0; i < strlen(letrasFase); i++) {
        char c = toupper(letrasFase[i]);
        freqLetras[(int)c]++;
    }

    for (int i = 0; i < strlen(tentativa); i++) {
        char c = toupper(tentativa[i]);

        if (!isalpha(c))
            return 0;

        freqTentativa[(int)c]++;
    }

    for (int i = 0; i < 256; i++) {
        if (freqTentativa[i] > freqLetras[i]) {
            return 0;
        }
    }

    return 1;
}