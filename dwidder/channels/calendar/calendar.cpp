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

#include "calendar.h"

#include <Console.h>
#include <Core.h>
#include <DataDefs.h>
#include <Export.h>
#include <PluginManager.h>
#include <RemoteClient.h>
#include <RemoteServer.h>
#include <VersionInfo.h>
#include <cstdint>

/*
DF Calendar Months
01: Granite
02: Slate
03: Felsite
04: Hematite
05: Malachite
06: Galena
07: Limestone
08: Sandstone
09: Timber
10: Moonstone
11: Opal
12: Obsidian

Each month has 28 days
*/
