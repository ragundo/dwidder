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

#include "building_channel.h"

#include <df/building_actual.h>
#include <df/world.h>

#include "../../DwidderApp.h"
#include "../../dwidder_utils.h"

QString decode_building_type(df::building_type p_building_type)
{
    switch (p_building_type)
    {
        case -1:
            return "NONE";
        case 0:
            return "Chair";
        case 1:
            return "Bed";
        case 2:
            return "Table";
        case 3:
            return "Coffin";
        case 4:
            return "FarmPlot";
        case 5:
            return "Furnace";
        case 6:
            return "TradeDepot";
        case 7:
            return "Shop";
        case 8:
            return "Door";
        case 9:
            return "Floodgate";
        case 10:
            return "Box";
        case 11:
            return "Weaponrack";
        case 12:
            return "Armorstand";
        case 13:
            return "Workshop";
        case 14:
            return "Cabinet";
        case 15:
            return "Statue";
        case 16:
            return "WindowGlass";
        case 17:
            return "WindowGem";
        case 18:
            return "Well";
        case 19:
            return "Bridge";
        case 20:
            return "RoadDirt";
        case 21:
            return "RoadPaved";
        case 22:
            return "SiegeEngine";
        case 23:
            return "Trap";
        case 24:
            return "AnimalTrap";
        case 25:
            return "Support";
        case 26:
            return "ArcheryTarget";
        case 27:
            return "Chain";
        case 28:
            return "Cage";
        case 29:
            return "Stockpile";
        case 30:
            return "Civzone";
        case 31:
            return "Weapon";
        case 32:
            return "Wagon";
        case 33:
            return "ScrewPump";
        case 34:
            return "Construction";
        case 35:
            return "Hatch";
        case 36:
            return "GrateWall";
        case 37:
            return "GrateFloor";
        case 38:
            return "BarsVertical";
        case 39:
            return "BarsFloor";
        case 40:
            return "GearAssembly";
        case 41:
            return "AxleHorizontal";
        case 42:
            return "AxleVertical";
        case 43:
            return "WaterWheel";
        case 44:
            return "Windmill";
        case 45:
            return "TractionBench";
        case 46:
            return "Slab";
        case 47:
            return "Nest";
        case 48:
            return "NestBox";
        case 49:
            return "Hive";
        case 50:
            return "Rollers";
        case 51:
            return "Instrument";
        case 52:
            return "Bookcase";
        case 53:
            return "DisplayFurniture";
        default:
            break;
    }
    return "unkmown building";
}

building_channel::building_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void building_channel::init()
{
    m_known_buildings = (df::global::world)->buildings.all.size();
    // Copy all the announcements ids to the set
    for (size_t i = 0; i < m_known_buildings; i++)
    {
        df::building* l_building = (df::global::world)->buildings.all[i];
        if (l_building)
        {
            m_processed_buildings.insert(std::make_pair(l_building->getType(),
                                                        l_building->id));
        }
    }
}

void building_channel::do_work()
{
    size_t l_num_buildings = (df::global::world)->buildings.all.size();

    // Check for buildings in construction
    auto l_pending_buildings = m_in_construction_buildings.size();
    if (l_pending_buildings > 0)
    {
        for (auto it : m_in_construction_buildings)
        {
            int32_t l_id = it;
            for (auto i = l_num_buildings - 1; i >= 0; i--)
            {
                auto l_building = (df::global::world)->buildings.all[i];
                if (l_building->id == l_id)
                {
                    // Check for end of construction
                    auto l_building_actual = (df::building_actual*)(l_building);
                    if (l_building_actual->construction_stage == 1)
                    {
                        // Building finished
                        QString l_text = "Building " + QString::number(l_building->id) + " Finished";
                        m_parent->addText(l_text);
                        m_in_construction_buildings.erase(it);
                        df::coord l_pos(l_building->centerx,
                                        l_building->centery,
                                        l_building->z);
                        revealInMap(l_pos);
                    }
                    break;
                }
            }
            if (m_in_construction_buildings.size() == 0)
                break;
        }
    }

    // Check if there are new buildings
    if (l_num_buildings > m_known_buildings)
    {
        // Check for new buildings created
        for (auto t = m_known_buildings; t < l_num_buildings; t++)
        {
            df::building* l_building = (df::global::world)->buildings.all[t];
            m_in_construction_buildings.insert(l_building->id);
            QString l_text = "New building " + QString::number(l_building->id) + decode_building_type(l_building->getType());
            m_parent->addText(l_text);
        }
    }
    else if (l_num_buildings < m_known_buildings)
    {
        // Check for buildings destroyed
        QString l_text = "Building destroyed";
        //m_parent->addText(l_text);
    }
    m_known_buildings = l_num_buildings;
}
