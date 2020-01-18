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

#ifndef DWIDDER_UTILS_H
#define DWIDDER_UTILS_H

#include <DataDefs.h>
#include <df/coord.h>

#include <QString>

struct calendar_data;
namespace df
{
struct unit;
struct world_site;
} // namespace df

QString   DateAsString(int p_year, int p_month, int p_day);
QString   DateAsString(calendar_data* p_calendar_data);
QString   GetDFDate();
QString   coord_2_string(const df::coord& p_coord);
df::unit* getUnitById(int32_t p_id);
QString   getUnitName(int32_t p_id);
QString   getUnitName(df::unit* p_unit);
QString   getUnitRace(int32_t p_id);
QString   getUnitRace(df::unit* p_unit);

QString getItemDesciption(df::item_type p_item);

QString getSiteName(int32_t p_id);
QString getSiteName(df::world_site* p_site);

void revealInMap(df::coord& p_pos);

#endif // DWIDDER_UTILS_H
