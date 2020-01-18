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

#ifndef ITEMS_CHANNEL_H
#define ITEMS_CHANNEL_H

#include <map>
#include <set>

#include "DataDefs.h"

#include <df/item_type.h>

class DwidderApp;

class items_channel
{
  public:
    items_channel(DwidderApp* p_parent);
    void do_work();
    void init();

  private:
    std::multimap<df::enums::item_type::item_type, int32_t> m_processed_items;
    std::set<int32_t>                                       m_in_construction_items;

  private:
    DwidderApp* m_parent;
    size_t      m_known_items;
};

#endif