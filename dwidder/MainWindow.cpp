/*
 * Copyright 2019 Rafael Agundo
 *
 * This file is part of dwarfexplorer plugin for DFHack
 * The code is based on Clement Vuchener qtlabors plugin for DFHack
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <Console.h>
#include <Core.h>
#include <DataDefs.h>
#include <Export.h>
#include <PluginManager.h>
#include <RemoteClient.h>
#include <RemoteServer.h>
#include <VersionInfo.h>
#include <cstdint>

#include <df/report.h>
#include <df/world.h>

#include <modules/Gui.h>

#include "MainWindow.h"

#include "EventProxy.h"
#include <Core.h>
#include <QDirModel>
#include <QSettings>

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QTimer>
#include <QUrl>

#include <cmath>

/*
DF Calendar Months
01: Granite
02: Slate
03: Felsite
04: Hematite
05: Malachite
06: Galena
07: Limestone
08: Sandstone
09: Timber
10: Moonstone
11: Opal
12: Obsidian

Each month has 28 days
*/

static constexpr struct in_place_t
{
} in_place;

class MainWindowPrivate
{
  public:
    MainWindowPrivate(MainWindow* p_parent, std::shared_ptr<EventProxy>&& p_proxy)
        : m_parent(p_parent), m_event_proxy(std::move(p_proxy)), m_core_suspender(nullptr),
          m_suspended(false)
    {
        m_logger = new QPlainTextEdit(m_parent);
        m_logger->setReadOnly(true);

        m_timer         = new QTimer(m_parent);
        m_cur_year_tick = -1;
    }

    MainWindow*                 m_parent;
    std::shared_ptr<EventProxy> m_event_proxy;
    DFHack::CoreSuspender*      m_core_suspender;
    bool                        m_suspended;
    // Qt
    QPlainTextEdit* m_logger;
    QTimer*         m_timer;

    int m_cur_year_tick;
    int m_world_status_reports_size;
    int m_world_status_announcements_size;
};

//
//---------------------------------------------------------------------------------------
//
MainWindow::MainWindow(std::shared_ptr<EventProxy>&& p_proxy, QWidget* p_parent)
    : QMainWindow(p_parent), m_pimpl(new MainWindowPrivate(this, std::move(p_proxy)))
{
    setWindowTitle(tr("Dwidder"));
    auto file = menuBar()->addMenu(tr("&File"));

    file->addAction(
        QIcon(":/img/document-open.png"), tr("Open"), this, &MainWindow::openGif, tr("Ctrl+O"));

    file->addSeparator();

    file->addAction(
        QIcon(":/img/document-save.png"), tr("Save"), this, &MainWindow::saveGif, tr("Ctrl+S"));

    file->addAction(
        QIcon(":/img/document-save-as.png"), tr("Save As"), this, &MainWindow::saveGifAs);

    file->addSeparator();

    file->addAction(
        QIcon(":/img/application-exit.png"), tr("Quit"), this, &MainWindow::quit, tr("Ctrl+Q"));

    connect(m_pimpl->m_timer, &QTimer::timeout, this, &MainWindow::tick);

    setCentralWidget(m_pimpl->m_logger);
    m_pimpl->m_timer->start(1000);
}

//
//---------------------------------------------------------------------------------------
//
MainWindow::~MainWindow()
{
    if (m_pimpl->m_core_suspender)
    {
        m_pimpl->m_core_suspender->unlock();
        delete m_pimpl->m_core_suspender;
    }
}

void MainWindow::quit()
{
    QApplication::quit();
}

/*
//
//---------------------------------------------------------------------------------------
//
void MainWindow::on_suspend_action_triggered()
{
	if (m_core_suspender == nullptr)
		m_core_suspender = new DFHack::CoreSuspender;
	else
		m_core_suspender->lock();

	m_suspended = true;
}

//
//---------------------------------------------------------------------------------------
//
void MainWindow::on_resume_action_triggered()
{

}

//
//---------------------------------------------------------------------------------------
//
void MainWindow::on_treeView_expanded(const QModelIndex& p_index)
{
}

//
//---------------------------------------------------------------------------------------
//
void MainWindow::on_actionOpen_in_new_window_triggered()
{

}


//
//---------------------------------------------------------------------------------------
//
void MainWindow::on_actionOpen_in_hex_viewer_triggered()
{

}

//
//---------------------------------------------------------------------------------------
//
void MainWindow::on_actionOpenPointer_in_hex_viewer_triggered()
{

}

void MainWindow::on_filter_textChanged(const QString &arg1)
{
}
*/

void MainWindow::closeEvent(QCloseEvent* p_event)
{
    // Do the thing
    // quit();
    p_event->accept();
}

// void MainWindow::resumed_signal()
//{
//}

void MainWindow::openGif() {}

void MainWindow::saveGif() {}

void MainWindow::saveGifAs() {}

