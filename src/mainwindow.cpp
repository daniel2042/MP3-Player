#include <QApplication>
#include "../inc/mainwindow.h"
#include "../inc/about.h"


#define TIME_SLIDER_MAX 100

/**
 \brief Konstruktor glownego okna

 Funkcja odpowiedzialna za:
 - layout okna
 - konfiguracje oddtwarzacza
 - wczytanie piosenek
 - wczytanie sciezek
 - ustawienie ogolnych skrotow klawiszowych
 - polaczenie sygnalow i slotow
 */
MainWindow::MainWindow()
{

    // Nie ruszac!!!
    srand(time(NULL));
    song_id = -1;
    playlist_song_amount = 0;
    song_amount = 0;
    path_window = 0;
    about_window = 0;


    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("UTF-8"));
    QTextCodec::codecForName("UTF-8");

    QPixmap play_mask(":/play.png");
    QPixmap next_mask(":/next.png");
    QPixmap prev_mask(":/prev.png");

    QFont app_font ("Arial", 10, 0,false);
    QFont song_font("Arial", 14, 0,false);
    qApp->setFont(app_font);


    this->setGeometry(100,100,400,480);
    this->setFixedSize(this->width(),this->height());
    this->setWindowIcon(QIcon(":/ico.ico"));
    this->setWindowTitle("MP3 Player @Daniel Majchrzycki");

    play_button = new QPushButton (this);
    play_button->setGeometry(175, 65, 50, 50);
    play_button->setMask(play_mask.mask());
    play_button->setStyleSheet(PLAYBUTTON_STYLESHEET);

    next_button = new QPushButton(this);
    next_button->setGeometry(240,75,30,30);
    next_button->setMask(next_mask.mask());
    next_button->setStyleSheet(NEXTBUTTON_STYLESHEET);

    prev_button = new QPushButton(this);
    prev_button->setGeometry(130,75,30,30);
    prev_button->setMask(prev_mask.mask());
    prev_button->setStyleSheet(PREVBUTTON_STYLESHEET);

    song_name = new QLabel(this);
    song_name->setGeometry(10,35,390,22);
    song_name->setText(tr("---"));
    song_name->setAlignment(Qt::AlignCenter);
    song_name->scroll(100,0);
    song_name->setFont(song_font);

    song_amount_label = new QLabel(this);
    song_amount_label->setGeometry(180,120,70,20);
    song_amount_label->setText("0/0");

    song_length = new QLabel(this);
    song_length->setGeometry(350,425,50,18);
    song_length->setText(tr("0:00"));

    song_position = new QLabel(this);
    song_position->setGeometry(10,425,50,18);
    song_position->setText(tr("0:00"));

    sort_model = new QSortFilterProxyModel;
    song_list_model = new QStandardItemModel();
    song_list = new QListView(this);
    song_list->setGeometry(10, 120, 380,260);
    song_list->setModel(sort_model);
    song_list->setEditTriggers(0);
    song_list->setContextMenuPolicy(Qt::CustomContextMenu);
    sort_model->setSourceModel(song_list_model);

    playlist_model = new QStandardItemModel();
    playlist = new QListView(this);
    playlist->setGeometry(10, 120, 380,260);
    playlist->setModel(playlist_model);
    playlist->setEditTriggers(0);
    playlist->setContextMenuPolicy(Qt::CustomContextMenu);

    volume_slider = new QSlider(Qt::Horizontal,this);
    volume_slider->setMinimum(0);
    volume_slider->setMaximum(100);
    volume_slider->setValue(50);
    volume_slider->setGeometry(300,80,90,20);

    time_slider = new QSlider(Qt::Horizontal,this);  // Slider czasowy
    time_slider->setMinimum(0);
    time_slider->setMaximum(TIME_SLIDER_MAX);
    time_slider->setValue(0);
    time_slider->setGeometry(10,400,380,20);
    time_slider->setStyleSheet(TIMESLIDER_STYLESHEET);

    loop_checkbox = new QCheckBox(this);
    loop_checkbox->setGeometry(10,60,100,20);
    loop_checkbox->setText("Loop");

    random_checkbox = new QCheckBox(this);
    random_checkbox->setGeometry(10,80,70,20);
    random_checkbox->setText("Random");
    random_checkbox->setChecked(true);

    playlist_checkbox = new QCheckBox(this);
    playlist_checkbox->setGeometry(10,100,70,20);
    playlist_checkbox->setText("Playlist");

    find_song_name = new QLineEdit(this);
    find_song_name->setGeometry(10,450,380,20);

    tab = new QTabWidget(this);
    tab->setGeometry(10,130,380,260);
    tab->addTab(song_list,"Songs");
    tab->addTab(playlist ,"Playlist");

    next_shortcut = new QAction(this);
    next_shortcut->setShortcut(Qt::Key_Right);

    prev_shortcut = new QAction(this);
    prev_shortcut->setShortcut(Qt::Key_Left);

    volume_up_shortcut = new QAction(this);
    volume_up_shortcut->setShortcut(Qt::Key_Up);

    volume_down_shortcut = new QAction(this);
    volume_down_shortcut->setShortcut(Qt::Key_Down);

    loop_shortcut = new QAction(this);
    loop_shortcut->setShortcut(Qt::Key_L);

    random_shortcut = new QAction(this);
    random_shortcut->setShortcut(Qt::Key_R);

    playlist_shortcut = new QAction(this);
    playlist_shortcut->setShortcut(Qt::Key_P);

    play_shortcut = new QAction(this);
    play_shortcut->setShortcut(Qt::Key_Space);


    memory = new QSharedMemory(SHARED_MEMORY_ADRESS);

    timer = new QTimer(this);
    timer->setInterval(250);
    timer->start();

    this->addAction(next_shortcut);
    this->addAction(prev_shortcut);
    this->addAction(volume_up_shortcut);
    this->addAction(volume_down_shortcut);
    this->addAction(loop_shortcut);
    this->addAction(random_shortcut);
    this->addAction(playlist_shortcut);
    this->addAction(playlist_shortcut);

    Reload_Paths();
    Reload_Songs();
    Create_Menu();
    Player_Init();
    Random_Songs();
    QString name = sort_model->index(song_queue[song_id],0).data(Qt::DisplayRole).toString();
    name.chop(4);
    Load_Song(name);
    song_name->setText(name);
    Update_Song_Label();
    Play_Pressed();


    connect(qApp                  , SIGNAL(aboutToQuit())                                   ,this,       SLOT(About_To_Quit()));
    connect(timer                 , SIGNAL(timeout())                                       ,this,       SLOT(Timeout         ()));
    connect(play_button           , SIGNAL(clicked())                                       ,this,       SLOT(Play_Pressed    ()));
    connect(next_button           , SIGNAL(clicked())                                       ,this,       SLOT(Next_Pressed    ()));
    connect(prev_button           , SIGNAL(clicked())                                       ,this,       SLOT(Prev_Pressed    ()));
    connect(next_shortcut         , SIGNAL(triggered())                                     ,this,       SLOT(Next_Pressed    ()));
    connect(prev_shortcut         , SIGNAL(triggered())                                     ,this,       SLOT(Prev_Pressed    ()));
    connect(volume_up_shortcut    , SIGNAL(triggered())                                     ,this,       SLOT(Volume_Up       ()));
    connect(volume_down_shortcut  , SIGNAL(triggered())                                     ,this,       SLOT(Volume_Down     ()));
    connect(loop_shortcut         , SIGNAL(triggered())                                     ,this,       SLOT(Loop_Toggle     ()));
    connect(random_shortcut       , SIGNAL(triggered())                                     ,this,       SLOT(Random_Toggle   ()));
    connect(playlist_shortcut     , SIGNAL(triggered())                                     ,this,       SLOT(Playlist_Toggle ()));
    connect(play_shortcut         , SIGNAL(triggered())                                     ,this,       SLOT(Play_Pressed    ()));
    connect(find_song_name        , SIGNAL(returnPressed())                                 ,this,       SLOT(Load_First_Song ()));
    connect(volume_slider         , SIGNAL(valueChanged(int))                               ,this,       SLOT(Volume_Change   (int)));
    connect(time_slider           , SIGNAL(sliderMoved (int))                               ,this,       SLOT(Time_Pressed    (int)));
    connect(find_song_name        , SIGNAL(textChanged(QString))                            ,sort_model, SLOT(setFilterFixedString    (QString)));
    connect(player                , SIGNAL(positionChanged(qint64))                         ,this,       SLOT(Time_Changed            (qint64 )));
    connect(song_list             , SIGNAL(customContextMenuRequested(QPoint))              ,this,       SLOT(Song_List_Context_Menu  (QPoint )));
    connect(playlist              , SIGNAL(customContextMenuRequested(QPoint))              ,this,       SLOT(Playlist_Context_Menu   (QPoint )));
    connect(song_list             , SIGNAL(doubleClicked(const QModelIndex& ))              ,this,       SLOT(List_Pressed            (const QModelIndex &)));
    connect(playlist              , SIGNAL(doubleClicked(const QModelIndex& ))              ,this,       SLOT(Playlist_Pressed        (const QModelIndex &)));
    connect(player                , SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus))   ,this,       SLOT(Song_End                (QMediaPlayer::MediaStatus)));

}

