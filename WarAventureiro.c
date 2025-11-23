#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Struct do território */
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

/* Protótipos */
void imprimirMapa(struct Territorio *mapa, int n);
void atacar(struct Territorio *atacante, struct Territorio *defensor);
void liberarMemoria(struct Territorio *mapa);
void lerString(char *destino, int tamanho, const char *mensagem);
int lerInteiro(const char *mensagem);

/* Função robusta para ler string evitando ENTER solto */
void lerString(char *destino, int tamanho, const char *mensagem) {
    while (1) {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("Erro ao ler entrada.\n");
            continue;
        }

        destino[strcspn(destino, "\n")] = '\0'; // remove \n

        if (strlen(destino) == 0) {
            printf("Entrada vazia não é permitida. Tente novamente.\n");
            continue;
        }
        break;
    }
}

/* Função robusta para ler inteiros, limpando buffer sempre */
int lerInteiro(const char *mensagem) {
    int valor;
    char buffer[100];

    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &valor) != 1) {
            printf("Valor inválido. Digite um número inteiro.\n");
            continue;
        }

        return valor;
    }
}

int main() {
    int n, i;
    struct Territorio *mapa = NULL;

    srand((unsigned int) time(NULL));

    printf("=== Simulação de Territórios ===\n\n");

    /* Leitura robusta do número de territórios */
    n = lerInteiro("Informe o número de territórios a cadastrar: ");
    if (n <= 0) {
        printf("Número inválido.\n");
        return 1;
    }

    mapa = (struct Territorio *) calloc((size_t)n, sizeof(struct Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    /* Cadastro */
    for (i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);

        lerString(mapa[i].nome, sizeof(mapa[i].nome), "Nome: ");
        lerString(mapa[i].cor, sizeof(mapa[i].cor), "Cor do exército (dono): ");

        /* Ler tropas com validação */
        mapa[i].tropas = lerInteiro("Número de tropas (>= 0): ");
        while (mapa[i].tropas < 0) {
            mapa[i].tropas = lerInteiro("Valor inválido. Digite >= 0: ");
        }
    }

    /* Mostra mapa inicial */
    printf("\nMapa cadastrado:\n");
    imprimirMapa(mapa, n);

    /* Loop de ataques */
    while (1) {
        printf("\nDigite -1 para sair.\n");

        int idxAtacante = lerInteiro("Número do território atacante: ");
        if (idxAtacante == -1) break;

        int idxDefensor = lerInteiro("Número do território defensor: ");
        if (idxDefensor == -1) break;

        if (idxAtacante < 1 || idxAtacante > n ||
            idxDefensor < 1 || idxDefensor > n) {
            printf("Índices inválidos.\n");
            continue;
        }

        if (idxAtacante == idxDefensor) {
            printf("Um território não pode atacar a si mesmo.\n");
            continue;
        }

        struct Territorio *atac = &mapa[idxAtacante - 1];
        struct Territorio *def = &mapa[idxDefensor - 1];

        if (strcmp(atac->cor, def->cor) == 0) {
            printf("Ataque inválido: ambos pertencem à cor %s.\n", atac->cor);
            continue;
        }

        if (atac->tropas <= 0) {
            printf("Território atacante não tem tropas.\n");
            continue;
        }

        atacar(atac, def);
        imprimirMapa(mapa, n);
    }

    liberarMemoria(mapa);
    printf("\nPrograma encerrado.\n");
    return 0;
}

/* Imprime o mapa */
void imprimirMapa(struct Territorio *mapa, int n) {
    int i;
    printf("\n--- Estado Atual dos Territórios ---\n");
    for (i = 0; i < n; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome : %s\n", mapa[i].nome);
        printf("  Cor  : %s\n", mapa[i].cor);
        printf("  Tropas: %d\n", mapa[i].tropas);
    }
}

/* Lógica do ataque */
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- Confronto ---\n");
    printf("%s (tropas: %d) lança: %d\n",
           atacante->nome, atacante->tropas, dadoAtacante);
    printf("%s (tropas: %d) lança: %d\n",
           defensor->nome, defensor->tropas, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        int transferencia = atacante->tropas / 2;
        if (transferencia < 1) transferencia = 1;

        strcpy(defensor->cor, atacante->cor);

        atacante->tropas -= transferencia;
        defensor->tropas += transferencia;

        printf("ATACANTE VENCEU! %d tropas foram transferidas.\n", transferencia);
        printf("%s agora pertence à cor %s.\n", defensor->nome, defensor->cor);

    } else {
        if (atacante->tropas > 0)
            atacante->tropas -= 1;

        printf("ATACANTE PERDEU! %s perdeu 1 tropa.\n", atacante->nome);
    }
}

/* Libera memória */
void liberarMemoria(struct Territorio *mapa) {
    free(mapa);
}
