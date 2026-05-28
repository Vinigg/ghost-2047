# 👻 GH0ST: 2047 - Neural Guessing System

Um jogo de adivinhação com tema cyberpunk/hacker desenvolvido em C, utilizando a Raylib.

![Version](https://img.shields.io/badge/version-3.14-00FF9C)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20MacOS-00FF9C)
![License](https://img.shields.io/badge/license-MIT-00FF9C)

> 🆘 **Problemas para compilar?** Veja o [GUIA-RAPIDO.md](GUIA-RAPIDO.md) com soluções para os erros mais comuns!

## 🎮 Sobre o Jogo

**GH0ST: 2047**é um jogo de adivinhação numérica ambientado em um universo cyberpunk, desenvolvido em linguagem C como parte de um projeto interdisciplinar universitário.

O jogador terá 7 tentativas para tentar invadir um sistema neural de segurança descobrindo um número secreto entre 1 e 100 antes que o sistema complete o rastreamento ativo da sua localização.

### 🚨 Sistema de Alertas Progressivos
- 🟢 **Tentativas 1-2**: Sistema Estável
- 🟡 **Tentativas 3-4**: Alerta Detectado
- 🔴 **Tentativas 5-7**: Rastreamento Ativo
- 💀 **FALHA**: BLOQUEADO

## ✨ Funcionalidades

### 🎨 Interface e Experiência

* Interface cyberpunk com efeitos neon e partículas
* Efeitos visuais inspirados em Matrix (rain, glow e particles)
* Sistema progressivo de alertas
* Navegação entre múltiplas telas
* Feedback visual dinâmico durante o gameplay

### 🎮 Gameplay

* Menu interativo com seleção de dificuldade (Fácil/Difícil)
* Geração aleatória de números
* Feedback em tempo real a cada tentativa (maior/menor)
* Sistema progressivo de tentativas
* IA com sugestões baseadas em busca binária
* Condição de vitória e derrota
* Reinicialização completa de partidas

### 📊 Estatísticas e Persistência

* Sistema de recordes (High Score)
* Estatísticas de desempenho do jogador
* Persistência de dados entre sessões
* Logging detalhado de gameplay em CSV
* Exportação automática de sessões para análise

### 🛡️ Sistema e Robustez

* Tratamento robusto de erros de entrada
* Validação de dados do usuário
* Estrutura modular em C
* Compatibilidade multiplataforma
  
 
> 📊 **Análise de Dados**: O jogo gera automaticamente `ghost2047_sessions.csv` com dados detalhados de cada partida. Veja [LOGGING-CSV.md](LOGGING-CSV.md) para análise estatística!


## 🛠️ Tecnologias Utilizadas

| Tecnologia   | Função                          |
| ------------ | ------------------------------- |
| C11          | Linguagem principal             |
| Raylib       | Renderização gráfica e áudio    |
| Premake5     | Geração de projetos e makefiles |
| Make / MinGW | Sistema de build                |
| Git & GitHub | Controle de versão              |


## 📊 Estrutura do Projeto


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

## 🕹️ Controles do Jogo

- **Teclado Numérico**: Digite seu palpite (1-100)
- **ENTER**: Confirmar palpite
- **BACKSPACE**: Apagar dígito
- **ESC**: Sair do jogo
- **Mouse**: Clicar nos botões para navegar

## 📊 Planejamento do Projeto

Quadro Kanban

Protótipo Inicial

--------------

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

Encontrou algum erro? Consulte a tabela abaixo para soluções rápidas:

| 🚨 Erro | 🔍 Causa | 📖 Solução Detalhada |
|---------|----------|---------------------|
| **"No targets specified and no makefile found"** | Makefiles não foram gerados | [Ver solução →](GUIA-RAPIDO.md#-problema-1-no-targets-specified-and-no-makefile-found) |
| **"64-bit mode not compiled in"** | GCC 32-bit ou antigo no PATH | [Ver solução →](GUIA-RAPIDO.md#-problema-2-64-bit-mode-not-compiled-in) |


### 🔧 Solução Rápida (Windows)

Se você está tendo problemas pela **primeira vez**, tente isto:

```powershell
# Configure o compilador correto
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"

# Gere os makefiles
cd build
./premake5.exe gmake
cd ..

# Compile
mingw32-make config=debug_x64

# Execute
./bin/Debug/ghost-2047.exe
```

📚 **Para soluções completas e passo a passo**, consulte o **[GUIA-RAPIDO.md](GUIA-RAPIDO.md)**



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
## 👥 Equipe de Desenvolvimento

Integrante	                                Função
Vinicius Pessoa de Albuquerque	           Tech Lead / Arquitetura
Pedro Pessoa de Albuquerque Neto	           Desenvolvimento / QA
Roberto Henrique Cavalcanti Freitas	        Back-end / Persistência
Saulo Eduardo Almeida dos Santos	           Estatísticas
Thayna Vercosa de Andrade	                 Product Owner
Thiago Cardozo da Conceição	              Heurísticas
Vinicius Wagner Gomes Germano	              Interface e Gameplay
Vitória Gabrielly Gomes da Silva	           QA
Wesley Yuri da Silva	                       Parsing e Reinicialização
Yasmin Karolina Silva de M. Godinho	        Gameplay

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