/**
 * @brief Funkcja usuwajaca wspolna pamiec
 */
void MainWindow::About_To_Quit()
{
    memory->detach();
}

/**
 * \brief Play/Pause piosenki
 *
 * Funkcja odpowiedzialna za obsluge wcisniecia przycisku play/pause.
 * Polaczona z play_button.
 */
void MainWindow::Play_Pressed()
{
    if(!song_amount) return;                                                                            //Brak dostepnych piosenek

    if(player->state() == QMediaPlayer::PausedState || player->state() == QMediaPlayer::StoppedState)
    {
        player->play();
        play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
    }
    else
    {
        player->pause();
        play_button->setStyleSheet(PLAYBUTTON_STYLESHEET);
    }
}
/**
 * \brief Menu okna
 *
 * Tworzy menu okna glownego:
 * - dodaje do niego akcje
 * - dodaje odpowiednie skroty
 * - laczy akcje z SLOTami
 */
void MainWindow::Create_Menu()
{
    QMenuBar  *menu_bar     = menuBar();
    QMenu     *menu_songs   = new QMenu("Songs"   ,this);
    QMenu     *menu_options = new QMenu("Options" ,this);
    QMenu     *menu_about   = new QMenu("About"   ,this);

    menu_bar->addMenu(menu_songs  );
    menu_bar->addMenu(menu_options);
    menu_bar->addMenu(menu_about  );


    random = new QAction(tr("Random")  ,this);
    reload = new QAction(tr("Reload")  ,this);
    exit   = new QAction(tr("Exit")    ,this);
    about  = new QAction(tr("About")   ,this);
    path   = new QAction(tr("Add Path"),this);

    reload->setShortcut(Qt::Key_F5);
    random->setShortcut(Qt::Key_F6);

    connect (exit  , SIGNAL(triggered()), qApp, SLOT(quit())        );
    connect (reload, SIGNAL(triggered()), this, SLOT(Reload_Songs()));
    connect (random, SIGNAL(triggered()), this, SLOT(Random_Songs()));
    connect (about , SIGNAL(triggered()), this, SLOT(Create_About()));
    connect (path  , SIGNAL(triggered()), this, SLOT(Create_Path()) );


    menu_songs->addAction(random);
    menu_songs->addAction(reload);
    menu_about->addAction(about );
    menu_about->addAction(exit  );
    menu_options->addAction(path);
}

