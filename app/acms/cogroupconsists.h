#ifndef COGROUPCONSISTS_H
#define COGROUPCONSISTS_H

#include <QWidget>

namespace Ui {
class CoGroupConsists;
}

class CoGroupConsists : public QWidget
{
    Q_OBJECT

public:
    explicit CoGroupConsists(QWidget *parent = nullptr);
    ~CoGroupConsists();

private:
    Ui::CoGroupConsists *ui;
};

#endif // COGROUPCONSISTS_H
