#pragma once

#include <QtWidgets/QWidget>
#include<QTimer>
#include<iostream>
#include "ui_stationaryseedsearch.h"
#include "../DB-CUI/db-create.hpp"
#include "../DB-CUI/db-sort.hpp"

class StationarySeedSearch : public QWidget
{
    Q_OBJECT

public:
    StationarySeedSearch(QWidget *parent = Q_NULLPTR);

private:
    Ui::StationarySeedSearchClass ui;
    DBCreator m_dbc;
    DBSort m_dbs;
    enum class createState 
    {
        Init,
        Creating,
        Aborting,
    } m_cState;

    enum class sortState 
    {
        Init,
        Creating
    } m_sState;

    void changeCreateButtonText(createState st)
    {
        switch (st)
        {
        case StationarySeedSearch::createState::Creating: 
        case StationarySeedSearch::createState::Aborting:
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
    void update_sortProcess();
};
