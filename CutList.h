// #include <string>
// #include <vector>
// #include <algorithm>
// #include <iostream>
// #include <math.h>
// #include "Lumber.cpp"

// #define SAW_KERF 0.125

// class CutListItem : public Lumber {
//     private:
//         std::string name;
//         int quantity;
//     public:
//         CutListItem()
//         : Lumber() {
//             name = "NULL";
//             quantity = 0;
//         }
//         CutListItem(float l, float w, float t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV, std::string n, int q)
//         : Lumber(a, aV) {
//             name = n;
//             quantity = q;
//         }
//         int getQuantity() {return quantity;}
// };

// // Generating a list of CutlistItems that the given Lumber item has matching attributes with (non-exclusive)
// std::vector <int> findMatching(Lumber item, std::vector <CutListItem> cutList) {
//     std::vector <int> validItems;
//     for (int i = 0; i < cutList.size(); i++) {
//         CutListItem c = cutList.at(i);
//         std::vector <float> iDimensions = item.getDimensions();
//         std::vector <std::string> iAttributes = item.getAttributes();
//         std::vector <std::string> iAttributeValues = item.getAttributeValues();

//         std::vector <float> cDimensions = c.getDimensions();
//         std::vector <std::string> cAttributes = c.getAttributes();
//         std::vector <std::string> cAttributeValues = c.getAttributeValues();
        
//         bool sameDims = iDimensions.at(1) == cDimensions.at(1) && iDimensions.at(2) == cDimensions.at(2);
//         bool smallEnough = iDimensions[0] >= cDimensions[0];
//         bool sameSpecies = item.getSpecies() == c.getSpecies();
//         bool gradeMatch = item.getGrade() <= c.getGrade();
//         int matchingAttributes = 0;
//         for (int k = 0; k < cAttributes.size(); k++) {
//             std::string cHeader = cAttributes.at(k);
//             std::string cData = cAttributeValues.at(k);

//             for (int l = 0; l < iAttributes.size(); l++) {
//                 std::string iHeader = iAttributes.at(l);
//                 std::string iData = iAttributeValues.at(l);

//                 if (iHeader == cHeader && iData == cData)
//                     matchingAttributes++;
//             }
//         }
//         if (sameDims && smallEnough && sameSpecies && gradeMatch && matchingAttributes == cAttributes.size()) {
//             for(int j = 0; j < cutList.at(i).getQuantity(); j++)
//                 validItems.push_back(i);
//         }
//     }
//     return validItems;
// }

// // Generates a list of all possible combinations of CutListItems to be lower or equal to a given sum and accounts for saw width.
// // TODO: Add quantity ability for subsets, currently is one of each CutListItem instead of quantity based
// std::vector <std::vector <int>> lumberSubset(std::vector <CutListItem> cutList, float sum, std::vector <std::vector <int>> set, int i) {
//     if (i == cutList.size()) {
//         float setSum = 0;
//         for (int j : set.at(0)) {
//             setSum += cutList.at(j).getDimensions().at(0);
//         }
//         setSum += (set.at(0).size() - 1) * SAW_KERF;
//         if (setSum <= sum)
//             return set;
//         std::vector <std::vector <int>> empty;
//         return empty;
//     }

//     std::vector <std::vector <int>> set1 = lumberSubset(cutList, sum, set, i + 1);
//     set.at(0).push_back(i);
//     std::vector <std::vector <int>> set2 = lumberSubset(cutList, sum, set, i + 1);

//     std::vector <std::vector <int>> combinedSet;
//     for (std::vector <int> j : set1)
//         combinedSet.push_back(j);
//     for (std::vector <int> j : set2)
//         combinedSet.push_back(j);
//     return combinedSet;
// }

// // Uses a subset-sum algorithm to determine the best CutListItems for any given Lumber item.
// std::vector <std::vector <int>> bestFitItems(Lumber item, std::vector <CutListItem> cutList) {
//     std::vector <int> validItemsIndexes = findMatching(item, cutList);
//     std::vector <CutListItem> validItems;
//     for (int i : validItemsIndexes) {
//         validItems.push_back(cutList.at(i));
//     }
//     float sum = item.getDimensions().at(0);
//     std::vector <std::vector <int>> empty;
//     std::vector <int> empty2;
//     empty.push_back(empty2);
//     std::vector <std::vector <int>> possibleSetsSub = lumberSubset(validItems, sum, empty, 0);
//     std::vector <std::vector <int>> possibleSets;
//     for (std::vector <int> i : possibleSetsSub) {
//         std::vector <int> transformed;
//         for (int j : i) {
//             transformed.push_back(validItemsIndexes.at(j));
//         }
//         possibleSets.push_back(transformed);
//     }
//     return possibleSets;
// }

