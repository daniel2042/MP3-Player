#ifndef OPTIONS_H
#define OPTIONS_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QCloseEvent>

#include <fstream>

class OptionWindow : public QMainWindow
{
    private:
        Q_OBJECT

        QFileDialog         *file_dialog;

        QLineEdit           *new_path;

        QListView           *path_list;

        QPushButton         *add_button;
        QPushButton         *browse_button;

        QStandardItemModel  *model;


        QAction             *path_list_remove;

        QPoint              contextmenupoint;

        std::fstream        *settings_file;

        int                 path_amount;
        void                closeEvent(QCloseEvent *event);
        void                Load_Paths();
    public:
             OptionWindow();
    signals:
        /// \brief Sygnal zamkniecia okna
        void Window_Close();
    private slots:
        void Save_Paths             ();
        void Add_Pressed            ();
        void Browse_Pressed         ();
        void Remove_From_Path_List  ();
        void Path_List_Context_Menu (QPoint point);
};

#endif // OPTIONS