/**
 * \brief Funkcja odswiezajaca liste piosenek
 *
 * Funkcja przeszukujaca sciezki i dodajaca znalezione piosenki do listy
 * - ogranicza liczbe wczytanych piosenek do MAX_SONGS
 * - filtruje pliki - mp3
 */
void MainWindow::Reload_Songs()
{

    int k = 0;
    int i = 0;
    QDir dir;
    QString             rep;
    QStringList         filters;
    QStringList         list;
    QStandardItem     **item    = new QStandardItem* [MAX_SONGS];
    QMessageBox        *Mess    = new QMessageBox();

    Mess->setText(tr("Too many songs to load!"));
    Mess->setWindowTitle("Error!");
    Mess->setIcon(QMessageBox::Warning);
    filters << "*.mp3";                                                             // Ustawienie filtrow plikow
    song_list_model->clear();


    for(int j = 0; j <= path_amount ; j++)                                          // Petla po sciezkach
    {
        dir.setNameFilters(filters);
        dir.setPath(paths[j]);
        list = dir.entryList(filters);


        while(k < list.size())                                                       // Petla po piosenkach
        {
               item[i] = new QStandardItem();
               item[i]->setText(list.at(k));
               if(rep.indexOf(list.at(k),0) == -1)
               {
                  rep += list.at(k);
                  song_list_model->setItem(i,item[i]);
                  i++;
               }

               k++;

               if(i == MAX_SONGS) // Za duzo piosenek
               {
                  Mess->exec();
               }
               song_id = 0;
        }

        song_amount = i;                                                              // Przypisanie ilosci wczytanych piosenek
        k = 0;
    }



    sort_model->sort(0);    // Restart sortowania
    Update_Song_Label();    // Oswiezenie liczby piosenek
    Random_Songs();         // Ponowne wylosowanie kolejnosci

}

