import time


class Singleton(type):  # класс для паттерна синглтон
    _instances = {}  # экземпляры класса

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]


class Logger(metaclass=Singleton):
    E_NORMAL = 0  # normal event
    E_WARNING = 1  # warning event
    E_ERROR = 2  # error event
    COLORS = ['\033[92m', '\033[93m', '\033[91m']  # экранированные последовательности цветов
    NAMES = ['NORMAL', 'WARNING', 'ERROR']  # названия событий
    ENDC = '\033[0m'  # экранированная последовательность в конце формата

    def __init__(self):  # Конструктор. Обнуляет лист событий
        self._events = []  # обнуление листа

    @property
    def events(self):  # геттер листа событий
        return self._events

    @property
    def count(self):
        return len(self.events)  # геттер счетчика событий

    def print(self, n=None, level=E_NORMAL):  # Выводит форматированные события.
        real_n = min(n, self.count) if n is not None else self.count  # подсчет элементов в логгере ивентов
        for event in self.events[
                     self.count - real_n:self.count]:  # задается цикл, который проходит по всем элементам ивента
            e_type = event[0]  # приравниваем к переменной e_type уровень ивента
            if e_type >= level:  # если уровень ивента больше заданного уровеня для вывода, то выводится информация об ивенте
                print(Logger.COLORS[e_type]  # указываем цвет для вывода
                      + '[' + Logger.NAMES[e_type] + '] '  # указываем тип ивента
                      + str(time.ctime(event[2])) + ' '  # время ивента
                      + event[1]  # счетчик евента
                      + Logger.ENDC)  # перенос строки

    def __add(self, e_type, e_text, e_time):  # добавляет событие в список событий
        self.events.append((e_type, e_text, e_time))

    def warning(self, e_text):  # добавляет предупреждение в список
        self.__add(Logger.E_WARNING, e_text, time.time())

    def error(self, e_text):  # добавляет ошибку в список
        self.__add(Logger.E_ERROR, e_text, time.time())

    def normal(self, e_text):  # добавляет нормльное в список
        self.__add(Logger.E_NORMAL, e_text, time.time())


if __name__ == '__main__':
    print("Lab 5. Singleton. ")
    print("Command 'n' - normal event, 'w' - warning, 'e' - error, 'exit' - exit and print last 10 events.")
    command = ''
    while command != 'exit':
        command = input("Enter command: ")
        if command == 'n':
            Logger().normal("Normal event. Count = " + str(Logger().count))
        elif command == 'w':
            Logger().warning("Warning event. Count = " + str(Logger().count))
        elif command == 'e':
            Logger().error("Error event. Count = " + str(Logger().count))
    print('\033[1m' + "\nPRINTING LAST 10 EVENTS\n" + '\033[0m')
    Logger().print(10)
