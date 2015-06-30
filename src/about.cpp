#include <QApplication>
#include "../inc/about.h"

/**
 * @brief Konstruktor okna ABOUT
 *
 * Funkcja odpowiedzialna za:
 * - wyglad okna
 * - laczenie sygnalow
 */
AboutWindow::AboutWindow()
{
    this->setGeometry    (100,100,300,200);
    this->setFixedHeight (200);
    this->setFixedWidth  (300);
    this->setWindowTitle ("About");

    setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );

    ok_button = new QPushButton (this);
    ok_button->setText          ("OK");
    ok_button->setGeometry      (130,160,40,30);

    about_label = new QLabel(this);
    about_label->setGeometry(10,10,280,130);
    about_label->setText    ("Odtwarzacz plikow MP3.\n"
                             "Napisany aby nauczyc sie Qt.\n\n"

                             "Autor:    Daniel Majchrzycki\n"
                             "Data:     Maj.2015\n"
                             "Wersja:  1.0\n"
                             "QT:        5.4.1\n"
                             "Deployment: -");

    connect(ok_button, SIGNAL(clicked()),this,SLOT(ok_press()));
}

/**
 * @brief Wcisniecie przycisku OK
 *
 * Funkcja zamyka okno
 */
void AboutWindow::ok_press()
{
    this->close();
}
