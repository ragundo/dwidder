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

#ifndef DWIDDERAPP_H
#define DWIDDERAPP_H

#include "EventProxy.h"
#include "announcements_channel.h"
#include "calendar_channel.h"
#include <Core.h>

class MainWindow;

class DwidderApp
{
  public:
    DwidderApp(MainWindow* p_parent, std::shared_ptr<EventProxy>&& p_proxy);

    void DF_suspend();
    void DF_resume();

    void tick();

    void addText(QString& p_string);
    int  get_cur_year_tick();

  protected:
    MainWindow*                            m_parent;
    std::shared_ptr<EventProxy>            m_event_proxy;
    std::unique_ptr<DFHack::CoreSuspender> m_core_suspender;
    bool                                   m_suspended;

    std::unique_ptr<calendar_channel>      m_calendar_channel;
    std::unique_ptr<announcements_channel> m_announcements_channel;

    int m_cur_year_tick;

    void init();
};

#endif
