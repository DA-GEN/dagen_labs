#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Game.hpp" // Підключаємо нашу гру

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
    // Слот для оновлення інтерфейсу (коли гра надсилає сигнал)
    void updateUI();

private:
    Ui::MainWindow *ui;
    Game *game; // Вказівник на об'єкт гри
};

#endif // MAINWINDOW_H
