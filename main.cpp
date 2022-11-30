#include "Gui.h"
#include "FileRepository.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileRepository adminRepo{};
    Repository userRepo{};
    Service serv{ adminRepo, userRepo };
    InputValidator val{};
    Gui gui{ adminRepo, serv, val };
    gui.show();
    return a.exec();
}
