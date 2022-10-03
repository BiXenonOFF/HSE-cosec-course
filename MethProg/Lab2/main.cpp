#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <map>
#include <string>


struct Ship {
    std::string sNames;
    int iRoomNumber;
    int iRoomType;
    std::string sDestination;
};


/// Перегрузки ///
bool operator > (Ship Left, Ship Right) {
    if (Left.sNames > Right.sNames) { return true; }
    if (Left.sNames < Right.sNames) { return false; }
    if (Left.iRoomNumber > Right.iRoomNumber) { return true; }
    if (Left.iRoomNumber < Right.iRoomNumber) { return false; }
    if (Left.sDestination > Right.sDestination) { return true; }
    if (Left.sDestination < Right.sDestination) { return false; }

    return false;
}

bool operator >= (Ship Left, Ship Right) {
    if (Left.sNames > Right.sNames) { return true; }
    if (Left.sNames < Right.sNames) { return false; }
    if (Left.iRoomNumber > Right.iRoomNumber) { return true; }
    if (Left.iRoomNumber < Right.iRoomNumber) { return false; }
    if (Left.sDestination > Right.sDestination) { return true; }
    if (Left.sDestination < Right.sDestination) { return false; }
    return true;
}

bool operator < (Ship Left, Ship Right) {
    if (Left.sNames > Right.sNames) { return false; }
    if (Left.sNames < Right.sNames) { return true; }
    if (Left.iRoomNumber > Right.iRoomNumber) { return false; }
    if (Left.iRoomNumber < Right.iRoomNumber) { return true; }
    if (Left.sDestination > Right.sDestination) { return false; }
    if (Left.sDestination < Right.sDestination) { return true; }
    return false;
}

bool operator <= (Ship Left, Ship Right) {
    if (Left.sNames > Right.sNames) { return false; }
    if (Left.sNames < Right.sNames) { return true; }
    if (Left.iRoomNumber > Right.iRoomNumber) { return false; }
    if (Left.iRoomNumber < Right.iRoomNumber) { return true; }
    if (Left.sDestination > Right.sDestination) { return false; }
    if (Left.sDestination < Right.sDestination) { return true; }
    return true;
}






// импортирование Базы Данных в вектор классов
std::vector<Ship> upload(std::string path) {
    std::ifstream opened(path);
    std::vector<Ship> returned;
    std::string gl;
    std::vector<std::string> gl_string;
    while (std::getline(opened, gl))
    {
        gl_string.clear();
        char* str = (char*)gl.c_str();// конвертируем const* char -> char*, тк функция строк принимает в качестве аргумента char*
        Ship cruise;
        char* pch = strtok(str, "/");
        while (pch != NULL)
        {
            gl_string.push_back(pch);
            pch = strtok(NULL, "/");
        }
        if (!gl_string.empty()) {
            cruise.sNames = gl_string.at(0);
            cruise.iRoomNumber = stoi(gl_string.at(1));
            cruise.iRoomType = gl_string.at(2) == "luxe" ? 0 : stoi(gl_string.at(2));
            cruise.sDestination = gl_string.at(3);

            returned.push_back(cruise);
        }
    }
    return returned;
}


