#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widgetGraph1->SetBackgroundColor(QColor(239,239,239));
    ui->widgetGraph1->SetPadding(40,20,20,40);
    ui->widgetGraph1->SetLinesColor(QColor(0,0,0));
    ui->widgetGraph1->SetValuesColor(QColor(0,0,0));
    ui->widgetGraph1->SetXAxis(11, true, true, 0, 12);
    ui->widgetGraph1->SetYAxis(3, true, true, 1);
    ui->widgetGraph1->SetSerieName("serie 1", QFont("Arial", 15), QColor(100,150,50));
    ui->widgetGraph1->SetVerticalGradient(QColor(0,100,0,100), QColor(0,100,0,10));

    ui->widgetGraph2->SetBackgroundColor(QColor(31,48,68));
    ui->widgetGraph2->SetPadding(40,40,20,20);
    ui->widgetGraph2->SetLinesColor(QColor(255,255,255));
    ui->widgetGraph2->SetValuesColor(QColor(255,255,255));
    ui->widgetGraph2->SetXAxis(11, true, true, 0, 12);
    ui->widgetGraph2->SetYAxis(3, true, true, 1);
    ui->widgetGraph2->SetDrawRect(false);
    ui->widgetGraph2->SetSerieName("serie 2", QFont("Arial", 15), QColor(200,100,100), Qt::AnchorPoint::AnchorBottom);
    ui->widgetGraph2->SetVerticalGradient(QColor(200,100,100,100), QColor(100,0,0,10));

    ui->widgetGraph3->SetBackgroundColor(QColor(239,239,239));
    ui->widgetGraph3->SetPadding(40,20,20,40);
    ui->widgetGraph3->SetLinesColor(QColor(0,0,0));
    ui->widgetGraph3->SetValuesColor(QColor(0,0,0));
    ui->widgetGraph3->SetXAxis(11, true, true, 0, 12);
    ui->widgetGraph3->SetYAxis(3, true, true, 1);
    ui->widgetGraph3->SetDrawRect(false);
    ui->widgetGraph3->SetSerieName("serie 3", QFont("Arial", 15), QColor(50,100,150));
    ui->widgetGraph3->SetVerticalGradient(QColor(0,0,100,100), QColor(0,0,100,10));

    UpdateGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateGraph()
{
    QVector<double> serie1;
    QVector<double> serie2;
    QVector<double> serie3;
    for (int i = 0; i < 100; i++)
    {
        double t = i * 0.1;
        serie1.push_back(sin(t) * 3.0);
        serie2.push_back(sin(t + sin(t) * 3.0) * 3.0);
        serie3.push_back(sin(t + sin(t + sin(t) * 3.0) * 3.0) * 3.0);
    }

    ui->widgetGraph1->SetSerie(serie1, QColor(100,150,50), 5.0);
    ui->widgetGraph1->update();

    ui->widgetGraph2->SetSerie(serie2, QColor(200,100,100), 5.0);
    ui->widgetGraph2->update();

    ui->widgetGraph3->SetSerie(serie3, QColor(50,100,150), 5.0);
    ui->widgetGraph3->update();
}
