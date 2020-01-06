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

#include <memory>

#include "DwidderApp.h"
#include "MainWindow.h"
#include "channels/announcements/announcements_channel.h"
#include "channels/calendar/calendar_channel.h"

#include "DataDefs.h"
#include <df/global_objects.h>

DwidderApp::DwidderApp(MainWindow* p_parent, std::shared_ptr<EventProxy>&& p_proxy)
    : m_parent(p_parent),
      m_event_proxy(std::move(p_proxy)),
      m_core_suspender(nullptr),
      m_suspended(false)
{
    m_cur_year_tick         = -1;
    m_announcements_channel = std::make_unique<announcements_channel>(this);
    m_calendar_channel      = std::make_unique<calendar_channel>(this);
    m_weather_channel       = std::make_unique<weather_channel>(this);
}

void DwidderApp::init()
{
    m_calendar_channel->update();
    m_announcements_channel->init();
    m_weather_channel->init();
}

void DwidderApp::DF_suspend()
{
    // Check if the core suspender was created or not. If not
    // create it. Then suspend DF
    if (!m_core_suspender)
        m_core_suspender = std::make_unique<DFHack::CoreSuspender>();
    else
        m_core_suspender->lock();

    m_suspended = false;
}

void DwidderApp::DF_resume()
{
    m_core_suspender->unlock();
    m_suspended = false;
}

void DwidderApp::tick()
{
    DF_suspend();

    // First scan
    if (m_cur_year_tick == -1)
    {
        init();
    }

    // Normal scan
    // if the df tick is the same, the game will be probably paused
    if (m_cur_year_tick != *df::global::cur_year_tick)
    {
        //Update dwidder tick with th df one
        m_cur_year_tick = *df::global::cur_year_tick;

        // calendar channel
        m_calendar_channel->do_work();

        // announcements channel
        m_announcements_channel->do_work();

        // weather channel
        m_weather_channel->do_work();
    }

    DF_resume();
}

void DwidderApp::addText(QString& p_string)
{
    m_parent->addText(p_string);
}

int DwidderApp::get_cur_year_tick()
{
    return m_cur_year_tick;
}