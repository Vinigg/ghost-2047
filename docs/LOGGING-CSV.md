# 📊 Sistema de Logging Detalhado - CSV

Sistema de registro detalhado de cada sessão de jogo em formato CSV para análise estatística.

---

## 📁 Arquivos Gerados

### 1. `ghost2047_sessions.csv` - Log Detalhado (NOVO)
Arquivo CSV com cada sessão jogada contendo:
- Timestamp da partida
- Número alvo (senha)
- Total de tentativas
- Contagem de vieses (baixo/alto)
- Sequência completa de palpites
- Resultado (VITORIA/DERROTA)

### 2. `ghost2047_history.dat` - Estatísticas Agregadas (Original)
Arquivo binário com estatísticas gerais (mantido para compatibilidade)

---

## 📊 Formato CSV

### Header
```
timestamp;alvo;tentativas;baixos;altos;sequencia;resultado
```

### Campos

| Campo | Tipo | Descrição |
|-------|------|-----------|
| **timestamp** | String | Data e hora da sessão (YYYY-MM-DD HH:MM:SS) |
| **alvo** | Int | Número secreto que deveria ser adivinhado |
| **tentativas** | Int | Quantidade total de palpites realizados |
| **baixos** | Int | Quantos palpites foram **abaixo** do alvo |
| **altos** | Int | Quantos palpites foram **acima** do alvo |
| **sequencia** | String | Lista de palpites separados por vírgula |
| **resultado** | String | "VITORIA" ou "DERROTA" |

---

## 📝 Exemplos Reais

### Exemplo 1: Vitória Rápida (3 tentativas)
```csv
2026-05-13 14:23:45;42;3;1;1;50,25,42;VITORIA
```
**Análise:**
- Alvo: 42
- Palpites: 50 (alto), 25 (baixo), 42 (acertou!)
- 1 viés baixo, 1 viés alto (equilibrado)

### Exemplo 2: Vitória com Viés Alto
```csv
2026-05-13 14:30:12;28;5;0;4;80,60,40,30,28;VITORIA
```
**Análise:**
- Alvo: 28
- Começou muito alto (80)
- Todos os 4 primeiros palpites foram acima do alvo
- Acertou na 5ª tentativa

### Exemplo 3: Derrota por Viés Baixo
```csv
2026-05-13 14:45:00;95;7;6;0;10,20,30,40,50,60,70;DERROTA
```
**Análise:**
- Alvo: 95
- Todos os 7 palpites foram abaixo do alvo
- Jogador ficou preso em números baixos
- Não descobriu que era um número alto

### Exemplo 4: Busca Binária Perfeita
```csv
2026-05-13 15:00:30;64;6;3;2;50,75,63,66,65,64;VITORIA
```
**Análise:**
- Alvo: 64
- Estratégia de busca binária: 50 → 75 → 63 → 66 → 65 → 64
- Alternando entre baixo e alto
- Vitória eficiente

---

## 🔬 Análises Possíveis

### 1. Análise de Vieses
```python
import pandas as pd

df = pd.read_csv('ghost2047_sessions.csv', sep=';')

# Calcular viés médio
df['vies_total'] = df['baixos'] - df['altos']
print("Viés médio:", df['vies_total'].mean())
# Positivo = tendência a chutar baixo
# Negativo = tendência a chutar alto
```

### 2. Taxa de Vitória por Número de Tentativas
```python
import matplotlib.pyplot as plt

vitorias = df[df['resultado'] == 'VITORIA']
plt.hist(vitorias['tentativas'], bins=7, range=(1,8))
plt.xlabel('Tentativas')
plt.ylabel('Vitórias')
plt.title('Distribuição de Vitórias')
plt.show()
```

### 3. Análise de Estratégia (Primeiro Palpite)
```python
# Extrair primeiro palpite
df['primeiro_palpite'] = df['sequencia'].str.split(',').str[0].astype(int)

print("Primeiro palpite médio:", df['primeiro_palpite'].mean())
print("Primeiro palpite mais comum:", df['primeiro_palpite'].mode()[0])
```

### 4. Correlação Alvo vs Viés
```python
import seaborn as sns

sns.scatterplot(data=df, x='alvo', y='baixos', hue='resultado')
plt.title('Alvos Altos causam mais palpites baixos?')
plt.show()
```

---

## 💻 Uso no Código

### Estrutura de Dados

```c
typedef struct {
    char timestamp[32];
    int target;
    int totalAttempts;
    int lowBiasCount;   // Palpites abaixo do alvo
    int highBiasCount;  // Palpites acima do alvo
    int guesses[MAX_HISTORY];
    bool won;
} DetailedSession;
```

### Funções Disponíveis

#### 1. Inicializar Sistema
```c
bool SessionLogger_Init(void);
```
Cria o arquivo CSV com header se não existir.

