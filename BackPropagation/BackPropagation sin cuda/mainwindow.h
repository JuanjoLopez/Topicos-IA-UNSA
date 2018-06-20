#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <valarray>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>


using namespace std;
#include <algorithm>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();


    //vector <vector<double> > w11;
    //vector <vector<double> > w22;


    void on_cargar_pesos_clicked();

    void on_graficar_error_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
