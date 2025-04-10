#ifndef LUMBER_CPP
#define LUMBER_CPP
#include <string>
#include <vector>
#include <math.h>

class Lumber {
    protected:
        std::string length;
        std::string width;
        std::string thickness;
        std::string species;
        int grade;
        int quantity;
        std::string id;
        std::string description;
        std::vector <std::string> attributes;
        std::vector <std::string> attributeValues;
        float price;
    public:
        Lumber() {
            length = "NULL";
            width = "NULL";
            thickness = "NULL";
            species = "NULL";
            description = "NULL";
            grade = 0;
            quantity = 0;
            price = 0;
        }
        Lumber(std::vector <std::string> a, std::vector <std::string> aV) : Lumber() {
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (aV[i] == "Length")
                {
                    length = a[i];
                }
                else if (aV[i] == "Width")
                {
                    width = a[i];
                }
                else if (aV[i] == "Thickness")
                {
                    thickness = a[i];
                }
                else if (aV[i] == "Species")
                {
                    species = a[i];
                }
                else if (aV[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
                else if (aV[i] == "Quantity")
                {
                    quantity = std::stoi(a[i]);
                }
                else if (aV[i] == "ID")
                {
                    id = a[i];
                }
                else if (aV[i] == "Description")
                {
                    description = a[i];
                }
                else if (aV[i] == "Price")
                {
                    price = std::stof(a[i]);
                }
            }
        }
        std::vector <float> getDimensions() {
            std::vector <float> dimensions;
            dimensions.push_back(getLength());
            dimensions.push_back(getWidth());
            dimensions.push_back(getThickness());
            return dimensions;
        }
        float getLength(){
            float lengthnum = std::stof(length.substr(0, length.find('\''))) * 12 + std::stof(length.substr(length.find('\'') + 2, length.size() - 1));

            return lengthnum;
        }
        float getWidth(){
            float widthnum = std::stof(width.substr(0, width.find('\''))) * 12 + std::stof(width.substr(width.find('\'') + 2, width.size() - 1));

            return widthnum;
        }
        float getThickness(){
            float thicknessnum = std::stof(thickness.substr(0, thickness.find('/'))) / std::stof(thickness.substr(thickness.find('/') + 1, thickness.size() - 1));

            return thicknessnum;
        }
        std::string getLengthDisplay(){
            return length;
        }
        std::string getWidthDisplay(){
            return width;
        }
        std::string getThicknessDisplay(){
            return thickness;
        }
        std::string getID(){
            return id;
        }
        std::string getDescription(){
            return description;
        }
        std::vector <std::string> getAttributes() {return attributes;}
        std::vector <std::string> getAttributeValues() {return attributeValues;}
        std::string getSpecies() {return species;}
        int getGrade() {return grade;}
        int getQuantity() {return quantity;}
        float getPrice() {return price;}

        void setLength(std::string l) {
            length = l;
        }
        void setLength(float l) {
            float inches = std::fmod(l, 12);
            int feet = (l - inches) / 12;
            std::string lengthString = std::to_string(feet) + "'-" + std::to_string(inches) + "\"";
            length = lengthString;
        }

        std::string toString(){
            std::string csvline;
            for(int i = 0; i < attributeValues.size(); i++){
                csvline.append(attributes[i] + ";");
            }
            csvline.append("\n");
            return csvline;
        }
};

class Piece : public Lumber {
    private:
        float price;
    public:
        Piece() 
        : Lumber() {}
        Piece(std::vector <std::string> a, std::vector <std::string> aV) : Lumber(a, aV){
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (aV[i] == "Price")
                {
                    price = std::stof(a[i]);
                }
            }
        }
        float getPrice() {return price;}
};

class Bulk : public Lumber {
    private:
        float price;
    public:
        Bulk() 
        : Lumber() {
            price = 0;
        }
        Bulk(std::vector <std::string> a, std::vector <std::string> aV) : Lumber(a, aV) {
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (aV[i] == "Price")
                {
                    price = std::stof(a[i]);
                }
            }
        }
        float getPrice() {return price;}

        void setPrice(float p){
            price = p;
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
        Log(std::vector <std::string> a, std::vector <std::string> aV) : Lumber(a, aV) {
            for(int i = 0; i < aV.size(); i++){
                if (aV[i] == "Date")
                {
                    date = a[i];
                }
            }
        }

        void setDate(std::string d){
            date = d;
        }
};

#endif
