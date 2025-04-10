#ifndef SOLDINVENTORYVIEWER_H
#define SOLDINVENTORYVIEWER_H

#include <QDialog>
#include "Lumber.cpp"

namespace Ui {
class soldinventoryviewer;
}

class soldinventoryviewer : public QDialog
{
    Q_OBJECT

public:
    explicit soldinventoryviewer(std::vector<std::string> attributes, std::vector<Lumber*> soldinv, QWidget *parent = nullptr);
    ~soldinventoryviewer();

private:
    Ui::soldinventoryviewer *ui;
};

#endif // SOLDINVENTORYVIEWER_H
