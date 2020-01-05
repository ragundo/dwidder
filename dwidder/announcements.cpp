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

#include "../include/utils.h"

#include "announcements.h"
#include <df/coord2d.h>
#include <df/language_name.h>
#include <df/report.h>
#include <df/unit.h>
#include <df/world.h>

#include <modules/Translation.h>

announcement_data::announcement_data(df::report* p_df_announcement)
{
    m_type    = p_df_announcement->type;
    m_text    = DF2QT(p_df_announcement->text);
    m_flags   = p_df_announcement->flags;
    m_pos     = p_df_announcement->pos;
    m_id      = p_df_announcement->id;
    m_year    = p_df_announcement->year;
    m_time    = p_df_announcement->time;
    m_unit_id = -1;

    bool process = false;
    switch (m_type)
    {
        case df::announcement_type::MASTERFUL_IMPROVEMENT:
        case df::announcement_type::RECRUIT_PROMOTED:
        case df::announcement_type::CITIZEN_BECOMES_NONSOLDIER:
        case df::announcement_type::CITIZEN_BECOMES_SOLDIER:
        case df::announcement_type::ITEM_ATTACHMENT: //comma
        case df::announcement_type::STRESSED_CITIZEN:
        case df::announcement_type::SOLDIER_BECOMES_MASTER:
        case df::announcement_type::NEW_MANDATE:
        case df::announcement_type::CANCEL_JOB:
            process = true;
        default: // SEASON_DRY SEASON_AUTUMN
            break;
    }

    if (process)
    {
        QStringList l_split = m_text.split(" ");
        if (l_split.size() >= 2)
        {
            QString l_unit_name = l_split.at(0) + " " + l_split.at(1);

            for (int i = 0; i < (df::global::world)->units.active.size(); i++)
            {
                df::unit*          l_unit          = (df::global::world)->units.active[i];
                df::language_name* l_lang          = &(l_unit->name);
                std::string        l_unit_name_std = DFHack::Translation::TranslateName(l_lang, false, false);
                QString            l_name          = QString::fromStdString(DF2UTF(l_unit_name_std));
                if (l_name == l_unit_name)
                {
                    m_unit_id = l_unit->id;
                    break;
                }
            }
        }
    }
}
