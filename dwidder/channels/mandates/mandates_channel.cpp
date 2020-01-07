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

#include "mandates_channel.h"

#include <df/global_objects.h>
#include <df/world.h>

#include "../../DwidderApp.h"
#include "../../dwidder_utils.h"

#include <modules/Gui.h>
#include <modules/Items.h>
#include <modules/Units.h>

mandates_channel::mandates_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void mandates_channel::do_work()
{
    if ((df::global::world)->mandates.size() != m_vector_size)
    {
        QString l_text("new mandate");
        m_parent->addText(l_text);
    }
}

void mandates_channel::init()
{
    for (size_t i = 0; i < (df::global::world)->mandates.size(); i++)
    {
        df::mandate* l_df_mandate = (df::global::world)->mandates[i];
        auto         l_mandate    = mandate(l_df_mandate);
        // Get the noble for this mandate
        QString pepe        = "pepe";
        auto    l_key_value = std::pair<QString, mandate>(pepe, l_mandate);
        m_processed_mandates.insert(l_key_value);

        m_vector_size = (df::global::world)->mandates.size();
    }
}

mandate::mandate(df::mandate* p_df_mandate)
{
    m_unit_id          = p_df_mandate->unit->id;
    m_mode             = p_df_mandate->mode;
    m_item_type        = p_df_mandate->item_type;
    m_item_subtype     = p_df_mandate->item_subtype;
    m_mat_type         = p_df_mandate->mat_type;
    m_mat_index        = p_df_mandate->mat_index;
    m_amount_total     = p_df_mandate->amount_total;
    m_amount_remaining = p_df_mandate->amount_remaining;
    m_timeout_counter  = p_df_mandate->timeout_counter;
    m_timeout_limit    = p_df_mandate->timeout_limit;
    m_punishment       = p_df_mandate->punishment;
    m_punish_multiple  = p_df_mandate->punish_multiple;
}
