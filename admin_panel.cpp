

#include "admin_panel.h"

AdminPanel::AdminPanel(QWidget *parent) : QWidget(parent) {
    setupUI();
    applyStyles();
    loadData();
    setWindowTitle("Админ-панель чата (Chad)");
    resize(900, 600);
}

void AdminPanel::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QLabel *userLabel = new QLabel("Управление пользователями:");
    userTable = new QTableWidget(0, 3);
    userTable->setHorizontalHeaderLabels({"Логин", "Пароль", "Статус (1=Бан)"});
    userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *userBtns = new QHBoxLayout();
    QPushButton *btnBan = new QPushButton("Переключить Бан/Разбан");
    QPushButton *btnRefresh = new QPushButton("Обновить данные");
    userBtns->addWidget(btnBan);
    userBtns->addWidget(btnRefresh);

    QLabel *msgLabel = new QLabel("Мониторинг всех сообщений (включая приватные):");
    messageTable = new QTableWidget(0, 3);
    messageTable->setHorizontalHeaderLabels({"Отправитель", "Получатель", "Текст"});
    messageTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(userTable);
    mainLayout->addLayout(userBtns);
    mainLayout->addWidget(msgLabel);
    mainLayout->addWidget(messageTable);

    connect(btnRefresh, &QPushButton::clicked, this, &AdminPanel::loadData);
    connect(btnBan, &QPushButton::clicked, this, &AdminPanel::toggleBanStatus);
}

void AdminPanel::loadData() {
    
    userTable->setRowCount(0);
    QFile uFile(dataPath);
    if (uFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&uFile);
        while (!in.atEnd()) {
            QStringList parts = in.readLine().split(":");
            if (parts.size() >= 3) {
                int row = userTable->rowCount();
                userTable->insertRow(row);
                userTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
                userTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
                userTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
                
                if (parts[2] == "1") {
                    userTable->item(row, 0)->setBackground(Qt::red);
                }
            }
        }
        uFile.close();
    }

   
    messageTable->setRowCount(0);
    QFile mFile(logPath);
    if (mFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&mFile);
        while (!in.atEnd()) {
            QStringList parts = in.readLine().split("|");
            if (parts.size() >= 3) {
                int row = messageTable->rowCount();
                messageTable->insertRow(row);
                messageTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
                messageTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
                messageTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
            }
        }
        mFile.close();
    }
}

void AdminPanel::toggleBanStatus() {
    int row = userTable->currentRow();
    if (row < 0) return;

    QString login = userTable->item(row, 0)->text();
    QString currentStatus = userTable->item(row, 2)->text();
    QString newStatus = (currentStatus == "1") ? "0" : "1";

    QStringList lines;
    QFile file(dataPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(":");
            if (parts[0] == login) {
                lines << QString("%1:%2:%3").arg(parts[0], parts[1], newStatus);
            } else {
                lines << line;
            }
        }
        file.close();
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString &l : lines) out << l << "\n";
        file.close();
    }
    loadData();
}

void AdminPanel::applyStyles() {
    this->setStyleSheet(
        "QWidget { background-color: #2b2b2b; color: #ffffff; font-family: 'Segoe UI'; }"
        "QTableWidget { gridline-color: #444; background-color: #333; border: 1px solid #555; }"
        "QHeaderView::section { background-color: #444; color: white; border: 1px solid #555; }"
        "QPushButton { background-color: #0078d4; border: none; padding: 8px; border-radius: 4px; min-width: 100px; }"
        "QPushButton:hover { background-color: #1086e0; }"
        "QLabel { font-weight: bold; margin-top: 10px; }"
    );
}

void AdminPanel::kickUser() {
    QMessageBox::information(this, "Kick", "Пользователь будет отключен при следующем запросе.");
}

