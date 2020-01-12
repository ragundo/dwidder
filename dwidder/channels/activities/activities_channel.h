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

#ifndef ACTIVITIES_CHANNEL_H
#define ACTIVITIES_CHANNEL_H

#include <set>

#include "DataDefs.h"

class DwidderApp;

class activities_channel
{
  public:
    activities_channel(DwidderApp* p_parent);
    void do_work();
    void init();

  private:
    std::set<int32_t> m_id_set;

  private:
    DwidderApp* m_parent;
};

#endif