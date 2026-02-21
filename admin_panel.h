

#ifndef ADMIN_PANEL_H
#define ADMIN_PANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>

class AdminPanel : public QWidget {
    Q_OBJECT

public:
    AdminPanel(QWidget *parent = nullptr);

private slots:
    void loadData();
    void toggleBanStatus();
    void kickUser();

private:
    QTableWidget *userTable;
    QTableWidget *messageTable;
    void setupUI();
    void applyStyles();
    const QString dataPath = "chat_data.txt";
    const QString logPath = "chat_history.txt";
};

#endif