/**
 * @brief Handle the formatting of the Dwarf Fortress date
 *
 * Thanks to Kurik Amudnil for the date stuff
 * http://www.bay12forums.com/smf/index.php?PHPSESSID=669fc6cc7664043c4b34992a301abb0c&topic=91166.msg4247785#msg4247785
 * @return QString with the date inf format YYYY/M/D
 */
QString GetDFDate()
{
    //    -- Would it be useful to return a part of the DF date?
    //    -- local absTick = 1200*28*12*df.global.cur_year +
    //    df.global.cur_year_tick
    int32_t dfYear  = *df::global::cur_year;
    int32_t dfMonth = floor((*df::global::cur_year_tick / 33600) + 1);
    int32_t dfDay   = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;

    QString dfDateString = QString::number(dfYear);
    dfDateString         = dfDateString.append('-');
    dfDateString         = dfDateString.append(QString::number(dfMonth));
    dfDateString         = dfDateString.append('-');
    dfDateString         = dfDateString.append(QString::number(dfDay));
    return dfDateString;
}

/**
 * @brief  Convert a df.coord (x,y,z) to a string
 *
 * @param p_coord df.coord object
 * @return QString string representation of the object
 */
QString coord_2_string(const df::coord& p_coord)
{
    QString l_result = "[";
    l_result.append(QString::number(p_coord.x));
    l_result.append(",");
    l_result.append(QString::number(p_coord.y));
    l_result.append(",");
    l_result.append(QString::number(p_coord.z));
    l_result.append(",");
    l_result.append("]");

    return l_result;
}

/**
 * @brief Called each second
 *
 */
void MainWindow::tick()
{
    // Check if the core suspender was created or not. If not
    // create it. Then suspend DF
    if (m_pimpl->m_core_suspender == nullptr)
        m_pimpl->m_core_suspender = new DFHack::CoreSuspender;
    else
        m_pimpl->m_core_suspender->lock();

    // DF is suspended
    m_pimpl->m_suspended = true;

    // First scan
    if (m_pimpl->m_cur_year_tick == -1)
    {
        // Init vector sizes
        m_pimpl->m_world_status_reports_size       = (df::global::world)->status.reports.size();
        m_pimpl->m_world_status_announcements_size = (df::global::world)->status.announcements.size();
    }

    // Normal scan
    // if the df tick is the same, the game will be probably paused
    if (m_pimpl->m_cur_year_tick != *df::global::cur_year_tick)
    {
        //Update dwidder tick with th df one
        m_pimpl->m_cur_year_tick = *df::global::cur_year_tick;

        // If the dwidder vector size is different from the df one, we need to work
        if (m_pimpl->m_world_status_reports_size != (df::global::world)->status.reports.size())
        {
            // Check how many new items we have
            int         l_num_new_items = (df::global::world)->status.reports.size() - m_pimpl->m_world_status_reports_size;
            int         l_last_entry    = (df::global::world)->status.reports.size() - 1;
            df::report* l_report        = (df::global::world)->status.reports[l_last_entry];
            QString     l_pos           = coord_2_string(l_report->pos);
            QString     l_text          = QString::fromStdString(l_report->text);
            m_pimpl->m_logger->appendPlainText(l_pos + " " + l_text);
            DFHack::Gui::setViewCoords(l_report->pos.x, l_report->pos.y, l_report->pos.z);
            DFHack::Gui::setCursorCoords(l_report->pos.x, l_report->pos.y, l_report->pos.z);

            // Update the dwidder vector with the df one
            m_pimpl->m_world_status_reports_size = (df::global::world)->status.reports.size();
            m_pimpl->m_logger->appendPlainText(QString::number(m_pimpl->m_cur_year_tick) + "/" + GetDFDate());
        }

        if (m_pimpl->m_world_status_announcements_size != (df::global::world)->status.announcements.size())
        {
            // Check how many new items we have
            int l_num_new_items = (df::global::world)->status.announcements.size() - m_pimpl->m_world_status_announcements_size;
            int l_last_entry    = (df::global::world)->status.announcements.size() - 1;
            for (int i = m_pimpl->m_world_status_announcements_size; i <= l_last_entry; i++)
            {
                df::report* l_report = (df::global::world)->status.announcements[i];
                QString     l_pos    = coord_2_string(l_report->pos);
                QString     l_text   = QString::fromStdString(l_report->text);
                m_pimpl->m_logger->appendPlainText(l_pos + " " + l_text);
            }
            df::report* l_report = (df::global::world)->status.announcements[l_last_entry];
            // Center window
            DFHack::Gui::setViewCoords(l_report->pos.x,
                                       l_report->pos.y,
                                       l_report->pos.z);

            DFHack::Gui::setCursorCoords(l_report->pos.x,
                                         l_report->pos.y,
                                         l_report->pos.z);

            m_pimpl->m_world_status_announcements_size = (df::global::world)->status.announcements.size();
            m_pimpl->m_logger->appendPlainText(QString::number(m_pimpl->m_cur_year_tick) + "/" + GetDFDate());
        }
    }

    m_pimpl->m_core_suspender->unlock();
    m_pimpl->m_suspended = false;
}
