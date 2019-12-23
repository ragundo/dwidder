/*
 * Copyright 2019 Clement Vuchener
 *
 * This file is part of qtlabors plugin for DFHack
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

#include <QMainWindow>

#include <Core.h>
#include <QCloseEvent>
#include <QSortFilterProxyModel>
#include <QScopedPointer>

#include <memory>
#include <utility>
#include <vector>

    class EventProxy;
    class MainWindowPrivate;

    class MainWindow: public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(std::shared_ptr<EventProxy> &&proxy, QWidget *parent = nullptr);
        ~MainWindow() override;

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

    protected:
        void closeEvent(QCloseEvent* p_close_event) override;
    private:
        QScopedPointer<MainWindowPrivate> m_pimpl;
    };

#endif
