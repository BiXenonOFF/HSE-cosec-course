import matplotlib.pyplot as plt

dimensions = [100, 500, 1000, 5000, 10000, 50000, 100000]
linear = [1.0175e-05, 9.9979e-05, 0.000192205, 0.000717719, 0.00189989, 0.00466244, 0.0103348] # прямой поиск
binary = [6.352e-06, 2.6471e-05, 0.00011177, 0.000330342, 0.00154674, 0.00390998, 0.00749274] # бинарный поиск
multi = [4.09e-07, 4.54e-07, 1.258e-06, 8.65e-07, 8.98e-07, 3.348e-06, 2.227e-05] #multimap

plt.plot(dimensions, linear, label="Прямой поиск")
plt.plot(dimensions, binary, label="Бинарный поиск")
plt.plot(dimensions, multi, label="Поиск по ключу в multimap")


plt.xlabel("Размер массива")
plt.ylabel("Время поиска")
plt.legend()

plt.yscale('log')
plt.show()
