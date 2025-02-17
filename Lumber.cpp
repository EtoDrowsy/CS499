#include <string>
#include <vector>

class Lumber {
    private:
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
        Lumber(float l, float w, float t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV) {
            length = l;
            width = w;
            thickness = t;
            species = s;
            grade = g;
            attributes = a;
            attributeValues = aV;
        }
        float* getDimensions() {
            float dimensions[3] = {length, width, thickness};
            return dimensions;
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
        Piece(float l, float w, float t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV) 
        : Lumber(l, w, t, s, g, a, aV) {}
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
        Bulk(float l, float w, float t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV, int q, float p) 
        : Lumber(l, w, t, s, g, a, aV) {
            quantity = q;
            price = p;
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
        Log(float l, float w, float t, std::string s, int g, std::vector <std::string> a, std::vector <std::string> aV, std::string d)
        : Lumber(l, w, t, s, g, a, aV) {
            date = d;
        }

        void setDate(std::string d){
            date = d;
        }
};


Piece createPiece(std::vector<std::string> a, std::vector<std::string> aV){
    Piece retPiece;
    retPiece.setAttributes(a);
    retPiece.setAttributeValues(aV);
    for(int i = 0; i < aV.size(); i++){
        if (a[i] == "Length")
        {
            retPiece.setLength(std::stof(a[i]));
        }
        else if (a[i] == "Width")
        {
            retPiece.setWidth(std::stof(a[i]));
        }
        else if (a[i] == "Thickness")
        {
            retPiece.setThickness(std::stof(a[i]));
        }
        else if (a[i] == "Species")
        {
            retPiece.setSpecies(a[i]);
        }
        else if (a[i] == "Grade")
        {
            retPiece.setGrade(std::stoi(a[i]));
        }
    }
    return retPiece;
}

Bulk createBulk(std::vector<std::string> a, std::vector<std::string> aV){
    Bulk retBulk;
    retBulk.setAttributes(a);
    retBulk.setAttributeValues(aV);
    for(int i = 0; i < aV.size(); i++){
        if (a[i] == "Length")
        {
            retBulk.setLength(std::stof(a[i]));
        }
        else if (a[i] == "Width")
        {
            retBulk.setWidth(std::stof(a[i]));
        }
        else if (a[i] == "Thickness")
        {
            retBulk.setThickness(std::stof(a[i]));
        }
        else if (a[i] == "Species")
        {
            retBulk.setSpecies(a[i]);
        }
        else if (a[i] == "Grade")
        {
            retBulk.setGrade(std::stoi(a[i]));
        }
        else if (a[i] == "Quantity")
        {
            retBulk.setQuantity(std::stoi(a[i]));
        }
        else if (a[i] == "Price")
        {
            retBulk.setPrice(std::stof(a[i]));
        }
    }
    return retBulk;
}

Log createLog(std::vector<std::string> a, std::vector<std::string> aV){
    Log retLog;
    retLog.setAttributes(a);
    retLog.setAttributeValues(aV);
    for(int i = 0; i < aV.size(); i++){
        if (a[i] == "Length")
        {
            retLog.setLength(std::stof(a[i]));
        }
        else if (a[i] == "Width")
        {
            retLog.setWidth(std::stof(a[i]));
        }
        else if (a[i] == "Thickness")
        {
            retLog.setThickness(std::stof(a[i]));
        }
        else if (a[i] == "Species")
        {
            retLog.setSpecies(a[i]);
        }
        else if (a[i] == "Grade")
        {
            retLog.setGrade(std::stoi(a[i]));
        }
        else if (a[i] == "Date")
        {
            retLog.setDate(a[i]);
        }
    }
}