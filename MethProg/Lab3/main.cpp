#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <map>
#include <cmath>
#include <string>

int bhc_counter = 0;
int ghc_counter = 0;


/// <summary>
/// Converting string to bad hash
/// </summary>
/// <param name="sParameter"></param>
/// <returns></returns>
uint64_t bad_hash(std::string sParameter) 
{
	uint64_t hash = 0;

	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < sParameter.size(); j++) 
		{
			hash += static_cast<uint64_t>((j * static_cast<uint64_t>(sParameter[j])) ^ hash * 11) & 0xFFFFFFFF;
		}
	}

	return hash;
}



/// <summary>
/// Converting string to good hash
/// </summary>
/// <param name="sParameter"></param>
/// <returns></returns>
uint64_t good_hash(std::string sParameter) 
{
	uint64_t hash = 0;

	for (int i = 0; i < sParameter.size(); i++) 
	{
		hash += static_cast<uint64_t>((i * static_cast<uint64_t>(sParameter[i])) ^ hash * 11) & 0xFFFFFFFF;
	}
	return hash;
}

struct Ship 
{
	std::string sNames;
	int iRoomNumber;
	int iRoomType;
	std::string sDestination;
	uint64_t bad_hash;
	uint64_t good_hash;
};

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


std::vector<Ship> upload(std::string path) 
{
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
			cruise.iRoomNumber = std::stoi(gl_string.at(1));
			cruise.iRoomType = gl_string.at(2) == "luxe" ? 0 : stoi(gl_string.at(2));
			cruise.sDestination = gl_string.at(3);
			cruise.bad_hash = bad_hash(cruise.sNames);
			cruise.good_hash = good_hash(cruise.sNames);
			returned.push_back(cruise);
		}
	}
	return returned;
}


// Сортировка простыми вставками
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

// Рекурсивная Быстрая Сортировка
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

// Быстрая Сортировка
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

// Сортировка Выбором
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



// Прямой поиск
Ship linear_search(std::vector<Ship> data, std::string parameter) {
    for (int i = 0; i < data.size(); i++) {  //  итерация от первого элемента
        if (data.at(i).sNames == parameter) // сравниваем итерируемые элементы с параметром
            return data.at(i); // если нашли параметр, то возвращаем его
    }
}

// Рекурсивный бинарный поиск
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

// Бинарный поиск
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

// Конвертирование вектора в Map
class ship_map {
public:
    int room_number;
    int room_type;
    std::string destination_point;
};

std::multimap<std::string, ship_map> vectormap(std::vector<Ship> data) { // функция конвертации вектора в мультимэп
    std::multimap<std::string, ship_map> data_map; //объявляем мультимэп стринга и класса
    for (int i = 0; i < data.size(); i++) { // цикл для записи данных в мультимэп
        ship_map data_of_map;// объявляем класс для записи в мультимэп
        data_of_map.room_number = data.at(i).iRoomNumber; // в этой и двух строчках ниже записываем интовые данные из вектора
        data_of_map.room_type = data.at(i).iRoomType;
        data_of_map.destination_point = data.at(i).sDestination;
        std::string key = data.at(i).sNames;// записываем из вектора строку в переменную строку
        data_map.insert({ key,data_of_map });// вставляем данные выше в мультимэп
    }
    return data_map; // возвращаем мультимэп
}

//создаем таблицу из плохих хешей
std::map<uint64_t, std::vector<int>> bad_hash_table(std::vector<Ship> a)
{
    bhc_counter = 0;
    std::map<uint64_t, std::vector<int>> returned;//объявляем таблицу
    std::vector<int> index; index.push_back(0);//объявляем массив индексов относящихся к определенному хешу, добавляем в него первый элемент
    returned[a[0].bad_hash] = index;//приравниваем в таблице массив индексов к хешу
    for (int i = 1; i < a.size(); i++)
    {
        std::vector<int> index;
        if (returned.find(a[i].bad_hash) != returned.cend())//проверка, если хеш уже существует(происходит коллизия), то мы добавляем к массиву индексов найденный массив. 
        {
            index = returned[a[i].bad_hash];// возвращаем из таблицы индексов массив индексов
            index.push_back(i);// запихиваем в него найденный элемент 
            returned[a[i].bad_hash] = index;// обновленный массив записываем обратно
            bhc_counter++;
        }
        else//в случае если коллизия не произошла, то просто создаем новую строчку в хеш-таблице и записываем в нее единственный индекс
        {
            index.push_back(i);
            returned[a[i].bad_hash] = index;

        }

    }

    return returned;
}

std::map<uint64_t, std::vector<int>> good_hash_table(std::vector<Ship> a)
{
    ghc_counter = 0;
    std::map<uint64_t, std::vector<int>> returned;
    std::vector<int> index; index.push_back(0);
    returned[a[0].good_hash] = index;
    for (int i = 1; i < a.size(); i++)
    {
        std::vector<int> index;
        if (returned.find(a[i].good_hash)!=returned.cend())
        {
            index = returned[a[i].good_hash];
            index.push_back(i);
            returned[a[i].good_hash] = index;
            ghc_counter++;
        }
        else
        {
            index.push_back(i);
            returned[a[i].good_hash] = index;

        }

    }
    return returned;
}


Ship bad_hash_find(std::map<uint64_t, std::vector<int>> a, std::string needtofind, std::vector<Ship> b)//поиск по таблице, содержащей плохой хеш
{
    if (a[bad_hash(needtofind)].size() > 0)//проверка на то, что в искомом хеше не происходит коллизия, если она происходит, то проходимся по каждому элементу коллизии и сравниваем строки
    {
        for (int i = 0; i < a[bad_hash(needtofind)].size(); i++)//цикл поиска нужного индекс
        {
            if (b[a[bad_hash(needtofind)][i]].sNames == needtofind)//условие на то, что нашли нужный индекс
                return b[a[bad_hash(needtofind)][i]];//если верно, то возвращаем нужный нам класс
        }

    }
    else
        return b[a[bad_hash(needtofind)][0]];//если коллизий нет, то возвращаем единственный класс
}



Ship good_hash_find(std::map<uint64_t, std::vector<int>> a, std::string needtofind, std::vector<Ship> b)
{
    if (a[good_hash(needtofind)].size() > 0)
    {
        for (int i = 0; i < a[good_hash(needtofind)].size(); i++)
        {
            if (b[a[good_hash(needtofind)][i]].sNames == needtofind)
                return b[a[good_hash(needtofind)][i]];
        }
    }
    else
        return b[a[good_hash(needtofind)][0]];
}

int main() 
{
    return 0;
}
