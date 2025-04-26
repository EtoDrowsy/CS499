#include "header/inventorywindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  InventoryWindow w;

  w.show();

  return a.exec();
}
