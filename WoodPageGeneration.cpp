// #include <iostream>
// #include <fstream>
// #include <vector>

// class WoodEntry {
// public:
//     std::string species;
//     std::string dimensions;
//     int quantity;
//     double price;

//     WoodEntry(std::string sp, std::string dim, int qty, double pr)
//         : species(sp), dimensions(dim), quantity(qty), price(pr) {}
// };

// void generateHTML(const std::vector<WoodEntry>& woodList, const std::string& filename) {
//     std::ofstream file(filename);
    
//     if (!file) {
//         std::cerr << "Error opening file for writing.\n";
//         return;
//     }

//     file << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
//     file << "    <meta charset=\"UTF-8\">\n";
//     file << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
//     file << "    <title>Your Wood Order</title>\n";
//     file << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\n";
//     file << "</head>\n<body>\n";

//     for (const auto& wood : woodList) {
//         file << "    <div class=\"wood-item\">\n";
//         file << "        <h3>Species: " << wood.species << "</h3>\n";
//         file << "        <p>Dimensions: " << wood.dimensions << " <br>\n";
//         file << "        Quantity: " << wood.quantity << " <br>\n";
//         file << "        Price: $" << wood.price << "</p>\n";
//         file << "    </div>\n";
//     }

//     file << "</body>\n</html>\n";
//     file.close();
//     std::cout << "HTML file generated successfully: " << filename << std::endl;
// }

// int main() {
//     std::vector<WoodEntry> woodList = {
//         {"Cedar", "3'-9\", 9'-9\", 5/4\"", 2, 40.07},
//         {"Walnut", "1'-3\", 7'-9\", 7/4\"", 4, 50.78},
//         {"Oak", "3'-9\", 6'-0\", 7/4\"", 3, 132.27},
//         {"Pine", "3'-9\", 9'-6\", 1/4\"", 7, 161.85},
//         {"Ash", "3'-9\", 2'-9\", 7/4\"", 5, 248.34}
//     };

//     generateHTML(woodList, "index.html");
//     return 0;
// }
