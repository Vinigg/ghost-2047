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
    return df

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
    print(f"\nMédia de tentativas: {df['tentativas'].mean():.2f}")
    print(f"Melhor resultado: {df[df['resultado']=='VITORIA']['tentativas'].min()} tentativas")
    
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
