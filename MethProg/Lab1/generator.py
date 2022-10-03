from faker import Faker
import random
fake = Faker('en_US')
list = ['Moscow', 'Saint-Petersburg', 'Berlin', 'Ottava', 'Luxembourg', 'Vena', 'Oslo', 'Warshaw', 'Bruxelles', 'Paris', 'Marselle', 'Nice']


f = open("in1.txt", 'w')

for i in range(100000):
    f.write(f"{fake.name()};{random.randint(0,2700)};{random.randint(0,3)};{random.choice(list)};\n")

f.close()