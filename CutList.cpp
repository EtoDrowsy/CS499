#ifndef CUTLIST_CPP
#define CUTLIST_CPP
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
    std::string getName() {return name;}
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
    if (lumberList.size() == 0)
        return "Cut List can not be fulfilled.";
    std::string returnString = "";
    for (int i = 0; i < lumberList.size(); i++) {
        returnString.append(std::to_string(cutList.at(i)->getQuantity()) + "x " + cutList.at(i)->getName() + " " + cutList.at(i)->getLengthDisplay() + " x " + cutList.at(i)->getWidthDisplay() + " x " + cutList.at(i)->getThicknessDisplay() + " is cut from:\n");
        std::vector <int> indexes;
        std::vector <int> count;
        for (int j : lumberList.at(i)) {
            bool isUnique = true;
            for (int k = 0; k < indexes.size(); k++) {
                if (j == indexes.at(k)) {
                    isUnique = false;
                    count.at(k)++;
                }
            }
            if (isUnique) {
                indexes.push_back(j);
                count.push_back(1);
            }
        }
        for (int j = 0; j < indexes.size(); j++) {
            Lumber* currentLumber = inventory.at(indexes.at(j));
            returnString.append("    " + std::to_string(count.at(j)) + "x ID #" + currentLumber->getID() + ": " + currentLumber->getSpecies() + " " + currentLumber->getLengthDisplay() + " x " + currentLumber->getWidthDisplay() + " x " + currentLumber->getThicknessDisplay() + "\n");
            float leftovers = currentLumber->getLength() - SAW_KERF - cutList.at(i)->getLength();
            if (leftovers > 0) {
                float inches = std::fmod(leftovers, 12);
                int feet = (leftovers - inches) / 12;
                std::string leftoversString = std::to_string(feet) + "'-" + std::to_string(int(inches)) + "\"";
                returnString.append("        " + leftoversString + " of leftover material for each item.\n");
            }
            else
                returnString.append("        No leftover material.\n");
        }
        returnString.append("\n");
    }
    return returnString;
}

static std::vector <std::vector <int>> cutListToLumberList(std::vector <Lumber*> inventory, std::vector <CutListItem*> cutList) {
    std::vector <std::vector <int>> lumberList;
    std::vector <std::vector <int>> returnEmpty;
    for (int c = 0; c < cutList.size(); c++) {
        std::vector <int> empty;
        lumberList.push_back(empty);
        std::vector <int> validItems = findMatching(cutList.at(c), inventory);
        if (validItems.size() == 0)
            return returnEmpty;
        for (int j = 0; j < cutList.at(c)->getQuantity(); j++) {
            int minIndex = -1;
            bool invalidStart = false;
            for (int i : validItems) {
                int taken = 0;
                for (std::vector <int> j : lumberList) {
                    for (int k : j)
                        if (i == k) {
                            taken++;
                        }
                }
                float iCutRatio;
                if (cutList.at(c)->getLength() + SAW_KERF < inventory.at(i)->getLength())
                    iCutRatio = (cutList.at(c)->getLength() + SAW_KERF) / inventory.at(i)->getLength();
                else
                    iCutRatio = 1.0;

                float minCutRatio;
                if (minIndex != -1 && cutList.at(c)->getLength() + SAW_KERF < inventory.at(minIndex)->getLength())
                    minCutRatio = (cutList.at(c)->getLength() + SAW_KERF) / inventory.at(minIndex)->getLength();
                else
                    minCutRatio = 1.0;

                if (minIndex == -1 && taken < inventory.at(i)->getQuantity())
                    minIndex = i;
                if (taken < inventory.at(i)->getQuantity() && inventory.at(i)->getPrice() * iCutRatio < inventory.at(minIndex)->getPrice() * minCutRatio)
                    minIndex = i;
            }
            if (minIndex == -1)
                return returnEmpty;
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
            leftovers = std::trunc(leftovers);
            Lumber* newWood = new Lumber(inventory.at(j)->getAttributes(), inventory.at(j)->getAttributeValues());
            newWood->setLength(leftovers);
            newWood->setDescription("Piece");
            newWood->setQuantity(1);
            float fPrice = std::trunc((leftovers / inventory.at(j)->getLength() * inventory.at(j)->getPrice()) * 100) / 100;
            newWood->setPrice(fPrice);
            newLumber.push_back(newWood);
        }
    }
    confirmer returnee;
    returnee.IDs = IDs;
    returnee.newLumber = newLumber;
    return returnee;
}
#endif
