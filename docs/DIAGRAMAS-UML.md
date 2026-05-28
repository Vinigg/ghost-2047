<div align="center">
  <h1>📐 Diagramas UML das Funcionalidades</h1>
</div>

Diagramas de fluxo representando a lógica de cada história de usuário do GH0ST: 2047.

---

## Índice

| # | Funcionalidade | Prioridade |
|---|---------------|------------|
| [UH01](#uh01-interface-principal-menu) | Interface Principal (Menu) | Alta |
| [UH02](#uh02-geração-de-número) | Geração de Número | Alta |
| [UH03](#uh03-ciclo-de-jogo-com-feedback) | Ciclo de Jogo com Feedback | Alta |
| [UH04](#uh04-palpite-básico) | Palpite Básico | Alta |
| [UH05](#uh05-condição-de-vitória) | Condição de Vitória | Alta |
| [UH06](#uh06-limite-de-tentativas) | Limite de Tentativas | Alta |
| [UH07](#uh07-persistência-de-dados) | Persistência de Dados | Média |
| [UH08](#uh08-tratamento-de-erros) | Tratamento de Erros | Média |
| [UH09](#uh09-reinicialização) | Reinicialização | Média |
| [UH10](#uh10-cálculos-base--recursão) | Cálculos Base (Recursão) | Baixa |
| [UH11](#uh11-recorde-local--high-score) | Recorde Local (High Score) | Baixa |
| [UH12](#uh12-estatísticas-agregadas) | Estatísticas Agregadas | Baixa |
| [UH13](#uh13-heurística-de-estratégia) | Heurística de Estratégia | Baixa |
| [UH14](#uh14-leitura-e-parsing-do-histórico) | Leitura e Parsing do Histórico | Média |

---

## UH01. Interface Principal (Menu)

O jogador acessa o menu principal com as opções Jogar, Analisar e Sair.

```mermaid
flowchart TD
    A([Início]) --> B[Exibir Menu Principal]
    B --> C{Escolha do usuário}
    C -->|Jogar| D[Iniciar nova partida]
    C -->|Analisar| E[Exibir estatísticas / análise]
    C -->|Sair| F[Encerrar sistema]
    D --> G[Fim / Retorno ao menu]
    E --> G
    F --> H([Fim do programa])
```

---

## UH02. Geração de Número

O sistema gera um número aleatório secreto dentro do intervalo definido (1–100).

```mermaid
flowchart TD
    A([Início da nova partida]) --> B[Definir intervalo]
    B --> C[Gerar número aleatório]
    C --> D[Armazenar como número secreto]
    D --> E[Ocultar número do jogador]
    E --> F[Iniciar jogo com número secreto]
    F --> G([Fim / Aguardar palpites])
    G -.->|Nova partida| B
```

---

## UH03. Ciclo de Jogo com Feedback

O jogador insere palpites e recebe dicas de "maior" ou "menor" até acertar.

```mermaid
flowchart TD
    A([Início da partida]) --> B[Receber palpite do jogador]
    B --> C[Comparar com número secreto]
    C --> D{Palpite correto?}
    D -->|Sim| E[Exibir mensagem de vitória]
    E --> F([Fim da partida])
    D -->|Não| G{Palpite maior que o número secreto?}
    G -->|Sim| H[Informar: número secreto é menor]
    G -->|Não| I[Informar: número secreto é maior]
    H --> B
    I --> B
```

---

## UH04. Palpite Básico

Captura e validação da entrada numérica do jogador via teclado.

```mermaid
flowchart TD
    A([Solicitar entrada do jogador]) --> B[Receber valor do teclado]
    B --> C{Entrada é numérica?}
    C -->|Não| D[Exibir erro: valor inválido]
    D --> A
    C -->|Sim| E{Está dentro do intervalo permitido?}
    E -->|Não| F[Exibir erro: fora do intervalo]
    F --> A
    E -->|Sim| G[Registrar palpite válido]
    G --> H[Enviar para lógica do jogo]
```

---

## UH05. Condição de Vitória

Detecção de acerto e exibição de mensagem de sucesso ao jogador.

```mermaid
flowchart TD
    A([Receber resultado da comparação]) --> B{Palpite é igual ao número secreto?}
    B -->|Não| C[Continuar jogo]
    B -->|Sim| D[Exibir mensagem de sucesso]
    D --> E[Exibir total de tentativas]
    E --> F{Próxima ação}
    F -->|Reiniciar| G[Iniciar nova partida]
    F -->|Menu| H[Voltar ao menu principal]
    F -->|Sair| I[Encerrar jogo]
```

---

## UH06. Limite de Tentativas

Controle do número máximo de tentativas (7) com encerramento por derrota.

```mermaid
flowchart TD
    A([Início da partida]) --> B[Definir limite de tentativas]
    B --> C[Inicializar contador = 0]
    C --> D[Receber palpite]
    D --> E[Incrementar contador]
    E --> F{Acertou o número?}
    F -->|Sim| G[Encerrar partida com vitória]
    F -->|Não| H{Atingiu limite de tentativas?}
    H -->|Não| D
    H -->|Sim| I[Encerrar partida por derrota]
    I --> J[Exibir mensagem de limite atingido]
```

---

## UH07. Persistência de Dados

Salvamento e carregamento automático de resultados em arquivo CSV.

```mermaid
flowchart TD
    A([Iniciar sistema]) --> B{Arquivo de dados existe?}
    B -->|Sim| C[Carregar dados do arquivo]
    B -->|Não| D[Criar estrutura de dados vazia]
    C --> E[Dados disponíveis em memória]
    D --> E
    E --> F[Fim da partida]
    F --> G[Coletar resultados do jogador]
    G --> H[Atualizar dados em memória]
    H --> I[Salvar no arquivo]
    I --> J{Salvamento bem-sucedido?}
    J -->|Sim| K[Confirmar persistência]
    J -->|Não| L[Tratar erro / evitar corrupção]
    K --> M([Fim])
    L --> M
```

---

## UH08. Tratamento de Erros

Validação de entrada numérica e tratamento de caracteres inválidos.

```mermaid
flowchart TD
    A([Solicitar entrada do usuário]) --> B[Receber valor]
    B --> C{Entrada é numérica válida?}
    C -->|Não| D[Exibir mensagem de erro]
    D --> E[Solicitar nova entrada]
    E --> B
    C -->|Sim| F[Permitir processamento do valor]
    F --> G[Continuar fluxo do jogo]
```

---

## UH09. Reinicialização

Opção de jogar novamente após fim da partida com reset completo de estado.

```mermaid
flowchart TD
    A([Fim da partida]) --> B[Exibir opção: jogar novamente?]
    B --> C{Escolha do jogador}
    C -->|Sim| D[Resetar estado do jogo]
    D --> E[Gerar novo número secreto]
    E --> F[Zerar tentativas]
    F --> G[Iniciar nova partida]
    C -->|Não| H[Encerrar jogo]
```

---

## UH10. Cálculos Base — Recursão

Funções recursivas para soma, mínimo, máximo e soma dos quadrados dos palpites.

```mermaid
flowchart TD
    A([Receber lista de palpites]) --> B[Chamar função recursiva]
    B --> C{Lista vazia?}
    C -->|Sim| D[Retornar valor base]
    C -->|Não| E[Processar elemento atual]
    E --> F[Chamar função recursiva com restante da lista]
    F --> G[Combinar resultado atual com recursivo]
    G --> H{Tipo de cálculo}
    H -->|Soma| I[Somar valores]
    H -->|Mínimo| J[Comparar menor]
    H -->|Máximo| K[Comparar maior]
    H -->|Soma dos quadrados| L[Somar quadrados]
    I --> M[Retornar resultado]
    J --> M
    K --> M
    L --> M
```

---

## UH11. Recorde Local — High Score

Registro e exibição da menor quantidade de tentativas entre todas as sessões.

```mermaid
flowchart TD
    A([Iniciar sistema]) --> B[Carregar recorde do arquivo]
    B --> C{Recorde existe?}
    C -->|Sim| E[Recorde disponível]
    C -->|Não| D[Inicializar recorde vazio]
    D --> E
    E --> F[Fim da partida]
    F --> G[Obter nº de tentativas da partida]
    G --> H{Novo recorde?}
    H -->|Sim| I[Atualizar recorde]
    I --> J[Salvar novo recorde no arquivo]
    J --> K[Exibir recorde ao jogador]
    H -->|Não| L[Manter recorde atual]
    L --> K
    K --> M([Fim])
```

---

## UH12. Estatísticas Agregadas

Cálculo e exibição de média de tentativas e desvio padrão do jogador.

```mermaid
flowchart TD
    A([Menu principal]) --> B[Selecionar opção: Analisar]
    B --> C[Carregar dados históricos]
    C --> D{Existem dados?}
    D -->|Não| E[Exibir mensagem: sem dados disponíveis]
    D -->|Sim| F[Calcular média de tentativas]
    F --> G[Calcular desvio padrão]
    G --> H[Exibir resultados ao jogador]
    E --> I([Fim])
    H --> I
```

---

## UH13. Heurística de Estratégia

Sugestões textuais de estratégia baseadas no intervalo atual (busca binária).

```mermaid
flowchart TD
    A([Início / Nova tentativa]) --> B[Obter intervalo atual]
    B --> C[Analisar estado do jogo e palpites anteriores]
    C --> D[Calcular sugestão]
    D --> E[Exibir sugestão textual ao jogador]
    E --> F[Jogador realiza novo palpite]
    F --> G[Atualizar intervalo com base no resultado]
    G --> H{Jogo terminou?}
    H -->|Não| A
    H -->|Sim| I([Fim da partida])
```

---

## UH14. Leitura e Parsing do Histórico

Carregamento e parsing do arquivo CSV com reconstrução de sessões em memória.

```mermaid
flowchart TD
    A([Iniciar aplicação]) --> B[Abrir arquivo de histórico]
    B --> C{Arquivo existe?}
    C -->|Não| D[Inicializar estrutura vazia]
    C -->|Sim| E[Ler linha com fgets]
    E --> F{Fim do arquivo?}
    F -->|Não| G[Separar campos por delimitador ';']
    G --> H[Converter dados]
    H --> I[Reconstruir sessão em memória]
    I --> E
    F -->|Sim| J[Dados prontos em memória]
    D --> J
    J --> K[Disponibilizar dados para: estatísticas, recursão e estratégia]
```

---

<div align="center">

**[⬅️ Voltar ao README](../README.md)**

</div>
