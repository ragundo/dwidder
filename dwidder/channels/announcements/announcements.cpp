/*
 * Copyright 2020 Rafael Agundo
 *
 * This file is part of dwidder plugin for DFHack
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

#include "../include/utils.h"

#include "announcements.h"
#include <df/coord2d.h>
#include <df/language_name.h>
#include <df/report.h>

announcement_data::announcement_data(df::report* p_df_announcement)
{
    m_type     = p_df_announcement->type;
    m_text     = DF2QT(p_df_announcement->text);
    m_flags    = p_df_announcement->flags;
    m_pos      = p_df_announcement->pos;
    m_id       = p_df_announcement->id;
    m_year     = p_df_announcement->year;
    m_time     = p_df_announcement->time;
    m_has_unit = false;
    m_unit_id  = -1;
}

std::optional<QString> announcement_data::check_for_unit_name()
{
    switch (m_type)
    {
        case df::announcement_type::MASTERFUL_IMPROVEMENT:
        case df::announcement_type::MASTERPIECE_CRAFTED:
        case df::announcement_type::RECRUIT_PROMOTED:
        case df::announcement_type::CITIZEN_BECOMES_NONSOLDIER:
        case df::announcement_type::CITIZEN_BECOMES_SOLDIER:
        case df::announcement_type::ITEM_ATTACHMENT: //comma
        case df::announcement_type::STRESSED_CITIZEN:
        case df::announcement_type::SOLDIER_BECOMES_MASTER:
        case df::announcement_type::NEW_MANDATE: // TODO deberia no procesare puesto que lo hace en mandates_channel
        case df::announcement_type::CANCEL_JOB:
        case df::announcement_type::GUEST_ARRIVAL:
        case df::announcement_type::PROFESSION_CHANGES:
            return get_dwarf_name(0);
        case df::announcement_type::ARTWORK_DEFACED:
            return get_dwarf_name(1);
        default: // SEASON_DRY SEASON_AUTUMN
            return {};
    }
}

std::optional<QString> announcement_data::get_dwarf_name(int p_mode)
{
    if (p_mode == 0) // Vucar Zsaedk. spearman has ....
    {
        QStringList l_split = m_text.split(" ");
        if (l_split.size() >= 2)
        {
            QString l_unit_name = l_split.at(0) + " " + l_split.at(1);

            // Remove end comma after name, if any
            QStringList l_no_commas = l_unit_name.split(",");
            return l_no_commas.at(0);
        }
        return {};
    }
    if (p_mode == 1) // A masterwork of Vucar Zasidistan has been lost!
    {
        QStringList l_split = m_text.split(" ");
        if (l_split.size() >= 4)
        {
            QString l_unit_name = l_split.at(3) + " " + l_split.at(4);

            // Remove end comma after name, if any
            QStringList l_no_commas = l_unit_name.split(",");
            return l_no_commas.at(0);
        }
        return {};
    }
    return {};
}