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

#include "utils.h"
#include <cmath>
#include <memory>

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

struct announcement_data
{
    df::announcement_type m_type;
    QString               m_text;
    df::report::T_flags   m_flags;
    df::coord             m_pos;
    int32_t               m_id;
    int32_t               m_year;
    int32_t               m_time;

    announcement_data(df::report* p_df_announcement);
};

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

    int           m_cur_year_tick;
    std::set<int> m_processed_announcements;

    bool    process_announcements(int p_num_new_announcements);
    QString process_announcement(announcement_data& p_data);
    int     check_new_announcements();
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

void MainWindow::closeEvent(QCloseEvent* p_event)
{
    // Do the thing
    // quit();
    p_event->accept();
}

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

    // DF is suspended now
    m_pimpl->m_suspended = true;

    // First scan
    if (m_pimpl->m_cur_year_tick == -1)
    {
        // Copy all the announcements ids to the set
        for (int i = 0; i < (df::global::world)->status.announcements.size(); i++)
        {
            df::report* l_report = (df::global::world)->status.announcements[i];
            m_pimpl->m_processed_announcements.insert(l_report->id);
            //Update dwidder tick with th df one
            m_pimpl->m_cur_year_tick = *df::global::cur_year_tick;
        }
    }

    // Normal scan
    // if the df tick is the same, the game will be probably paused
    if (m_pimpl->m_cur_year_tick != *df::global::cur_year_tick)
    {
        //Update dwidder tick with th df one
        m_pimpl->m_cur_year_tick = *df::global::cur_year_tick;

        // Check if there are new announcements
        int l_new_announcements = m_pimpl->check_new_announcements();
        if (l_new_announcements > 0)
        {
            m_pimpl->process_announcements(l_new_announcements);
        }
    }

    m_pimpl->m_core_suspender->unlock();
    m_pimpl->m_suspended = false;
}

bool MainWindowPrivate::process_announcements(int p_num_new_announcements)
{
    std::vector<std::unique_ptr<announcement_data>> l_initial_data_vector;
    std::vector<std::unique_ptr<announcement_data>> l_final_data_vector;

    for (int i = (df::global::world)->status.announcements.size() - p_num_new_announcements, j = 0; i < (df::global::world)->status.announcements.size(); i++)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        auto        ptr      = std::make_unique<announcement_data>(l_report);
        l_initial_data_vector.push_back(std::move(ptr));
    }

    for (int i = l_initial_data_vector.size() - 1; i >= 0; i--)
    {
        announcement_data* l_data = l_initial_data_vector[i].get();
        if (l_data->m_flags.whole & 0x1)
        {
            announcement_data* l_data_prev = l_initial_data_vector[i - 1].get();
            l_data_prev->m_text            = l_data_prev->m_text + " " + l_data->m_text;
        }
    }

    for (int i = l_initial_data_vector.size() - 1; i >= 0; i--)
    {
        announcement_data* l_data = l_initial_data_vector[i].get();
        if (l_data->m_flags.whole & 0x1)
            continue;
        l_final_data_vector.push_back(std::move(l_initial_data_vector[i]));
    }

    for (int i = 0; i < l_final_data_vector.size(); i++)
    {
        announcement_data* l_data   = l_final_data_vector[i].get();
        QString            l_result = process_announcement(*l_data);
        m_logger->appendPlainText(l_result);
    }

    return true;
}

QString MainWindowPrivate::process_announcement(announcement_data& p_data)
{
    QString l_result = QString::number(m_cur_year_tick) + "/" + GetDFDate() + "-";
    QString l_pos    = coord_2_string(p_data.m_pos);

    // Center window
    DFHack::Gui::revealInDwarfmodeMap(p_data.m_pos, true);

    DFHack::Gui::setCursorCoords(p_data.m_pos.x,
                                 p_data.m_pos.y,
                                 p_data.m_pos.z);

    l_result.append(l_pos + " " + p_data.m_text);

    return l_result;
}

int MainWindowPrivate::check_new_announcements()
{
    int l_result = 0;
    for (int i = (df::global::world)->status.announcements.size() - 1; i >= 0; i--)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        if (m_processed_announcements.find(l_report->id) != m_processed_announcements.end())
            break;
        l_result++;
        m_processed_announcements.insert(l_report->id);
    }
    return l_result;
}

announcement_data::announcement_data(df::report* p_df_announcement)
{
    m_type  = p_df_announcement->type;
    m_text  = DF2QT(p_df_announcement->text);
    m_flags = p_df_announcement->flags;
    m_pos   = p_df_announcement->pos;
    m_id    = p_df_announcement->id;
    m_year  = p_df_announcement->year;
    m_time  = p_df_announcement->time;
}
