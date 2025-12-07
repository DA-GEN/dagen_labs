#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ========================================================
    // НАЛАШТУВАННЯ ВІЗУАЛЬНОГО СТИЛЮ (ТЕМНА ТЕМА)
    // ========================================================

    // Встановлюємо загальний стиль для всього вікна
    this->setStyleSheet(
        // Темний фон вікна
        "QMainWindow { background-color: #2b2b2b; }"

        // Стиль для поля логу (схожий на старий пергамент або термінал)
        "QTextBrowser#gameLog {"
        "   background-color: #1e1e1e;"
        "   color: #e0e0e0;"
        "   border: 2px solid #5c5c5c;"
        "   border-radius: 5px;"
        "   font-family: 'Consolas', 'Courier New', monospace;"
        "   font-size: 14px;"
        "   padding: 10px;"
        "}"

        // Стиль для кнопок (загальний)
        "QPushButton {"
        "   background-color: #4a4a4a;"
        "   color: white;"
        "   border: 1px solid #3a3a3a;"
        "   border-radius: 4px;"
        "   padding: 8px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: #5a5a5a; }" // При наведенні світлішає
        "QPushButton:pressed { background-color: #3a3a3a; }" // При натисканні темніє
        "QPushButton:disabled { background-color: #333333; color: #777777; }" // Неактивна кнопка

        // Спеціальний стиль для кнопки АТАКИ (червона)
        "QPushButton#btnAttack {"
        "   background-color: #c0392b;"
        "   border: 1px solid #a93226;"
        "}"
        "QPushButton#btnAttack:hover { background-color: #e74c3c; }"

        // Спеціальний стиль для кнопки НОВА ГРА (зелена)
        "QPushButton#btnStart {"
        "   background-color: #27ae60;"
        "}"
        "QPushButton#btnStart:hover { background-color: #2ecc71; }"

        // Стиль для смужки здоров'я
        "QProgressBar {"
        "   border: 2px solid #5c5c5c;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "   color: white;"
        "   background-color: #1e1e1e;"
        "}"
        );

    game = new Game(this);

    // --- 1. СИГНАЛИ ВІД ГРИ ---

    // Логування тексту
    connect(game, &Game::logMessage, this, [this](QString msg){
        ui->gameLog->append(msg);
        ui->gameLog->verticalScrollBar()->setValue(ui->gameLog->verticalScrollBar()->maximum());
    });

    // Оновлення кнопок і HP, коли щось змінюється в грі
    connect(game, &Game::statsUpdated, this, &MainWindow::updateUI);
    connect(game, &Game::roomUpdated, this, &MainWindow::updateUI);
    connect(game, &Game::gameStarted, this, &MainWindow::updateUI);

    // Обробка кінця гри
    connect(game, &Game::gameOver, this, [this](bool victory){
        updateUI(); // Це викличе нашу нову логіку, яка сховає зайві кнопки

        if(victory) {
            ui->gameLog->append("\n🏆 ВІТАЄМО! ВИ ВИГРАЛИ! (Всі вороги знищені)");
            // При перемозі теж ховаємо все зайве
            ui->btnAttack->setVisible(false);
            ui->btnMove1->setVisible(false);
            ui->btnMove2->setVisible(false);
            ui->btnStart->setVisible(true);
        }
        else {
            ui->gameLog->append("\n💀 ГРА ЗАКІНЧЕНА. Спробуйте ще раз!");
        }
    });

    // --- 2. КНОПКИ (UI -> ГРА) ---

    // Нова гра
    connect(ui->btnStart, &QPushButton::clicked, this, [this](){
        ui->gameLog->clear();
        ui->hpBar->reset(); // Скидання кольору
        // Вмикаємо кнопки назад
        ui->btnMove1->setEnabled(true);
        ui->btnMove2->setEnabled(true);

        game->startNewGame("Герой", 0); // 0 = Воїн
    });

    // Атака
    connect(ui->btnAttack, &QPushButton::clicked, game, &Game::actionAttack);

    // Рух (Кнопка 1)
    connect(ui->btnMove1, &QPushButton::clicked, this, [this](){
        // Якщо кнопка активна, значить вихід 0 існує
        game->actionMove(0);
    });

    // Рух (Кнопка 2)
    connect(ui->btnMove2, &QPushButton::clicked, this, [this](){
        game->actionMove(1);
    });

    // Початковий стан: ховаємо кнопки бою і руху до старту гри
    ui->btnAttack->setVisible(false);
    ui->btnMove1->setVisible(false);
    ui->btnMove2->setVisible(false);
    ui->hpBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUI()
{
    // 1. Оновлення HP (це робимо завжди, навіть якщо мертвий)
    int hp = game->getPlayerHP();
    int maxHp = game->getPlayerMaxHP();
    ui->hpBar->setMaximum(maxHp);
    ui->hpBar->setValue(hp);
    ui->hpBar->setFormat("%v / %m HP");

    // Кольори смужки
    if (hp > maxHp * 0.5) ui->hpBar->setStyleSheet("QProgressBar::chunk { background-color: #2ecc71; }");
    else if (hp > maxHp * 0.25) ui->hpBar->setStyleSheet("QProgressBar::chunk { background-color: #f1c40f; }");
    else ui->hpBar->setStyleSheet("QProgressBar::chunk { background-color: #e74c3c; }");

    // --- ВАЖЛИВА ЗМІНА: ЯКЩО ГРАВЕЦЬ МЕРТВИЙ - ХОВАЄМО ВСЕ І ВИХОДИМО ---
    if (hp <= 0) {
        ui->btnAttack->setVisible(false);
        ui->btnMove1->setVisible(false);
        ui->btnMove2->setVisible(false);
        ui->btnStart->setVisible(true); // Кнопку "Нова гра" завжди показуємо при смерті
        return;
    }
    // -------------------------------------------------------------------

    // 2. Логіка кнопки АТАКИ
    if (game->getEnemyHP() > 0) {
        ui->btnAttack->setVisible(true);
        ui->btnAttack->setEnabled(true);
        ui->btnAttack->setText(QString("АТАКА (%1 HP)").arg(game->getEnemyHP()));
    } else {
        ui->btnAttack->setVisible(false);
    }

    // 3. Логіка кнопок РУХУ
    // (Ми сюди дійдемо тільки якщо гравець живий, бо вище стоїть return)

    // Якщо є живий ворог - рух заборонено
    if (game->getEnemyHP() > 0) {
        ui->btnMove1->setVisible(false);
        ui->btnMove2->setVisible(false);
    }
    else {
        // Якщо ворогів немає - показуємо виходи
        QVector<QString> exits = game->getAvailableExits();

        if (exits.size() > 0) {
            ui->btnMove1->setVisible(true);
            ui->btnMove1->setText(exits[0]);
        } else {
            ui->btnMove1->setVisible(false);
        }

        if (exits.size() > 1) {
            ui->btnMove2->setVisible(true);
            ui->btnMove2->setText(exits[1]);
        } else {
            ui->btnMove2->setVisible(false);
        }
    }
}
