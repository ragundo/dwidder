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

#include "crime_channel.h"

#include <df/global_objects.h>
#include <df/ui.h>
#include <df/world.h>

#include "../../DwidderApp.h"
#include "../../dwidder_utils.h"
#include "modules/Units.h"

crime_channel::crime_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

QString crime_description(df::crime::T_mode p_crime_type)
{
    switch (p_crime_type)
    {
        case df::crime::ProductionOrderViolation:
            return "Production Order Violation";
        case df::crime::ExportViolation:
            return "Export Violation";
        case df::crime::JobOrderViolation:
            return "Job Order Violation";
        case df::crime::ConspiracyToSlowLabor:
            return "Conspiracy To Slow Labor";
        case df::crime::Murder:
            return "Murder";
        case df::crime::DisorderlyBehavior:
            return "Disorderly Behavior";
        case df::crime::BuildingDestruction:
            return "Building Destruction";
        case df::crime::Vandalism:
            return "Vandalism";
        case df::crime::Theft:
            return "Theft";
        case df::crime::Robbery:
            return "Robbery";
        case df::crime::BloodDrinking:
            return "Blood Drinking";
        default:
            break;
    }
    return "";
}

void crime_channel::do_work()
{
    size_t l_new_size = (df::global::world)->crimes.all.size();
    if (l_new_size != m_vector_size)
    {
        if (l_new_size > m_vector_size)
        {
            for (size_t i = m_vector_size; i < l_new_size; i++)
            {
                df::crime* l_df_crime = (df::global::world)->crimes.all[i];
                if (l_df_crime->site == (df::global::ui)->site_id)
                    if (l_df_crime->event_year == *df::global::cur_year)
                    {
                        // Criminal
                        int32_t   l_criminal_id   = l_df_crime->criminal;
                        df::unit* l_criminal_unit = getUnitById(l_criminal_id);
                        QString   l_criminal_race = getUnitRace(l_criminal_unit);
                        QString   l_criminal_name;

                        if (l_criminal_unit)
                        {
                            l_criminal_name = getUnitName(l_criminal_id);
                        }

                        // Victim
                        int32_t   l_victim_id   = l_df_crime->victim;
                        df::unit* l_victim_unit = getUnitById(l_victim_id);
                        QString   l_victim_race = getUnitRace(l_victim_unit);
                        QString   l_victim_name;

                        if (l_victim_unit)
                        {
                            l_victim_name = getUnitName(l_victim_id);

                            revealInMap(l_victim_unit->pos);
                        }

                        // Look for this unit
                        int32_t l_entity = l_df_crime->entity;
                        // Busca este id en entities vector

                        QString l_text("new crime " +
                                       crime_description(l_df_crime->mode) +
                                       " Criminal:" +
                                       l_criminal_name +
                                       "," +
                                       l_criminal_race +
                                       ", Victim: " +
                                       l_victim_name +
                                       "," +
                                       l_victim_race);

                        m_parent->addText(l_text);
                    }
            }
        }
        m_vector_size = (df::global::world)->crimes.all.size();
    }
}

void crime_channel::init()
{
    m_vector_size = (df::global::world)->crimes.all.size();
}