// // Calculates the price of a given set of cuts to the nearest inch.
// float calculateCost(std::vector <CutListItem> cutList, std::vector <int> cutListIndexes, Piece item) {
//     float sum = 0;
//     for (int i : cutListIndexes) {
//         sum += cutList.at(i).getDimensions().at(0);
//     }
//     sum += (cutListIndexes.size() - 1) * SAW_KERF;
//     if (sum == item.getDimensions().at(0))
//         return item.getPrice();
//     else
//         return item.getPrice() * item.getDimensions().at(0) / ceil(sum + SAW_KERF);
// }
// float calculateCost(std::vector <CutListItem> cutList, std::vector <int> cutListIndexes, Bulk item) {
//     float sum = 0;
//     for (int i : cutListIndexes) {
//         sum += cutList.at(i).getDimensions().at(0);
//     }
//     sum += (cutListIndexes.size() - 1) * SAW_KERF;
//     if (sum == item.getDimensions().at(0))
//         return item.getPrice();
//     else
//         return item.getPrice() * item.getDimensions().at(0) / ceil(sum + SAW_KERF);
// }
// struct knapsack {
//     float cost;
//     std::vector <int> pieces;
//     std::vector <std::vector <int>> bulks;
// };
// std::vector <std::vector <int>> recursiveBulkQuantity(int i, int count, std::vector <std::vector <int>> set, int quantity, std::vector <std::vector <int>> bulkConfig) {
//     if (i == bulkConfig.size()) {
//         return set;
//     }
//     std::vector <std::vector <int>> sets;
//     for (int j = 0; count + j <= quantity; j++) {
//         std::vector <std::vector <int>> tempSet = set;
//         tempSet.at(0).push_back(j);
//         std::vector <std::vector <int>> newSet = recursiveBulkQuantity(i + 1, count + j, tempSet, quantity, bulkConfig);
//         for (std::vector <int> k : newSet) {
//             sets.push_back(k);
//         }
//     }
//     return sets;
// }

// std::vector <knapsack> recursiveKnapsack(int i, std::vector <int> bulkQuantities, std::vector <std::vector <std::vector <int>>> pieceConfigs, std::vector <std::vector <std::vector <int>>> bulkConfigs, std::vector <std::vector <float>> pieceCost, std::vector <std::vector <float>> bulkCost) {
//     if (i >= pieceConfigs.size() + bulkConfigs.size()) {
//         std::vector <knapsack> empty;
//         knapsack zero;
//         zero.cost = 0;
//         empty.push_back(zero);
//         return empty;
//     }
//     int newI = i;
//     bool bulkSet = false;
//     std::vector <knapsack> knapsacks;
//     if (newI >= pieceConfigs.size()) {
//         newI -= pieceConfigs.size();
//         bulkSet = true;
//     }
//     std::vector <knapsack> prevKnapsacks = recursiveKnapsack(i + 1, bulkQuantities, pieceConfigs, bulkConfigs, pieceCost, bulkCost);
//     if (!bulkSet) {
//         for (int j = 0; j <= pieceConfigs.at(newI).size(); j++) {
//             for (knapsack k : prevKnapsacks) {
//                 knapsack newKnapsack = k;
//                 if (j - 1 >= 0)
//                     newKnapsack.cost += pieceCost.at(newI).at(j - 1);
//                 newKnapsack.pieces.push_back(j - 1);
//                 knapsacks.push_back(newKnapsack);
//             }
//         }
//         return knapsacks;
//     }
//     else if (bulkSet) {
//         std::vector <std::vector <int>> currentConfig = bulkConfigs.at(newI);
//         std::vector <std::vector <int>> empty2d;
//         std::vector <int> empty1d;
//         empty2d.push_back(empty1d);
//         std::vector <std::vector <int>> quantities = recursiveBulkQuantity(0, 0, empty2d, bulkQuantities.at(newI), currentConfig);
//         for (knapsack k : prevKnapsacks) {
//             for(int j = 0; j < quantities.size(); j++) {
//                 knapsack newKnapsack = k;
//                 for(int l = 0; l < quantities.at(j).size(); l++) {
//                     newKnapsack.cost += bulkCost.at(newI).at(l) * quantities.at(j).at(l);
//                 }
//                 newKnapsack.bulks.push_back(quantities.at(j));
//                 knapsacks.push_back(newKnapsack);
//             }
//         }
//         return knapsacks;
//     }
//     std::vector <knapsack> empty;
//     return empty;
// }
// struct lumberList {
//     std::vector <int> pieces;
//     std::vector <std::vector <int>> pieceFulfillment;
//     std::vector <int> bulks;
//     std::vector <int> bulkQuantities;
//     std::vector <std::vector <int>> bulkFulfillment;
// };
// // Determines the ideal subset of Pieces and Bulks that will fulfill the Cut List
// lumberList cutListToLumberList(std::vector <Bulk> bulkList, std::vector <Piece> pieceList, std::vector <CutListItem> cutList) {
//     std::vector <std::vector <float>> pieceCost;
//     std::vector <std::vector <float>> bulkCost;
//     std::vector <std::vector <std::vector <int>>> pieceConfigurations;
//     std::vector <std::vector <std::vector <int>>> bulkConfigurations;
    