/**
 * \brief Funkcja odpowiedzialna za obsluge wcisniecia listy
 *
 * Funkcja wywolywana po podwojnym wcisnieciu wybranej piosenki na liscie
 * - wczytuje piosenke
 * - ustawia nazwe piosenki
 * - wlacza oddtwarzanie
 * - wylacza playliste
 * - losuje kolenosc piosenek
 *
 * \param index - index wcisnietej piosenki
 */
void MainWindow::List_Pressed(const QModelIndex & index)
{
   QString name = index.model()->data(index, Qt::DisplayRole).toString(); // Wczytuje nazwe piosenki
   name.chop(4);                                                          // odcina .mp3 z nazwy
   song_id   = index.row();                                               // ustawia index aklualnie wczytanej piosenki
   song_name->setText(name);                                              // ustawia nazwe piosenki
   Load_Song(name);                                                       // wczytuje piosenke (sprawdza poprawnosc)
   player->play();
   play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);                    // zmienia wyglad przycisku PLAY/PAUSE
   playlist_checkbox->setChecked(false);                                  // wylacza playliste
   Update_Song_Label();
   Random_Songs();
}

/**
 * \brief Funkcja odpowiedzialna za obluge wcisniecia playlisty
 *
 * Funkcja wywolywana po podwojnym wcisnieciu wybranej piosenki na playliscie
 * - wczytuje piosenke
 * - ustawia nazwe piosenki
 * - wlacza oddtwarzanie
 * - wlacza playliste
 *
 * \param index - index wcisnietej piosenki
 */
void MainWindow::Playlist_Pressed(const QModelIndex & index)
{
   QString name = index.model()->data(index, Qt::DisplayRole).toString();
   name.chop(4);

   Load_Song(name);
   song_id = index.row();
   song_name->setText(name);
   player->play();
   play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
   playlist_checkbox->setChecked(true);
   Update_Song_Label();
}


/**
 * \brief Funkcja inicjujaca oddtwarzacz
 *
 * Funkcja wywolywane w konstruktorze glownego okna
 * - tworzy nowy oddtwarzacz
 * - ustawia poczatkowa glosnosc
 * - wczytuje piosenke
 * - ustawia nazwe piosenki
 */
void MainWindow::Player_Init()
{
    player = new QMediaPlayer;
    player->setVolume(START_VOLUME_LEVEL);

    if( song_id < MAX_SONGS && song_id >= 0)
    {
        QString name = sort_model->index(song_id,0).data(Qt::DisplayRole).toString();
        name.chop(4);
        song_name->setText(name);
        Load_Song(name);
        Update_Song_Label();
    }
}

/**
 * \brief Funkcja zmieniajaca czas na QString
 *
 * Funkcja zmienniajaca liczbe milisekund na QString postaci mm:ss
 *
 * \param ms - liczba milisekund
 * \return czas w formie (np)"3:34"
 */
QString Ms_To_Time(int ms)
{
    QString min = QString::number( ms/60000   );
    QString sec = QString::number((ms/1000)%60);

    QString tmp;

    if((ms/1000)%60 < 10)
        sec = "0" + sec;

    tmp = min + ":" + sec;

    if(ms < 0)
    {
        tmp = "0:00";

    }
    return tmp;
}

/**
 * \brief Funkcja zmieniajaca glosnosc oddtwarzania
 *
 * \param n - glosnosc w skali 0-100
 */
void MainWindow::Volume_Change(int n)
{
    player->setVolume(n);
}

/**
 * @brief Zmiana glosnosci rolka myszy
 *
 * @param ev - zdarzenie obrotu
 */
void MainWindow::wheelEvent(QWheelEvent *ev)
{
    volume_slider->setValue(volume_slider->value() + ev->angleDelta().y()/10);
}

/**
 * \brief Funkcja zmieniajaca polozenie suwaka czasu(oddtwarzacz)
 *
 * \param n - pozycja suwaka w skali 0-100
 */
