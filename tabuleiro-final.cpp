#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    char nome[5];
    int pontuacao;
} Jogador;

//Exibe o tabuleiro
void exibir_tabuleiro(char** tabuleiro, int tamanho) {
    printf("\n  ");
    for (int i = 1; i <= tamanho; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < tamanho; i++) {
        printf("%d ", i + 1);
        for (int j = 0; j < tamanho; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//Utiliza condições para verificar a vitória
int verificar_vitoria(char** tabuleiro, int tamanho, char jogador) {
    for (int i = 0; i < tamanho; i++) {
        int linha = 1;
        int coluna = 1;
        for (int j = 0; j < tamanho; j++) {
            if (tabuleiro[i][j] != jogador) {
                linha = 0;
            }
            if (tabuleiro[j][i] != jogador) {
                coluna = 0;
            }
        }
        if (linha || coluna) {
            return 1;  // Jogador venceu
        }
    }

    int diagonal1 = 1;
    int diagonal2 = 1;
    for (int i = 0; i < tamanho; i++) {
        if (tabuleiro[i][i] != jogador) {
            diagonal1 = 0;
        }
        if (tabuleiro[i][tamanho - i - 1] != jogador) {
            diagonal2 = 0;
        }
    }

    if (diagonal1 || diagonal2) {
        return 1;  // Jogador venceu
    }

    return 0;  // Ninguém venceu ainda
}

//Caso todas as casas estejam ocupadas e a vitória não aconteça, considera empate
int verificar_empate(char** tabuleiro, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (tabuleiro[i][j] == ' ') {
                return 0;  // Ainda há espaços vazios, o jogo não está empatado
            }
        }
    }
    return 1;  // Todas as posições estão ocupadas, o jogo está empatado
}

//Ao ganhar, adiciona 1 ao ranking do jogador vencedor
void atualizar_ranking(Jogador ranking[], int num_jogadores, char jogador) {
    for (int i = 0; i < num_jogadores; i++) {
        if (ranking[i].nome[0] == jogador) {
            ranking[i].pontuacao++;
            return;
        }
    }
}

//Controla as jogadas do bot, usando 33% de chance na vertical e 33% na horizontal, apenas em casas vazias
void jogada_bot(char** tabuleiro, int tamanho, char jogador) {
    int linha, coluna;

    printf("Jogador %c fazendo a jogada...\n", jogador);

    do {
        linha = rand() % tamanho;
        coluna = rand() % tamanho;
    } while (tabuleiro[linha][coluna] != ' ');

    tabuleiro[linha][coluna] = jogador;
}

//Exibe o ranking
void exibir_ranking(Jogador ranking[], int num_jogadores) {
    printf("\nRanking:\n");
    for (int i = 0; i < num_jogadores; i++) {
        printf("%s: %d\n", ranking[i].nome, ranking[i].pontuacao);
    }
    printf("\n");
}

//Salva o ranking em um arquivo "ranking.txt" externo
void salvar_ranking_em_arquivo(Jogador ranking[], int num_jogadores) {
    FILE* arquivo = fopen("ranking.txt", "w+");
    fprintf(arquivo, "Ranking:\n");
    for (int i = 0; i < num_jogadores; i++) {
        fprintf(arquivo, "%s %d\n", ranking[i].nome, ranking[i].pontuacao);
    }
    fclose(arquivo);
}

//reseta o ranking para zero
void resetar_ranking(Jogador ranking[], int num_jogadores) {
    for (int i = 0; i < num_jogadores; i++) {
        ranking[i].pontuacao = 0;
    }

    FILE* arquivo = fopen("ranking.txt", "w");
    fprintf(arquivo, "Ranking:\n");
    for (int i = 0; i < num_jogadores; i++) {
        fprintf(arquivo, "%s %d\n", ranking[i].nome, ranking[i].pontuacao);
    }
    fclose(arquivo);

    printf("---------\nRanking resetado.\n----------\n"); //isso foi difícil de conseguir fazer
}


//Ao exibir o ranking, puxa as informações do "ranking.txt" e exibe no "exibir_ranking"
void carregar_ranking_do_arquivo(Jogador ranking[], int num_jogadores) {
    FILE* arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de ranking.\n");
        return;
    }

    char buffer[100];
    fgets(buffer, sizeof(buffer), arquivo);

    for (int i = 0; i < num_jogadores; i++) {
        fscanf(arquivo, "%s %d", ranking[i].nome, &ranking[i].pontuacao);
    }

    fclose(arquivo);
}

