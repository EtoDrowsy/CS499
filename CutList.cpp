#include "Lumber.cpp"
#include <iostream>
#include <math.h>

#define SAW_KERF 0.125

class CutListItem : public Lumber {
private:
    std::string name;
    int quantity;
public:
    CutListItem()
        : Lumber() {
        name = "NULL";
        quantity = 0;
    }
    CutListItem(std::string l, std::string w, std::string t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV, std::string n, int q)
        : Lumber() {
        length = l;
        width = w;
        thickness = t;
        species = s;
        grade = g;
        attributes = a;
        attributeValues = aV;
        name = n;
        quantity = q;
    }
    int getQuantity() {return quantity;}
};

static std::vector <int> findMatching(CutListItem* item, std::vector <Lumber*> inventory) {
    std::vector <int> validItems;
    for (int i = 0; i < inventory.size(); i++) {
        std::vector <float> lDimensions = inventory.at(i)->getDimensions();
        std::vector <float> iDimensions = item->getDimensions();

        bool cuttable = inventory.at(i)->getDescription() == "Bulk" || inventory.at(i)->getDescription() == "Piece";
        bool sameDims = iDimensions.at(1) == lDimensions.at(1) && iDimensions.at(2) == lDimensions.at(2);
        bool smallEnough = iDimensions.at(0) <= lDimensions.at(0);
        bool sameSpecies = item->getSpecies()== inventory.at(i)->getSpecies();
        bool gradeMatch = item->getGrade() <= inventory.at(i)->getGrade();
        bool nonZero = inventory.at(i)->getQuantity() > 0;
        if (cuttable && sameDims && smallEnough && sameSpecies && gradeMatch && nonZero)
            validItems.push_back(i);
    }
    return validItems;
}

static void printLumberList(std::vector <std::vector <int>> lumberList, std::vector <Lumber*> inventory, std::vector <CutListItem*> cutList) {
    for (int i = 0; i < lumberList.size(); i++) {
        std::cout << "Cut List Item #" << i + 1 << " is fulfilled by:\n";
        for (int j : lumberList.at(i)) {
            std::cout << "\tInventory ID #" << inventory.at(j)->getID() << ",\n";
            float leftovers = inventory.at(j)->getLength() - SAW_KERF - cutList.at(i)->getLength();
            if (leftovers < 0)
                leftovers = 0;
            std::cout << "\tLeftover Material: " << leftovers << " in.\n";
        }
    }
}

static std::vector <std::vector <int>> cutListToLumberList(std::vector <Lumber*> inventory, std::vector <CutListItem*> cutList) {
    std::vector <std::vector <int>> lumberList;
    for (int c = 0; c < cutList.size(); c++) {
        std::vector <int> empty;
        lumberList.push_back(empty);
        std::vector <int> validItems = findMatching(cutList.at(c), inventory);
        for (int j = 0; j < cutList.at(c)->getQuantity(); j++) {
            int minIndex = validItems.front();
            bool invalidStart = false;
            for (int i : validItems) {
                int taken = 0;
                for (std::vector <int> j : lumberList) {
                    for (int k : j)
                        if (i == k) {
                            taken++;
                        }
                }
                if (i == minIndex && taken >= inventory.at(minIndex)->getQuantity()) {
                    invalidStart = true;
                    continue;
                }
                float iCutRatio;
                if (cutList.at(c)->getLength() + SAW_KERF < inventory.at(i)->getLength())
                    iCutRatio = (cutList.at(c)->getLength() + SAW_KERF) / inventory.at(i)->getLength();
                else
                    iCutRatio = 1.0;

                float minCutRatio;
                if (cutList.at(c)->getLength() + SAW_KERF < inventory.at(minIndex)->getLength())
                    minCutRatio = (cutList.at(c)->getLength() + SAW_KERF) / inventory.at(minIndex)->getLength();
                else
                    minCutRatio = 1.0;

                if (invalidStart && taken < inventory.at(i)->getQuantity()|| (inventory.at(i)->getPrice() * iCutRatio < inventory.at(minIndex)->getPrice() * minCutRatio && taken < inventory.at(i)->getQuantity())) {
                    minIndex = i;
                    invalidStart = false;
                }
            }
            lumberList.at(c).push_back(minIndex);
        }
    }
    printLumberList(lumberList, inventory, cutList);
    return lumberList;
}