void MainWindow::Time_Changed(qint64 n)
{

    long int position = player->position();
    long int size     = player->duration();

    if(!time_slider->isSliderDown())                        // Sprawdzenie czy suwak nie jest trzymany przez uzytkownika
        time_slider->setValue(TIME_SLIDER_MAX * n/size);

    // Aktualizacja wyswietlanego czasu
    song_length  ->setText(Ms_To_Time(size));
    song_position->setText(Ms_To_Time(position));

}

/**
 * @brief Funkcja zmieniajaca czas oddtwarzacza
 *
 * @param n - postep piosenki(0-100%)
 */
void MainWindow::Time_Pressed(int n)
{
    long int size = player->duration();
    player->setPosition(size*n/TIME_SLIDER_MAX);
    player->play();
    play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
}

/**
 * @brief Funkcja komunikujaca sie z innymi instancjami
 *sprea
 * Funkcja komunikujaca sie z innymi instancjami poprzez QSharedMemory
 * - Funkcja odczytuje piosenke z ktora zostala wywolana inna instancja
 */
void MainWindow::Timeout()
{
   int tab[256] = {0};
   if (!memory->attach())
   {
       memory->lock();
          QString tmp((char*)memory->data());
          memcpy(memory->data(),tab, 256);
       memory->unlock();
       memory->detach();




       if(!tmp.isEmpty())
       {
          song_id = 0;
          Load_And_Play_Song(tmp);
          this->setWindowState(Qt::WindowActive);
          this->showNormal();
       }
   }
}

/**
 * @brief Funkcja wywolywana przy zmianie stanu oddtwarzacza
 *
 * Funkcja wywolywana przy zmianie stanu oddtwarzacza
 * - sprawdza czy piosenka zostala zakonczona
 *   - uruchamia kolejna z playlisty     (checkbox playlist) break;
 *   - uruchamia kolejna losowa piosenke (checkbox random  ) break;
 *   - uruchamia te sama piosenke        (checkbox loop    ) break;
 *
 * @param state - stan oddtwarzacza
 */
void MainWindow::Song_End(QMediaPlayer::MediaStatus state )
{
    if(state == QMediaPlayer::EndOfMedia)
    {
        if(playlist_checkbox->isChecked())                                                              // Playlista
        {
            if(playlist_song_amount > 0)                                                                // Czy sa piosenki w playliscie
            {
                song_id++;                                                                              // Kolejna piosenka
                sort_model->setFilterFixedString("");                                                   // Kasowanie filtra nazwy
                song_id %= playlist_song_amount;                                                        // Zabezpieczenie przed przepelnieniem
                QString name = playlist_model->item(song_id,0)->data(Qt::DisplayRole).toString();       // Wczytanie nazwy
                name.chop(4);                                                                           // Usuniecie ".mp3"
                Load_Song(name);                                                                        // Wczytanie piosenki
                player->play();
                play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);                                     // Zmiana wygladu przycisku PLAY/PAUSE
                song_name->setText(name);                                                               // Ustawienie nazyw piosenki
                Update_Song_Label();                                                                    // Odswiezenie liczby piosenek
                return;
            }
        }
        else
            if(random_checkbox->isChecked())                                                            // Random
            {
                song_id++;
                sort_model->setFilterFixedString("");
                QString name = sort_model->index(song_queue[song_id] ,0).data(Qt::DisplayRole).toString();
                name.chop(4);
                Load_Song(name);
                player->play();
                song_name->setText(name);
                Update_Song_Label();
                return;

            }
        if(loop_checkbox->isChecked())                                                                   // Loop
        {
            player->play();
            return;
        }
        play_button->setStyleSheet(PLAYBUTTON_STYLESHEET);                                               // Zadne - Zmiana wyglacu przycisku PLAY/PAUSE
    }

}

/**
 * @brief Wcisniecie przycisku NEXT
 *
 * Funkcja odpowiedzialna za przetworzenie wcisniecia przycisku NEXT
 *   - uruchamia kolejna z playlisty     (checkbox playlist) break;
 *   - uruchamia kolejna losowa piosenke (checkbox random  ) break;
 *   - uruchamia kolejna piosenke
 */
