/*
Author: Thomas Pierce
Source file for implementing photo ui
*/

#include "../header/photodialog.h"
#include "../ui/ui_photodialog.h"

#include <QFileDialog>
#include <QMessageBox>

photodialog::photodialog(QWidget *parent, int id)
    : QDialog(parent)
    , ui(new Ui::photodialog)
{
    //Setting up photo dialog
    ui->setupUi(this);
    imageUploaded = false;
    photoID = id;
    ui->idText->setText(QString::number(photoID));
}

photodialog::~photodialog()
{
    delete ui;
}

void photodialog::on_pushButton_clicked()
{
    //Getting user-selected photo and displaying a preview to the user
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

std::string photodialog::getFileName(){
    return upfilename.toStdString();
}

bool photodialog::getImageUpStatus(){
    return imageUploaded;
}
