#include "photodialog.h"
#include "ui_photodialog.h"

#include <QFileDialog>
#include <QMessageBox>

photodialog::photodialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::photodialog)
{
    ui->setupUi(this);
    imageUploaded = false;
}

photodialog::~photodialog()
{
    delete ui;
}

void photodialog::on_pushButton_clicked()
{
    upfilename = QFileDialog::getOpenFileName(this,tr("Attach Image"),"",tr("Images (*.png *.jpg)"));
    std::string nameCheck = upfilename.toStdString();

    if(nameCheck.empty()){
        QMessageBox::information(this, "No File Selected", "Please select a file.");
        return;
    }
    else if(nameCheck.find(".jpg") == std::string::npos && nameCheck.find(".png") == std::string::npos){
        QMessageBox::information(this, "Wrong File Type", "Please select a .jpg or .png file.");
        return;
    }
    else{
        imageUploaded = true;
        QPixmap image(upfilename);
        if (ui->previewBox->scene()){
            delete ui->previewBox->scene();
        }
        QGraphicsScene *scene = new QGraphicsScene(this);
        ui->previewBox->setScene(scene);
        ui->previewBox->scene()->addPixmap(image);
    }
}

int photodialog::getIDValue(){
    bool checkValid = false;
    int value = ui->idText->toPlainText().toInt(&checkValid);

    if(checkValid){
        return value;
    }
    else{
        return -1;
    }
}

std::string photodialog::getFileName(){
    return upfilename.toStdString();
}

bool photodialog::getImageUpStatus(){
    return imageUploaded;
}
