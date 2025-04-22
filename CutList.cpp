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

static std::string printLumberList(std::vector <std::vector <int>> lumberList, std::vector <Lumber*> inventory, std::vector <CutListItem*> cutList) {
    std::string returnString = "";
    for (int i = 0; i < lumberList.size(); i++) {
        returnString.append("Cut List Item #" + std::to_string(i+1) + " is fulfilled by:\n");
        for (int j : lumberList.at(i)) {
            returnString.append("\tInventory ID #" + inventory.at(j)->getID() + ",\n");
            float leftovers = inventory.at(j)->getLength() - SAW_KERF - cutList.at(i)->getLength();
            if (leftovers < 0)
                leftovers = 0;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(3) << leftovers;
            std::string precLeftovers = ss.str();
            returnString.append("\tLeftover Material: " + precLeftovers + " in.\n");
        }
    }
    return returnString;
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
    return lumberList;
}
struct confirmer {
    std::vector <std::string> IDs;
    std::vector <Lumber*> newLumber;
};

static confirmer confirmLumberList(std::vector <Lumber*> inventory, std::vector <CutListItem*> cutList, std::vector <std::vector <int>> lumberList) {
    std::vector <std::string> IDs;
    std::vector <Lumber*> newLumber;
    for (int i = 0; i < lumberList.size(); i++) {
        for (int j : lumberList.at(i)) {
            float leftovers = inventory.at(j)->getLength() - SAW_KERF - cutList.at(i)->getLength();
            IDs.push_back(inventory.at(j)->getID());
            if (leftovers < 0)
                continue;
            leftovers = int(std::floor(leftovers));
            Lumber* newWood = new Lumber(inventory.at(j)->getAttributes(), inventory.at(j)->getAttributeValues());
            newWood->setLength(leftovers);
            newWood->setDescription("Piece");
            newWood->setQuantity(1);
            float fPrice = std::floor((leftovers / inventory.at(j)->getLength() * inventory.at(j)->getPrice()) * 100) / 100;
            newWood->setPrice(fPrice);
            newLumber.push_back(newWood);
        }
    }
    confirmer returnee;
    returnee.IDs = IDs;
    returnee.newLumber = newLumber;
    return returnee;
}