//     for (Piece i : pieceList) {
//         pieceConfigurations.push_back(bestFitItems(i, cutList));
//     }
//     for (Bulk i : bulkList) {
//         bulkConfigurations.push_back(bestFitItems(i, cutList));
//     }

//     for (int i = 0; i < pieceConfigurations.size(); i++) {
//         std::vector <float> empty;
//         pieceCost.push_back(empty);
//         for(std::vector <int> j : pieceConfigurations.at(i)) {
//             pieceCost.at(i).push_back(calculateCost(cutList, j, pieceList.at(i)));
//         }
//     }

//     for (int i = 0; i < bulkConfigurations.size(); i++) {
//         std::vector <float> empty;
//         bulkCost.push_back(empty);
//         for(std::vector <int> j : bulkConfigurations.at(i)) {
//             bulkCost.at(i).push_back(calculateCost(cutList, j, bulkList.at(i)));
//         }
//     }

//     std::vector <int> cutListQuantities;
//     std::vector <int> zeroCount;
//     for (CutListItem i : cutList) {
//         cutListQuantities.push_back(i.getQuantity());
//         zeroCount.push_back(0);
//     }
//     std::vector <int> bulkQuantities;
//     for (Bulk i : bulkList) {
//         bulkQuantities.push_back(i.getQuantity());
//     }

//     knapsack endSack;
//     {
//         std::vector <knapsack> possibleKnapsacks;
//         {
//             std::vector <knapsack> allKnapsacks = recursiveKnapsack(0, bulkQuantities, pieceConfigurations, bulkConfigurations, pieceCost, bulkCost);
//             for (knapsack k : allKnapsacks) {
//                 std::vector <int> knapsackCount = zeroCount;
//                 for (int i = 0; i < k.pieces.size(); i++) {
//                     int pieceIndex = k.pieces.size() - i - 1;
//                     int configIndex = k.pieces.at(i);
//                     if (configIndex == -1) {
//                         continue;
//                     }
//                     for (int j : pieceConfigurations.at(pieceIndex).at(configIndex)) {
//                         knapsackCount.at(j) += 1;
//                     }
//                 }
//                 for (int i = 0; i < k.bulks.size(); i++) {
//                     for (int j = 0; j < k.bulks.at(i).size(); j++) {
//                         int bulkIndex = k.bulks.size() - i - 1;
//                         for (int l = 0; l < bulkConfigurations.at(bulkIndex).at(j).size(); l++) {
//                             // knapsack and bulk configs have inverted first index ranges
//                             knapsackCount.at(bulkConfigurations.at(bulkIndex).at(j).at(l)) += k.bulks.at(i).at(j);
//                         }
//                     }
//                 }
//                 bool fulfills = true;
//                 for (int i = 0; i < knapsackCount.size(); i++) {
//                     if (knapsackCount.at(i) < cutListQuantities.at(i))
//                         fulfills = false;
//                 }
//                 if (fulfills)
//                     possibleKnapsacks.push_back(k);
//             }
//         }
//         int minIndex = 0;
//         for (int i = 0; i < possibleKnapsacks.size(); i++) {
//             if (possibleKnapsacks.at(i).cost < possibleKnapsacks.at(minIndex).cost) {
//                 minIndex = i;
//             }
//         }

//         endSack = possibleKnapsacks.at(minIndex);
//     }

//     lumberList returnList;
//     for (int i = 0; i < endSack.pieces.size(); i++) {
//         int pieceIndex = endSack.pieces.size() - i - 1;
//         int configIndex = endSack.pieces.at(i);
//         if (configIndex == -1) {
//             continue;
//         }
//         else {
//             returnList.pieces.push_back(pieceIndex);
//             std::vector <int> empty;
//             returnList.pieceFulfillment.push_back(empty);
//             for (int j : pieceConfigurations.at(pieceIndex).at(configIndex)) {
//                 returnList.pieceFulfillment.back().push_back(j);
//             }
//         }
//     }
//     for (int i = 0; i < endSack.bulks.size(); i++) {
//         int bulkIndex = bulkConfigurations.size() - i - 1;
//         std::vector <int> empty;
//         int count = 0;
//         std::vector <int> cutCount = zeroCount;
//         for (int j = 0; j < endSack.bulks.at(i).size(); j++) {
//             count += endSack.bulks.at(i).at(j);
//         }
//         if (count > 0) {
//             returnList.bulks.push_back(bulkIndex);
//             returnList.bulkQuantities.push_back(count);
//             returnList.bulkFulfillment.push_back(empty);
//             for (int j = 0; j < endSack.bulks.at(i).size(); j++) {
//                 for (int l = 0; l < endSack.bulks.at(i).at(j); l++) {
//                     for (int k : bulkConfigurations.at(bulkIndex).at(j)) {
//                         returnList.bulkFulfillment.back().push_back(k);
//                     }
//                 }
//             }
//         }
//     }
//     return returnList;
// }
