#ifndef SOLDINVENTORYVIEWER_H
#define SOLDINVENTORYVIEWER_H

#include <QDialog>
#include "../source/Lumber.cpp"

namespace Ui {
class soldinventoryviewer;
}

class soldinventoryviewer : public QDialog
{
    Q_OBJECT

public:
    explicit soldinventoryviewer(std::vector<std::string> attributes, std::vector<Lumber*> soldinv, QWidget *parent = nullptr);
    ~soldinventoryviewer();

private slots:
    void on_sortButton_clicked();

    void on_searchButton_clicked();

    void on_clearSearchButton_clicked();

private:
    Ui::soldinventoryviewer *ui;
    std::vector<std::string> tableAttributes;
    std::vector<std::vector<std::string>> soldarray;
    std::vector<std::string> soldAttributes;
};

#endif // SOLDINVENTORYVIEWER_H
