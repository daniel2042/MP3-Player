#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include "../inc/mainwindow.h"

/**
 * @brief Glowna funkcja programu
 *
 * Funkcja:
 * - tworzy instancje
 * - sprawdza istnienie innej instancji i przekazuje jej parametry wywolania (break;)
 * - interpretuje parametry wywolania
 *
 * @param argc - liczba argumentow wywolania
 * @param argv - argumenty wywolania
 *
 * @return stan aplikacji
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory shared(SHARED_MEMORY_ADRESS);

    QMessageBox mess;
    mess.setText("MP3 Player already exist");
    mess.setIcon(QMessageBox::Information);

    if(shared.create(256,QSharedMemory::ReadWrite)!= true)  // Proba utworzenia wspolnej pamieci; niepowodzenie oznacza istnienie innej instancji
    {
        if(shared.attach() && argc > 1)
        {
            shared.lock();
                memcpy(shared.data(), argv[1], 256);
            shared.unlock();
        }
        else
            mess.exec();

        shared.detach();
        exit(0);
    }

    MainWindow w;
    w.show();

    if(argc > 1)                                              // Wywolanie programu z piosenka do oddtworzenia
    {
        w.Load_And_Play_Song(argv[1]);
    }

    return a.exec();
}

