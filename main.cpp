

#include <QApplication>
#include "admin_panel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AdminPanel w;
    w.show();
    return a.exec();
}
