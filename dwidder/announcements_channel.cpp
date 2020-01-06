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

#include "announcements_channel.h"
#include "DataDefs.h"
#include "announcements.h"
#include "dwidder_utils.h"
#include <df/global_objects.h>
#include <df/report.h>
#include <df/world.h>
#include <modules/Gui.h>

#include "DwidderApp.h"

announcements_channel::announcements_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void announcements_channel::init()
{
    // Copy all the announcements ids to the set
    for (int i = 0; i < (df::global::world)->status.announcements.size(); i++)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        m_processed_announcements.insert(l_report->id);
    }
}

void announcements_channel::do_work()
{
    // Check if there are new announcements
    int l_num_new_announcements = check_for_new_announcements();
    if (l_num_new_announcements > 0)
    {
        process_announcements(l_num_new_announcements);
    }
}

int announcements_channel::check_for_new_announcements()
{
    int l_result = 0;
    for (int i = (df::global::world)->status.announcements.size() - 1; i >= 0; i--)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        if (m_processed_announcements.find(l_report->id) != m_processed_announcements.end())
            break;
        l_result++;
        m_processed_announcements.insert(l_report->id);
    }
    return l_result;
}

bool announcements_channel::process_announcements(int p_num_new_announcements)
{
    std::vector<std::unique_ptr<announcement_data>> l_initial_data_vector;
    std::vector<std::unique_ptr<announcement_data>> l_final_data_vector;

    for (int i = (df::global::world)->status.announcements.size() - p_num_new_announcements, j = 0; i < (df::global::world)->status.announcements.size(); i++)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        auto        l_ptr    = std::make_unique<announcement_data>(l_report);
        l_initial_data_vector.push_back(std::move(l_ptr));
    }

    for (int i = l_initial_data_vector.size() - 1; i >= 0; i--)
    {
        announcement_data* l_data = l_initial_data_vector[i].get();
        if (l_data->m_flags.whole & 0x1)
        {
            announcement_data* l_data_prev = l_initial_data_vector[i - 1].get();
            l_data_prev->m_text            = l_data_prev->m_text + " " + l_data->m_text;
        }
    }

    for (int i = l_initial_data_vector.size() - 1; i >= 0; i--)
    {
        announcement_data* l_data = l_initial_data_vector[i].get();
        if (l_data->m_flags.whole & 0x1)
            continue;
        l_final_data_vector.push_back(std::move(l_initial_data_vector[i]));
    }

    for (int i = 0; i < l_final_data_vector.size(); i++)
    {
        announcement_data* l_data    = l_final_data_vector[i].get();
        QString            l_result  = process_announcement(l_data);
        QString            l_result2 = process_announcement(l_data);

        // New event
        m_parent->addText(l_result2);
    }
    return true;
}

QString announcements_channel::process_announcement(announcement_data* p_data)
{
    QString l_result = QString::number(m_parent->get_cur_year_tick()) + "/" + GetDFDate() + "-";
    QString l_pos    = coord_2_string(p_data->m_pos);

    // Center window
    DFHack::Gui::revealInDwarfmodeMap(p_data->m_pos, true);

    DFHack::Gui::setCursorCoords(p_data->m_pos.x,
                                 p_data->m_pos.y,
                                 p_data->m_pos.z);

    l_result.append(l_pos + " " + p_data->m_text);

    return l_result;
}
