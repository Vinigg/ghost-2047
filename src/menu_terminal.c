#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para salvar o resultado no arquivo txt
void salvar_historico(const char* status, int tentativas, int senha) {
    // O "a" significa "append" (adiciona no final do arquivo sem apagar o resto)
    FILE *arquivo = fopen("historico.txt", "a");
    
    if (arquivo == NULL) {
        printf("\n[ERRO] Falha ao acessar o banco de dados do historico.\n");
        return;
    }
    
    fprintf(arquivo, "[LOG] Status: %s | Tentativas: %d/7 | Senha: %d\n", status, tentativas, senha);
    fclose(arquivo);
}

void iniciar_jogo() {
    int senha_secreta, palpite;
    int tentativas = 0, max_tentativas = 7;
    int min_atual = 1, max_atual = 100;

    // Gera um número aleatório entre 1 e 100
    srand(time(NULL));
    senha_secreta = rand() % 100 + 1;

    system("cls || clear");
    printf("==========================================\n");
    printf("           PROTOCOLO INICIADO             \n");
    printf("==========================================\n");

    while (tentativas < max_tentativas) {
        printf("\nTENTATIVA %d/%d | Intervalo Seguro: [%d - %d]\n", tentativas + 1, max_tentativas, min_atual, max_atual);
        printf(">>> Digite a senha numerica: ");
        
        if (scanf("%d", &palpite) != 1) {
            while(getchar() != '\n'); 
            printf("[ERRO] Entrada invalida.\n");
            continue;
        }

        tentativas++;

        if (palpite == senha_secreta) {
            printf("\n[!!!] ACESSO CONCEDIDO [!!!]\n");
            printf("Senha descoberta com sucesso!\n");
            salvar_historico("VITORIA", tentativas, senha_secreta);
            break;
        } else if (palpite < senha_secreta) {
            printf("[X] ALERTA: A senha correta e MAIOR que %d.\n", palpite);
            if (palpite >= min_atual) min_atual = palpite + 1;
        } else {
            printf("[X] ALERTA: A senha correta e MENOR que %d.\n", palpite);
            if (palpite <= max_atual) max_atual = palpite - 1;
        }
    }

    if (tentativas >= max_tentativas && palpite != senha_secreta) {
        printf("\n[BLOQUEADO] RASTREAMENTO CONCLUIDO.\n");
        printf("Acesso negado. A senha era: %d\n", senha_secreta);
        salvar_historico("DERROTA", tentativas, senha_secreta);
    }

    printf("\nPressione ENTER para voltar ao menu...");
    getchar(); 
    getchar(); 
}

void exibir_analise() {
    system("cls || clear");
    printf("==========================================\n");
    printf("        ANALISE ESTATISTICA: SESSÕES      \n");
    printf("==========================================\n\n");

    // O "r" significa "read" (apenas leitura do arquivo)
    FILE *arquivo = fopen("historico.txt", "r");
    
    if (arquivo == NULL) {
        printf("NENHUM DADO DISPONIVEL. INICIE UMA SESSAO PRIMEIRO.\n");
    } else {
        char linha[150];
        int total_sessoes = 0;
        
        // Lê linha por linha do arquivo de texto
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            printf("%s", linha);
            total_sessoes++;
        }
        fclose(arquivo);
        
        printf("\n------------------------------------------\n");
        printf("TOTAL DE SESSOES REGISTRADAS: %d\n", total_sessoes);
    }

    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
    getchar();
}

int main() {
    int opcao = 0;

  
    do {
        // Limpa a tela para dar a sensação de navegação real 
        system("cls || clear"); 

        printf("==========================================\n");
        printf("               GH0ST: 2047                \n");
        printf("         NEURAL GUESSING SYSTEM           \n");
        printf("==========================================\n\n");
        printf("/!\\ 7 TENTATIVAS ANTES DO BLOQUEIO TOTAL /!\\ \n\n");

        printf(".------------------------------------------------.\n");
        printf("| >_ SYSTEM_LOG.TXT                              |\n");
        printf("|                                                |\n");
        printf("| > INITIALIZING GHOST PROTOCOL...               |\n");
        printf("| > LOADING NEURAL NETWORK...                    |\n");
        printf("| > PARSING HISTORY FILE...                      |\n");
        printf("| > TIMESTAMP: 2047-04-23 03:42:17               |\n");
        printf("| > SENHA NUMERICA: GERADA                       |\n");
        printf("| > TENTATIVAS MAXIMAS: 7                        |\n");
        printf("| > SISTEMA DE ALERTA: ATIVO              |\n");
        printf("'------------------------------------------------'\n\n");
                
        printf("Escolha uma ação no terminal:\n\n");
     
        printf("  [1] >_ JOGAR\n");
        printf("  [2] |li ANALISAR\n");
        printf("  [3] (I) SAIR\n\n");
        
        printf(">>> Digite o numero da opcao: ");
        
        // Proteção caso o usuário digite uma letra sem querer
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); // Limpa o buffer
            opcao = 0;
        }

        switch(opcao) {
            case 1:
                iniciar_jogo();
                break;
            case 2:
                exibir_analise();
                break;
            case 3:
                printf("\n[!] Encerrando a aplicacao corretamente. Desconectando...\n");
                break;
            default:
                printf("\n[ERRO] Comando invalido. Tente novamente.\n");
                printf("Pressione ENTER para continuar...");
                getchar();
                getchar();
                break;
        }

    } while (opcao != 3); 

    return 0;
}

