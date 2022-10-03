import math
import time
import random
import numpy as np
from scipy import stats
from scipy.stats import chi2
from datetime import datetime
from collections import defaultdict
from matplotlib import pyplot as plt

sample_sizes = np.array([50, 50, 50, 50, 50, 50, 50, 50, 50, 50])  # массив размеров выборок
samples = [];  # пустой массив выборок

a = 23  # сид
b = 12345  # сид
c = 32768  # сид

seed_fibo_1 = 1  # сиды для фибоначчи
seed_fibo_2 = 2


def lineark(size):  # модернизированный линейный конгруэнтный генератор псч
    seed = int(str(time.time())[-5:-1])  # создание сида на основе времени
    if size == 1:
        return math.ceil(
            math.fmod(a * math.ceil(seed) + b, c))  # если размер выборки = 1, то возвращаем 1 сгенерированное число
    ar = [0 for i in range(size)]  # создает массив размера сайз заполненный нулями
    ar[0] = math.ceil(seed)  # первый элемент округленный сид
    for i in range(1, size):  # проходимся по всем остальным элементам
        ar[i] = math.ceil(math.fmod((a * ar[i - 1] + b),
                                    c))  # записываем во все остальные ячейки массива сгенерированный числа ф-мод-остаток от деления
    return ar[1:size]


timez = int(str(time.time())[-1])  # создание сида на основе реального времени


def fibo():  # модернизированный фибоначчи с запаздыванием
    c = 16384  # модуль (2^14)
    global seed_fibo_1, seed_fibo_2, timez  # объявляем глобальные переменные в функции
    temp = (seed_fibo_1 + seed_fibo_2 ** timez + seed_fibo_1 ^ seed_fibo_2) % c + 1
    # изменённая формула fibo
    timez = (timez + seed_fibo_2) % 4  # меняем степень
    seed_fibo_2 = seed_fibo_1  # перезаписываем последние значения элементов
    seed_fibo_1 = temp
    if seed_fibo_1 > 10000:  # исключаем значения от 10001 до 16383
        return fibo()
    return seed_fibo_1


def fibogen(size):  # для алгоритма фибоначчи создаем лист с н-размерным количеством элементов выборки
    sm = []
    for i in range(1, size):
        sm.append(fibo())  # впихиваем числа сгенерированные фибоначчи
    return sm


def grouping(n, x):
    int_number = 1 + math.floor(math.log(n, 2))
    width = (math.floor(x[-1]) + 1 - math.floor(x[0])) / int_number
    arr = []
    counter = 0
    left_side = math.floor(x[0])
    right_side = left_side + width
    arr_middle = []
    i = 0
    while i < len(x):
        if x[i] >= right_side:
            arr_middle.append(left_side)
            arr.append(counter)
            counter = 0
            left_side = right_side
            right_side += width
        else:
            counter += 1
            i += 1
    arr_middle.append(left_side)
    arr.append(counter)
    return (arr_middle, width, arr)


def xi_square(xissample):
    alpha = 0.05  # уровень значимости
    x, width, p = grouping(len(xissample), xissample)
    sigma = 0
    minimum = min(xissample)
    maximum = max(xissample)
    for i in range(len(x) - 1):
        theoretical = dist_func(x[i + 1]) - dist_func(x[i])
        sigma += (p[i] - len(xissample) * theoretical) ** 2 / (len(xissample) * theoretical)
    theoretical = dist_func(x[-1] + width) - dist_func(x[-1])
    sigma += (p[-1] - len(xissample) * theoretical) ** 2 / (len(xissample) * theoretical)
    talpha = chi2.ppf(1 - alpha, df=len(x) - 1)
    if sigma <= talpha:
        print("гипотеза принята")
    else:
        print("гипотеза отвергнута")


def dist_func(x):
    mm = (x - minimum) / maximum
    return mm


for i in sample_sizes:
    arl = fibogen(i)
    arl.sort()
    samples.append(arl)

for i, k in enumerate(samples):
    arrnp = np.array(k)
    print(i + 1, ") Среднее:", arrnp.mean(), ", Отклонение:", arrnp.std(), ", Коэффициент вариации:",
          arrnp.std() / arrnp.mean(), sep="")
    if abs(int((arrnp.std() / arrnp.mean()) * 100)) <= 35:
        print("Однородна")
    else:
        print("Не однородна")
    minimum = min(k)
    maximum = max(k)
    xi_square(k)

sizes = [1000]  # первое число в массиве
for i in range(100000, 1000000, 10000):
    sizes.append(i)
time_lineark = []
time_fibogen = []
time_randint = []

for size in sizes:

    temp_time = time.time()
    for i in range(size):
        random.randint(0, size)
    time_randint.append(time.time() - temp_time)

    temp_time = time.time()
    lineark(size)
    time_lineark.append(time.time() - temp_time)

    temp_time = time.time()
    fibogen(size)
    time_fibogen.append(time.time() - temp_time)

plt.plot(sizes, time_lineark, label='Первый метод')
plt.plot(sizes, time_fibogen, label='Второй метод')
plt.plot(sizes, time_randint, label='Встроенный метод')

plt.xlabel("Размер выборки")
plt.ylabel("Время генерации")
plt.legend()

plt.show()
