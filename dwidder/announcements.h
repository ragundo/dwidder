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

#ifndef ANNOUNCEMENTS_H
#define ANNOUNCEMENTS_H

#include <DataDefs.h>
#include <df/announcement_type.h>
#include <df/coord.h>
#include <df/report.h>

#include <QString>
#include <QStringList>

struct announcement_data
{
    df::announcement_type m_type;
    QString               m_text;
    df::report::T_flags   m_flags;
    df::coord             m_pos;
    int32_t               m_id;
    int32_t               m_year;
    int32_t               m_time;
    int                   m_unit_id;

    announcement_data(df::report* p_df_announcement);
};

#endif // ANNOUNCEMENTS_H