void MainWindow::Next_Pressed()
{
    if(!song_amount) return;                                                                        // Brak piosenek
    sort_model->setFilterFixedString("");                                                           // Zerowanie filtra nazw
    song_id++;                                                                                      // Kolejna piosenka
    song_id %= song_amount;                                                                         // Zabezpieczenie przed przepelnieniem

    if(playlist_checkbox->isChecked())                                                              // Playlista
    {
        if(playlist_song_amount > 0)
        {
            song_id %= playlist_song_amount;
            QString name = playlist_model->item(song_id,0)->data(Qt::DisplayRole).toString();
            name.chop(4);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            song_name->setText(name);
            Update_Song_Label();
            return;
        }
    }
    else
        if(random_checkbox->isChecked())                                                                // Random
        {
            QString name = sort_model->index(song_queue[song_id],0).data(Qt::DisplayRole).toString();
            name.chop(4);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);

            song_name->setText(name);
            Update_Song_Label();

        }
        else                                                                                             // Next
        {
            QString name = sort_model->index(song_id,0).data(Qt::DisplayRole).toString();
            name.chop(4);
            song_name->setText(name);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            Update_Song_Label();
        }
}

/**
 * @brief Wcisniecie przycisku PREV
 *
 * Funkcja odpowiedzialna za przetworzenie wcisniecia przycisku PREV
 *   - uruchamia poprzednia z playlisty     (checkbox playlist) break;
 *   - uruchamia poprzednia losowa piosenke (checkbox random  ) break;
 *   - uruchamia kolejna piosenke
 */
void MainWindow::Prev_Pressed()
{
    if(!song_amount) return;                                                                        // Brak piosenek
    song_id--;
    sort_model->setFilterFixedString("");
    if(song_id < 0 || song_id > MAX_SONGS) song_id = song_amount - 1;                               // Zabezpieczenie przed przekroczeniem zakresu

    if(playlist_checkbox->isChecked())                                                              // Playlist
    {
        if(playlist_song_amount > 0)
        {
            if(song_id < 0 || song_id >= playlist_song_amount) song_id = playlist_song_amount - 1;
            QString name = playlist_model->item(song_id,0)->data(Qt::DisplayRole).toString();
            name.chop(4);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            song_name->setText(name);
            Update_Song_Label();
        }
    }
    else
        if(random_checkbox->isChecked())                                                                // Random
        {
            QString name = sort_model->index(song_queue[song_id],0).data(Qt::DisplayRole).toString();
            name.chop(4);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            song_name->setText(name);
            Update_Song_Label();

        }
        else                                                                                             // Prev
        {
            QString name = sort_model->index(song_id,0).data(Qt::DisplayRole).toString();
            name.chop(4);
            song_name->setText(name);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            Update_Song_Label();
        }

}

/**
 * @brief Aktualizuje numer aktualnej piosenki oraz liczbe wczytanych piosenek
 */
void MainWindow::Update_Song_Label()
{
    QString song_tmp;
    if(playlist_checkbox->isChecked())
        song_tmp = QString::number(song_id + 1) + "/" + QString::number(playlist_song_amount);
    else
        song_tmp = QString::number(song_id + 1) + "/" + QString::number(song_amount);

    song_amount_label->setText(song_tmp);
}

/**
 * @brief Dodaje piosenke do playlisty
 *
 * Funkcja dodaje piosenke wybrana z menu kontekstowego do playlisty
 */
void MainWindow::Add_To_Playlist()
{
    QModelIndex index = song_list->indexAt(contextmenu_point);
    QStandardItem *itm = new QStandardItem();
    QString tmp = index.data(Qt::DisplayRole).toString();
    itm->setText(tmp);
    playlist_model->setItem(playlist_model->rowCount(),itm);
    playlist_song_amount++;
    Update_Song_Label();
}

/**
 * @brief Funkcja usuwa z playlisty element wybrany z menu kontekstowego
 *
 * Funkcja usuwa z playlisty element wybrany z menu kontekstowego
 * - jezeli usunieta piosenka byla oddtwarzana
 *    - uruchamia nastepna piosenke z playlisty
 * - jezeli byla to ostatnia piosenka na playliscie
 * - wlacza kolejna losowa albo nastepna
 */
