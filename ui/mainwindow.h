#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Слот для оновлення інтерфейсу (коли контролер надсилає сигнал)
    void updateUI();

private:
    Ui::MainWindow *ui;
    GameController *game; // Адаптер над незалежним від Qt ігровим рушієм (GameEngine)
};

#endif // MAINWINDOW_H
