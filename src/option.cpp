#include <QApplication>
#include "../inc/option.h"
#include "../inc/stylesheet.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300


/**
 * @brief Funkcja Tworzaca okno do wyboru sciezek
 *
 * Funkcja odpowiadajaca za:
 * - wyglad okna
 * - polaczenie sygnalow i slotow
 * - wczytanie sciezek z pliku
 */
OptionWindow::OptionWindow()
{
    path_amount = 0;

    this->setGeometry(200,200,WINDOW_WIDTH,WINDOW_HEIGHT);
    this->setWindowTitle("Add PATH");
    this->setFixedHeight(WINDOW_HEIGHT);
    this->setFixedWidth(WINDOW_WIDTH);

    new_path =  new QLineEdit(this);
    new_path->setGeometry(10,10,270,20);

    model = new QStandardItemModel();
    path_list = new QListView(this);
    path_list->setGeometry(10,40,380,250);
    path_list->setModel(model);
    path_list->setEditTriggers(QListView::NoEditTriggers);
    path_list->setContextMenuPolicy(Qt::CustomContextMenu);

    add_button = new QPushButton(this);
    add_button->setGeometry(290,7,40,26);
    add_button->setText("Add");

    browse_button = new QPushButton(this);
    browse_button->setGeometry(340,7,50,26);
    browse_button->setText("Browse");

    connect (add_button     , SIGNAL(clicked())                             , this, SLOT(Add_Pressed    ()));
    connect (browse_button  , SIGNAL(clicked())                             , this, SLOT(Browse_Pressed ()));
    connect (path_list      , SIGNAL(customContextMenuRequested(QPoint))    , this, SLOT(Path_List_Context_Menu(QPoint)));

    Load_Paths();
}

/**
 * @brief Funkcja wczytujaca sciezki z pliku
 */
void OptionWindow::Load_Paths()
{
    std::fstream path_file;
    std::string     tmp;
    QString buf;

    path_file.open("paths.pth",std::ios::in);
    if(path_file.good())
    {
        while(!path_file.eof())
        {
            std::getline(path_file,tmp);
            QStandardItem *itm = new QStandardItem();
            buf = QString::fromStdString(tmp);
            itm->setText(buf);
            model->setItem(model->rowCount(),itm);
            path_amount++;
        }
        path_file.close();
    }
}

/**
 * @brief Funkcja wywolywana po wcisnieciu przycisku ADD
 */
void OptionWindow::Add_Pressed()
{
    QStandardItem *itm = new QStandardItem();
    QString tmp = new_path->text();

    if(!tmp.isEmpty())
    {
        itm->setText(tmp);
        model->setItem(model->rowCount(),itm);
        path_amount++;
    }
}

/**
 * @brief Funkcja wywolywana po wcisnieciu przycisku BROWSE
 */
void OptionWindow::Browse_Pressed()
{
   new_path->setText(QFileDialog::getExistingDirectory(this,"PATH Folder","",0));
}

/**
 * @brief Funkcja wywolywane przez menu kontekstowe
 *
 * Funkcja usuwa aciezke z listy
 */
void OptionWindow::Remove_From_Path_List()
{
    QModelIndex index = path_list->indexAt(contextmenupoint);
    path_list->model()->removeRow(index.row());
    path_amount--;
}

/**
 * @brief Funkcja tworzaca menu kontekstowe
 *
 * @param point - wsporzedne menu kontekstowego
 */
void OptionWindow::Path_List_Context_Menu(QPoint point)
{
    QMenu *menu = new QMenu(this);
    path_list_remove = new QAction(tr("Remove from PATH"), this);
    connect (path_list_remove  , SIGNAL(triggered()), this, SLOT(Remove_From_Path_List()));
    menu->addAction(path_list_remove);

    contextmenupoint = point;
    QModelIndex index = path_list->indexAt(point);
    if(index.isValid())
       menu->exec(path_list->mapToGlobal(point));

    delete menu;
}

/**
 * @brief Funkcje zapisujaca sciezki do pliku
 */
void OptionWindow::Save_Paths()
{
    settings_file = new std::fstream;
    settings_file->open("paths.pth",std::ios::out | std::ios::trunc);

    for(int i = 0; i < path_amount; i++)
    {
        if(i)
            (*settings_file) << std::endl;
        (*settings_file) << (model->item(i,0)->data(Qt::DisplayRole).toString()).toStdString();
    }
    settings_file->close();

    delete settings_file;

}

/**
 * @brief Sygnal emitowany przy zamykaniu okna
 *
 * Funkcja zapisuje wczytane sciezki
 * @param event
 */
void OptionWindow::closeEvent(QCloseEvent *event)
{
       Save_Paths();
       emit Window_Close();
       event->accept();
}
