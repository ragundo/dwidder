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

#ifndef ANNOUNCEMENTS_CHANNEL_H
#define ANNOUNCEMENTS_CHANNEL_H

#include <set>

#include <QString>

struct announcement_data;
class DwidderApp;

class announcements_channel
{
  public:
    announcements_channel(DwidderApp* p_parent);
    void do_work();
    void init();

  private:
    std::set<int> m_processed_announcements;

  private:
    int         check_for_new_announcements();
    bool        process_announcements(int p_num_new_announcements);
    QString     process_announcement(announcement_data* p_data);
    DwidderApp* m_parent;
};

#endif