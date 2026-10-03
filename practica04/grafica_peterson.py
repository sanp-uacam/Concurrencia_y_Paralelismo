import matplotlib.pyplot as plt

hilos = [2, 4, 8, 16]
tiempos = [0.840285, 0.417484, 0.241340, 0.285300]

plt.figure(figsize=(8, 5))
plt.plot(hilos, tiempos, marker='o', linewidth=2,
         color='#e74c3c', markersize=10, label='Peterson')

for h, t in zip(hilos, tiempos):
    plt.annotate(f'{t:.4f}s', (h, t),
                 textcoords="offset points",
                 xytext=(0, 12), ha='center', fontsize=10)

plt.title('Peterson: Número de hilos vs. Tiempo de ejecución')
plt.xlabel('Número de hilos')
plt.ylabel('Tiempo (segundos)')
plt.xticks(hilos)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig('grafica_peterson.png', dpi=150)
plt.show()