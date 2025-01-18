#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void readCSV(std::string filepath){
	std::fstream inputcsv;
	inputcsv.open(filepath);
	std::string line;
	std::vector<std::string> v;
	while(!inputcsv.eof()){
		std::getline(inputcsv, line);
		std::stringstream ss(line);
		while (ss.good()){
			std::string substr;
			getline(ss,substr,';');
			v.push_back(substr);
		}
	}
	inputcsv.close();

	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i];
	return;
}

int main() {
	std::string path = "PrestonLumberInventory.csv";
	readCSV(path);
	return 0;
}