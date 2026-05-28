# 📥 Guia de Instalação e Compilação - GH0ST: 2047

Este guia fornece instruções detalhadas para baixar, compilar e executar o **GH0ST: 2047** em Windows, Linux e MacOS.

## 📋 Índice

- [Pré-requisitos](#-pré-requisitos)
- [Método 1: Download Direto](#-método-1-download-direto-mais-simples)
- [Método 2: Git Clone](#-método-2-usando-git-clone)
- [Método 3: Scripts Rápidos (Windows)](#-método-3-scripts-rápidos-windows)
- [Compilação Rápida](#-compilação-rápida)
- [Solucionando Problemas](#-solucionando-problemas)

## 🔧 Pré-requisitos

### Windows
- **MinGW-W64** (recomendado: MSYS2)
- **Git** (opcional, para clonar o repositório)

### Linux
- **GCC** (geralmente já instalado)
- **Make**
- Bibliotecas de desenvolvimento: `libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev`

```bash
# Ubuntu/Debian
sudo apt install build-essential git libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev

# Fedora
sudo dnf install gcc make git mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel
```

### MacOS
- **Xcode Command Line Tools**
```bash
xcode-select --install
```

---

## 🚀 Método 1: Download Direto (Mais Simples)

### 1. Baixe o projeto
- Clique no botão verde **Code** no GitHub
- Selecione **Download ZIP**
- Extraia o arquivo para uma pasta de sua escolha

### 2. Configure o compilador (Windows)
```powershell
# Adicione o MinGW ao PATH (ajuste o caminho se necessário)
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"
```

### 3. Compile o projeto

**Windows (PowerShell):**
```powershell
cd ghost-2047
cd build
./premake5.exe gmake
cd ..
mingw32-make config=debug_x64
```

**Linux/MacOS:**
```bash
cd ghost-2047
cd build
./premake5 gmake        # Linux
./premake5.osx gmake    # MacOS
cd ..
make config=debug_x64
```

### 4. Execute o jogo!

**Windows:**
```powershell
./bin/Debug/ghost-2047.exe
```

**Linux/MacOS:**
```bash
./bin/Debug/ghost-2047
```

---

## 🔧 Método 2: Usando Git Clone

### 1. Clone o repositório
```bash
git clone https://github.com/seu-usuario/ghost-2047.git
cd ghost-2047
```

### 2. Siga os passos 2-4 do Método 1

---

## ⚡ Método 3: Scripts Rápidos (Windows)

Para usuários Windows, use os scripts batch incluídos:

```powershell
# Compilar com MinGW-W64
./build-MinGW-W64.bat

# Ou com Visual Studio 2022
./build-VisualStudio2022.bat
```

---

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

---

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

---

## 💻 Configurações Avançadas de Compilação

### Debug (recomendado para desenvolvimento)
```bash
make config=debug_x64
# ou
mingw32-make config=debug_x64  # Windows
```
- Inclui símbolos de debug
- Sem otimizações
- Executável maior, mais lento

### Release (para distribuição)
```bash
make config=release_x64
# ou
mingw32-make config=release_x64  # Windows
```
- Otimizado para performance
- Sem símbolos de debug
- Executável menor, mais rápido

---

## 🔙 Voltar

⬅️ [Voltar para README principal](../README.md)
