#include <stdio.h>
#include <string.h> // Necessário para manipulação de strings

struct Territorio {
    char nome [30];
    char cor [10];
    int tropas;
};

int main () {
    struct Territorio territorios [5]; // Vetor para armazenar 5 territórios
    int i;

    printf("====== Cadastro de Território ====== \n\n");

    //Entradas de dados dos 5 Territórios
    for (i = 0; i < 5; i++) {
        printf("Território %d: \n", i + 1);
       
        // Lendo o nome do território
        printf("Nome: ");
        // fgets é mais seguro do que scanf(%s) pois permite espaço
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // remove o \n

        // Lendo a cor do Exército
        printf("Cor do Exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        // Lendo o número de tropas
        printf("Número de Tropas: ");
        scanf("%d", &territorios[i].tropas);

        getchar(); // Limpar o buffer do Enter após o scanf

        printf("\n");
    }
    
    // Exibição dos dados cadastrados
    printf("====== Territórios Cadastrados ====== \n");
    for (i = 0; i < 5; i++) {
    printf("Território: %d \n", i + 1);
    printf("Nome: %s \n", territorios[i].nome);
    printf("Cor do Exército: %s \n", territorios[i].cor);
    printf("Tropas: %d \n\n", territorios[i].tropas);
    }

    return 0;
}
