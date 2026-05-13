# 🚀 Guia Rápido - Resolução de Problemas Comuns

Este guia resolve os problemas mais frequentes ao compilar e rodar o **GH0ST: 2047**.

---

## ❌ Problema 1: "No targets specified and no makefile found"

### Por que acontece?
Você tentou compilar sem gerar os arquivos Makefile primeiro.

### Solução:
```powershell
# Windows
cd build
./premake5.exe gmake
cd ..
mingw32-make config=debug_x64
```

```bash
# Linux/MacOS
cd build
./premake5 gmake    # ou ./premake5.osx gmake no MacOS
cd ..
make config=debug_x64
```

📝 **Lembre-se**: Este passo é necessário apenas na **primeira compilação** ou após rodar `make clean`.

---

## ❌ Problema 2: "64-bit mode not compiled in"

### Por que acontece?
Você tem um GCC antigo ou 32-bit no PATH (como MinGW.org GCC 6.3.0).

### Como identificar:
```powershell
gcc --version
# Se mostrar "MinGW.org GCC-6.3.0" ou versão < 8.0, está usando o GCC errado
```

### Solução Passo a Passo:

**1. Adicione o MSYS2 MinGW64 ao PATH** (temporário):
```powershell
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"
```

**2. Confirme que está usando o GCC correto**:
```powershell
gcc --version
# Deve mostrar: "gcc.exe (Rev13, Built by MSYS2 project) 15.x.x"
```

**3. Recompile**:
```powershell
mingw32-make config=debug_x64
```

### 🔧 Solução Permanente (Windows):
Adicione `C:\msys64\mingw64\bin` nas **Variáveis de Ambiente** do sistema:

1. Windows Search → "Variáveis de Ambiente"
2. Editar variável `Path` do usuário ou sistema
3. Adicionar novo caminho: `C:\msys64\mingw64\bin`
4. **Importante**: Mova para o **topo da lista** (antes de outros MinGW)
5. Reinicie o PowerShell

---

## ✅ Checklist Completo para Primeira Compilação

Use este checklist se nunca compilou o projeto antes:

```powershell
# 1. Verifique o GCC
gcc --version
# Deve ser 8.0+ (idealmente 11.0+)

# 2. Se necessário, adicione MSYS2 ao PATH
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"

# 3. Gere os makefiles (PRIMEIRA VEZ)
cd build
./premake5.exe gmake
cd ..

# 4. Compile
mingw32-make config=debug_x64

# 5. Execute
./bin/Debug/ghost-2047.exe
```

---

## 🆘 Ainda com problemas?

### Comandos de diagnóstico:

```powershell
# Verifique qual GCC está no PATH
where gcc
gcc --version

# Verifique se o MSYS2 está instalado
Test-Path "C:\msys64\mingw64\bin\gcc.exe"

# Liste todos os arquivos gerados
Get-ChildItem -Recurse -Filter "*.make"
```

### Limpeza completa (último recurso):

```powershell
# Remove todos os arquivos compilados
mingw32-make clean

# Remove makefiles gerados
Remove-Item *.make
Remove-Item Makefile

# Regenere tudo do zero
cd build
./premake5.exe gmake
cd ..
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"
mingw32-make config=debug_x64
```

---

## 📖 Documentação Completa

Para informações detalhadas sobre o projeto, veja o [README.md](README.md).

**Principais seções**:
- [Instalação de Pré-requisitos](README.md#pré-requisitos)
- [Compilação Completa](README.md#compilação-rápida)
- [Troubleshooting Avançado](README.md#solucionando-problemas)
