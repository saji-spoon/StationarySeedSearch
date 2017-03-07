#pragma once

#include <QtWidgets/QWidget>
#include<QTimer>
#include<iostream>
#include "ui_stationaryseedsearch.h"
#include "../DB-CUI/db-create.hpp"

class StationarySeedSearch : public QWidget
{
    Q_OBJECT

public:
    StationarySeedSearch(QWidget *parent = Q_NULLPTR);

private:
    Ui::StationarySeedSearchClass ui;
    DBCreator m_dbc;
    enum createState 
    {
        Init,
        Creating,
        Aborting,
    } m_state;

    void changeCreateButtonText(createState st)
    {
        switch (st)
        {
        case StationarySeedSearch::Creating: 
        case StationarySeedSearch::Aborting:
            ui.createButton->setText("Abort");
            break;
        default:
            ui.createButton->setText("Create DB");
            break;
        }
    }

private slots:
    void on_createButton_clicked();
    void on_sortButton_clicked();
    void update_createProcess();
};
