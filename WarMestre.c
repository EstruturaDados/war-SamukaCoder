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

/* Novas funções de missão */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(const char *missao, struct Territorio *mapa, int tamanho, const char *corJogador, const char *corAdversario);

void lerString(char *destino, int tamanho, const char *mensagem) {
    while (1) {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("Erro ao ler entrada. Tente novamente.\n");
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

int lerInteiro(const char *mensagem) {
    int valor;
    char buffer[128];
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

/* Sorteia uma missão e copia para 'destino' (destino deve ter espaço alocado) */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]); // destino deve ter espaço suficiente
}

/*
  verificarMissao:
  - missao: string com a descrição da missão
  - mapa/tamanho: vetor de territórios
  - corJogador: cor do jogador que tem essa missão
  - corAdversario: cor do adversário (quando necessário)
  Retorna 1 se cumpriu, 0 caso contrário.
  Implementa lógica simples baseada no texto da missão.
*/
int verificarMissao(const char *missao, struct Territorio *mapa, int tamanho, const char *corJogador, const char *corAdversario) {
    int i;
    int countControlados = 0;
    int tropasJogador = 0;
    int tropasAdversario = 0;
    int maxTropas = -1;
    int controlaMax = 0;

    for (i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            countControlados++;
            tropasJogador += mapa[i].tropas;
        }
        if (strcmp(mapa[i].cor, corAdversario) == 0) {
            tropasAdversario += mapa[i].tropas;
        }
        if (mapa[i].tropas > maxTropas) {
            maxTropas = mapa[i].tropas;
            controlaMax = (strcmp(mapa[i].cor, corJogador) == 0) ? 1 : 0;
        } else if (mapa[i].tropas == maxTropas) {
            /* caso de empate no maior, se algum dos controladores for o jogador, considera controle como 1 */
            if (strcmp(mapa[i].cor, corJogador) == 0) controlaMax = 1;
        }
    }

    /* Comparações por missão (verificações simples) */
    if (strcmp(missao, "Controlar 3 territórios") == 0) {
        return (countControlados >= 3) ? 1 : 0;
    } else if (strcmp(missao, "Ter mais tropas que o adversario") == 0) {
        return (tropasJogador > tropasAdversario) ? 1 : 0;
    } else if (strcmp(missao, "Eliminar todas as tropas do adversario") == 0) {
        return (tropasAdversario == 0) ? 1 : 0;
    } else if (strcmp(missao, "Ter pelo menos 20 tropas no total") == 0) {
        return (tropasJogador >= 20) ? 1 : 0;
    } else if (strcmp(missao, "Controlar o território com mais tropas") == 0) {
        return controlaMax ? 1 : 0;
    }

    /* Se missão desconhecida, retorna 0 (não cumprida) */
    return 0;
}

/* Função para imprimir o estado do mapa */
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

/* Lógica do ataque (mantida igual) */
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

        /* Atualiza cor do defensor para a cor do atacante */
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';

        /* Transfere tropas */
        if (transferencia > atacante->tropas) transferencia = atacante->tropas;
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

/* Libera memória alocada para o mapa */
void liberarMemoria(struct Territorio *mapa) {
    free(mapa);
}

