#include "inventorywindow.h"

#include <QApplication>

#include "CutList.cpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void testreadCSV(std::string filepath){
	std::fstream inputcsv;
	inputcsv.open(filepath);
	std::string line;
	std::vector<std::vector<std::string>> array;
	std::vector<std::vector<std::string>>::iterator it1;
	std::vector<std::string>::iterator it2;
	
	while(!inputcsv.eof()){
		std::getline(inputcsv, line);
		std::stringstream ss(line);
		std::vector<std::string> row;
		while (ss.good()){
			std::string substr;
			getline(ss,substr,';');
			row.push_back(substr);
		}
		array.push_back(row);
	}
	inputcsv.close();

    // for (it1 = array.begin(); it1 != array.end(); it1++){
    // 	for (it2 = it1->begin(); it2 != it1->end(); it2++){
    // 		std::cout << *it2;
    // 	}
    // 	std::cout << std::endl;
    // }

	return;
}

int main(int argc, char* argv[]) {
	//std::string path = "PrestonLumberInventory.csv";
	//readCSV(path);
  QApplication a(argc, argv);

  InventoryWindow w;

  w.show();

  return a.exec();
}
