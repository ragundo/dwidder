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
#ifndef MANDATES_CHANNEL_H
#define MANDATES_CHANNEL_H

#include <map>
#include <memory>
#include <utility>

#include <QString>

#include "DataDefs.h"
#include <df/item_type.h>
#include <df/mandate.h>

class DwidderApp;

struct mandate
{
    int32_t                   m_unit_id;
    df::mandate::T_mode       m_mode;
    df::item_type             m_item_type;
    int16_t                   m_item_subtype;
    int16_t                   m_mat_type;
    int32_t                   m_mat_index;
    int16_t                   m_amount_total;
    int16_t                   m_amount_remaining;
    int32_t                   m_timeout_counter; /*!< counts once per 10 frames */
    int32_t                   m_timeout_limit; /*!< once counter passes limit, mandate ends */
    df::mandate::T_punishment m_punishment;
    uint8_t                   m_punish_multiple;

    mandate(df::mandate* p_df_mandate);
};

class mandates_channel
{
  public:
    mandates_channel(DwidderApp* p_parent);
    void do_work();
    void init();

  private:
    std::multimap<QString, mandate> m_processed_mandates;

  private:
    DwidderApp* m_parent;
    size_t      m_vector_size;
};

#endif