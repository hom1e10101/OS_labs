import subprocess
import os
import matplotlib.pyplot as plt
import numpy as np


times = []
arr_size = 2**17


def run_code(n):
    result = subprocess.run(["./src/kek"], input=f"{arr_size}\n{n}\n",capture_output=True, text=True)
    if (result.returncode == 1):
        print("Error")
        return
    print(f"num_treads = {n} tested\noutput is {result.stdout}")
    times.append(float(result.stdout))


compile_result = subprocess.run(
    ["g++", "-std=c++20", "./src/kek.cpp", "-o", "./src/kek"],
    capture_output=True,
    text=True
)

if compile_result.returncode != 0:
    print("Ошибка компиляции:")
    print(compile_result.stderr)

print("файл скомпилировался")

for i in [1, 2, 4, 8, 16, 32, 64]:
    run_code(i)

threads = [1, 2, 4, 8, 16, 32, 64]
T1 = times[0]

speedup = [T1 / t for t in times]
efficiency = [s / n for s, n in zip(speedup, threads)]

plt.figure(figsize=(10, 5))
plt.plot(threads, speedup, marker='o', label='Ускорение (S = T1 / Tn)')
plt.xlabel('Количество потоков')
plt.ylabel('Ускорение')
plt.title(f'Зависимость ускорения от количества потоков (Массив {arr_size} элементов)')
plt.grid(True)
plt.legend()

plt.xticks(threads, labels=[1, 2, 4, 8, 16, 32, 64])

plt.savefig('speedup_2**17.png')
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(threads, efficiency, marker='o', color='red', label='Эффективность (E = S / n)')
plt.xlabel('Количество потоков')
plt.ylabel('Эффективность')
plt.title(f'Зависимость эффективности от количества потоков (Массив {arr_size} элементов)')
plt.grid(True)
plt.legend()

plt.xticks(threads, labels=[1, 2, 4, 8, 16, 32, 64])

plt.savefig('efficiency_2**17.png')
plt.show()


