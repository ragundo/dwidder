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



#include <cstdint>
#include <DataDefs.h>
#include <Core.h>
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>
#include <RemoteServer.h>
#include <RemoteClient.h>
#include <VersionInfo.h>
#include "df_all.h"



#include "MainWindow.h"


#include <Core.h>
#include <QSettings>
#include <QDirModel>
#include "EventProxy.h"

#include <QDebug>
#include <QUrl>
#include <QMenuBar>
#include <QIcon>
#include <QApplication>
#include <QPlainTextEdit>
#include <QTimer>


static constexpr struct in_place_t {} in_place;

class MainWindowPrivate
{
public:
    MainWindowPrivate(MainWindow* p_parent,
                      std::shared_ptr<EventProxy> &&p_proxy
                     )
    : m_parent(p_parent)
    , m_event_proxy(std::move(p_proxy))
    , m_core_suspender(nullptr)
    , m_suspended(false)
    {
        m_logger = new QPlainTextEdit(m_parent);
        m_logger->setReadOnly(true);

        m_timer = new QTimer(m_parent);
    }

    MainWindow*                       m_parent;
    std::shared_ptr<EventProxy>       m_event_proxy;
    DFHack::CoreSuspender*            m_core_suspender;
    bool                              m_suspended;
    // Qt
    QPlainTextEdit*                   m_logger;
    QTimer*                           m_timer;

};

//
//---------------------------------------------------------------------------------------
//
MainWindow::MainWindow(std::shared_ptr<EventProxy> &&p_proxy, QWidget* p_parent)
    : QMainWindow(p_parent)
    , m_pimpl(new MainWindowPrivate(this, std::move(p_proxy)))
{
    setWindowTitle(tr("Dwidder"));
    auto file = menuBar()->addMenu( tr( "&File" ) );

    file->addAction(QIcon(":/img/document-open.png" )
                    , tr( "Open" )
                    , this
                    , &MainWindow::openGif
                    , tr( "Ctrl+O" )
                    );

    file->addSeparator();

    file->addAction(QIcon(":/img/document-save.png" )
                    , tr( "Save" )
                    , this
                    , &MainWindow::saveGif
                    , tr( "Ctrl+S" )
                    );

    file->addAction(QIcon(":/img/document-save-as.png" )
                    , tr( "Save As" )
                    , this
                    , &MainWindow::saveGifAs
                    );

    file->addSeparator();

    file->addAction(QIcon(":/img/application-exit.png")
                    , tr( "Quit" )
                    , this
                    , &MainWindow::quit
                    , tr( "Ctrl+Q" )
                    );

    connect(m_pimpl->m_timer
                     , &QTimer::timeout
                     , this
                     , &MainWindow::tick
                     );

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
    //quit();
    p_event->accept();
}

//void MainWindow::resumed_signal()
//{
//}

void MainWindow::openGif()
{}

void MainWindow::saveGif()
{}

void MainWindow::saveGifAs()
{}

void MainWindow::tick()
{
    if (m_pimpl->m_core_suspender == nullptr)
        m_pimpl->m_core_suspender = new DFHack::CoreSuspender;
    else
        m_pimpl->m_core_suspender->lock();

    m_pimpl->m_suspended = true;

    m_pimpl->m_logger->appendPlainText("tick");

    m_pimpl->m_core_suspender->unlock();
    m_pimpl->m_suspended = false;
}