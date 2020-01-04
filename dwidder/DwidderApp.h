#ifndef DWIDDERAPP_H
#define DWIDDERAPP_H

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
//#include <PluginManager.h>
//#include <RemoteClient.h>
//#include <RemoteServer.h>
//#include <VersionInfo.h>

#include <df/coord.h>
#include <df/coord2d.h>
#include <df/language_name.h>
#include <df/report.h>
#include <df/unit.h>

#include <cstdint>

#include "EventProxy.h"

#include "../include/utils.h"
#include <cmath>
#include <memory>

#include <modules/Gui.h>
#include <modules/Translation.h>

#include "MainWindow.h"

struct announcement_data
{
    df::announcement_type m_type;
    QString               m_text;
    df::report::T_flags   m_flags;
    df::coord             m_pos;
    int32_t               m_id;
    int32_t               m_year;
    int32_t               m_time;
    int32_t               m_unit_id;

    announcement_data(df::report* p_df_announcement);
};

struct calendar_data
{
    int m_year;
    int m_month;
    int m_day;

    void update();
};

class DwidderApp
{
  public:
    DwidderApp(MainWindow* p_parent, std::shared_ptr<EventProxy>&& p_proxy);

    MainWindow*                 m_parent;
    std::shared_ptr<EventProxy> m_event_proxy;
    DFHack::CoreSuspender*      m_core_suspender;
    bool                        m_suspended;
    calendar_data               m_calendar_data;

    int           m_cur_year_tick;
    std::set<int> m_processed_announcements;

    int     check_for_new_announcements();
    bool    process_announcements(int p_num_new_announcements);
    QString process_announcement(announcement_data& p_data);

    QString process_calendar();

    void tick();
};

#endif