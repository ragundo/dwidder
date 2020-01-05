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

#include "dwidder_utils.h"

// #include <Console.h>
// #include <Core.h>
// #include <DataDefs.h>
// #include <Export.h>
// #include <PluginManager.h>
// #include <RemoteClient.h>
// #include <RemoteServer.h>
// #include <VersionInfo.h>

#include "calendar.h"

QString DateAsString(int p_year, int p_month, int p_day)
{
    QString dfDateString = QString::number(p_year);
    dfDateString         = dfDateString.append('-');
    dfDateString         = dfDateString.append(QString::number(p_month));
    dfDateString         = dfDateString.append('-');
    dfDateString         = dfDateString.append(QString::number(p_day));
    return dfDateString;
}

QString DateAsString(calendar_data* p_calendar_data)
{
    return DateAsString(p_calendar_data->m_year,
                        p_calendar_data->m_month,
                        p_calendar_data->m_day);
}

/**
 * @brief Handle the formatting of the Dwarf Fortress date
 *
 * Thanks to Kurik Amudnil for the date stuff
 * http://www.bay12forums.com/smf/index.php?PHPSESSID=669fc6cc7664043c4b34992a301abb0c&topic=91166.msg4247785#msg4247785
 * @return QString with the date inf format YYYY/M/D
 */
QString GetDFDate()
{
    //    -- Would it be useful to return a part of the DF date?
    //    -- local absTick = 1200*28*12*df.global.cur_year +
    //    df.global.cur_year_tick
    int32_t dfYear  = *df::global::cur_year;
    int32_t dfMonth = floor((*df::global::cur_year_tick / 33600) + 1);
    int32_t dfDay   = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;

    return DateAsString(dfYear, dfMonth, dfDay);
}

/**
 * @brief  Convert a df.coord (x,y,z) to a string
 *
 * @param p_coord df.coord object
 * @return QString string representation of the object
 */
QString coord_2_string(const df::coord& p_coord)
{
    QString l_result = "[";
    l_result.append(QString::number(p_coord.x));
    l_result.append(",");
    l_result.append(QString::number(p_coord.y));
    l_result.append(",");
    l_result.append(QString::number(p_coord.z));
    l_result.append(",");
    l_result.append("]");

    return l_result;
}
