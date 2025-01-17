#include <string>

class pieces{
    public:
        int serialNumber;
        std::string location;
        std::string species;
        std::string surface;
        std::string length;
        std::string width;
        std::string thickness;
        std::string dried;
};

class bulk{
    public:
        int catalogNumber;
        std::string location;
        int quantity;
        std::string units;
        std::string species;
        std::string description;
        std::string length;
        std::string width;
        std::string thickness;
        std::string price;
};