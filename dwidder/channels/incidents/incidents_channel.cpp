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

#include "incidents_channel.h"

#include <df/global_objects.h>
#include <df/ui.h>
#include <df/world.h>

#include "../../DwidderApp.h"
#include "../../dwidder_utils.h"
#include "modules/Units.h"

incidents_channel::incidents_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void incidents_channel::do_work()
{
    size_t l_new_size = (df::global::world)->incidents.all.size();
    if (l_new_size != m_vector_size)
    {
        if (l_new_size > m_vector_size)
        {
            for (size_t i = m_vector_size; i < l_new_size; i++)
            {
                df::incident* l_incident = (df::global::world)->incidents.all[i];

                std::vector<QString> l_witnesses;
                QString              l_witness_names = "Witness:";
                for (size_t j = 0; j < l_incident->witnesses.size(); j++)
                {
                    df::unit* l_unit_witness = getUnitById(l_incident->witnesses[j]);
                    QString   l_witnes_name  = getUnitName(l_incident->witnesses[j]);
                    QString   l_witnes_race  = getUnitRace(l_unit_witness);
                    l_witness_names.append(l_witnes_name +
                                           "," +
                                           l_witnes_race +
                                           ".");
                }

                if (l_incident->death_cause != -1)
                {
                    QString l_victim_name;
                    QString l_victim_race;
                    QString l_killer_name;
                    QString l_killer_race;
                    QString l_death_cause = QString::fromStdString(ENUM_KEY_STR(death_type, l_incident->death_cause));

                    if (l_incident->victim != -1)
                    {
                        df::unit* l_victim_unit = getUnitById(l_incident->victim);

                        //revealInMap(l_victim_unit->pos);

                        QString l_text = "Incident";
                        l_victim_name  = getUnitName(l_incident->victim);
                        l_victim_race  = getUnitRace(l_victim_unit);

                        if (l_incident->killer != -1)
                        {
                            df::unit* l_killer_unit = getUnitById(l_incident->killer);
                            l_killer_name           = getUnitName(l_incident->killer);
                            l_killer_race           = getUnitRace(l_killer_unit);

                            l_text.append(" " +
                                          l_death_cause +
                                          " victim " +
                                          l_victim_name +
                                          ":" +
                                          l_victim_race +
                                          " killer " +
                                          l_killer_name +
                                          ":" +
                                          l_killer_race);
                            l_text.append(l_witness_names);
                            m_parent->addText(l_text);
                        }
                    }
                }
            }

            m_vector_size = l_new_size;
        }
    }
}

void incidents_channel::init()
{
    m_vector_size = (df::global::world)->incidents.all.size();
}
