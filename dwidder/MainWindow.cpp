/*
 * Copyright 2020 Rafael Agundo
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

#include "MainWindow.h"
#include "DwidderApp.h"

static constexpr struct in_place_t
{
} in_place;

//
//---------------------------------------------------------------------------------------
//
MainWindow::MainWindow(std::shared_ptr<EventProxy>&& p_proxy, QWidget* p_parent)
    : QMainWindow(p_parent), m_app(new DwidderApp(this, std::move(p_proxy)))
{
    m_logger = new QPlainTextEdit(this);
    m_timer  = new QTimer(this);
    m_logger->setReadOnly(true);

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

    connect(m_timer, &QTimer::timeout, this, &MainWindow::tick);

    setCentralWidget(m_logger);
    m_timer->start(1000);
}

//
//---------------------------------------------------------------------------------------
//
MainWindow::~MainWindow()
{
    m_app->DF_resume();
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
 * @brief Called each second
 *
 */
void MainWindow::tick()
{
    m_app->tick();
}
