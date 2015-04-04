#ifndef CALIBRER_H
#define CALIBRER_H

#include <QDialog>
#include "parameters.h"

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
    void update();
private slots:
    void size_change(int val);
    void offset_change(int val);
    void other_change(int val);
    void on_pb_close_clicked();
};

#endif // CALIBRER_H