int main() {
    int n, i;
    struct Territorio *mapa = NULL;

    /* Lista de missões possíveis (pelo menos 5) */
    char *missoes[] = {
        "Controlar 3 territórios",
        "Ter mais tropas que o adversario",
        "Eliminar todas as tropas do adversario",
        "Ter pelo menos 20 tropas no total",
        "Controlar o território com mais tropas"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    /* Inicializa RNG */
    srand((unsigned int) time(NULL));

    printf("=== Simulação de Territórios COM MISSÕES ===\n\n");

    /* Ler número de territórios */
    n = lerInteiro("Informe o número de territórios a cadastrar: ");
    if (n <= 0) {
        printf("Número inválido.\n");
        return 1;
    }

    /* Aloca mapa */
    mapa = (struct Territorio *) calloc((size_t)n, sizeof(struct Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    /* Ler dados dos territórios */
    for (i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);
        lerString(mapa[i].nome, sizeof(mapa[i].nome), "Nome: ");
        lerString(mapa[i].cor, sizeof(mapa[i].cor), "Cor do exército (dono): ");
        mapa[i].tropas = lerInteiro("Número de tropas (>= 0): ");
        while (mapa[i].tropas < 0) {
            mapa[i].tropas = lerInteiro("Valor inválido. Digite >= 0: ");
        }
    }

    /* --------- Cadastro dos jogadores e atribuição de missões --------- */
    char jogador1Nome[30], jogador2Nome[30];
    char jogador1Cor[10], jogador2Cor[10];

    printf("\n--- Cadastro de Jogadores ---\n");
    lerString(jogador1Nome, sizeof(jogador1Nome), "Nome do Jogador 1: ");
    lerString(jogador1Cor, sizeof(jogador1Cor), "Cor do Jogador 1 (ex: Vermelho): ");

    lerString(jogador2Nome, sizeof(jogador2Nome), "Nome do Jogador 2: ");
    lerString(jogador2Cor, sizeof(jogador2Cor), "Cor do Jogador 2 (ex: Azul): ");

    /* Aloca dinamicamente a missão de cada jogador (malloc) */
    char *missaoJog1 = (char *) malloc(100 * sizeof(char)); // 100 bytes para segurança
    char *missaoJog2 = (char *) malloc(100 * sizeof(char));
    if (!missaoJog1 || !missaoJog2) {
        printf("Erro ao alocar memória para missões.\n");
        free(missaoJog1);
        free(missaoJog2);
        liberarMemoria(mapa);
        return 1;
    }

    atribuirMissao(missaoJog1, missoes, totalMissoes);
    atribuirMissao(missaoJog2, missoes, totalMissoes);

    printf("\nMissão de %s: %s\n", jogador1Nome, missaoJog1);
    printf("Missão de %s: %s\n", jogador2Nome, missaoJog2);

    /* Mostra mapa inicial */
    imprimirMapa(mapa, n);

    /* Loop principal de ataques com verificação de missão após cada turno */
    while (1) {
        printf("\nDigite -1 para encerrar.\n");
        int idxAtacante = lerInteiro("Número do território atacante: ");
        if (idxAtacante == -1) break;
        int idxDefensor = lerInteiro("Número do território defensor: ");
        if (idxDefensor == -1) break;

        if (idxAtacante < 1 || idxAtacante > n || idxDefensor < 1 || idxDefensor > n) {
            printf("Índices inválidos. Tente novamente.\n");
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

        /* Realiza ataque */
        atacar(atac, def);

        /* Mostra mapa atualizado */
        imprimirMapa(mapa, n);

        /* Verifica missões de ambos os jogadores */
        int cumpriu1 = verificarMissao(missaoJog1, mapa, n, jogador1Cor, jogador2Cor);
        int cumpriu2 = verificarMissao(missaoJog2, mapa, n, jogador2Cor, jogador1Cor);

        if (cumpriu1 && cumpriu2) {
            printf("\nEmpate! Ambos os jogadores cumpriram suas missões ao mesmo tempo.\n");
            break;
        } else if (cumpriu1) {
            printf("\nParabéns, %s! Você cumpriu sua missão: %s\n", jogador1Nome, missaoJog1);
            break;
        } else if (cumpriu2) {
            printf("\nParabéns, %s! Você cumpriu sua missão: %s\n", jogador2Nome, missaoJog2);
            break;
        }
    }

    /* Libera missões e mapa */
    free(missaoJog1);
    free(missaoJog2);
    liberarMemoria(mapa);

    printf("\nPrograma encerrado.\n");
    return 0;
}
