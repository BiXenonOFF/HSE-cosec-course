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


// ���������� �������� ���������
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

// ����������� ������� ����������
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

// ������� ����������
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



// ������ �����
Ship linear_search(std::vector<Ship> data, std::string parameter) {
    for (int i = 0; i < data.size(); i++) {  //  �������� �� ������� ��������
        if (data.at(i).sNames == parameter) // ���������� ����������� �������� � ����������
            return data.at(i); // ���� ����� ��������, �� ���������� ���
    }
}

// ����������� �������� �����
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

// �������� �����
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

// ��������������� ������� � Map
class ship_map {
public:
    int room_number;
    int room_type;
    std::string destination_point;
};

std::multimap<std::string, ship_map> vectormap(std::vector<Ship> data) { // ������� ����������� ������� � ���������
    std::multimap<std::string, ship_map> data_map; //��������� ��������� ������� � ������
    for (int i = 0; i < data.size(); i++) { // ���� ��� ������ ������ � ���������
        ship_map data_of_map;// ��������� ����� ��� ������ � ���������
        data_of_map.room_number = data.at(i).iRoomNumber; // � ���� � ���� �������� ���� ���������� ������� ������ �� �������
        data_of_map.room_type = data.at(i).iRoomType;
        data_of_map.destination_point = data.at(i).sDestination;
        std::string key = data.at(i).sNames;// ���������� �� ������� ������ � ���������� ������
        data_map.insert({ key,data_of_map });// ��������� ������ ���� � ���������
    }
    return data_map; // ���������� ���������
}

//������� ������� �� ������ �����
std::map<uint64_t, std::vector<int>> bad_hash_table(std::vector<Ship> a)
{
    bhc_counter = 0;
    std::map<uint64_t, std::vector<int>> returned;//��������� �������
    std::vector<int> index; index.push_back(0);//��������� ������ �������� ����������� � ������������� ����, ��������� � ���� ������ �������
    returned[a[0].bad_hash] = index;//������������ � ������� ������ �������� � ����
    for (int i = 1; i < a.size(); i++)
    {
        std::vector<int> index;
        if (returned.find(a[i].bad_hash) != returned.cend())//��������, ���� ��� ��� ����������(���������� ��������), �� �� ��������� � ������� �������� ��������� ������. 
        {
            index = returned[a[i].bad_hash];// ���������� �� ������� �������� ������ ��������
            index.push_back(i);// ���������� � ���� ��������� ������� 
            returned[a[i].bad_hash] = index;// ����������� ������ ���������� �������
            bhc_counter++;
        }
        else//� ������ ���� �������� �� ���������, �� ������ ������� ����� ������� � ���-������� � ���������� � ��� ������������ ������
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


Ship bad_hash_find(std::map<uint64_t, std::vector<int>> a, std::string needtofind, std::vector<Ship> b)//����� �� �������, ���������� ������ ���
{
    if (a[bad_hash(needtofind)].size() > 0)//�������� �� ��, ��� � ������� ���� �� ���������� ��������, ���� ��� ����������, �� ���������� �� ������� �������� �������� � ���������� ������
    {
        for (int i = 0; i < a[bad_hash(needtofind)].size(); i++)//���� ������ ������� ������
        {
            if (b[a[bad_hash(needtofind)][i]].sNames == needtofind)//������� �� ��, ��� ����� ������ ������
                return b[a[bad_hash(needtofind)][i]];//���� �����, �� ���������� ������ ��� �����
        }

    }
    else
        return b[a[bad_hash(needtofind)][0]];//���� �������� ���, �� ���������� ������������ �����
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
