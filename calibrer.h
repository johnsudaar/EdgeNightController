#ifndef CALIBRER_H
#define CALIBRER_H

#include <QDialog>
#include "parameters.h"
#include <QSettings>
#include <QFile>

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
    bool loading;
    Ui::calibrer *ui;
    Screen *screen;
    void update();
    void saveSettings();
    void loadSettings();
private slots:
    void size_change(int val);
    void offset_change(int val);
    void other_change(int val);
    void on_pb_close_clicked();
    void on_pb_save_clicked();
    void on_pb_load_clicked();
};

#endif // CALIBRER_H
