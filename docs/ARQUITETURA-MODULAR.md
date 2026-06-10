# 🔄 Arquitetura Modular - GH0ST: 2047

Este documento descreve a **arquitetura modular** do projeto, com separação clara de responsabilidades.

---

## 📁 Estrutura de Arquivos

```
ghost-2047/
├── include/
│   ├── config.h          # Constantes de configuração do jogo
│   ├── types.h           # Estruturas de dados e enums
│   ├── theme.h           # Paleta de cores cyberpunk
│   └── resource_dir.h    # Helper para localizar recursos
│
├── src/
│   ├── main.c            # Orquestrador principal (196 linhas)
│   │
│   ├── audio/            # Módulo de áudio
│   │   ├── music_manager.h
│   │   └── music_manager.c    (~100 linhas)
│   │
│   ├── effects/          # Módulo de efeitos visuais
│   │   ├── matrix_effect.h
│   │   ├── matrix_effect.c    (~60 linhas)
│   │   ├── glitch_effect.h
│   │   └── glitch_effect.c    (~60 linhas)
│   │
│   ├── ui/               # Módulo de interface
│   │   ├── ui_components.h
│   │   ├── ui_components.c    (~50 linhas)
│   │   ├── screens.h
│   │   └── screens.c          (~600 linhas)
│   │
│   ├── game/             # Módulo de lógica do jogo
│   │   ├── game_logic.h
│   │   ├── game_logic.c       (~130 linhas)
│   │   ├── logic_questions.h
│   │   ├── logic_questions.c  (~100 linhas)
│   │   ├── analytics.h
│   │   └── analytics.c        (~130 linhas)
│   │
│   └── persistence/      # Módulo de persistência
│       ├── statistics.h
│       ├── statistics.c       (~60 linhas)
│       ├── session_logger.h
│       ├── session_logger.c   (~100 linhas)
│       ├── history.h
│       └── history.c          (~70 linhas)
```

---

## 📊 Métricas

| Aspecto | Antes | Depois |
|---------|-------|--------|
| **main.c** | 1200+ linhas | 196 linhas |
| **Número de módulos** | 1 | 7 |
| **Maior arquivo** | 1200+ linhas | ~600 linhas (screens.c) |
| **Acoplamento** | Alto | Baixo |
| **Testabilidade** | Difícil | Fácil |

---

## 🎯 Módulos

### 1. **main.c** - Orquestrador (196 linhas)

Responsável apenas por:
- Inicializar janela, áudio e subsistemas
- Game loop (`Update` → `Draw`)
- Conversão de coordenadas de mouse (virtual screen)
- Fullscreen toggle (F11)

```c
// Fluxo simplificado
InitGame() → while(!WindowShouldClose()) { UpdateGame(); DrawGame(); } → UnloadGame()
```

### 2. **audio/music_manager** - Sistema de Áudio (~100 linhas)

| Função | Descrição |
|--------|-----------|
| `MusicManager_Init()` | Carrega músicas e inicia menu music |
| `MusicManager_Update()` | Atualiza stream de música ativo |
| `MusicManager_Unload()` | Libera recursos de áudio |
| `MusicManager_SwitchToMenu()` | Troca para música de menu |
| `MusicManager_SwitchToGame1()` | Troca para música de jogo (início) |
| `MusicManager_SwitchToGame2()` | Troca para música de jogo (tensão) |
| `MusicManager_GetCurrentTrack()` | Retorna track atual (0-3) |

### 3. **ui/screens** - Lógica de Telas (~600 linhas)

Contém Update/Draw para todas as telas do jogo, acessando estado compartilhado via `ScreenContext`:

| Função | Tela |
|--------|------|
| `Screen_UpdateMainMenu()` / `Screen_DrawMainMenu()` | Menu principal |
| `Screen_UpdateDifficultySelect()` / `Screen_DrawDifficultySelect()` | Seleção de dificuldade |
| `Screen_UpdateGame()` / `Screen_DrawGame()` | Tela de jogo |
| `Screen_UpdateResult()` / `Screen_DrawResult()` | Resultado |
| `Screen_UpdateStats()` / `Screen_DrawStats()` | Estatísticas |

**ScreenContext** — struct que conecta as telas ao estado global:
```c
typedef struct {
    GameScreen* currentScreen;
    GameState* gameState;
    Statistics* stats;
    QuestionBank* questionBank;
    LogicQuestion** currentQuestion;
    DifficultyLevel* currentDifficulty;
    float* glowPulse;
    Vector2 (*getVirtualMousePosition)(void);
} ScreenContext;
```

### 4. **ui/ui_components** - Componentes Reutilizáveis (~50 linhas)

| Função | Descrição |
|--------|-----------|
| `UI_DrawGlowText()` | Texto com efeito de brilho pulsante |
| `UI_DrawCyberButton()` | Botão estilo cyberpunk com hover |
| `UI_DrawAlertBox()` | Caixa de alerta com borda |

### 5. **game/game_logic** - Mecânicas do Jogo (~130 linhas)

