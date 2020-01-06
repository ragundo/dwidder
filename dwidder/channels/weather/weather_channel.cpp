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

#include "weather_channel.h"
#include <utility>

#include "../../DwidderApp.h"

std::pair<bool, bool> get_current_weather()
{
    bool l_is_raining = false;
    bool l_is_snowing = false;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if ((*(df::global::current_weather))[i][j] == df::enums::weather_type::Rain)
            {
                l_is_raining = true;
                break;
            }
            if ((*(df::global::current_weather))[i][j] == df::enums::weather_type::Snow)
            {
                l_is_snowing = true;
                break;
            }
        }
        if (l_is_raining || l_is_snowing)
            break;
    }

    std::pair<bool, bool> l_result;
    l_result.first  = l_is_raining;
    l_result.second = l_is_snowing;

    return l_result;
}

weather_channel::weather_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void weather_channel::init()
{
    auto l_raining_snowing = get_current_weather();
    m_is_raining           = l_raining_snowing.first;
    m_is_snowing           = l_raining_snowing.second;
}

void weather_channel::do_work()
{
    auto l_raining_snowing = get_current_weather();
    bool l_is_raining      = l_raining_snowing.first;
    bool l_is_snowing      = l_raining_snowing.second;

    if (m_is_raining)
    {
        if (!l_is_raining)
        {
            // no more rain
            QString l_event = "The weather has cleared";
            m_parent->addText(l_event);
        }
    }
    else
    {
        if (l_is_raining)
        {
            // rain
            QString l_event = "It has started raining";
            m_parent->addText(l_event);
        }
    }

    if (m_is_snowing)
    {
        if (!l_is_snowing)
        {
            QString l_event = "The weather has cleared";
            m_parent->addText(l_event);
        }
    }
    else
    {
        if (l_is_snowing)
        {
            // snow
            QString l_event = "It has started snowing";
            m_parent->addText(l_event);
        }
    }
    m_is_raining = l_is_raining;
    m_is_snowing = l_is_snowing;
}
