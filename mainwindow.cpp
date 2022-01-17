#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->valor1SpinBox->setRange(std::numeric_limits<int>::min(),std::numeric_limits<int>::max());
    ui->valor2SpinBox->setRange(std::numeric_limits<int>::min(),std::numeric_limits<int>::max());
    ui->valor3DoubleSpinBox->setRange(std::numeric_limits<double>::min(),std::numeric_limits<double>::max());
    ui->valor4DoubleSpinBox->setRange(std::numeric_limits<double>::min(),std::numeric_limits<double>::max());

    QStringList colores;
    colores.append("Rojo");
    colores.append("verde");
    colores.append("azul");
    colores.append("Blanco");
    colores.append("Negro");
    colores.append("Amarillo");

    ui->colorComboBox->addItems(colores);
    ui->colorComboBox->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_abrirArchivoPushButton_clicked()
{
    QString json_filter = "JSON (*.json)";
    QString nombreArchivo = QFileDialog::getOpenFileName(this,
                                                        tr("Abrir Archivo"),
                                                        "/",
                                                        tr("Cualquier Archivo (*);;%1").arg(json_filter),
                                                        &json_filter,
                                                        QFileDialog::DontUseNativeDialog);
    if(nombreArchivo.isEmpty()){

    }else{
        QJsonDocument doc;
        QJsonObject obj;
        QByteArray data_json;
        QFile input(nombreArchivo);
        if(input.open(QIODevice::ReadOnly | QIODevice::Text)){
            data_json = input.readAll();
            doc = doc.fromJson(data_json);
            obj = doc.object();
            const int valor1 = obj["valor1"].toInt();
            const int valor2 = obj["valor2"].toInt();
            const double valor3 = obj["valor3"].toDouble();
            const double valor4 = obj["valor4"].toDouble();
            QString valor5 = obj["valor5"].toString();
            QDate fecha1 = QDate::fromString(obj["fecha1"].toString());
            QDateTime fecha2 = QDateTime::fromString(obj["fecha2"].toString());
            QString color = obj["color"].toString();

            ui->valor1SpinBox->setValue(valor1);
            ui->valor2SpinBox->setValue(valor2);
            ui->valor3DoubleSpinBox->setValue(valor3);
            ui->valor4DoubleSpinBox->setValue(valor4);
            ui->valor5LineEdit->setText(valor5);
            ui->fecha1DateEdit->setDate(fecha1);
            ui->fecha2DateTimeEdit->setDateTime(fecha2);
            ui->colorComboBox->setCurrentText(color);

            QMessageBox::information(this,tr("Mensaje"),tr("Documento Json leído correctamente"));
        }else{
            QMessageBox::critical(this,"Error",input.errorString());
        }

    }
}


void MainWindow::on_guardarArchivoPushButton_clicked()
{
    QString json_filter = "JSON (*.json)";
    QString nombre_archivo = QFileDialog::getSaveFileName(this,
                                                          tr("Guardar Archivo"),"/",
                                                          tr("Cualquier Archivo (*);;%1").arg(json_filter),
                                                          &json_filter,
                                                          QFileDialog::DontUseNativeDialog);
    if(nombre_archivo.isEmpty()){

    }else{
        const int valor1 = ui->valor1SpinBox->value();
        const int valor2 = ui->valor2SpinBox->value();
        const double valor3 = ui->valor3DoubleSpinBox->value();
        const double valor4 = ui->valor4DoubleSpinBox->value();
        QString valor5 = ui->valor5LineEdit->text();
        QDate fecha1 = ui->fecha1DateEdit->date();
        QDateTime fecha2 = ui->fecha2DateTimeEdit->dateTime();
        QString color = ui->colorComboBox->currentText();

        QJsonDocument doc;
        QJsonObject obj;
        obj["valor1"] = valor1;
        obj["valor2"] = valor2;
        obj["valor3"] = valor3;
        obj["valor4"] = valor4;
        obj["valor5"] = valor5;
        obj["fecha1"] = fecha1.toString();
        obj["fecha2"] = fecha2.toString();
        obj["color"] = color;

        doc.setObject(obj);
        QByteArray data_json = doc.toJson();
        QFile output(nombre_archivo);
        if(output.open(QIODevice::WriteOnly | QIODevice::Text)){
            output.write(data_json);
            output.close();
            QMessageBox::information(this,tr("Mensaje"),tr("Documento Json guardado correctamente"));
        }else{
            QMessageBox::critical(this,tr("Error"),output.errorString());
        }

    }

}

