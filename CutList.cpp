#include <string>
#include <vector>
#include "Lumber.cpp"

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
        CutListItem(float l, float w, float t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV, std::string n, int q)
        : Lumber(a, aV) {
            name = n;
            quantity = q;
        }
};
