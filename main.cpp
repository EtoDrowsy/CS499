#include "header/inventorywindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("windowsvista"));

  InventoryWindow w;

  w.show();

  return a.exec();
}
