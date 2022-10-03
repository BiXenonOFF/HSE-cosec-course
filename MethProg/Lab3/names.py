from faker import Faker
import random
fake = Faker('en_US')
list = ['Toulon', 'Barcelona', 'Naples', 'Rome', 'Florence', 'Nicea']
for i in range(1000):
	print(fake.name(),"/",random.randint(0,2700),"/",random.randint(0,3),"/",random.choice(list),sep='')
