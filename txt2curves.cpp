#include "txt2curves.h"
#include "ui_txt2curves.h"

txt2curves::txt2curves(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::txt2curves)
{
    ui->setupUi(this);
}

txt2curves::~txt2curves()
{
    delete ui;
}

/* Выбираем фаил исходника */
void txt2curves::on_inputButton_clicked()
{
    inputName = QFileDialog::getOpenFileName(this, "Input", "" ,"Text files (*.txt)");
    ui->lineInput->setText(inputName);
}


void txt2curves::on_okButton_clicked()
{
    /* Открываем исходник, если он открывается, в режиме только для чтения */
    QFile inputFile(ui->lineInput->displayText());
    if (!inputFile.open(QFile::ReadOnly)){
        QMessageBox::critical(this, "Holly shit!", "This file does not want be open");
        return;
    }

    /* Берём имя файла исходника, отрываем последние
     * 4 символа (.txt) и ставим, взамен, нужное раcширение
     * Если кривая делится то - два output */
    QFile outputFileUpper;
    QFile outputFileLower;
    QFile outputFile;
    QTextStream out;
    out.setRealNumberPrecision(6);
    if(ui->checkBox->checkState()){
        outputNameUpper = inputName;
        outputNameUpper.chop(4);
        outputNameUpper.append("_upper.sldcrv");
        outputFileUpper.setFileName(outputNameUpper);
        outputFileUpper.open(QFile::WriteOnly);

        outputNameLower = inputName;
        outputNameLower.chop(4);
        outputNameLower.append("_lower.sldcrv");
        outputFileLower.setFileName(outputNameLower);
        outputFileLower.open(QFile::WriteOnly);

        out.setDevice(&outputFileUpper);
    } else {
        outputName = inputName;
        outputName.chop(4);
        outputName.append(".sldcrv");
        outputFile.setFileName(outputName);
        if (!outputFile.open(QFile::WriteOnly)){
            return;
        }
        out.setDevice(&outputFile);
    }

    QString input;
    QStringList listInputs;

    /*Мы против того чтобы нам давали боль
     * ше 3-х осей*/
    if (listInputs.size() > 3){
        qDebug() << "more then 3 axis";
        return;
    }

    double x, y, z = 0;
    double previousX = 10000;
    bool fuckingBool = 1;

    while (!inputFile.atEnd())
    {
        input = inputFile.readLine();

        /* Если это текст, то мы валим */
        if (!input.at(0).isNumber())
            continue;

        /* читаем 2-3 координаты */

        listInputs = input.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        x = listInputs.at(0).toDouble();
        y = listInputs.at(1).toDouble();

        /* Домножаем на масштаб */
        x *= ui->doubleSpinBoxScale->value();
        y *= ui->doubleSpinBoxScale->value();

        /* Если Z был сразу - меняем на отличный от нуля указанный,
         * если нет - указанный. */
        if ( (listInputs.size()>2) && (z = 0) )
            z = listInputs.at(2).toDouble();
        else
            z = ui->doubleSpinBoxZ->value();

        /*Пишем в out*/
        out << x << " " << y << " " << z << endl;
        if ( (previousX < x)&&fuckingBool ){ //когда X перестанет падать - меняем out
            out.flush();
            out.setDevice(&outputFileLower);
            out << x << " " << y << " " << z << endl;//дописываем прошлую точку ещё раз в новый out
            fuckingBool = 0;
        }
        previousX = x;
    }

    /* Закрываем всё */
    inputFile.close();
    if (ui->checkBox->checkState()){
        outputFileLower.close();
        outputFileUpper.close();
    } else
        outputFile.close();

    QMessageBox::information(this, "bla-bla", "Success");
}
