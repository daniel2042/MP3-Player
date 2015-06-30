#ifndef ABOUT_H
#define ABOUT_H

#include <QPushButton>
#include <QLabel>
#include <QMainWindow>


class AboutWindow : public QMainWindow
{
    private:
        Q_OBJECT

        QPushButton *ok_button;
        QLabel      *about_label;

    public:
        AboutWindow();
    private slots:
        void ok_press();

};

#endif // ABOUT_H
