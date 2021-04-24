/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2021 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 29.03.21                                             **
**          Version: 2.1.0                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  include <QDesktopWidget>
#endif

#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

#include "StateSpace.h"
#include "DiscreteStateSpace.h"
#include "Matrix.h"

MainWindow::MainWindow(int option, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    setupDemo(option);
    //setupPlayground(ui->customPlot);
    // 0:  setupStateSpaceDemo(ui->customPlot);
    // 1:  setupDiscreteStateSpaceDemo5Hz(ui->customPlot);
    // 2:  setupSincScatterDemo(ui->customPlot);
    // 3:  setupScatterStyleDemo(ui->customPlot);

    // for making screenshots of the current demo or all demos (for website screenshots):
    //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
    //QTimer::singleShot(4000, this, SLOT(screenShot()));
}

void MainWindow::setupDemo(int demoIndex) {
    switch (demoIndex) {
        case 0:
            setupStateSpaceDemo(ui->customPlot);
            break;
        case 1:
            setupDiscreteStateSpaceDemo5(ui->customPlot);
            break;
        case 2:
            setupDiscreteStateSpaceDemo25(ui->customPlot);
            break;
        case 3:
            setupDiscreteStateSpaceDemo100(ui->customPlot);
            break;
    }
    setWindowTitle("QCustomPlot: " + demoName);
    statusBar()->clearMessage();
    currentDemoIndex = demoIndex;
    ui->customPlot->replot();
}


