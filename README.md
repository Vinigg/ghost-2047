# 👻 GH0ST: 2047 - Neural Guessing System

Um jogo de adivinhação com tema cyberpunk/hacker construído com Raylib. Baseado em protótipos React do Figma, adaptado para C com efeitos visuais cyberpunk.

![Version](https://img.shields.io/badge/version-3.14-00FF9C)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20MacOS-00FF9C)
![License](https://img.shields.io/badge/license-MIT-00FF9C)

## 🎮 Sobre o Jogo

**GH0ST: 2047** é um jogo de adivinhação numérica ambientado no futuro cyberpunk. Você tem **7 tentativas** para decifrar uma senha numérica (1-100) antes que o sistema seja bloqueado permanentemente.

### 🚨 Sistema de Alertas Progressivos
- 🟢 **Tentativas 1-2**: Sistema Estável
- 🟡 **Tentativas 3-4**: Alerta Detectado
- 🔴 **Tentativas 5-7**: Rastreamento Ativo
- 💀 **FALHA**: BLOQUEADO

### ✨ Características
- Interface cyberpunk com efeitos visuais (Matrix rain, glow, particles)
- Sistema de range dinâmico que se ajusta aos seus palpites
- IA com sugestões baseadas em busca binária
- Sistema completo de estatísticas
- 4 telas navegáveis (Menu, Jogo, Resultado, Estatísticas)

## 📥 Como Baixar e Rodar o Jogo

### Pré-requisitos

#### Windows
- **MinGW-W64** (recomendado: MSYS2)
- **Git** (opcional, para clonar o repositório)

#### Linux
- **GCC** (geralmente já instalado)
- **Make**
- Bibliotecas de desenvolvimento: `libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev`

```bash
# Ubuntu/Debian
sudo apt install build-essential git libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev

# Fedora
sudo dnf install gcc make git mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel
```

#### MacOS
- **Xcode Command Line Tools**
```bash
xcode-select --install
```

### 🚀 Método 1: Download Direto (Mais Simples)

1. **Baixe o projeto**
   - Clique no botão verde **Code** no GitHub
   - Selecione **Download ZIP**
   - Extraia o arquivo para uma pasta de sua escolha

2. **Configure o compilador (Windows)**
   ```powershell
   # Adicione o MinGW ao PATH (ajuste o caminho se necessário)
   $env:PATH = "C:\msys64\mingw64\bin;$env:PATH"
   ```

3. **Compile o projeto**
   ```powershell
   # Windows (PowerShell)
   cd ghost-2047
   cd build
   ./premake5.exe gmake
   cd ..
   mingw32-make config=debug_x64
   ```

   ```bash
   # Linux/MacOS
   cd ghost-2047
   cd build
   ./premake5 gmake        # Linux
   ./premake5.osx gmake    # MacOS
   cd ..
   make config=debug_x64
   ```

4. **Execute o jogo!**
   ```powershell
   # Windows
   ./bin/Debug/ghost-2047.exe
   ```

   ```bash
   # Linux/MacOS
   ./bin/Debug/ghost-2047
   ```

### 🔧 Método 2: Usando Git Clone

1. **Clone o repositório**
   ```bash
   git clone https://github.com/seu-usuario/ghost-2047.git
   cd ghost-2047
   ```

2. **Siga os passos 2-4 do Método 1**

### ⚡ Método 3: Scripts Rápidos (Windows)

Para usuários Windows, use os scripts batch incluídos:

```powershell
# Compilar com MinGW-W64
./build-MinGW-W64.bat

# Ou com Visual Studio 2022
./build-VisualStudio2022.bat
```

## 🎯 Compilação Rápida

⚠️ **IMPORTANTE**: Na primeira vez (ou após limpar o projeto), você DEVE gerar os makefiles com premake5!

### Windows (MSYS2/MinGW64)
```powershell
# Configurar PATH (apenas uma vez por sessão)
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"

# PRIMEIRA VEZ: Gerar makefiles
cd build
./premake5.exe gmake
cd ..

# Compilar e rodar
mingw32-make config=debug_x64 && ./bin/Debug/ghost-2047.exe
```

### Linux
```bash
# PRIMEIRA VEZ: Gerar makefiles
cd build
./premake5 gmake
cd ..

# Compilar e rodar
make config=debug_x64 && ./bin/Debug/ghost-2047
```

### MacOS
```bash
# PRIMEIRA VEZ: Gerar makefiles
cd build
./premake5.osx gmake
cd ..

# Compilar e rodar
make config=debug_x64 && ./bin/Debug/ghost-2047
```

## 🐛 Solucionando Problemas

### Erro "No targets specified and no makefile found"
**Causa**: Você tentou compilar sem gerar os makefiles primeiro

**Solução**:
```powershell
# Windows
cd build
./premake5.exe gmake
cd ..
mingw32-make config=debug_x64
```

```bash
# Linux
cd build
./premake5 gmake
cd ..
make config=debug_x64
```

```bash
# MacOS
cd build
./premake5.osx gmake
cd ..
make config=debug_x64
```

📝 **Nota**: O premake5 gera os arquivos `Makefile`, `ghost-2047.make` e `raylib.make` necessários para a compilação. Este passo é obrigatório na primeira compilação!

### Windows: Erro "64-bit mode not compiled in"
**Causa**: Compilador MinGW antigo ou incorreto no PATH

**Solução**:
```powershell
# Verifique a versão do GCC
gcc --version
# Deve mostrar GCC 8.0+ (idealmente 11.0+)

# Se a versão estiver antiga, adicione o MSYS2 ao PATH:
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"

# Ou use o compilador completo:
mingw32-make config=debug_x64 CC=C:/msys64/mingw64/bin/gcc.exe
```

### Windows: Erro "unrecognized command line option '-std=c17'"
**Solução**: Já corrigido no projeto (usa C11). Se persistir, regenere os makefiles:
```powershell
cd build
./premake5.exe gmake
cd ..
```

### Linux: "raylib not found"
O projeto baixa o raylib automaticamente. Se falhar:
```bash
cd build/external
rm -rf raylib-master*
cd ..
./premake5 gmake
```

## 🕹️ Controles do Jogo

- **Teclado Numérico**: Digite seu palpite (1-100)
- **ENTER**: Confirmar palpite
- **BACKSPACE**: Apagar dígito
- **ESC**: Sair do jogo
- **Mouse**: Clicar nos botões para navegar

## 📊 Estrutura do Projeto

```
ghost-2047/
├── src/
│   └── main.c              # Código principal do jogo
├── include/
│   └── resource_dir.h      # Utilitário para encontrar recursos
├── resources/              # Assets do jogo (texturas, sons, etc)
├── build/
│   ├── premake5.exe        # Gerador de makefiles (Windows)
│   ├── premake5            # Gerador de makefiles (Linux)
│   └── premake5.lua        # Configuração do projeto
├── bin/
│   └── Debug/              # Executável compilado
├── prototype/              # Protótipos React originais (Figma Make)
└── README.md
```

## 💻 Desenvolvimento

### Usando VSCode (Recomendado)

1. **Abra o projeto no VSCode**
   ```bash
   code ghost-2047
   ```

2. **Execute a tarefa de build**
   - Pressione `CTRL+SHIFT+B` (Windows/Linux) ou `CMD+SHIFT+B` (MacOS)
   - Ou pressione `F5` para compilar e debugar

3. **Tarefas disponíveis** (no arquivo `.vscode/tasks.json`):
   - `build debug`: Compila em modo debug com símbolos
   - `build release`: Compila otimizado para release
   - `Clean`: Limpa arquivos de compilação

### Configurações de Compilação

#### Debug (recomendado para desenvolvimento)
```bash
make config=debug_x64
# ou
mingw32-make config=debug_x64  # Windows
```
- Inclui símbolos de debug
- Sem otimizações
- Executável maior, mais lento

#### Release (para distribuição)
```bash
make config=release_x64
# ou
mingw32-make config=release_x64  # Windows
```
- Otimizado para performance
- Sem símbolos de debug
- Executável menor, mais rápido

## 🎨 Customização

### Cores do Tema
Edite as constantes de cor em [src/main.c](src/main.c):

```c
#define COLOR_CYBER_GREEN (Color){0, 255, 156, 255}
#define COLOR_ALERT_YELLOW (Color){255, 215, 0, 255}
#define COLOR_DANGER_RED (Color){255, 59, 59, 255}
```

### Dificuldade do Jogo
Altere as constantes no início do arquivo:

```c
#define MAX_ATTEMPTS 7        // Número máximo de tentativas
#define MAX_PARTICLES 50      // Quantidade de partículas visuais
```

### Resolução da Janela
```c
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
```

## 🤝 Contribuindo


1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/MinhaFeature`)
3. Commit suas mudanças (`git commit -m 'Adiciona MinhaFeature'`)
4. Push para a branch (`git push origin feature/MinhaFeature`)
5. Abra um Pull Request

### Ideias para Implementar
- [ ] Sistema de som/música
- [ ] Mais efeitos visuais (scanlines, chromatic aberration)
- [ ] Fonte customizada monoespaçada
- [ ] Persistência de estatísticas em arquivo
- [ ] Múltiplos níveis de dificuldade
- [ ] Modo multiplayer local
- [ ] Tradução para outros idiomas

## 📜 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

**🎮 Divirta-se decifrando senhas no futuro cyberpunk de 2047! 👻**

