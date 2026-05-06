#include <stdio.h>
#include <stdlib.h>


void iniciar_jogo() {
    printf("\n[>] SISTEMA: Iniciando protocolo de jogo...\n");
    getchar(); 
}

void exibir_analise() {
    printf("\n[>] SISTEMA: Acessando banco de dados de ranking...\n");
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

