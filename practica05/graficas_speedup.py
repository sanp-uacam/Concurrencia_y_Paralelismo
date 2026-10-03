import matplotlib.pyplot as plt

# ====== TUS DATOS REALES ======
hilos = [1, 4, 8]
tiempos = [0.015912, 0.005334, 0.004464]
# ==============================

T1 = tiempos[0]

speedup = [T1 / t for t in tiempos]
eficiencia = [speedup[i] / hilos[i] for i in range(len(hilos))]
overhead = [hilos[i] * tiempos[i] - T1 for i in range(len(hilos))]

# ---- Gráfica 1: Tiempo ----
plt.figure(figsize=(8, 5))
bars = plt.bar(hilos, tiempos, color='#3498db', width=1.5)
for b, t in zip(bars, tiempos):
    plt.text(b.get_x() + b.get_width()/2, t + 0.0005,
             f'{t:.4f}s', ha='center', fontsize=11, fontweight='bold')
plt.title('Tiempo de ejecución vs Número de hilos')
plt.xlabel('Número de hilos')
plt.ylabel('Tiempo (segundos)')
plt.xticks(hilos)
plt.grid(True, linestyle='--', alpha=0.5, axis='y')
plt.tight_layout()
plt.savefig('grafica_tiempo.png', dpi=150)
plt.show()

# ---- Gráfica 2: Speedup ----
plt.figure(figsize=(8, 5))
plt.bar(hilos, speedup, color='#2ecc71', width=1.5, label='Speedup real')
plt.plot(hilos, hilos, 'r--o', label='Speedup ideal (lineal)')
for h, s in zip(hilos, speedup):
    plt.text(h, s + 0.05, f'{s:.2f}x', ha='center',
             fontsize=11, fontweight='bold')
plt.title('Speedup vs Número de hilos')
plt.xlabel('Número de hilos')
plt.ylabel('Speedup S(p)')
plt.xticks(hilos)
plt.grid(True, linestyle='--', alpha=0.5, axis='y')
plt.legend()
plt.tight_layout()
plt.savefig('grafica_speedup.png', dpi=150)
plt.show()

# ---- Gráfica 3: Eficiencia + Overhead ----
fig, ax1 = plt.subplots(figsize=(8, 5))
bars = ax1.bar(hilos, eficiencia, color='#9b59b6', width=1.5,
               alpha=0.7, label='Eficiencia')
ax1.set_xlabel('Número de hilos')
ax1.set_ylabel('Eficiencia', color='#9b59b6')
ax1.tick_params(axis='y', labelcolor='#9b59b6')
ax1.set_ylim(0, 1.15)
ax1.set_xticks(hilos)
for b, e in zip(bars, eficiencia):
    ax1.text(b.get_x() + b.get_width()/2, e + 0.02,
             f'{e*100:.1f}%', ha='center', fontsize=10,
             fontweight='bold', color='#9b59b6')

ax2 = ax1.twinx()
ax2.plot(hilos, overhead, 'o-', color='#e74c3c',
         linewidth=2, markersize=10, label='Overhead')
ax2.set_ylabel('Overhead (segundos)', color='#e74c3c')
ax2.tick_params(axis='y', labelcolor='#e74c3c')
for h, o in zip(hilos, overhead):
    ax2.text(h, o + 0.0005, f'{o:.4f}s', ha='center',
             fontsize=10, fontweight='bold', color='#e74c3c')

plt.title('Eficiencia y Overhead vs Número de hilos')
plt.tight_layout()
plt.savefig('grafica_eficiencia_overhead.png', dpi=150)
plt.show()

print("¡Gráficas generadas!")
print(f"Speedup: {[f'{s:.2f}' for s in speedup]}")
print(f"Eficiencia: {[f'{e*100:.1f}%' for e in eficiencia]}")
print(f"Overhead: {[f'{o:.4f}' for o in overhead]}")