void MainWindow::Remove_From_Playlist()
{
    QModelIndex index = playlist->indexAt(contextmenu_point);
    playlist->model()->removeRow(index.row());
    playlist_song_amount--;

    if(index.row() == song_id)
    {
        if(song_id != playlist_song_amount )
        {
            QString name = playlist_model->item(song_id,0)->data(Qt::DisplayRole).toString();
            name.chop(4);
            song_name->setText(name);
            Load_Song(name);
            player->play();
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            playlist_checkbox->setChecked(true);
            Update_Song_Label();
        }
        else
        {
            song_id = 0;
            if(playlist_song_amount != song_id)
            {
                QString name = playlist_model->item(song_id,0)->data(Qt::DisplayRole).toString();
                name.chop(4);
                song_name->setText(name);
                Load_Song(name);
                player->play();
                play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
                playlist_checkbox->setChecked(true);
                Update_Song_Label();
            }
            else
            {
                playlist_checkbox->setChecked(false);
                if(random_checkbox->isChecked())
                {
                    QString name = sort_model->index(song_queue[song_id],0).data(Qt::DisplayRole).toString();
                    name.chop(4);
                    Load_Song(name);
                    player->play();
                    play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
                    song_name->setText(name);
                    Update_Song_Label();
                }
                else
                {
                    QString name = sort_model->index(song_id,0).data(Qt::DisplayRole).toString();
                    name.chop(4);
                    song_name->setText(name);
                    Load_Song(name);
                    player->play();
                    play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
                    Update_Song_Label();
                }
            }
        }
    }
}


/**
 * @brief Menu kontekstowe listy piosenek
 *
 * Funkcja tworzaca menu kontekstowe dla listy
 * - menu daje mozliwosc dodania piosenki do playlisty
 * @param point - punkt wywolania menu pontekstowego
 */
void MainWindow::Song_List_Context_Menu(QPoint point)
{
    QMenu *menu = new QMenu( this);
    playlist_add = new QAction(tr("Add to playlist"), this);
    connect (playlist_add  , SIGNAL(triggered()), this, SLOT(Add_To_Playlist()));
    menu->addAction(playlist_add);

    contextmenu_point = point;
    QModelIndex index = song_list->indexAt(point);
    if(index.isValid())
       menu->exec(song_list->mapToGlobal(point));

    delete menu;

}

/**
 * @brief Menu kontekstowe Playlisty
 *
 * Funkcja tworzaca menu kontekstowe dla playlisty
 * - menu daje mozliwosc usuniecia piosenki z playlisty
 *
 * @param point - punkt wywolania menu pontekstowego
 */
void MainWindow::Playlist_Context_Menu(QPoint point)
{
    QMenu *menu = new QMenu( this);
    playlist_remove = new QAction(tr("Remove from playlist"), this);
    connect (playlist_remove  , SIGNAL(triggered()), this, SLOT(Remove_From_Playlist()));
    menu->addAction(playlist_remove);

    contextmenu_point = point;
    QModelIndex index = playlist->indexAt(point);
    if(index.isValid())
        menu->exec(playlist->mapToGlobal(point));

    delete menu;

}
/**
 * @brief Wczytuje i oddtwarza piosenke
 *
 * Funkcja wczytuje i oddtwarza piosenke name
 * - usuwa sciezke bezwzgledna z nazwy
 * - wczytuje piosenke
 * - wlacza oddtwarzanie piosenki
 * - zmienia wyglad przycisku PLAY/PAUSE
 * @param name
 */
void  MainWindow::Load_And_Play_Song(QString name)
{
    QString tmp = name;
    name.chop(4);                                   // Usuwa ".mp3"
    name.remove(0,name.lastIndexOf("/") + 1);       // Usuwa wszystko do ostatniego '/' w sciezce

    if(song_name->text() != name)                   // wczytuje piosenke tylko wtedy gdy jest inna niz dotychczas oddtwarzana
    {
        song_name->setText(name);
        player->setMedia(QUrl::fromLocalFile(tmp));
        player->play();
        play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
        Update_Song_Label();
    }
}

/**
 * @brief Funkcja losujaca nowa kolejnosc oddtwaraznia piosenek
 */
void MainWindow::Random_Songs()
{
    memset(song_queue,-1,2*MAX_SONGS * sizeof(int));
    int place = 0;
    for(int j = 0; j < song_amount; j++)
    {
       place = rand() % song_amount;
       if(song_queue[place] == -1)
           song_queue[place] = j;
       else
           j--;
    }

    int k = 0;
    for(unsigned int i = 0; i < 2*MAX_SONGS; i++)
    {
       if(song_queue[i] != -1)
       {
          song_queue[k] = song_queue[i];
          k++;
       }
    }
}

