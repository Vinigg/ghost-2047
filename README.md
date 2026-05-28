# 👻 GH0ST: 2047 - Neural Guessing System

Um jogo de adivinhação com tema cyberpunk/hacker desenvolvido em C, utilizando a Raylib.

![Version](https://img.shields.io/badge/version-3.14-00FF9C)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20MacOS-00FF9C)
![License](https://img.shields.io/badge/license-MIT-00FF9C)

> 🆘 **Problemas para compilar?** Veja o [GUIA-RAPIDO.md](docs/GUIA-RAPIDO.md) com soluções para os erros mais comuns!

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

### 🎥 Demo
[![Demo](./docs/assets/images/screenshots/thumb.png)](https://drive.google.com/file/d/1tXt2vLENOYa3dv0DsvBtMORv-oj52Lph/view?usp=sharing)

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
  
 
> 📊 **Análise de Dados**: O jogo gera automaticamente `ghost2047_sessions.csv` com dados detalhados de cada partida. Veja [LOGGING-CSV.md](docs/LOGGING-CSV.md) para análise estatística!


## 🛠️ Tecnologias Utilizadas

| Tecnologia   | Função                          |
| ------------ | ------------------------------- |
| C11          | Linguagem principal             |
| Raylib       | Renderização gráfica e áudio    |
| Premake5     | Geração de projetos e makefiles |
| Make / MinGW | Sistema de build                |
| Git & GitHub | Controle de versão              |


## 📊 Estrutura do Projeto

```bash
ghost-2047/
├── src/                    # Código-fonte do jogo
├── include/                # Headers do projeto
├── resources/              # Assets (texturas, sons, dados)
├── build/                  # Scripts de build e premake
├── bin/                    # Executáveis compilados
├── docs/                   # 📚 Documentação completa
│   ├── INSTALACAO.md
│   ├── GUIA-RAPIDO.md
│   ├── LOGGING-CSV.md
│   ├── ARQUITETURA-MODULAR.md
│   └── assets/             # Imagens e vídeos
└── README.md               # Este arquivo
```

## 🕹️ Controles do Jogo

| Tecla / Entrada  | Função                     |
| ---------------- | -------------------------- |
| Teclado Numérico | Digite seu palpite (1–100) |
| ENTER            | Confirmar palpite          |
| BACKSPACE        | Apagar dígito              |
| ESC              | Sair do jogo               |
| Mouse            | Navegar pelos botões       |


## 📊 Planejamento do Projeto

Quadro Kanban

Protótipo Inicial

--------------

## 📥 Instalação e Compilação

Para instruções detalhadas sobre como baixar, compilar e executar o jogo em Windows, Linux ou MacOS, consulte o guia completo:

**➡️ [INSTALACAO.md](docs/INSTALACAO.md) - Guia Completo de Instalação e Compilação**

> 📖 Consulte o guia de instalação para instruções detalhadas de pré-requisitos, compilação e troubleshooting para todas as plataformas.

## 💻 Desenvolvimento

### Usando VSCode
- Pressione `CTRL+SHIFT+B` para compilar
- Pressione `F5` para compilar e debugar

📖 Para configurações avançadas, consulte [INSTALACAO.md](docs/INSTALACAO.md)

## 👥 Programação em Pares

Durante o desenvolvimento do projeto, utilizamos a técnica de **Pair Programming** para implementar funcionalidades críticas. Esta abordagem colaborativa trouxe aprendizados valiosos para a equipe.

**➡️ [PAIR-PROGRAMMING.md](docs/PAIR-PROGRAMMING.md) - Relatos e Experiências da Equipe**

> 🤝 Confira os relatos de Vinícius Pessoa e Wesley Yuri sobre a experiência de programar em dupla, desafios enfrentados e aprendizados adquiridos.

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

| Integrante                          | Função                    |
| ----------------------------------- | ------------------------- |
| Vinicius Pessoa de Albuquerque      | Tech Lead / Arquitetura   |
| Pedro Pessoa de Albuquerque Neto    | Desenvolvimento / QA      |
| Roberto Henrique Cavalcanti Freitas | Back-end / Persistência   |
| Saulo Eduardo Almeida dos Santos    | Estatísticas              |
| Thayna Vercosa de Andrade           | Product Owner             |
| Thiago Cardozo da Conceição         | Heurísticas               |
| Vinicius Wagner Gomes Germano       | Interface e Gameplay      |
| Vitória Gabrielly Gomes da Silva    | QA                        |
| Wesley Yuri da Silva                | Parsing e Reinicialização |
| Yasmin Karolina Silva de M. Godinho | Gameplay                  |


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