// Сортировка вставками
std::vector<Ship> simple_push(std::vector<Ship> data) {
    std::vector<Ship> addit_vec; // У нас есть два вектора, эдит_век - это готовый, а дата - исходный
    addit_vec.push_back(data.at(0)); // добавляем первый элемент исходного вектора дата в готовый вектор эдит_век
    data.erase(data.begin());// в этой строчке удаляем элемент из исходного вектора, тем самым задаем первый элемент началом отсортированного вектора.
    for (int i = 0; i < data.size(); i++) { // Итерируем по элементам исходного вектора
        for (int j = 0; j < addit_vec.size(); j++) { // итерируем по элементам готового вектора
            if (data.at(i) < addit_vec.at(j)) { // сравниваем элементы готового и исходного вектора(мы берем элемент исходного вектора и сравниваем его до того момента пока не найдем элемент готового вектора, который будет больше проверяемого элемента исходного вектора)
                addit_vec.insert(addit_vec.begin() + j, data.at(i)); // в случае выполнения условия, описанного выше, мы добавляем проверяемый элемент перед элементом исходного вектора, на котором условие оказалось истинной
                break; // Тк условие выполнилось, то мы заканчиваем итерирование по элементам готового вектора и переходим к следующему элементу исходного вектора.
            }
            else if (j + 1 == addit_vec.size())
            {
                addit_vec.push_back(data.at(i));
                break;
            }
        }
    }
    return addit_vec; //возвращаем готовый вектор (он уже отсортирован)
}




//быстрая сортировка
int quick_sort_pointer(std::vector<Ship>* data, int first, int last) // нужно для рекурсии. То же самое, что и quick sort, только работа напрямую с аргументом через указатель
{
    if (first < last)
    {
        int left = first; int right = last; Ship mid = data->at((left + right) / 2);
        while (left < right)
        {
            while (data->at(left) < mid)
                left++;
            while (data->at(right) > mid)
                right--;
            if (left <= right)
            {
                Ship k = data->at(left);
                data->at(left) = data->at(right);
                data->at(right) = k;
                left++; right--;
            }

        }
        quick_sort_pointer(data, first, right);
        quick_sort_pointer(data, left, last);
    }
    return 0;
}
std::vector<Ship> quick_sort(std::vector<Ship> data)
{
    if (0 < data.size() - 1)// проверка на то, что размер вектора больше 1
    {
        int left = 0; int right = data.size() - 1; Ship mid = data.at(data.size() / 2);// объявление начала, конца и опорного элемента(середины)
        while (left < right)// пока начало меньше конца
        {
            while (data.at(left) < mid)// ищем элементы, которые больше опорного элемента (элементы находятся левее опорного элемента)
                left++;// при итерировании элемента, если выполняется условие цикла (элемент меньше опорного), то к left прибавляем единичку, то есть переходим к следующему элементу
            while (data.at(right) > mid)// ищем элементы, которые меньше опорного элемента (элементы находятся правее опорного элемента)
                right--;// при итерировании элемента, если выполняется условие цикла (элемент больше опорного), то у right вычитаем единичку, то есть переходим к следующему элементу
            if (left <= right)// проверка то что левый элементы меньше правого 
            {
                // обмен местами 
                Ship k = data.at(left);
                data.at(left) = data.at(right);
                data.at(right) = k;
                left++; right--;
            }

        }
        // дробим векторы еще раз и сортируем в них. И так делаем, пока основной вектор не отсортируется
        quick_sort_pointer(&data, 0, right);
        quick_sort_pointer(&data, left, data.size() - 1);
    }
    return data;
}

// сортировка выбором
std::vector<Ship> selection_sort(std::vector<Ship> data) {
    for (int i = 0; i < data.size() - 1; i++) {// итерируемся от начала исходного вектора 
        int ismallest = i;// объявляем переменную индекса наименьшего элемента (записываем в нее на данный момент индекс первого элемента исходной последовательности)
        for (int j = i + 1; j < data.size() - 1; ++j) {// итерируемся по след элементам исходной последовательности
            if (operator<(data[j], data[i]))// сравниваем последний элемент готовой последовательности (первый элемент пропускаем) с первым элементом исходной последовательности
                ismallest = j;// если выполняется условие, то в ismallest записываем индекс переменной
        }
        std::swap(data[i], data[ismallest]);// меняем местами наименьший с первым (если наименьший будет первым, то вся последовательность не изменится)
    }
    return data;
}



////Searches//////

Ship linear_search(std::vector<Ship> data, std::string parameter) {
    for (int i = 0; i < data.size(); i++) {  //  итерация от первого элемента
        if (data.at(i).sNames == parameter) // сравниваем итерируемые элементы с параметром
            return data.at(i); // если нашли параметр, то возвращаем его
    }
}