//Usa alocação de memória para criar o tabuleiro
char** criar_tabuleiro(int tamanho) {
    char** tabuleiro = (char**)malloc(tamanho * sizeof(char*));
    for (int i = 0; i < tamanho; i++) {
        tabuleiro[i] = (char*)malloc(tamanho * sizeof(char));
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
    return tabuleiro;
}

//Ao encerrar qualquer partida, reinicia o tabuleiro 
void reiniciar_tabuleiro(char** tabuleiro, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
}

//Ao reiniciar o tabuleiro, limpa a memória alocada e libera as casas
void liberar_tabuleiro(char** tabuleiro, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}

//Função principal de funcionamento do jogo, determinando os jogadores e seus comandos
int main() {
    setlocale(0, "Portuguese");
    int tamanho = 3; // Tamanho do tabuleiro
    char** tabuleiro = criar_tabuleiro(tamanho);
    int linha, coluna;
    char jogador = 'X';

    Jogador ranking[2] = {{"X", 0}, {"O", 0}};
    int num_jogadores = 2;
	
	//Mostra as opções do menu
    int opcao;
    do {
        printf("Menu:\n");
        printf("1. Iniciar jogo\n");
        printf("2. Jogar sozinho\n");
        printf("3. Exibir o ranking\n");
        printf("4. Resetar ranking\n");
        printf("5. Créditos\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
		
		//Usa um Switch case para escolher o menu, cada case determina qual o código que irá entrar em funcionamento
        switch (opcao) {
            case 1:
                do {
                    exibir_tabuleiro(tabuleiro, tamanho);

                    printf("Jogador %c, informe a linha (1-%d) e coluna (1-%d) separadas por espaco: ", jogador, tamanho, tamanho);
                    scanf("%d %d", &linha, &coluna);

                    if (linha >= 1 && linha <= tamanho && coluna >= 1 && coluna <= tamanho && tabuleiro[linha - 1][coluna - 1] == ' ') {
                        tabuleiro[linha - 1][coluna - 1] = jogador;

                        if (verificar_vitoria(tabuleiro, tamanho, jogador)) {
                            exibir_tabuleiro(tabuleiro, tamanho);
                            printf("Jogador %c venceu!\n", jogador);

                            atualizar_ranking(ranking, num_jogadores, jogador);
                            exibir_ranking(ranking, num_jogadores);
                            salvar_ranking_em_arquivo(ranking, num_jogadores);

                            reiniciar_tabuleiro(tabuleiro, tamanho);
                            break;
                        } else if (verificar_empate(tabuleiro, tamanho)) {
                            exibir_tabuleiro(tabuleiro, tamanho);
                            printf("O jogo terminou em empate!\n");

                            reiniciar_tabuleiro(tabuleiro, tamanho);
                            break;
                        }

                        jogador = (jogador == 'X') ? 'O' : 'X';
                    } else {
                        printf("Posicao invalida. Escolha outra.\n");
                    }
                } while (1);
                break;

            case 2:
                do {
                    exibir_tabuleiro(tabuleiro, tamanho);

                    printf("Jogador %c, informe a linha (1-%d) e coluna (1-%d) separadas por espaco: ", jogador, tamanho, tamanho);
                    scanf("%d %d", &linha, &coluna);

                    if (linha >= 1 && linha <= tamanho && coluna >= 1 && coluna <= tamanho && tabuleiro[linha - 1][coluna - 1] == ' ') {
                        tabuleiro[linha - 1][coluna - 1] = jogador;

                        if (verificar_vitoria(tabuleiro, tamanho, jogador)) {
                            exibir_tabuleiro(tabuleiro, tamanho);
                            printf("Jogador %c venceu!\n", jogador);

                            atualizar_ranking(ranking, num_jogadores, jogador);
                            exibir_ranking(ranking, num_jogadores);
                            salvar_ranking_em_arquivo(ranking, num_jogadores);

                            reiniciar_tabuleiro(tabuleiro, tamanho);
                            break;
                        } else if (verificar_empate(tabuleiro, tamanho)) {
                            exibir_tabuleiro(tabuleiro, tamanho);
                            printf("O jogo terminou em empate!\n");

                            reiniciar_tabuleiro(tabuleiro, tamanho);
                            break;
                        }

                        jogador = (jogador == 'X') ? 'O' : 'X';
                    } else {
                        printf("Posicao invalida. Escolha outra.\n");
                    }

                    jogada_bot(tabuleiro, tamanho, jogador);

                    if (verificar_vitoria(tabuleiro, tamanho, jogador)) {
                        exibir_tabuleiro(tabuleiro, tamanho);
                        printf("Jogador %c venceu!\n", jogador);

                        atualizar_ranking(ranking, num_jogadores, jogador);
                        exibir_ranking(ranking, num_jogadores);
                        salvar_ranking_em_arquivo(ranking, num_jogadores);

                        reiniciar_tabuleiro(tabuleiro, tamanho);
                        break;
                    } else if (verificar_empate(tabuleiro, tamanho)) {
                        exibir_tabuleiro(tabuleiro, tamanho);
                        printf("O jogo terminou em empate!\n");

                        reiniciar_tabuleiro(tabuleiro, tamanho);
                        break;
                    }

                    jogador = (jogador == 'X') ? 'O' : 'X';

                } while (1);
                break;

            case 3:
                carregar_ranking_do_arquivo(ranking, num_jogadores);
                exibir_ranking(ranking, num_jogadores);
                break;
                
            case 4:
            resetar_ranking(ranking, num_jogadores);
            break;

            case 5:
                printf("----------------\nCréditos:\nGustavo Hudson\nJosé Geraldo\nBeatriz Ramalho\nDaniel Ferreira\nPedro Henrique Porto\n------------------\n");
                break;

            case 6:
                printf("-----\nSaindo do jogo.\n-----\n");
                break;
        }

    } while (opcao != 6);

    liberar_tabuleiro(tabuleiro, tamanho);
//Retornar 0 significa exito, enquanto outros valores irão significar erro no funcionamento, por isso o uso do "return 0"
    return 0;
}