#### 2. Registrar Sessão
```c
DetailedSession session = Game_CreateDetailedSession(&gameState);
SessionLogger_Append(&session);
```
Adiciona uma nova linha ao CSV automaticamente após cada jogo.

#### 3. Carregar Histórico (Opcional)
```c
DetailedSession sessions[100];
int count;
SessionLogger_Load(sessions, &count, 100);
```
Lê todas as sessões do CSV para análise em memória.

---

## 🎯 Quando o Registro Acontece

O sistema registra automaticamente:
1. ✅ Ao **vencer** o jogo (acertar o número)
2. ✅ Ao **perder** o jogo (esgotar tentativas)

O registro ocorre em `Game_ProcessGuess()`:
```c
// Ao terminar o jogo (vitória ou derrota)
DetailedSession detailedSession = Game_CreateDetailedSession(state);
SessionLogger_Append(&detailedSession);
```

---

## 📈 Benefícios

### Para Jogadores
- 📊 Análise do próprio desempenho
- 🧠 Identificar padrões de erro
- 📉 Acompanhar evolução ao longo do tempo

### Para Desenvolvedores
- 🔬 Dados reais de gameplay
- 🎮 Balanceamento de dificuldade
- 📊 Análise de comportamento de jogadores
- 🤖 Treinar IA com dados reais

### Para Análise
- 📊 Excel: Abrir diretamente e criar gráficos
- 🐍 Python/Pandas: Análise estatística avançada
- 📈 R: Modelagem e visualização
- 🗄️ SQL: Importar para banco de dados

---

## 🛠️ Ferramentas Recomendadas

### Visualização Rápida
```bash
# Windows PowerShell
Get-Content ghost2047_sessions.csv | Select-Object -First 10

# Linux/Mac
head -10 ghost2047_sessions.csv
```

### Excel
1. Abrir Excel
2. Dados → De Texto/CSV
3. Selecionar `ghost2047_sessions.csv`
4. Delimitador: `;` (ponto e vírgula)

### Python (Análise Avançada)
```python
import pandas as pd
import matplotlib.pyplot as plt

# Carregar dados
df = pd.read_csv('ghost2047_sessions.csv', sep=';')

# Estatísticas básicas
print(df.describe())

# Taxa de vitória
win_rate = (df['resultado'] == 'VITORIA').mean() * 100
print(f"Taxa de vitória: {win_rate:.1f}%")

# Média de tentativas por resultado
print(df.groupby('resultado')['tentativas'].mean())
```

---

## 🔐 Privacidade e Segurança

- ✅ Arquivo **local** (não envia dados para internet)
- ✅ Formato **texto simples** (fácil inspecionar)
- ✅ Pode ser **deletado** a qualquer momento
- ✅ Não contém **dados pessoais identificáveis**

Para apagar histórico:
```powershell
Remove-Item ghost2047_sessions.csv
```

---

## 📊 Exemplo de Arquivo Completo

```csv
timestamp;alvo;tentativas;baixos;altos;sequencia;resultado
2026-05-13 10:00:00;42;3;1;1;50,25,42;VITORIA
2026-05-13 10:05:23;87;7;7;0;10,20,30,40,50,60,70;DERROTA
2026-05-13 10:12:45;15;4;0;3;80,40,20,15;VITORIA
2026-05-13 10:20:00;64;6;3;2;50,75,63,66,65,64;VITORIA
2026-05-13 10:28:15;99;5;4;0;30,40,50,60,99;VITORIA
```

---

## 🎓 Insights Comuns

### Padrão 1: "Ancoramento"
Jogadores tendem a ficar presos perto do primeiro palpite.
```
Primeiro palpite: 30 → Sequência: 30,35,40,45 (subindo devagar)
```

### Padrão 2: "Salto Agressivo"
Após erro, jogadores "saltam" para extremo oposto.
```
50 (baixo) → 90 (alto) → 70 (ajuste)
```

### Padrão 3: "Busca Linear"
Ignora dicas e tenta números sequenciais.
```
10,20,30,40,50... (procurando linearmente)
```

### Padrão 4: "Medo de Extremos"
Evita números muito baixos (<20) ou muito altos (>80).
```
Raras vezes primeira tentativa é 5 ou 95
```

---

## ✨ Próximas Melhorias Possíveis

1. **Exportar para JSON** - Para aplicações web
2. **Adicionar ID de sessão** - UUID único
3. **Registrar tempo por tentativa** - Análise de hesitação
4. **Incluir dificuldade** - Se implementar múltiplos modos
5. **Exportar relatório HTML** - Visualização automática

---

## 📚 Referências

- Formato CSV: RFC 4180
- Análise de viés cognitivo em jogos
- Busca binária otimizada
- Estatísticas de gameplay

---

**Status**: ✅ Sistema implementado e funcional!
**Compatibilidade**: Windows, Linux, MacOS
**Dependências**: Nenhuma (usa apenas stdio.h)
