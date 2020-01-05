/*
 * Copyright 202' Rafael Agundo
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

#include "calendar_channel.h"
#include "DwidderApp.h"
#include "MainWindow.h"
#include "dwidder_utils.h"

calendar_channel::calendar_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void calendar_channel::do_work()
{
    // Check for calendar changes
    QString calendar_event = process_calendar();
    if (calendar_event.size() > 0)
    {
        m_parent->addText(calendar_event);
    }
}

QString calendar_channel::process_calendar()
{
    bool l_new_month = false;
    bool l_new_year  = false;

    auto l_year   = *df::global::cur_year;
    auto l__month = floor((*df::global::cur_year_tick / 33600) + 1);
    auto l_day    = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;

    if (l_day != m_calendar_data.m_day)
    {
        if (l_day == 1)
        {
            // new month
            l_new_month = true;
        }
    }

    if (l_year != m_calendar_data.m_year)
    {
        // new year
        l_new_year = true;
    }

    m_calendar_data.m_day   = l_day;
    m_calendar_data.m_month = l__month;
    m_calendar_data.m_year  = l_year;

    if (l_new_year)
    {
        return DateAsString(&m_calendar_data) + " New Year!!";
    }
    else if (l_new_month)
    {
        return DateAsString(&m_calendar_data) + " New month!!";
    }
    return "";
}

void calendar_channel::update()
{
    m_calendar_data.m_year  = *df::global::cur_year;
    m_calendar_data.m_month = floor((*df::global::cur_year_tick / 33600) + 1);
    m_calendar_data.m_day   = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;
}