/**
 * @brief Funkcja wczytujaca sciezki
 *
 * Funkcja wczytujaca sciezki(z pliku paths.pth w ktorych program ma szukac piosenek
 */
void MainWindow::Reload_Paths()
{
    std::fstream path_file;
    std::string tmp;
    int i = 1;
    paths[0] = "";
    path_file.open("paths.pth",std::ios::in);
    if(path_file.good())
    {
        while(!path_file.eof())
        {
            std::getline(path_file,tmp);
            paths[i] =  QString::fromStdString(tmp) + "/";
            i++;
        }
        path_file.close();
    }
    else
    {
        QMessageBox war;
        war.setText("Unable to load PATHS!");
        war.setIcon(QMessageBox::Warning);
        war.exec();
    }
    path_amount = i - 1;
}

/**
 * @brief Tworzy nowe okno ABOUT
 */
void MainWindow::Create_About()
{
   if(about_window == 0)                    // Jezeli takie okno nie istnieje
        about_window = new AboutWindow();
   about_window->show();
}

/**
 * @brief Tworzy nowe okno ladowania sciezek
 */
void MainWindow::Create_Path()
{
    if(path_window == 0)
        path_window = new OptionWindow();
    path_window->show();
    connect(path_window  , SIGNAL(Window_Close()) ,this, SLOT(Reload_Paths()));
}
/**
 * @brief Wczytuje piosenke ze sprawdzeniem jej istnienia
 *
 * @param name - nazwa piosenki - bez ".mp3"!!!
 */
void MainWindow::Load_Song(QString name)
{
    std::fstream file;
    for(int j = 0; j <= path_amount; j++)
    {
        QString tmp = paths[j] + name + ".mp3";
        file.open(tmp.toStdString().c_str(),std::ios::in);
        if(file.good())
        {
            player->setMedia(QUrl::fromLocalFile(tmp));
            break;
        }
    }
}


/**
 * @brief Oddtwarza pierwsza piosenke na liscie
 *
 * Funkcja oddtwarzajaca pierwsza pisenke na liscie
 * - wczytuje piosenke
 * - ustwaia nazwe piosenki
 * - wlacza oddtwarzanie piosenki
 * - ustawia focus na okno glowne
 */
void MainWindow::Load_First_Song()
{
    std::fstream file;
    QString name = sort_model->index(0,0).data(Qt::DisplayRole).toString();

    for(int j = 0; j <= path_amount; j++)
    {
        QString tmp = paths[j] + name;

        file.open(tmp.toStdString().c_str(),std::ios::in);
        if(file.good())
        {
            name.chop(4);
            song_name->setText(name);
            play_button->setStyleSheet(PAUSEBUTTON_STYLESHEET);
            playlist_checkbox->setChecked(false);
            player->setMedia(QUrl::fromLocalFile(tmp));
            player->play();
            song_id = 0;
            Update_Song_Label();
            Random_Songs();
            this->setFocus();
            break;
        }
    }
}

/**
 * @brief Obsluga skrotu klawiszowego KEY_DOWN
 */
void MainWindow::Volume_Down()
{
    if(volume_slider->value() > 10)
        volume_slider->setValue(volume_slider->value() - 10);
    else
        volume_slider->setValue(0);
}

/**
 * @brief Obsluga skrotu klawiszowego KEY_UP
 */
void MainWindow::Volume_Up()
{
    if(volume_slider->value() < 90)
        volume_slider->setValue(volume_slider->value() + 10);
    else
        volume_slider->setValue(100);
}

/**
 * @brief Obsluga skrotu klawiszowego KEY_L
 */
void MainWindow::Loop_Toggle()
{
    loop_checkbox->setChecked(!loop_checkbox->isChecked());
}

/**
 * @brief Obsluga skrotu klawiszowego KEY_R
 */
void MainWindow::Random_Toggle()
{
    random_checkbox->setChecked(!random_checkbox->isChecked());
}

/**
 * @brief Obsluga skrotu klawiszowego KEY_P
 */
void MainWindow::Playlist_Toggle()
{
    playlist_checkbox->setChecked(!playlist_checkbox->isChecked());
}


/**
 \bug
  Brak obslugi polskich znakow w nazwach piosenek przy wczytywaniu z linii komend(tylko Windows)
*/










