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

#ifndef DWARFEXPLORER_MAIN_WINDOW_H
#define DWARFEXPLORER_MAIN_WINDOW_H

#include <memory>

#include <QMainWindow>

#include <QCloseEvent>
#include <QDirModel>
#include <QSettings>

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QScopedPointer>
#include <QTimer>
#include <QUrl>

#include "EventProxy.h"

class DwidderApp;

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(std::shared_ptr<EventProxy>&& proxy,
                        QWidget*                      parent = nullptr);
    ~MainWindow() override;

  public:
    void addText(QString& p_text)
    {
        m_logger->appendPlainText(p_text);
    }

  protected:
    void closeEvent(QCloseEvent* p_close_event) override;

  private:
    QPlainTextEdit*            m_logger;
    QTimer*                    m_timer;
    QScopedPointer<DwidderApp> m_app;

  signals:
    void resumed_signal();

  private slots:
    //! Open GIF.
    void openGif();
    //! Save GIF.
    void saveGif();
    //! Save GIF as.
    void saveGifAs();
    //! Quit.
    void quit();

    void tick();
};

#endif
