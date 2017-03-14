#include "stationaryseedsearch.h"

StationarySeedSearch::StationarySeedSearch(QWidget *parent)
    : QWidget(parent), m_cState(createState::Init), m_sState(sortState::Init)
{
    ui.setupUi(this);

    QTimer* createUpdateTimer = new QTimer(this);
    connect(createUpdateTimer, SIGNAL(timeout()), this, SLOT(update_createProcess()));
    connect(createUpdateTimer, SIGNAL(timeout()), this, SLOT(update_sortProcess()));
    createUpdateTimer->start();
}

void StationarySeedSearch::on_createButton_clicked() 
{
    const DBCreator::State threadState = m_dbc.getState();

    switch (m_cState)
    {
    case StationarySeedSearch::createState::Init:
    {
        m_dbc = DBCreator{};

        std::thread th(std::ref(m_dbc));
        th.detach();

        m_cState = createState::Creating;
        changeCreateButtonText(m_cState);

        ui.sortButton->setEnabled(false);

        break;
    }
    case StationarySeedSearch::createState::Creating:
        ui.createStatusLabel->setText("Aborting...");
        m_dbc.abort();
        m_cState = createState::Aborting;
        break;
    case StationarySeedSearch::createState::Aborting:
        //During aborting, button is unavailable.
        break;
    default:
        break;
    }

}
void StationarySeedSearch::on_sortButton_clicked()
{
    switch (m_sState)
    {
    case StationarySeedSearch::sortState::Init:
    {
        m_dbs = DBSort{};
        std::thread th(std::ref(m_dbs));
        th.detach();
        ui.createButton->setEnabled(false);
        m_sState = sortState::Creating;
        break;
    }
    case StationarySeedSearch::sortState::Creating:
        break;
    default:
        break;
    }
}

void StationarySeedSearch::update_createProcess() 
{
    switch (m_cState)
    {
    case StationarySeedSearch::createState::Init:
        ui.sortButton->setEnabled(true);
        break;
    case StationarySeedSearch::createState::Creating:
        ui.createStatusLabel->setText(QString::number(m_dbc.getProgress()*100.0)+"%...");

        if (m_dbc.getState() == DBCreator::Finished) 
        {
            ui.createStatusLabel->setText("Completed.");
            m_cState = createState::Init;
            changeCreateButtonText(m_cState);
        }
        else if (m_dbc.getState() == DBCreator::Error && !m_dbc.isAborted()) 
        {
            ui.createStatusLabel->setText(QString::fromStdString(m_dbc.getErrString()));

            m_cState = createState::Init;
            changeCreateButtonText(m_cState);
        }
        break;
    case StationarySeedSearch::createState::Aborting:
        if (m_dbc.isAborted()) 
        {
            ui.createStatusLabel->setText(QString::fromStdString(m_dbc.getErrString()));
            m_cState = createState::Init;
            changeCreateButtonText(m_cState);
        }
        break;
    default:
        break;
    }

}

void StationarySeedSearch::update_sortProcess()
{
    switch (m_sState) 
    {
    case StationarySeedSearch::sortState::Init:
        ui.createButton->setEnabled(true);
        break;
    case StationarySeedSearch::sortState::Creating:
        ui.sortStatusLabel->setText(QString::number(m_dbs.getProgress()*100.0) + "%...");
        if (m_dbs.getState() == DBSort::Finished)
        {
            ui.sortStatusLabel->setText("Completed.");
            m_sState = sortState::Init;
        }
        else if (m_dbs.getState() == DBSort::Error)
        {
            ui.sortStatusLabel->setText("Error.");
            m_sState = sortState::Init;
        }
        break;
    default:
        break;
    }
}