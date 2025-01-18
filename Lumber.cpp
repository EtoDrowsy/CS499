#include <string>

class Lumber {
    private:
        std::string location;
        std::string species;
        float length;
        float width;
        float thickness;
    public:
        Lumber() {
            location = "NULL";
            species = "NULL";
            length = 0;
            width = 0;
            thickness = 0;
        }
        Lumber(std::string l, std::string s, float le, float w, float t) {
            location = l;
            species = s;
            length = le;
            width = w;
            thickness = t;
        }
        float* getDimensions() {
            float dimensions[3] = {length, width, thickness};
            return dimensions;
        }
};

class Piece : Lumber {
    private:
        std::string serialNumber;
        std::string surface;
        std::string dried;
    public:
        Piece() 
        : Lumber() {
            serialNumber = "0000";
            surface = "NULL";
            dried = "NULL";
        }
        Piece(std::string l, std::string s, float le, float w, float t, std::string se, std::string su, std::string d) 
        : Lumber(l, s, le, w, t) {
            serialNumber = se;
            surface = su;
            dried = d;
        }
};

class Bulk : Lumber {
    private:
        std::string catalogNumber;
        int quantity;
        std::string units;
        std::string description;
        float price;
    public:
        Bulk() 
        : Lumber() {
            catalogNumber = "0000";
            quantity = 0;
            units = "NULL";
            description = "NULL";
            price = 0;
        }
        Bulk(std::string l, std::string s, float le, float w, float t, std::string c, int q, std::string u, std::string d, float p) 
        : Lumber(l, s, le, w, t) {
            catalogNumber = c;
            quantity = q;
            units = u;
            description = d;
            price = p;
        }
        float getPriceTotal(int q) {
            if(q <= quantity) {return q * price;}
            return -1;
        }
};
