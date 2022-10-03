import matplotlib.pyplot as plt

dimensions = [100, 500, 1000, 5000, 10000, 50000,100000]

bad_hash = [1, 2, 14, 172, 680, 10276, 29974] #плохой хеш
good_hash = [1, 2, 13, 168, 677, 10260, 29929] #хороший хеш

plt.plot(dimensions, bad_hash, label="Число коллизий в таблице плохих хешей")
plt.plot(dimensions, good_hash, label="Число коллизий в таблице хороших хешей")

plt.xlabel("Размер массива")
plt.ylabel("Количество коллизий")
plt.legend()

plt.show()
