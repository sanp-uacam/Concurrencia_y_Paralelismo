import matplotlib.pyplot as plt

hilos = [2, 8, 16]
tiempos = [0.008322, 0.004397, 0.004834]

plt.figure(figsize=(8,5))
plt.plot(hilos, tiempos, marker='o', linewidth=2, color='#1f77b4')
for h, t in zip(hilos, tiempos):
    plt.annotate(f'{t:.6f}s', (h, t), textcoords="offset points",
                 xytext=(0,10), ha='center')

plt.title('Número de hilos vs. Tiempo de ejecución')
plt.xlabel('Número de hilos')
plt.ylabel('Tiempo (segundos)')
plt.xticks(hilos)
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.savefig('grafica_pi.png', dpi=150)
plt.show()