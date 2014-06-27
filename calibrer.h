#ifndef CALIBRER_H
#define CALIBRER_H

#include <QDialog>
#include "point.h"
#include "screen.h"

class Screen;

namespace Ui {
class calibrer;
}

class calibrer : public QDialog
{
    Q_OBJECT

public:
    explicit calibrer(QWidget *parent = 0, Screen *screen = 0);
    ~calibrer();

private:
    Ui::calibrer *ui;
    Screen *screen;
private slots:
    void slide_change(int val);
    void on_pb_close_clicked();
};

#endif // CALIBRER_H
