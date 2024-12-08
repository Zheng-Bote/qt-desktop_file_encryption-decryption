#pragma once

#include <QMainWindow>
class QWidget;
class QResizeEvent;
class QCloseEvent;

class Template : public QMainWindow
{
    Q_OBJECT

public:
    Template(QMainWindow *parent = 0);

private:
    QWidget *background;
};

#pragma once
