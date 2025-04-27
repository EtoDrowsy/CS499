/*
Author: Thomas Pierce
Source file for implementing lumber class
*/

#ifndef LUMBER_CPP
#define LUMBER_CPP
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
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
        std::string photoPath;
        std::vector <std::string> attributes;
        std::vector <std::string> attributeValues;
        float price;
        std::string dateSold;
        int invoiceNumber;
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
                if (aV[i] == "Length"){
                    length = a[i];
                }
                else if (aV[i] == "Width"){
                    width = a[i];
                }
                else if (aV[i] == "Thickness"){
                    thickness = a[i];
                }
                else if (aV[i] == "Species"){
                    species = a[i];
                }
                else if (aV[i] == "Grade"){
                    grade = std::stoi(a[i]);
                }
                else if (aV[i] == "Quantity"){
                    quantity = std::stoi(a[i]);
                }
                else if (aV[i] == "ID"){
                    id = a[i];
                }
                else if (aV[i] == "Description"){
                    description = a[i];
                }
                else if (aV[i] == "Photo"){
                    photoPath = a[i];
                }
                else if (aV[i] == "Price"){
                    price = std::stof(a[i].erase(0,1));
                }
            }
        }

        //Returning dimensions as a vector
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

        //Getter functions
        std::string getLengthDisplay(){return length;}
        std::string getWidthDisplay(){return width;}
        std::string getThicknessDisplay(){return thickness;}

        std::vector <std::string> getAttributes() {return attributes;}
        std::vector <std::string> getAttributeValues() {return attributeValues;}

        std::string getID(){return id;}
        std::string getDescription(){ return description;}
        std::string getSpecies() {return species;}
        int getGrade() {return grade;}
        int getQuantity() {return quantity;}
        std::string getPhotoPath() {return photoPath;}
        float getPrice() {return price;}
        std::string getDateSold(){return dateSold;}
        int getInvoiceNumber(){return invoiceNumber;}

        //Setter functions
        void setID(std::string i) {
            for(int j = 0; j < attributeValues.size(); j++){
                if (attributeValues[j] == "ID"){
                    attributes[j] = i;
                }
            }
            id = i;
        }
        void setLength(std::string l) {
            length = l;
        }
        void setWidth(float w){
            width = w;
        }
        void setThickness(float t){
            thickness = t;
        }

        void setAttributes(std::vector <std::string> a){
            attributes = a;
        }
        void setAttributeValues(std::vector <std::string> aV){
            attributeValues = aV;
        }

        void setSpecies(std::string s){
            species = s;
        }
        void setGrade(int g){
            grade = g;
        }
        void setDateSold(std::string d){
            dateSold = d;
        }
        void setInvoiceNumber(int iN){
            invoiceNumber = iN;
        }
        void setQuantity(int q){
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Quantity"){
                    attributes[i] = std::to_string(q);
                }
            }
            quantity = q;
        }
        void setPrice(float p) {
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Price"){
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(2) << p;
                    std::string pricestring = ss.str();

                    attributes[i] = "$" + pricestring;
                }
            }
            price = p;
        }
        void setDescription(std::string d) {
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Description"){
                    attributes[i] = d;
                }
            }
            description = d;
        }
        void setLength(float l) {
            float inches = std::fmod(l, 12);
            int feet = (l - inches) / 12;
            std::string lengthString = std::to_string(feet) + "'-" + std::to_string(int(inches)) + "\"";
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Length"){
                    attributes[i] = lengthString;
                }
            }
            length = lengthString;
        }
        void setPhotoPath(std::string p){
            photoPath = p;
        }
        void setNotes(std::string n){
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Notes"){
                    attributes[i].append(n);
                }
            }
        }
        std::string getNotes(){
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Notes"){
                    return attributes[i];
                }
            }
            return "";
        }
        std::string getPriceDisplay(){
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Price"){
                    return attributes[i];
                }
            }
            return "";
        }
        void setBulkPrice(float newPrice){
            for (int i = 0; i < attributeValues.size(); i++){
                if (attributeValues[i] == "Price"){
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(2) << newPrice;
                    std::string pricestring = ss.str();

                    attributes[i] = "$" + pricestring;
                }
            }
            price = newPrice;
            return;
        }

        std::string toString(){
            std::string csvline;
            for(int i = 0; i < attributeValues.size(); i++){
                csvline.append(attributes[i] + ";");
            }
            return csvline;
        }
};

#endif
