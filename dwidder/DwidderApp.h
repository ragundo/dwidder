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

#include "channels/activities/activities_channel.h"
#include "channels/announcements/announcements_channel.h"
#include "channels/calendar/calendar_channel.h"
#include "channels/crime/crime_channel.h"
#include "channels/incidents/incidents_channel.h"
#include "channels/mandates/mandates_channel.h"
#include "channels/weather/weather_channel.h"

#include <Core.h>

#include <QMap>

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
    int  get_dwarf_id_by_name_in_text(QString& p_dwarf_name);

  protected:
    MainWindow*                            m_parent;
    std::shared_ptr<EventProxy>            m_event_proxy;
    std::unique_ptr<DFHack::CoreSuspender> m_core_suspender;
    bool                                   m_suspended;

    std::unique_ptr<calendar_channel>      m_calendar_channel;
    std::unique_ptr<announcements_channel> m_announcements_channel;
    std::unique_ptr<weather_channel>       m_weather_channel;
    std::unique_ptr<mandates_channel>      m_mandates_channel;
    std::unique_ptr<crime_channel>         m_crime_channel;
    std::unique_ptr<activities_channel>    m_activity_channel;
    std::unique_ptr<incidents_channel>     m_incidents_channel;

    int m_cur_year_tick;

    QMap<QString, int> m_map_dwarf_name_2_id;

    void init();
};

#endif
