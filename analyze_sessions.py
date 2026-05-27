#!/usr/bin/env python3
"""
GH0ST: 2047 - Analisador de Sessões CSV
Análise estatística das sessões de jogo
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
from pathlib import Path

def load_sessions(filename='ghost2047_sessions.csv'):
    """Carrega o arquivo CSV de sessões"""
    if not Path(filename).exists():
        print(f"❌ Arquivo não encontrado: {filename}")
        print("💡 Jogue algumas partidas primeiro para gerar dados!")
        return None
    
    df = pd.read_csv(filename, sep=';')
    print(f"✅ Carregadas {len(df)} sessões")
    if df.empty:
        print("💡 O arquivo existe, mas ainda não possui sessões registradas.")
        return None
    return df

def recursive_sum(values, index=0):
    """Soma recursiva."""
    if index >= len(values):
        return 0
    return values[index] + recursive_sum(values, index + 1)

def recursive_min(values, index=0):
    """Menor valor recursivo."""
    if not values:
        return None
    if index == len(values) - 1:
        return values[index]
    tail_min = recursive_min(values, index + 1)
    return values[index] if values[index] < tail_min else tail_min

def recursive_max(values, index=0):
    """Maior valor recursivo."""
    if not values:
        return None
    if index == len(values) - 1:
        return values[index]
    tail_max = recursive_max(values, index + 1)
    return values[index] if values[index] > tail_max else tail_max

def recursive_sum_squares(values, mean, index=0):
    """Soma recursiva dos quadrados dos desvios."""
    if index >= len(values):
        return 0
    deviation = values[index] - mean
    return deviation ** 2 + recursive_sum_squares(values, mean, index + 1)

def calculate_recursive_aggregates(values):
    """Calcula média, melhor, pior e desvio usando funções recursivas."""
    if not values:
        return None

    total = recursive_sum(values)
    mean = total / len(values)
    variance = recursive_sum_squares(values, mean) / len(values)

    return {
        'media': mean,
        'melhor': recursive_min(values),
        'pior': recursive_max(values),
        'desvio': variance ** 0.5,
    }

def parse_guess_sequence(sequence):
    """Converte a sequência CSV de palpites em lista de inteiros."""
    return [int(value) for value in str(sequence).split(',') if value.strip()]

def longest_monotonic_run(guesses):
    """Maior sequência crescente ou decrescente consecutiva."""
    if len(guesses) < 2:
        return len(guesses)

    best = 1
    current = 1
    direction = 0

    for i in range(1, len(guesses)):
        diff = guesses[i] - guesses[i - 1]
        new_direction = 1 if diff > 0 else -1 if diff < 0 else 0

        if new_direction != 0 and new_direction == direction:
            current += 1
        elif new_direction != 0:
            current = 2
            direction = new_direction
        else:
            current = 1
            direction = 0

        best = max(best, current)

    return best

def has_repetitive_step_pattern(guesses):
    """Detecta passos repetidos, sinal de busca pouco adaptativa."""
    if len(guesses) < 4:
        return False

    steps = [abs(guesses[i] - guesses[i - 1]) for i in range(1, len(guesses))]
    repeated_steps = sum(1 for i in range(1, len(steps)) if steps[i] == steps[i - 1])
    return repeated_steps >= 2 or len(set(steps)) <= 2

def binary_search_similarity(guesses, target):
    """Percentual de palpites próximos ao meio do intervalo restante."""
    if not guesses:
        return 0

    low = 1
    high = 100
    matches = 0

    for guess in guesses:
        midpoint = (low + high) / 2
        if abs(guess - midpoint) <= 10:
            matches += 1

        if guess < target:
            low = max(low, guess + 1)
        elif guess > target:
            high = min(high, guess - 1)

    return matches / len(guesses) * 100

def analyze_basic_stats(df):
    """Estatísticas básicas"""
    print("\n" + "="*60)
    print("📊 ESTATÍSTICAS GERAIS")
    print("="*60)
    
    total = len(df)
    vitorias = (df['resultado'] == 'VITORIA').sum()
    derrotas = total - vitorias
    
    print(f"Total de sessões: {total}")
    print(f"Vitórias: {vitorias} ({vitorias/total*100:.1f}%)")
    print(f"Derrotas: {derrotas} ({derrotas/total*100:.1f}%)")
    attempts = df['tentativas'].astype(float).tolist()
    aggregates = calculate_recursive_aggregates(attempts)
    if aggregates:
        print(f"\nMédia de tentativas: {aggregates['media']:.2f}")
        print(f"Melhor resultado: {aggregates['melhor']:.0f} tentativas")
        print(f"Pior resultado: {aggregates['pior']:.0f} tentativas")
        print(f"Desvio padrão populacional: {aggregates['desvio']:.2f}")

    if vitorias > 0:
        best_idx = df[df['resultado']=='VITORIA']['tentativas'].idxmin()
        best_session = df.loc[best_idx]
        print(f"Melhor sessão: #{best_idx + 1} ({best_session['timestamp']}) - "
              f"{best_session['tentativas']} tentativas")

    worst_idx = df['tentativas'].idxmax()
    worst_session = df.loc[worst_idx]
    print(f"Pior sessão: #{worst_idx + 1} ({worst_session['timestamp']}) - "
          f"{worst_session['tentativas']} tentativas")
    
def analyze_bias(df):
    """Análise de viés cognitivo"""
    print("\n" + "="*60)
    print("🧠 ANÁLISE DE VIÉS")
    print("="*60)
    
    df['vies'] = df['baixos'] - df['altos']
    
    print(f"Viés médio: {df['vies'].mean():.2f}")
    if df['vies'].mean() > 0:
        print("   → Tendência a chutar BAIXO")
    elif df['vies'].mean() < 0:
        print("   → Tendência a chutar ALTO")
    else:
        print("   → Estratégia EQUILIBRADA")
    
    print(f"\nPalpites baixos médios: {df['baixos'].mean():.2f}")
    print(f"Palpites altos médios: {df['altos'].mean():.2f}")
    
    # Correlação alvo vs viés
    corr = df[['alvo', 'baixos', 'altos']].corr()
    print(f"\nCorrelação alvo ↔ palpites baixos: {corr.loc['alvo', 'baixos']:.3f}")
    print(f"Correlação alvo ↔ palpites altos: {corr.loc['alvo', 'altos']:.3f}")

def analyze_first_guess(df):
    """Análise do primeiro palpite"""
    print("\n" + "="*60)
    print("🎯 ANÁLISE DO PRIMEIRO PALPITE")
    print("="*60)
    
    # Extrair primeiro palpite
    df['primeiro'] = df['sequencia'].str.split(',').str[0].astype(int)
    
    print(f"Primeiro palpite médio: {df['primeiro'].mean():.1f}")
    print(f"Primeiro palpite mais comum: {df['primeiro'].mode().values[0]}")
    print(f"Desvio padrão: {df['primeiro'].std():.1f}")
    
    # Distribuição
    ranges = [(1,25), (26,50), (51,75), (76,100)]
    for low, high in ranges:
        count = ((df['primeiro'] >= low) & (df['primeiro'] <= high)).sum()
        print(f"   {low:2d}-{high:3d}: {count:2d} vezes ({count/len(df)*100:.1f}%)")

def analyze_by_target_range(df):
    """Análise por faixa do alvo"""
    print("\n" + "="*60)
    print("🎲 DESEMPENHO POR FAIXA DE ALVO")
    print("="*60)
    
    ranges = [
        (1, 25, "Muito Baixo"),
        (26, 50, "Baixo"),
        (51, 75, "Alto"),
        (76, 100, "Muito Alto")
    ]
    
    for low, high, label in ranges:
        subset = df[(df['alvo'] >= low) & (df['alvo'] <= high)]
        if len(subset) > 0:
            win_rate = (subset['resultado'] == 'VITORIA').mean() * 100
            avg_attempts = subset['tentativas'].mean()
            print(f"{label:12s} ({low:2d}-{high:3d}): "
                  f"{len(subset):2d} jogos, "
                  f"{win_rate:.0f}% vitórias, "
                  f"{avg_attempts:.1f} tent. médias")

def analyze_strategy_heuristics(df):
    """Gera heurísticas textuais sobre a estratégia do jogador."""
    print("\n" + "="*60)
    print("🧭 HEURÍSTICAS DE ESTRATÉGIA")
    print("="*60)

    if 'primeiro' not in df.columns:
        df['primeiro'] = df['sequencia'].str.split(',').str[0].astype(int)
    if 'vies' not in df.columns:
        df['vies'] = df['baixos'] - df['altos']

    win_rate = (df['resultado'] == 'VITORIA').mean() * 100
    avg_attempts = df['tentativas'].mean()
    avg_first_guess = df['primeiro'].mean()
    avg_bias = df['vies'].mean()
    avg_low = df['baixos'].mean()
    avg_high = df['altos'].mean()
    sequences = [parse_guess_sequence(sequence) for sequence in df['sequencia']]
    extreme_starts = sum(1 for sequence in sequences if sequence and (sequence[0] <= 20 or sequence[0] >= 80))
    monotonic_long = sum(1 for sequence in sequences if longest_monotonic_run(sequence) >= 4)
    repetitive_patterns = sum(1 for sequence in sequences if has_repetitive_step_pattern(sequence))
    binary_scores = [
        binary_search_similarity(sequence, target)
        for sequence, target in zip(sequences, df['alvo'])
        if sequence
    ]
    avg_binary_score = sum(binary_scores) / len(binary_scores) if binary_scores else 0
    extreme_rate = extreme_starts / len(sequences) * 100
    monotonic_rate = monotonic_long / len(sequences) * 100
    repetitive_rate = repetitive_patterns / len(sequences) * 100

    if avg_bias > 1:
        print("- Tendência dominante: chutes abaixo do alvo. Ajuste mais cedo para faixas altas.")
    elif avg_bias < -1:
        print("- Tendência dominante: chutes acima do alvo. Ajuste mais cedo para faixas baixas.")
    else:
        print("- Tendência dominante: equilíbrio entre chutes baixos e altos.")

    if 45 <= avg_first_guess <= 55:
        print("- Abertura: primeiro palpite perto do centro, coerente com busca binária.")
    elif avg_first_guess < 45:
        print("- Abertura: primeiro palpite geralmente baixo; pode atrasar alvos altos.")
    else:
        print("- Abertura: primeiro palpite geralmente alto; pode atrasar alvos baixos.")

    if extreme_rate >= 30:
        print(f"- Começo extremo: {extreme_rate:.0f}% das sessões começam muito baixo ou muito alto.")
    else:
        print(f"- Começo extremo: baixo ({extreme_rate:.0f}% das sessões).")

    if monotonic_rate >= 30:
        print(f"- Sequências monotônicas longas: {monotonic_rate:.0f}% das sessões sobem ou descem por tempo demais.")
    else:
        print(f"- Sequências monotônicas longas: ocorrência controlada ({monotonic_rate:.0f}%).")

    if repetitive_rate >= 30:
        print(f"- Padrão repetitivo: {repetitive_rate:.0f}% das sessões repetem passos parecidos e perdem eficiência.")
    else:
        print(f"- Padrão repetitivo: baixa incidência ({repetitive_rate:.0f}%).")

    if avg_binary_score >= 70:
        print(f"- Busca binária: forte aproximação do comportamento binário ({avg_binary_score:.0f}%).")
    elif avg_binary_score >= 40:
        print(f"- Busca binária: aproximação parcial do comportamento binário ({avg_binary_score:.0f}%).")
    else:
        print(f"- Busca binária: baixa aproximação do comportamento binário ({avg_binary_score:.0f}%).")

    if avg_attempts <= 4:
        print("- Eficiência: boa convergência, com poucas tentativas por sessão.")
    elif avg_attempts <= 6:
        print("- Eficiência: desempenho intermediário; reduzir saltos pode melhorar.")
    else:
        print("- Eficiência: muitas tentativas por sessão; priorize dividir o intervalo ao meio.")

    if win_rate >= 70:
        print("- Resultado geral: taxa de vitória alta, estratégia consistente.")
    elif win_rate >= 40:
        print("- Resultado geral: taxa de vitória moderada, com margem para ajustar decisões.")
    else:
        print("- Resultado geral: baixa taxa de vitória; use mais as dicas de MAIOR/MENOR.")

    if abs(avg_low - avg_high) <= 0.5:
        print("- Padrão de ajuste: alternância saudável entre correções para cima e para baixo.")

def plot_visualizations(df):
    """Gera visualizações"""
    print("\n" + "="*60)
    print("📈 GERANDO GRÁFICOS...")
    print("="*60)
    
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('GH0ST: 2047 - Análise de Sessões', fontsize=16, fontweight='bold')
    
    # 1. Distribuição de tentativas
    vitorias = df[df['resultado'] == 'VITORIA']
    axes[0, 0].hist(vitorias['tentativas'], bins=7, range=(1, 8), 
                    color='#00FF9C', edgecolor='black', alpha=0.7)
    axes[0, 0].set_xlabel('Número de Tentativas')
    axes[0, 0].set_ylabel('Frequência')
    axes[0, 0].set_title('Distribuição de Vitórias por Tentativas')
    axes[0, 0].grid(True, alpha=0.3)
    
    # 2. Taxa de vitória ao longo do tempo
    df['sessao_num'] = range(1, len(df) + 1)
    df['vitorias_acum'] = (df['resultado'] == 'VITORIA').cumsum()
    df['taxa_vitoria'] = df['vitorias_acum'] / df['sessao_num'] * 100
    
    axes[0, 1].plot(df['sessao_num'], df['taxa_vitoria'], 
                    color='#FF3B3B', linewidth=2, marker='o', markersize=4)
    axes[0, 1].axhline(y=50, color='gray', linestyle='--', alpha=0.5)
    axes[0, 1].set_xlabel('Número da Sessão')
    axes[0, 1].set_ylabel('Taxa de Vitória (%)')
    axes[0, 1].set_title('Evolução da Taxa de Vitória')
    axes[0, 1].grid(True, alpha=0.3)
    
    # 3. Viés por resultado
    df['vies'] = df['baixos'] - df['altos']
    vitorias_vies = df[df['resultado'] == 'VITORIA']['vies']
    derrotas_vies = df[df['resultado'] == 'DERROTA']['vies']
    
    axes[1, 0].boxplot([vitorias_vies, derrotas_vies],
                       labels=['Vitórias', 'Derrotas'],
                       patch_artist=True,
                       boxprops=dict(facecolor='#00FF9C', alpha=0.7))
    axes[1, 0].axhline(y=0, color='red', linestyle='--', alpha=0.5)
    axes[1, 0].set_ylabel('Viés (Baixos - Altos)')
    axes[1, 0].set_title('Viés Cognitivo por Resultado')
    axes[1, 0].grid(True, alpha=0.3)
    
    # 4. Scatter: Alvo vs Tentativas
    colors = df['resultado'].map({'VITORIA': '#00FF9C', 'DERROTA': '#FF3B3B'})
    axes[1, 1].scatter(df['alvo'], df['tentativas'], 
                      c=colors, alpha=0.6, s=100, edgecolors='black')
    axes[1, 1].set_xlabel('Número Alvo')
    axes[1, 1].set_ylabel('Tentativas')
    axes[1, 1].set_title('Alvo vs Tentativas (Verde=Vitória, Vermelho=Derrota)')
    axes[1, 1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    # Salvar
    output_file = 'ghost2047_analise.png'
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"✅ Gráficos salvos em: {output_file}")
    
    plt.show()

def main():
    """Função principal"""
    print("\n" + "="*60)
    print("👻 GH0ST: 2047 - ANALISADOR DE SESSÕES")
    print("="*60)
    
    # Carregar dados
    filename = 'ghost2047_sessions.csv'
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    
    df = load_sessions(filename)
    if df is None:
        return
    
    # Análises
    analyze_basic_stats(df)
    analyze_bias(df)
    analyze_first_guess(df)
    analyze_by_target_range(df)
    analyze_strategy_heuristics(df)
    
    # Visualizações (opcional)
    try:
        plot_visualizations(df)
    except Exception as e:
        print(f"\n⚠️  Erro ao gerar gráficos: {e}")
        print("💡 Instale matplotlib: pip install matplotlib")
    
    print("\n" + "="*60)
    print("✅ Análise concluída!")
    print("="*60 + "\n")

if __name__ == "__main__":
    main()
