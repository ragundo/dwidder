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

#ifndef DWIDDER_UTILS_H
#define DWIDDER_UTILS_H

#include <DataDefs.h>
#include <df/coord.h>

#include <QString>

struct calendar_data;

QString DateAsString(int p_year, int p_month, int p_day);
QString DateAsString(calendar_data* p_calendar_data);
QString GetDFDate();
QString coord_2_string(const df::coord& p_coord);

#endif // DWIDDER_UTILS_H