void MainWindow::setupStateSpaceDemo(QCustomPlot *customPlot) {
    demoName = "Continuous State Space Demo";
    // generate some data:
    auto t0 = 0;
    auto tf = 50;
    auto stride = 1e-3;
    unsigned N = (int) ((tf - t0) / stride);

    Matrix<3, 1> gen_X0 = Matrix<3, 1>(valarray<double_t>({1, 1, 10}));

    Matrix<3, 3> gen_A = Matrix<3, 3>(valarray<double_t>({-1, 0, 0,
                                                          0, -2, 0,
                                                          0, 0, 0}));

    Matrix<1, 3> gen_C = Matrix<1, 3>(valarray<double_t>({-1, 3, 1}));

    auto generator = new StateSpace<3, 1, 1>();
    generator->set_A(gen_A);
    generator->set_C(gen_C);
    generator->set_X(gen_X0);


    Matrix<3, 3> obj_A = Matrix<3, 3>(valarray<double_t>({0, 1, 0,
                                                          0, 0, 1,
                                                          -.5, -1, -.6}));

    Matrix<3, 1> obj_B = Matrix<3, 1>(valarray<double_t>({0, 0, 1}));

    Matrix<1, 3> obj_C = Matrix<1, 3>(valarray<double_t>({.1, 10, 1}));

    auto object = new StateSpace<3, 1, 1>();
    object->set_A(obj_A);
    object->set_B(obj_B);
    object->set_C(obj_C);

    QVector<double> t(N), u(N), y(N);
    for (int i = 0; i < N; ++i) {
        auto t_ = stride * i;
        t[i] = t_;
        u[i] = generator->compute(t_)(0, 0);
        y[i] = object->compute(t_, generator->get_output())(0, 0);
    }

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->addGraph();
    customPlot->graph(0)->setData(t, u);
    customPlot->graph(1)->setData(t, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("t");
    customPlot->yAxis->setLabel("u, y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 50);
    customPlot->yAxis->setRange(-80, 120);
}

void MainWindow::setupDiscreteStateSpaceDemo5(QCustomPlot *customPlot) {
    demoName = "Discrete 5Hz State Space Demo";
    // generate some data:
    auto t0 = 0;
    auto tf = 50;
    auto stride = 1e-3;
    unsigned N = (int) ((tf - t0) / stride);
    auto dt = 0.2;

    Matrix<3, 1> gen_X0 = Matrix<3, 1>(valarray<double_t>({1, 1, 10}));

    Matrix<3, 3> gen_A = Matrix<3, 3>(valarray<double_t>({
                                                                 .8187, 0, 0,
                                                                 0, .6703, 0,
                                                                 0, 0, 1
                                                         }));

    Matrix<1, 3> gen_C = Matrix<1, 3>(valarray<double_t>({-1, 3, 1}));

    auto generator = new DiscreteStateSpace<3, 1, 1>(dt);
    generator->set_A(gen_A);
    generator->set_C(gen_C);
    generator->set_X(gen_X0);


    Matrix<3, 3> obj_A = Matrix<3, 3>(valarray<double_t>({
                                                                 .9994, .1987, .0192,
                                                                 -.0096, .9802, .1872,
                                                                 -.0936, -.1968, .8679
                                                         }));

    Matrix<3, 1> obj_B = Matrix<3, 1>(valarray<double_t>({
                                                                 .0013,
                                                                 .0192,
                                                                 .1872
                                                         }));

    Matrix<1, 3> obj_C = Matrix<1, 3>(valarray<double_t>({.1, 10, 1}));

    auto object = new DiscreteStateSpace<3, 1, 1>(dt);
    object->set_A(obj_A);
    object->set_B(obj_B);
    object->set_C(obj_C);

    QVector<double> t(N), u(N), y(N);
    for (int i = 0; i < N; ++i) {
        auto t_ = stride * i;
        t[i] = t_;
        u[i] = generator->compute(t_)(0, 0);
        y[i] = object->compute(t_, generator->get_output())(0, 0);
    }

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->addGraph();
    customPlot->graph(0)->setData(t, u);
    customPlot->graph(1)->setData(t, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("t");
    customPlot->yAxis->setLabel("u, y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 50);
    customPlot->yAxis->setRange(-80, 120);
}

void MainWindow::setupDiscreteStateSpaceDemo25(QCustomPlot *customPlot) {
    demoName = "Discrete 25Hz State Space Demo";
    // generate some data:
    auto t0 = 0;
    auto tf = 50;
    auto stride = 1e-3;
    unsigned N = (int) ((tf - t0) / stride);
    auto dt = 0.04;

    Matrix<3, 1> gen_X0 = Matrix<3, 1>(valarray<double_t>({1, 1, 10}));

    Matrix<3, 3> gen_A = Matrix<3, 3>(valarray<double_t>({
                                                                 .9608, 0, 0,
                                                                 0, .9231, 0,
                                                                 0, 0, 1
                                                         }));

    Matrix<1, 3> gen_C = Matrix<1, 3>(valarray<double_t>({-1, 3, 1}));

    auto generator = new DiscreteStateSpace<3, 1, 1>(dt);
    generator->set_A(gen_A);
    generator->set_C(gen_C);
    generator->set_X(gen_X0);


    Matrix<3, 3> obj_A = Matrix<3, 3>(valarray<double_t>({
                                                                 1.000, .0400, .0008,
                                                                 -.0004, .9992, .0395,
                                                                 -.0198, -.0399, .9755
                                                         }));

    Matrix<3, 1> obj_B = Matrix<3, 1>(valarray<double_t>({
                                                                 .0000,
                                                                 .0008,
                                                                 .0395
                                                         }));

    Matrix<1, 3> obj_C = Matrix<1, 3>(valarray<double_t>({.1, 10, 1}));

    auto object = new DiscreteStateSpace<3, 1, 1>(dt);
    object->set_A(obj_A);
    object->set_B(obj_B);
    object->set_C(obj_C);

    QVector<double> t(N), u(N), y(N);
    for (int i = 0; i < N; ++i) {
        auto t_ = stride * i;
        t[i] = t_;
        u[i] = generator->compute(t_)(0, 0);
        y[i] = object->compute(t_, generator->get_output())(0, 0);
    }

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->addGraph();
    customPlot->graph(0)->setData(t, u);
    customPlot->graph(1)->setData(t, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("t");
    customPlot->yAxis->setLabel("u, y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 50);
    customPlot->yAxis->setRange(-80, 120);
}

void MainWindow::setupDiscreteStateSpaceDemo100(QCustomPlot *customPlot) {
    demoName = "Discrete 100Hz State Space Demo";
    // generate some data:
    auto t0 = 0;
    auto tf = 50;
    auto stride = 1e-3;
    unsigned N = (int) ((tf - t0) / stride);
    auto dt = 0.01;

    Matrix<3, 1> gen_X0 = Matrix<3, 1>(valarray<double_t>({1, 1, 10}));

    Matrix<3, 3> gen_A = Matrix<3, 3>(valarray<double_t>({
                                                                 .9900, 0, 0,
                                                                 0, .9802, 0,
                                                                 0, 0, 1.0
                                                         }));

    Matrix<1, 3> gen_C = Matrix<1, 3>(valarray<double_t>({-1, 3, 1}));

    auto generator = new DiscreteStateSpace<3, 1, 1>(dt);
    generator->set_A(gen_A);
    generator->set_C(gen_C);
    generator->set_X(gen_X0);

    Matrix<3, 3> obj_A = Matrix<3, 3>(valarray<double_t>({
                                                                 1.0000, 0.0100, 0.0000,
                                                                 -0.0000, 1.0000, 0.0100,
                                                                 -0.0050, -0.0100, 0.9940
                                                         }));

    Matrix<3, 1> obj_B = Matrix<3, 1>(valarray<double_t>({
                                                                 .0,
                                                                 .0,
                                                                 .01
                                                         }));

    Matrix<1, 3> obj_C = Matrix<1, 3>(valarray<double_t>({.1, 10, 1}));

    auto object = new DiscreteStateSpace<3, 1, 1>(dt);
    object->set_A(obj_A);
    object->set_B(obj_B);
    object->set_C(obj_C);

    QVector<double> t(N), u(N), y(N);
    for (int i = 0; i < N; ++i) {
        auto t_ = stride * i;
        t[i] = t_;
        u[i] = generator->compute(t_)(0, 0);
        y[i] = object->compute(t_, generator->get_output())(0, 0);
    }

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->addGraph();
    customPlot->graph(0)->setData(t, u);
    customPlot->graph(1)->setData(t, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("t");
    customPlot->yAxis->setLabel("u, y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 50);
    customPlot->yAxis->setRange(-80, 120);
}

void MainWindow::realtimeDataSlot() {
    static QTime timeStart = QTime::currentTime();
    // calculate two new data points:
    double key = timeStart.msecsTo(QTime::currentTime()) / 1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key - lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, qSin(key) + std::rand() / (double) RAND_MAX * 1 * qSin(key / 0.3843));
        ui->customPlot->graph(1)->addData(key, qCos(key) + std::rand() / (double) RAND_MAX * 0.5 * qSin(key / 0.4364));
        // rescale value (vertical) axis to fit the current data:
        //ui->customPlot->graph(0)->rescaleValueAxis();
        //ui->customPlot->graph(1)->rescaleValueAxis(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlot->graph(0)->data()->size() + ui->customPlot->graph(1)->data()->size()), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::bracketDataSlot() {
    double secs = QCPAxisTickerDateTime::dateTimeToKey(QDateTime::currentDateTime());

    // update data to make phase move:
    int n = 500;
    double phase = secs * 5;
    double k = 3;
    QVector<double> x(n), y(n);
    for (int i = 0; i < n; ++i) {
        x[i] = i / (double) (n - 1) * 34 - 17;
        y[i] = qExp(-x[i] * x[i] / 20.0) * qSin(k * x[i] + phase);
    }
    ui->customPlot->graph()->setData(x, y);

    itemDemoPhaseTracer->setGraphKey((8 * M_PI + fmod(M_PI * 1.5 - phase, 6 * M_PI)) / k);

    ui->customPlot->replot();

    // calculate frames per second:
    double key = secs;
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key - lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                QString("%1 FPS, Total Data points: %2")
                        .arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
                        .arg(ui->customPlot->graph(0)->data()->size()), 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

void MainWindow::setupPlayground(QCustomPlot *customPlot) {
    Q_UNUSED(customPlot)
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::screenShot() {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#elif QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#elif QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
#else
    QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x() - 7, this->y() - 7, this->frameGeometry().width() + 14,
                                                   this->frameGeometry().height() + 14);
#endif
    QString fileName = demoName.toLower() + ".png";
    fileName.replace(" ", "");
    pm.save("./screenshots/" + fileName);
    qApp->quit();
}

void MainWindow::allScreenShots() {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#elif QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
#elif QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()-7, this->y()-7, this->frameGeometry().width()+14, this->frameGeometry().height()+14);
#else
    QPixmap pm = qApp->primaryScreen()->grabWindow(0, this->x() - 7, this->y() - 7, this->frameGeometry().width() + 14,
                                                   this->frameGeometry().height() + 14);
#endif
    QString fileName = demoName.toLower() + ".png";
    fileName.replace(" ", "");
    pm.save("./screenshots/" + fileName);

    if (currentDemoIndex < 19) {
        if (dataTimer.isActive())
            dataTimer.stop();
        dataTimer.disconnect();
        delete ui->customPlot;
        ui->customPlot = new QCustomPlot(ui->centralWidget);
        ui->verticalLayout->addWidget(ui->customPlot);
        setupDemo(currentDemoIndex + 1);
        // setup delay for demos that need time to develop proper look:
        int delay = 250;
        if (currentDemoIndex == 10) // Next is Realtime data demo
            delay = 12000;
        else if (currentDemoIndex == 15) // Next is Item demo
            delay = 5000;
        QTimer::singleShot(delay, this, SLOT(allScreenShots()));
    } else {
        qApp->quit();
    }
}







































