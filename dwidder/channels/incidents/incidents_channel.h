/*
 * Copyright 2020 Rafael Agundo
 *
 * This file is part of  * This file is part of dwidder plugin for DFHack
 plugin for DFHack
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

#ifndef INCIDENTS_CHANNEL_H
#define INCIDENTS_CHANNEL_H

#include <utility>

#include <QString>

#include "DataDefs.h"
#include <df/incident.h>

class DwidderApp;

class incidents_channel
{
  public:
    incidents_channel(DwidderApp* p_parent);
    void do_work();
    void init();

  private:
    DwidderApp* m_parent;
    size_t      m_vector_size;
};

#endif