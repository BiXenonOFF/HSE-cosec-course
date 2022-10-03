import matplotlib.pyplot as plt

dimensions = [100, 500, 1000, 5000, 10000, 50000,100000]

bad_hash = [1, 2, 14, 172, 680, 10276, 29974] #������ ���
good_hash = [1, 2, 13, 168, 677, 10260, 29929] #������� ���

plt.plot(dimensions, bad_hash, label="����� �������� � ������� ������ �����")
plt.plot(dimensions, good_hash, label="����� �������� � ������� ������� �����")

plt.xlabel("������ �������")
plt.ylabel("���������� ��������")
plt.legend()

plt.show()
