#include "stationaryseedsearch.h"

StationarySeedSearch::StationarySeedSearch(QWidget *parent)
    : QWidget(parent), m_state(Init)
{
    ui.setupUi(this);

    QTimer* createUpdateTimer = new QTimer(this);
    connect(createUpdateTimer, SIGNAL(timeout()), this, SLOT(update_createProcess()));
    createUpdateTimer->start();
}

void StationarySeedSearch::on_createButton_clicked() 
{
    const DBCreator::State threadState = m_dbc.getState();

    switch (m_state)
    {
    case StationarySeedSearch::Init:
    {
        m_dbc = DBCreator{};

        std::thread th(std::ref(m_dbc));
        th.detach();

        m_state = Creating;
        changeCreateButtonText(m_state);

        break;
    }
    case StationarySeedSearch::Creating:
        ui.createStatusLabel->setText("Aborting...");
        m_dbc.abort();
        m_state = Aborting;
        break;
    case StationarySeedSearch::Aborting:
        //During aborting, button is unavailable.
        break;
    default:
        break;
    }

}
void StationarySeedSearch::on_sortButton_clicked(){}

void StationarySeedSearch::update_createProcess() 
{
    switch (m_state)
    {
    case StationarySeedSearch::Init:
        break;
    case StationarySeedSearch::Creating:
        ui.createStatusLabel->setText(QString::number(m_dbc.getProgress()*100.0)+"%...");

        if (m_dbc.getState() == DBCreator::Finished) 
        {
            ui.createStatusLabel->setText("Completed.");
            m_state = Init;
            changeCreateButtonText(m_state);
        }
        else if (m_dbc.getState() == DBCreator::Error && !m_dbc.isAborted()) 
        {
            ui.createStatusLabel->setText(QString::fromStdString(m_dbc.getErrString()));

            m_state = Init;
            changeCreateButtonText(m_state);
        }
        break;
    case StationarySeedSearch::Aborting:
        if (m_dbc.isAborted()) 
        {
            ui.createStatusLabel->setText(QString::fromStdString(m_dbc.getErrString()));
            m_state = Init;
            changeCreateButtonText(m_state);
        }
        break;
    default:
        break;
    }
}
