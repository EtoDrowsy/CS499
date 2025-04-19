#include "inventorywindow.h"

#include <QApplication>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  InventoryWindow w;

  w.show();

  return a.exec();
}