| Função | Descrição |
|--------|-----------|
| `Game_StartNew()` | Inicia novo jogo com número aleatório |
| `Game_ProcessGuess()` | Processa palpite e atualiza estado |
| `Game_AnswerQuestion()` | Processa resposta de lógica |
| `Game_GetAlertLevel()` | Calcula nível de alerta |
| `Game_GetAlertColor()` | Cor por nível de alerta |
| `Game_GetAlertText()` | Texto por nível de alerta |
| `Game_CreateDetailedSession()` | Empacota estado para CSV |

### 6. **game/analytics** - Análise Estatística (~130 linhas)

Funções recursivas para relatórios analíticos:

| Função | Descrição |
|--------|-----------|
| `Analytics_RecursiveSum()` | Soma recursiva de tentativas |
| `Analytics_RecursiveMin()` | Mínimo recursivo |
| `Analytics_RecursiveMax()` | Máximo recursivo |
| `Analytics_RecursiveSumSquares()` | Soma de quadrados (variância) |
| `Analytics_LongestMonotonicRun()` | Maior sequência monotônica |
| `Analytics_HasRepetitivePattern()` | Detecta padrão repetitivo |
| `Analytics_BinarySimilarity()` | Proximidade com busca binária |

### 7. **game/logic_questions** - Banco de Questões (~100 linhas)

| Função | Descrição |
|--------|-----------|
| `QuestionBank_Load()` | Carrega questões de arquivo .txt |
| `QuestionBank_GetRandom()` | Sorteia questão (sem repetir) |
| `QuestionBank_Unload()` | Libera memória |

### 8. **effects/** - Efeitos Visuais (~120 linhas total)

**matrix_effect.c** — Chuva de caracteres Matrix:
- `InitMatrixEffect()` / `UpdateMatrixEffect()` / `DrawMatrixEffect()`

**glitch_effect.c** — Explosão de partículas:
- `InitGlitchEffect(won)` / `UpdateGlitchEffect()` / `DrawGlitchEffect()`

### 9. **persistence/** - Persistência (~230 linhas total)

**statistics.c** — Gerenciamento de stats:
- `Stats_Init()` / `Stats_LoadFromCSV()` / `Stats_AddSession()` / `Stats_Calculate()`

**session_logger.c** — Log detalhado em CSV:
- `SessionLogger_Init()` / `SessionLogger_Append()` / `SessionLogger_Load()`

**history.c** — Persistência binária:
- `History_Save()` / `History_Load()`

---

## 🔗 Diagrama de Dependências

```
main.c (Orquestrador)
├── ui/screens ──────────┐
│   ├── ui/ui_components │
│   ├── game/game_logic  │
│   ├── game/analytics   │
│   ├── game/logic_questions
│   ├── effects/matrix_effect
│   ├── effects/glitch_effect
│   ├── audio/music_manager
│   └── persistence/*    │
├── audio/music_manager  │
├── effects/matrix_effect│
├── effects/glitch_effect│
├── game/logic_questions │
├── persistence/statistics
└── persistence/session_logger
```

---

## 🚀 Compilação

```powershell
# Regenerar makefiles (necessário ao adicionar novos .c)
cd build
./premake5.exe gmake
cd ..

# Compilar
mingw32-make                      # Debug (padrão)
mingw32-make config=release_x64   # Release
```

---

## ✨ Benefícios da Arquitetura

### Manutenibilidade
- Arquivos menores e focados
- Fácil localizar código específico
- Modificações isoladas não quebram outros módulos

### Testabilidade
- Cada módulo pode ser testado separadamente
- Funções puras em `analytics` facilitam unit tests
- `game_logic` independe da interface

### Escalabilidade
- Novo efeito visual → criar arquivo em `effects/`
- Nova mecânica → estender `game/`
- Novo componente UI → adicionar em `ui/`

### Colaboração
- Múltiplos devs podem trabalhar em módulos diferentes
- Menos conflitos de merge no Git
- Code review focado

---

## 💡 Convenções

1. **Headers**: Cada `.c` tem um `.h` correspondente com a API pública
2. **Estado interno**: Variáveis `static` dentro do `.c` (encapsulamento)
3. **Includes**: Usar paths relativos a `src/` (ex: `"game/analytics.h"`)
4. **Nomenclatura**: `Modulo_Funcao()` (ex: `MusicManager_Init()`, `Analytics_RecursiveSum()`)
5. **ScreenContext**: Telas acessam estado global via struct de contexto, não variáveis globais diretas

---

## ❓ Troubleshooting

### Erro: "undefined reference to..."
**Causa**: Novo `.c` não está no makefile  
**Solução**: Regenerar com `./premake5.exe gmake` no diretório `build/`

### Erro: "cannot find header"
**Causa**: Path de include incorreto  
**Solução**: Usar path relativo a `src/` (ex: `#include "audio/music_manager.h"`)

### Erro: "implicit declaration"
**Causa**: Faltando `#include` do header correspondente  
**Solução**: Adicionar o include necessário
