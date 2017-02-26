#pragma once

#include <QtWidgets/QWidget>
#include "ui_stationaryseedsearch.h"

class StationarySeedSearch : public QWidget
{
    Q_OBJECT

public:
    StationarySeedSearch(QWidget *parent = Q_NULLPTR);

private:
    Ui::StationarySeedSearchClass ui;

private slots:
    void on_createButton_clicked();
    void on_sortButton_clicked();
};
