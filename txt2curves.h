#ifndef TXT2CURVES_H
#define TXT2CURVES_H

#include <QMainWindow>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QStringList>

namespace Ui {
class txt2curves;
}

class txt2curves : public QMainWindow
{
    Q_OBJECT

public:
    explicit txt2curves(QWidget *parent = 0);
    ~txt2curves();

private slots:
    void on_inputButton_clicked();

    void on_okButton_clicked();

private:
    Ui::txt2curves *ui;

    QString inputName;
    QString outputName;
    QString outputNameUpper;
    QString outputNameLower;
};

#endif // TXT2CURVES_H
