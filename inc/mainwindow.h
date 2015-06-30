#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#define MAX_PATHS               32
#define MAX_SONGS               2048
#define START_VOLUME_LEVEL      15
#define SHARED_MEMORY_ADRESS    "61BB200D-3579-453e-22BA"


#include <QtMultimedia/QMediaPlayer>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QSharedMemory>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QTextCodec>
#include <QCheckBox>
#include <QLineEdit>
#include <QListView>
#include <QMenuBar>
#include <QAction>
#include <QSlider>
#include <QBitmap>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <QMenu>
#include <QUrl>


#include <fstream>
#include <ctime>

#include "../inc/stylesheet.h"
#include "../inc/option.h"
#include "../inc/about.h"


class MainWindow : public QMainWindow
{
    private:
        Q_OBJECT

        QAction               *exit;
        QAction               *path;
        QAction               *about;
        QAction               *random;
        QAction               *reload;
        QAction               *playlist_add;
        QAction               *loop_shortcut;
        QAction               *play_shortcut;
        QAction               *prev_shortcut;
        QAction               *next_shortcut;
        QAction               *playlist_remove;
        QAction               *random_shortcut;
        QAction               *playlist_shortcut;
        QAction               *volume_up_shortcut;
        QAction               *volume_down_shortcut;

        QLabel                *song_name;
        QLabel                *song_length;
        QLabel                *song_position;
        QLabel                *song_amount_label;

        QCheckBox             *loop_checkbox;
        QCheckBox             *random_checkbox;
        QCheckBox             *playlist_checkbox;

        QPushButton           *play_button;
        QPushButton           *next_button;
        QPushButton           *prev_button;

        QStandardItemModel    *song_list_model;
        QStandardItemModel    *playlist_model;
        QSortFilterProxyModel *sort_model;

        QListView             *song_list;
        QListView             *playlist;

        QSlider               *volume_slider;
        QSlider               *time_slider;

        QTimer                *timer;
        QPoint                 contextmenu_point;
        QString                paths[MAX_PATHS];
        QMenuBar              *menu_bar;
        QLineEdit             *find_song_name;
        QTabWidget            *tab;
        QMediaPlayer          *player;
        QSharedMemory         *memory;

        AboutWindow           *about_window;
        OptionWindow          *path_window;

        void Load_Song          (QString name);
        void Player_Init        ();
        void Create_Menu        ();
        void Update_Song_Label  ();

        int         song_queue[2*MAX_SONGS];
        int         playlist_song_amount;
        int         song_amount;
        int         path_amount;
        int         song_id;
    public:
              MainWindow         ();
        void  wheelEvent(QWheelEvent *ev);
        void  Load_And_Play_Song (QString name);
    private slots:

        void Timeout                ();
        void Volume_Up              ();
        void Loop_Toggle            ();
        void Volume_Down            ();
        void Create_Path            ();
        void Play_Pressed           ();
        void Next_Pressed           ();
        void Prev_Pressed           ();
        void Reload_Paths           ();
        void Reload_Songs           ();
        void Random_Songs           ();
        void Create_About           ();
        void About_To_Quit          ();
        void Random_Toggle          ();
        void Playlist_Toggle        ();
        void Add_To_Playlist        ();
        void Load_First_Song        ();
        void Remove_From_Playlist   ();
        void Song_End               (QMediaPlayer::MediaStatus state);
        void List_Pressed           (const  QModelIndex & index);
        void Time_Changed           (qint64 n);
        void Time_Pressed           (int    n);
        void Volume_Change          (int    n);
        void Playlist_Pressed       (const  QModelIndex & index);
        void Playlist_Context_Menu  (QPoint point);
        void Song_List_Context_Menu (QPoint point);
};

#endif // MAINWINDOW_H