int binary_search_recurtion(std::vector<Ship>& data, std::string& parameter, int min, int max) { //  тот же самый бинарный поиск, только для рекурсии
    int mid = ((min + max) / 2);
    if (data.at(mid).sNames < parameter) {
        binary_search_recurtion(data, parameter, mid + 1, max);
    }
    else if (data.at(mid).sNames > parameter) {
        binary_search_recurtion(data, parameter, min, mid + 1);
    }
    else
        return mid; //  возвращает индекс найденного элемента
}

Ship binary_search(std::vector<Ship> data, std::string parameter) {
    int mid = data.size() / 2; //  объявляем переменную середины
    int min = 0; //  объявляем минимум
    int max = data.size(); //  объявляем максимум
    if (data.at(mid).sNames < parameter) { // проверка условия того, что параметр меньше серединного элемента
        return data.at(binary_search_recurtion(data, parameter, mid + 1, max));
    } // если серединный элемент меньше параметра, то вызываем "рекурсивно" функцию для последовательности находящейся левее серединного элемента 
    else if (data.at(mid).sNames > parameter) { // проверка условия того, что параметр больше серединного элемента
        return data.at(binary_search_recurtion(data, parameter, min, mid + 1));
    }// если серединный элемент больше параметра, то вызываем "рекурсивно" функцию для последовательности находящейся правее серединного элемента 
    else
        return data.at(mid); // возвращаем искомый элемент
}

// объявляем класс для мултимэпа
struct Ship_map {
    int iRoomNumber;
    int iRoomType;
    std::string sDestination;
};

std::multimap<std::string, Ship_map> vectormap(std::vector<Ship> data) { // функция конвертации вектора в мультимэп
    std::multimap<std::string, Ship_map> data_map; //объявляем мультимэп стринга и класса
    for (int i = 0; i < data.size(); i++) { // цикл для записи данных в мультимэп
        Ship_map data_of_map;// объявляем класс для записи в мультимэп
        data_of_map.iRoomNumber = data.at(i).iRoomNumber; // в этой и двух строчках ниже записываем интовые данные из вектора
        data_of_map.iRoomType = data.at(i).iRoomType;
        data_of_map.sDestination = data.at(i).sDestination;
        std::string key = data.at(i).sNames;// записываем из вектора строку в переменную строку
        data_map.insert({ key,data_of_map });// вставляем данные выше в мультимэп
    }
    return data_map; // возвращаем мультимэп
}

int main() {
    std::vector<Ship> a = upload("100000.txt");
    std::vector<Ship> a2 = a;
    std::string name = "Rebecca Hawkins";
    // время по прямому поиску
    auto start = std::chrono::system_clock::now();
    linear_search(a, name);
    auto stop = std::chrono::system_clock::now();
    auto time = stop - start;
    std::cout << "============================================================================" << std::endl << a.size() << ":" << std::endl;
    std::cout << "Прямой поиск:" << (float(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / 1000000000) << std::endl << std::endl;


    // время с быстрой сортирвокой и бинарным поиском
    a = quick_sort(a);
    start = std::chrono::system_clock::now();
    binary_search(a, name);
    stop = std::chrono::system_clock::now();
    time = stop - start;
    std::cout << "Бинарный поиск:" << (float(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / 1000000000) << std::endl << std::endl;

    // поиск по мультимэпу
    std::multimap<std::string, Ship_map> map_done;
    map_done = vectormap(a2);

    start = std::chrono::system_clock::now();
    map_done.find(name);
    stop = std::chrono::system_clock::now();
    time = stop - start;
    std::cout << "Multimap поиск:" << (float(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / 1000000000) << std::endl << std::endl;
    std::cout << "============================================================================" << std::endl;


    return 0;
}
