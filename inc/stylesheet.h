#ifndef STYLESHEETS_H
#define STYLESHEETS_H
#include <QString>


#define PLAYBUTTON_STYLESHEET    "QPushButton {\
                                  background-color: transparent;\
                                  image: url(:/play.png);\
                                  background-repeat: none;    \
                                  border: none;\
                                  };"



#define PAUSEBUTTON_STYLESHEET   "QPushButton {\
                                  background-color: transparent;\
                                  image: url(:/pause.png);\
                                  background-repeat: none;    \
                                  border: none;\
                                  }"



#define NEXTBUTTON_STYLESHEET    "QPushButton {\
                                  background-color: transparent;\
                                  image: url(:/next.png);\
                                  background-repeat: none;    \
                                  border: none;\
                                  }"



#define PREVBUTTON_STYLESHEET    "QPushButton {\
                                  background-color: transparent;\
                                  image: url(:/prev.png);\
                                  background-repeat: none;    \
                                  border: none;\
                                  }"



#define PAGEBUTTON_STYLESHEET    "QPushButton {\
                                  background-color: transparent;\
                                  image: url(:/page1.png);\
                                  background-repeat: none;    \
                                  border: none;\
                                  font-size: 14px;\
                                  Text-align:left;\
                                  }\
                                  QPushButton:checked {\
                                  background-color: transparent;\
                                  image: url(:/page1p.png);\
                                  background-repeat: none;    \
                                  border: none;\
                                  }"



#define TIMESLIDER_STYLESHEET    "QSlider::groove:horizontal {\
                                  border: 1px solid #bbb;\
                                  background: white;\
                                  height: 10px;\
                                  border-radius: 4px;\
                                  }\
                                  \
                                  QSlider::sub-page:horizontal {\
                                  background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,\
                                  stop: 0 #66e, stop: 1 #bbf);\
                                  background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\
                                  stop: 0 #bbf, stop: 1 #55f);\
                                  border: 1px solid #777;\
                                  height: 10px;\
                                  border-radius: 4px;\
                                  }\
                                  \
                                  QSlider::add-page:horizontal {\
                                  background: #fff;\
                                  border: 1px solid #777;\
                                  height: 10px;\
                                  border-radius: 4px;\
                                  }\
                                  \
                                  QSlider::handle:horizontal {\
                                  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\
                                  stop:0 #eee, stop:1 #ccc);\
                                  border: 1px solid #777;\
                                  width: 13px;\
                                  margin-top: -2px;\
                                  margin-bottom: -2px;\
                                  border-radius: 4px;\
                                  }\
                                  \
                                  QSlider::handle:horizontal:hover {\
                                  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\
                                  stop:0 #fff, stop:1 #ddd);\
                                  border: 1px solid #444;\
                                  border-radius: 4px;\
                                  }\
                                  \
                                  QSlider::sub-page:horizontal:disabled {\
                                  background: #bbb;\
                                  border-color: #999;\
                                  }\
                                  \
                                  QSlider::add-page:horizontal:disabled {\
                                  background: #eee;\
                                  border-color: #999;\
                                  }\
                                  \
                                  QSlider::handle:horizontal:disabled {\
                                  background: #eee;\
                                  border: 1px solid #aaa;\
                                  border-radius: 4px;\
                                  }"
#endif // STYLESHEETS_H
