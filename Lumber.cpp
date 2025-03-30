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
        Lumber(std::vector <std::string> a, std::vector <std::string> aV) : Lumber() {
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (aV[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (aV[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (aV[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (aV[i] == "Species")
                {
                    species = a[i];
                }
                else if (aV[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
            }
        }
        float* getDimensions() {
            float dimensions[3] = {length, width, thickness};
            return dimensions;
        }
        std::string getSpecies(){
            return species;
        }
        float getLength(){
            return length;
        }
        float getWidth(){
            return width;
        }
        float getThickness(){
            return thickness;
        }

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
    public:
        Piece() 
        : Lumber() {}
        Piece(std::vector <std::string> a, std::vector <std::string> aV) : Lumber(){
            attributes = a;
            attributeValues = aV;
            for(int i = 0; i < aV.size(); i++){
                if (aV[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (aV[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (aV[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (aV[i] == "Species")
                {
                    species = a[i];
                }
                else if (aV[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
            }
        }
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
                if (aV[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (aV[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (aV[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
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
                else if (aV[i] == "Price")
                {
                    price = std::stof(a[i]);
                }
            }
        }
        float getPriceTotal(int q) {
            return (q <= quantity) ? q * price : -1;
        }

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
                if (aV[i] == "Length")
                {
                    length = std::stof(a[i]);
                }
                else if (aV[i] == "Width")
                {
                    width = std::stof(a[i]);
                }
                else if (aV[i] == "Thickness")
                {
                    thickness = std::stof(a[i]);
                }
                else if (aV[i] == "Species")
                {
                    species = a[i];
                }
                else if (aV[i] == "Grade")
                {
                    grade = std::stoi(a[i]);
                }
                else if (aV[i] == "Date")
                {
                    date = a[i];
                }
            }
        }

        void setDate(std::string d){
            date = d;
        }
};
