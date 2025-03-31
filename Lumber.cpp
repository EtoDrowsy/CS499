#include <string>
#include <vector>

#pragma once

class Lumber {
    protected:
        float length;
        float width;
        float thickness;
        std::string species;
        int grade;
        std::vector <std::string> attributes;
        std::vector <std::string> attributeValues;
    public:
        Lumber() {
            length = 0;
            width = 0;
            thickness = 0;
            species = "NULL";
            grade = 0;
        }
        Lumber(std::vector <std::string> a, std::vector <std::string> aV) {
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (a[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (a[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (a[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (a[i] == "Species")
                {
                    species = a[i];
                }
                else if (a[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
            }
        }
        std::vector <float> getDimensions() {
            std::vector <float> dimensions;
            dimensions.push_back(length);
            dimensions.push_back(width);
            dimensions.push_back(thickness);
            return dimensions;
        }
        std::vector <std::string> getAttributes() {return attributes;}
        std::vector <std::string> getAttributeValues() {return attributeValues;}
        std::string getSpecies() {return species;}
        int getGrade() {return grade;}

        void setLength(float l){
            length = l;
        }
        void setWidth(float w){
            width = w;
        }
        void setThickness(float t){
            thickness = t;
        }
        void setSpecies(std::string s){
            species = s;
        }
        void setGrade(int g){
            grade = g;
        }
        void setAttributes(std::vector <std::string> a){
            attributes = a;
        }
        void setAttributeValues(std::vector <std::string> aV){
            attributeValues = aV;
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
        Piece(std::vector <std::string> a, std::vector <std::string> aV) : Lumber(){
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (a[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (a[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (a[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (a[i] == "Species")
                {
                    species = a[i];
                }
                else if (a[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
                else if (a[i] == "Price")
                {
                    price = std::stof(a[i]);
                }
            }
        }
        float getPrice() {return price;}
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
        Bulk(std::vector <std::string> a, std::vector <std::string> aV) : Lumber() {
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (a[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (a[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (a[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (a[i] == "Species")
                {
                    species = a[i];
                }
                else if (a[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
                else if (a[i] == "Quantity")
                {
                    quantity = std::stoi(a[i]);
                }
                else if (a[i] == "Price")
                {
                    price = std::stof(a[i]);
                }
            }
        }
        float getPrice() {return price;}
        float getQuantity() {return quantity;}

        void setQuantity(int q){
            quantity = q;
        }
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
        Log(std::vector <std::string> a, std::vector <std::string> aV) : Lumber() {
            for(int i = 0; i < aV.size(); i++){
                if (a[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (a[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (a[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (a[i] == "Species")
                {
                    species = a[i];
                }
                else if (a[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
                else if (a[i] == "Date")
                {
                    date = a[i];
                }
            }
        }

        void setDate(std::string d){
            date = d;
        }
};
