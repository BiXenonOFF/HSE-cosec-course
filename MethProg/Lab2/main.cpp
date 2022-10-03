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


/// ���������� ///
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






// �������������� ���� ������ � ������ �������
std::vector<Ship> upload(std::string path) {
    std::ifstream opened(path);
    std::vector<Ship> returned;
    std::string gl;
    std::vector<std::string> gl_string;
    while (std::getline(opened, gl))
    {
        gl_string.clear();
        char* str = (char*)gl.c_str();// ������������ const* char -> char*, �� ������� ����� ��������� � �������� ��������� char*
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


// ���������� ���������
std::vector<Ship> simple_push(std::vector<Ship> data) {
    std::vector<Ship> addit_vec; // � ��� ���� ��� �������, ����_��� - ��� �������, � ���� - ��������
    addit_vec.push_back(data.at(0)); // ��������� ������ ������� ��������� ������� ���� � ������� ������ ����_���
    data.erase(data.begin());// � ���� ������� ������� ������� �� ��������� �������, ��� ����� ������ ������ ������� ������� ���������������� �������.
    for (int i = 0; i < data.size(); i++) { // ��������� �� ��������� ��������� �������
        for (int j = 0; j < addit_vec.size(); j++) { // ��������� �� ��������� �������� �������
            if (data.at(i) < addit_vec.at(j)) { // ���������� �������� �������� � ��������� �������(�� ����� ������� ��������� ������� � ���������� ��� �� ���� ������� ���� �� ������ ������� �������� �������, ������� ����� ������ ������������ �������� ��������� �������)
                addit_vec.insert(addit_vec.begin() + j, data.at(i)); // � ������ ���������� �������, ���������� ����, �� ��������� ����������� ������� ����� ��������� ��������� �������, �� ������� ������� ��������� ��������
                break; // �� ������� �����������, �� �� ����������� ������������ �� ��������� �������� ������� � ��������� � ���������� �������� ��������� �������.
            }
            else if (j + 1 == addit_vec.size())
            {
                addit_vec.push_back(data.at(i));
                break;
            }
        }
    }
    return addit_vec; //���������� ������� ������ (�� ��� ������������)
}




//������� ����������
int quick_sort_pointer(std::vector<Ship>* data, int first, int last) // ����� ��� ��������. �� �� �����, ��� � quick sort, ������ ������ �������� � ���������� ����� ���������
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
    if (0 < data.size() - 1)// �������� �� ��, ��� ������ ������� ������ 1
    {
        int left = 0; int right = data.size() - 1; Ship mid = data.at(data.size() / 2);// ���������� ������, ����� � �������� ��������(��������)
        while (left < right)// ���� ������ ������ �����
        {
            while (data.at(left) < mid)// ���� ��������, ������� ������ �������� �������� (�������� ��������� ����� �������� ��������)
                left++;// ��� ������������ ��������, ���� ����������� ������� ����� (������� ������ ��������), �� � left ���������� ��������, �� ���� ��������� � ���������� ��������
            while (data.at(right) > mid)// ���� ��������, ������� ������ �������� �������� (�������� ��������� ������ �������� ��������)
                right--;// ��� ������������ ��������, ���� ����������� ������� ����� (������� ������ ��������), �� � right �������� ��������, �� ���� ��������� � ���������� ��������
            if (left <= right)// �������� �� ��� ����� �������� ������ ������� 
            {
                // ����� ������� 
                Ship k = data.at(left);
                data.at(left) = data.at(right);
                data.at(right) = k;
                left++; right--;
            }

        }
        // ������ ������� ��� ��� � ��������� � ���. � ��� ������, ���� �������� ������ �� �������������
        quick_sort_pointer(&data, 0, right);
        quick_sort_pointer(&data, left, data.size() - 1);
    }
    return data;
}

// ���������� �������
std::vector<Ship> selection_sort(std::vector<Ship> data) {
    for (int i = 0; i < data.size() - 1; i++) {// ����������� �� ������ ��������� ������� 
        int ismallest = i;// ��������� ���������� ������� ����������� �������� (���������� � ��� �� ������ ������ ������ ������� �������� �������� ������������������)
        for (int j = i + 1; j < data.size() - 1; ++j) {// ����������� �� ���� ��������� �������� ������������������
            if (operator<(data[j], data[i]))// ���������� ��������� ������� ������� ������������������ (������ ������� ����������) � ������ ��������� �������� ������������������
                ismallest = j;// ���� ����������� �������, �� � ismallest ���������� ������ ����������
        }
        std::swap(data[i], data[ismallest]);// ������ ������� ���������� � ������ (���� ���������� ����� ������, �� ��� ������������������ �� ���������)
    }
    return data;
}



////Searches//////

Ship linear_search(std::vector<Ship> data, std::string parameter) {
    for (int i = 0; i < data.size(); i++) {  //  �������� �� ������� ��������
        if (data.at(i).sNames == parameter) // ���������� ����������� �������� � ����������
            return data.at(i); // ���� ����� ��������, �� ���������� ���
    }
}

int binary_search_recurtion(std::vector<Ship>& data, std::string& parameter, int min, int max) { //  ��� �� ����� �������� �����, ������ ��� ��������
    int mid = ((min + max) / 2);
    if (data.at(mid).sNames < parameter) {
        binary_search_recurtion(data, parameter, mid + 1, max);
    }
    else if (data.at(mid).sNames > parameter) {
        binary_search_recurtion(data, parameter, min, mid + 1);
    }
    else
        return mid; //  ���������� ������ ���������� ��������
}

Ship binary_search(std::vector<Ship> data, std::string parameter) {
    int mid = data.size() / 2; //  ��������� ���������� ��������
    int min = 0; //  ��������� �������
    int max = data.size(); //  ��������� ��������
    if (data.at(mid).sNames < parameter) { // �������� ������� ����, ��� �������� ������ ����������� ��������
        return data.at(binary_search_recurtion(data, parameter, mid + 1, max));
    } // ���� ���������� ������� ������ ���������, �� �������� "����������" ������� ��� ������������������ ����������� ����� ����������� �������� 
    else if (data.at(mid).sNames > parameter) { // �������� ������� ����, ��� �������� ������ ����������� ��������
        return data.at(binary_search_recurtion(data, parameter, min, mid + 1));
    }// ���� ���������� ������� ������ ���������, �� �������� "����������" ������� ��� ������������������ ����������� ������ ����������� �������� 
    else
        return data.at(mid); // ���������� ������� �������
}

// ��������� ����� ��� ���������
struct Ship_map {
    int iRoomNumber;
    int iRoomType;
    std::string sDestination;
};

std::multimap<std::string, Ship_map> vectormap(std::vector<Ship> data) { // ������� ����������� ������� � ���������
    std::multimap<std::string, Ship_map> data_map; //��������� ��������� ������� � ������
    for (int i = 0; i < data.size(); i++) { // ���� ��� ������ ������ � ���������
        Ship_map data_of_map;// ��������� ����� ��� ������ � ���������
        data_of_map.iRoomNumber = data.at(i).iRoomNumber; // � ���� � ���� �������� ���� ���������� ������� ������ �� �������
        data_of_map.iRoomType = data.at(i).iRoomType;
        data_of_map.sDestination = data.at(i).sDestination;
        std::string key = data.at(i).sNames;// ���������� �� ������� ������ � ���������� ������
        data_map.insert({ key,data_of_map });// ��������� ������ ���� � ���������
    }
    return data_map; // ���������� ���������
}

int main() {
    std::vector<Ship> a = upload("100000.txt");
    std::vector<Ship> a2 = a;
    std::string name = "Rebecca Hawkins";
    // ����� �� ������� ������
    auto start = std::chrono::system_clock::now();
    linear_search(a, name);
    auto stop = std::chrono::system_clock::now();
    auto time = stop - start;
    std::cout << "============================================================================" << std::endl << a.size() << ":" << std::endl;
    std::cout << "������ �����:" << (float(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / 1000000000) << std::endl << std::endl;


    // ����� � ������� ����������� � �������� �������
    a = quick_sort(a);
    start = std::chrono::system_clock::now();
    binary_search(a, name);
    stop = std::chrono::system_clock::now();
    time = stop - start;
    std::cout << "�������� �����:" << (float(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / 1000000000) << std::endl << std::endl;

    // ����� �� ����������
    std::multimap<std::string, Ship_map> map_done;
    map_done = vectormap(a2);

    start = std::chrono::system_clock::now();
    map_done.find(name);
    stop = std::chrono::system_clock::now();
    time = stop - start;
    std::cout << "Multimap �����:" << (float(std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / 1000000000) << std::endl << std::endl;
    std::cout << "============================================================================" << std::endl;


    return 0;
}
