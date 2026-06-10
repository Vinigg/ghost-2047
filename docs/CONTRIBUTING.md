# Guia de Contribuição - GH0ST: 2047

Primeiramente, obrigado por se interessar em contribuir para o **GH0ST: 2047 - Neural Guessing System**! Este projeto é um ecossistema de jogo com temática cyberpunk desenvolvido como parte do programa Embarque Digital na CESAR School.

Este documento serve como um guia prático para configurar o ambiente de desenvolvimento, compilar o código-fonte e garantir que as novas contribuições respeitem a arquitetura limpa e modular estabelecida pelo grupo.

---

## 🛠️ 1. Pré-requisitos e Configuração do Ambiente

O projeto é escrito puramente na linguagem **C** e utiliza a biblioteca **Raylib** para o gerenciamento de gráficos, áudio e captura de entradas (inputs) do usuário.

### Requisitos de Software:
* **Compilador C:** GCC (via MinGW-w64 no Windows, ou nativo no Linux/macOS).
* **Biblioteca Gráfica:** [Raylib v4.5 ou superior](https://www.raylib.com/).
* **Assets do Sistema:** A pasta `resources/` (contendo os arquivos de áudio `.mp3`, fontes cyberpunk e os arquivos de texto com as perguntas lógicas) deve estar localizada obrigatoriamente na raiz do repositório ou junto ao executável final compilado.

---

## 🏗️ 2. Arquitetura Modular do Código

Para contribuir com o projeto, você deve respeitar a estrutura modularizada implementada a partir do arquivo principal (`main.c`):

* `types.h`: Modifique aqui se precisar alterar a estrutura central de dados e variáveis de controle do jogo (`GameState`).
* `theme.h` / `ui/`: Componentes visuais, botões e paletas de cores neon. Qualquer alteração de design deve ser centralizada aqui.
* `game/`: Contém a lógica de processamento de palpites (`game_logic.h`) e o banco de questões lógicas (`logic_questions.h`). **A lógica de negócios não deve ser escrita diretamente na main.**
* `persistence/`: Gerencia o histórico local de sessões (`history.h`) e a geração de relatórios estatísticos em formato CSV (`session_logger.h` / `statistics.h`).

---

## 🚀 3. Como Compilar e Executar

### Via Prompt de Comando / Terminal (GCC Direto):
Navegue até a raiz do projeto e execute o comando de compilação apontando para as dependências do código e vinculando as flags nativas da Raylib:

```bash
gcc main.c -o ghost2047.exe -lraylib -lopengl32 -lgdi32 -lwinmm -I./include -L./lib
```

Para executar o sistema após a compilação bem-sucedida:

```bash
./ghost2047.exe
```

---

## 📊 4. Padrões de Código e Algoritmos Recursivos

Ao estender os módulos de análise matemática ou inteligência do jogo, atente-se às seguintes diretrizes:

1. **Recursividade Obrigatória:** As funções agregadas de cálculo no painel estatístico (como Soma de Tentativas, Mínimo, Máximo e Soma Quadrática para cálculo de Variância/Desvio Padrão) utilizam **recursão** em vez de laços estruturados (`for`/`while`) por critérios de arquitetura acadêmica. Mantenha esse padrão para novas métricas.
2. **Nomenclatura:** Utilize o padrão `camelCase` para variáveis locais e estruturas na `main.c`, e `snake_case` para propriedades internas nos arquivos de cabeçalho (`.h`).
3. **Gerenciamento de Memória:** Sempre que introduzir novos carregamentos de assets (Músicas ou Texturas), adicione a respectiva função de desalocação dentro do bloco `UnloadGame()` para evitar vazamento de memória (Memory Leaks).

---

## 🌿 5. Fluxo de Git para Submissões

1. Faça um **Fork** do repositório oficial do projeto.
2. Crie uma branch para a sua feature ou correção de bug:

```bash
git checkout -b feature/minha-nova-feature
```

3. Faça commits claros e granulares detalhando as mudanças (Exemplo: `git commit -m "feat: adiciona efeito glitch na transicao de dicas"`).
4. Certifique-se de rodar os testes de sistema locais antes de subir as alterações.
5. Abra um **Pull Request** detalhando as mudanças realizadas e vincule-o à respectiva Issue do GitHub Tracker.
