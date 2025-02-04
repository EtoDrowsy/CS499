#include <string>
#include <vector>

class Lumber {
    private:
        float length;
        float width;
        float thickness;
        std::vector <std::string> attributes;
        std::vector <std::string> attributeValues;
    public:
        Lumber() {
            length = 0;
            width = 0;
            thickness = 0;
        }
        Lumber(float l, float w, float t, std::vector <std::string> a, std::vector <std::string> aV) {
            length = l;
            width = w;
            thickness = t;
            attributes = a;
            attributeValues = aV;
        }
        float* getDimensions() {
            float dimensions[3] = {length, width, thickness};
            return dimensions;
        }
};

class Piece : public Lumber {
    public:
        Piece() 
        : Lumber() {}
        Piece(float l, float w, float t, std::vector <std::string> a, std::vector <std::string> aV) 
        : Lumber(l, w, t, a, aV) {}
};

class Bulk : public Lumber {
    private:
        int quantity;
        float price;
    public:
        Bulk() 
        : Lumber() {
            quantity = 0;
            price = 0;
        }
        Bulk(float l, float w, float t, std::vector <std::string> a, std::vector <std::string> aV, int q, float p) 
        : Lumber(l, w, t, a, aV) {
            quantity = q;
            price = p;
        }
        float getPriceTotal(int q) {
            return (q <= quantity) ? q * price : -1;
        }
};

class Log : public Lumber {
    private:
        std::string date;
    public:
        Log()
        : Lumber() {
            date = "NULL";
        }
        Log(float l, float w, float t, std::vector <std::string> a, std::vector <std::string> aV, std::string d)
        : Lumber(l, w, t, a, aV) {
            date = d;
        }
};
