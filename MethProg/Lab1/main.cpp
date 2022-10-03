#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>

#define DELIMITER ";"


class Passenger
{
public:

	typedef enum aRoomClass 
	{
		CLASS_1 = 0,
		CLASS_2,
		CLASS_3,
		VIP,
		UNDEF
	} RoomClass;

	Passenger(const std::string& sName, const int& iRoomNumber, const RoomClass oRoomClass, const std::string sDestination) 
	{
		this->m_Name = sName;
		this->m_RoomNumber = iRoomNumber;
		this->m_RoomClass = oRoomClass;
		this->m_Destination = sDestination;
	}

	virtual ~Passenger() {}


	bool operator == (const Passenger& oPassenger) 
	{
		if (this->m_Name == oPassenger.m_Name &&
			this->m_RoomNumber == oPassenger.m_RoomNumber &&
			this->m_Destination == oPassenger.m_Destination
			&& this->m_RoomClass == oPassenger.m_RoomClass)
		{
			return true;
		}

		return false;
	}


	bool operator > (const Passenger& oPassenger) 
	{
		if (this->m_RoomNumber > oPassenger.m_RoomNumber)
			return true;

		if (this->m_Destination.size() > oPassenger.m_Destination.size())
			return true;

		if (this->m_Name.size() > oPassenger.m_Name.size())
			return true;

		return false;
	}

	bool operator >= (const Passenger& oPassenger) 
	{
		if (*this > oPassenger || *this == oPassenger)
			return true;
		return false;
	}

	bool operator < (const Passenger& oPassenger) 
	{
		if (!(*this >= oPassenger))
			return true;
		return false;
	}

	bool operator <= (const Passenger& oPassenger) 
	{
		if (*this == oPassenger || *this < oPassenger)
			return true;

		return false;
	}

private:

	std::string m_Name;
	int m_RoomNumber;
	RoomClass m_RoomClass;
	std::string m_Destination;
};



void sortByChoice(std::vector<Passenger>& vInput) 
{
	for (int i = 0; i < vInput.size(); i++)
	{
		int smallest_index = i;
		for (int j = i; j < vInput.size(); j++) 
		{
			if (j != smallest_index && vInput.at(j) < vInput.at(smallest_index)) 
			{
				smallest_index = j;
			}

			if (smallest_index != i) 
				std::swap(vInput.at(i), vInput.at(smallest_index));
		}
	}

}

void sortByEmplace(std::vector<Passenger>& vInput) 
{
	std::vector<Passenger> vSorted;
	if (vInput.size() < 1)
		return;

	vSorted.emplace_back(vInput.at(0));
	
	for (int i = 1; i < vInput.size(); i++) 
	{
		for (int j = vSorted.size() - 1; j >= 0; j--) 
		{
			if (vInput.at(i) > vSorted.at(j))
			{
				vSorted.insert(vSorted.begin() + j, vInput.at(i));
				break;
			}

			if (j == 0) 
			{
				vSorted.insert(vSorted.begin(), vInput.at(i));
			}
		}
	}

	vInput = vSorted;
}

void doSort(std::vector<Passenger>& vData, int al_begin, int al_end) 
{
	if (al_begin < al_end && al_begin - al_end > 1) 
	{
		int leftBorder = al_begin;
		int rightBorder = al_end;

		int mid = (int)((al_end - al_begin) / 2);

		while (leftBorder < rightBorder)
		{
			while (vData.at(leftBorder) <= vData.at(mid))
				leftBorder += 1;
			
			while (vData.at(rightBorder) >= vData.at(mid))
				rightBorder += 1;

			if (leftBorder <= rightBorder)
			{
				std::swap(vData.at(leftBorder), vData.at(rightBorder));
				leftBorder += 1;
				rightBorder += 1;
			}
		}

		doSort(vData, al_begin, rightBorder);
		doSort(vData, leftBorder, al_end);

	}
}


void quickSort(std::vector<Passenger>& vInput)
{
	if (vInput.size() < 1)
		return;

	int leftBorder = 0;
	int rightBorder = vInput.size() - 1;

	int mid = (int)(vInput.size() / 2);

	while (vInput.at(leftBorder) < vInput.at(rightBorder)) 
	{
		while (vInput.at(leftBorder) <= vInput.at(mid)) 
			leftBorder += 1;

		while (vInput.at(rightBorder) >= vInput.at(mid))
			rightBorder -= 1;

		if (leftBorder <= rightBorder) 
		{
			std::swap(vInput.at(leftBorder), vInput.at(rightBorder));
			leftBorder += 1;
			rightBorder -= 1;
		}

		doSort(vInput, 0, rightBorder);
		doSort(vInput, leftBorder, vInput.size() - 1);
	}
}


int main(int argc, char* argv[])
{

	std::string sInput = argv[1]; // argv[1] = input file;
	int mode = std::stoi(argv[2]); // argv[2] = mode

	std::ifstream fInput;
	fInput.open(sInput, std::ios::in);  // file line:   NAME;NUMB;CLASS;DEST;

	if (!fInput.is_open()) 
	{
		exit(1);
	}

	std::string sParser;
	std::vector<Passenger> vPassangers;

	int iLine = 0;
	try 
	{
		sParser.clear();
		while (std::getline(fInput, sParser))
		{

			std::string sName = "";
			int iNumber = 0;
			Passenger::RoomClass oRoomClass = Passenger::RoomClass::UNDEF;
			std::string sDestiNation = "";

			std::string sDelim = DELIMITER;

			if (sParser.find(sDelim) != std::string::npos)
			{
				sName = sParser.substr(0,sParser.find(sDelim));
				sParser.erase(0, sParser.find(sDelim) + sDelim.length());
			}
			else 
				break;
			

			if (sParser.find(sDelim) != std::string::npos)
			{
				iNumber = std::stoi(sParser.substr(0,sParser.find(sDelim)));
				sParser.erase(0, sParser.find(sDelim) + sDelim.length());
			}
			else
				break;

			

			if (sParser.find(sDelim) != std::string::npos) 
			{
				oRoomClass = static_cast<Passenger::RoomClass>(std::stoi(sParser.substr(0,sParser.find(sDelim))));
				sParser.erase(0, sParser.find(sDelim) + sDelim.length());
			}
			else
				break;
			
			if (sParser.find(sDelim) != std::string::npos) 
			{
				sDestiNation = sParser.substr(0,sParser.find(sDelim));
				sParser.erase(0, sParser.find(sDelim) + sDelim.length());
			}
			else
				break;


			iLine += 1;
			vPassangers.push_back(Passenger(sName, iNumber, oRoomClass, sDestiNation));
			sParser.clear();
		}
	}
	catch (std::exception e) 
	{
		std::cerr << "Error " << e.what() << " at line: " << iLine << std::endl;
		exit(1);
	}



	try 
	{
		auto start = std::chrono::system_clock::now();

		switch (std::stoi(argv[2]))
		{
		case 1:

			sortByChoice(vPassangers);
			break;
		case 2:
			sortByEmplace(vPassangers);
			break;
		case 3:
			quickSort(vPassangers);
			break;
		default:
			throw new std::invalid_argument("argv is wrong");
		}

		auto finish = std::chrono::system_clock::now();

		auto totalTime = finish - start;

		std::cout << "Done in " << (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(totalTime).count() / std::pow(10, 9)) << std::endl;
	}
	catch (std::exception e) 
	{
		std::cerr << "Exception occured: " << e.what() << std::endl;
		exit(1);
	}

	return 0;

}