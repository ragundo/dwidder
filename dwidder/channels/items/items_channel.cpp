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

#include "items_channel.h"

#include <df/item_constructed.h>
#include <df/world.h>

#include "../../DwidderApp.h"
#include "../../dwidder_utils.h"

#include <modules/Items.h>

QString decode_item_type(df::item_type p_item_type)
{
    switch (p_item_type)
    {
        case -1:
            return "NONE";
        case 0:
            return "BAR";
        case 1:
            return "SMALLGEM";
        case 2:
            return "BLOCKS";
        case 3:
            return "ROUGH";
        case 4:
            return "BOULDER";
        case 5:
            return "WOOD";
        case 6:
            return "DOOR";
        case 7:
            return "FLOODGATE";
        case 8:
            return "BED";
        case 9:
            return "CHAIR";
        case 10:
            return "CHAIN";
        case 11:
            return "FLASK";
        case 12:
            return "GOBLET";
        case 13:
            return "INSTRUMENT";
        case 14:
            return "TOY";
        case 15:
            return "WINDOW";
        case 16:
            return "CAGE";
        case 17:
            return "BARREL";
        case 18:
            return "BUCKET";
        case 19:
            return "ANIMALTRAP";
        case 20:
            return "TABLE";
        case 21:
            return "COFFIN";
        case 22:
            return "STATUE";
        case 23:
            return "CORPSE";
        case 24:
            return "WEAPON";
        case 25:
            return "ARMOR";
        case 26:
            return "SHOES";
        case 27:
            return "SHIELD";
        case 28:
            return "HELM";
        case 29:
            return "GLOVES";
        case 30:
            return "BOX";
        case 31:
            return "BIN";
        case 32:
            return "ARMORSTAND";
        case 33:
            return "WEAPONRACK";
        case 34:
            return "CABINET";
        case 35:
            return "FIGURINE";
        case 36:
            return "AMULET";
        case 37:
            return "SCEPTER";
        case 38:
            return "AMMO";
        case 39:
            return "CROWN";
        case 40:
            return "RING";
        case 41:
            return "EARRING";
        case 42:
            return "BRACELET";
        case 43:
            return "GEM";
        case 44:
            return "ANVIL";
        case 45:
            return "CORPSEPIECE";
        case 46:
            return "REMAINS";
        case 47:
            return "MEAT";
        case 48:
            return "FISH";
        case 49:
            return "FISH_RAW";
        case 50:
            return "VERMIN";
        case 51:
            return "PET";
        case 52:
            return "SEEDS";
        case 53:
            return "PLANT";
        case 54:
            return "SKIN_TANNED";
        case 55:
            return "PLANT_GROWTH";
        case 56:
            return "THREAD";
        case 57:
            return "CLOTH";
        case 58:
            return "TOTEM";
        case 59:
            return "PANTS";
        case 60:
            return "BACKPACK";
        case 61:
            return "QUIVER";
        case 62:
            return "CATAPULTPARTS";
        case 63:
            return "BALLISTAPARTS";
        case 64:
            return "SIEGEAMMO";
        case 65:
            return "BALLISTAARROWHEAD";
        case 66:
            return "TRAPPARTS";
        case 67:
            return "TRAPCOMP";
        case 68:
            return "DRINK";
        case 69:
            return "POWDER_MISC";
        case 70:
            return "CHEESE";
        case 71:
            return "FOOD";
        case 72:
            return "LIQUID_MISC";
        case 73:
            return "COIN";
        case 74:
            return "GLOB";
        case 75:
            return "ROCK";
        case 76:
            return "PIPE_SECTION";
        case 77:
            return "HATCH_COVER";
        case 78:
            return "GRATE";
        case 79:
            return "QUERN";
        case 80:
            return "MILLSTONE";
        case 81:
            return "SPLINT";
        case 82:
            return "CRUTCH";
        case 83:
            return "TRACTION_BENCH";
        case 84:
            return "ORTHOPEDIC_CAST";
        case 85:
            return "TOOL";
        case 86:
            return "SLAB";
        case 87:
            return "EGG";
        case 88:
            return "BOOK";
        case 89:
            return "SHEET";
        case 90:
            return "BRANCH";
        default:
            break;
    }
    return "Unknow item";
}

items_channel::items_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void items_channel::init()
{
    m_known_items = (df::global::world)->items.all.size();
    // Copy all the announcements ids to the set
    for (size_t i = 0; i < m_known_items; i++)
    {
        df::item* l_item = (df::global::world)->items.all[i];
        if (l_item)
        {
            m_processed_items.insert(std::make_pair(l_item->getType(),
                                                    l_item->id));
        }
    }
}

void items_channel::do_work()
{
    size_t l_num_items = (df::global::world)->items.all.size();

    // Check for buildings in construction
    // auto l_pending_items = m_in_construction_items.size();
    // if (l_pending_items > 0)
    // {
    //     for (auto it : m_in_construction_items)
    //     {
    //         int32_t l_id = it;
    //         for (auto i = l_num_items - 1; i >= 0; i--)
    //         {
    //             auto l_item = (df::global::world)->items.all[i];
    //             if (l_item->id == l_id)
    //             {
    //                 // item finished
    //                 QString l_text = "Item " + QString::number(l_item->id) + " Finished";
    //                 m_parent->addText(l_text);
    //                 m_in_construction_items.erase(it);
    //             }
    //             break;
    //         }
    //     }
    // }

    if (l_num_items > m_known_items)
    {
        // Check for new buildings created
        for (auto t = m_known_items; t < l_num_items; t++)
        {
            df::item* l_item      = (df::global::world)->items.all[t];
            auto      l_item_type = l_item->getType();

            //m_in_construction_items.insert(l_item->id);
            QString l_item_description = QString::fromStdString(DFHack::Items::getDescription(l_item, l_item_type));
            QString l_text             = "New item " +
                             QString::number(l_item->id) +
                             " " +
                             decode_item_type(l_item_type) +
                             " " +
                             l_item_description;
            m_parent->addText(l_text);
            //revealInMap(l_item->pos);
        }
    }
    else if (l_num_items < m_known_items)
    {
        // Check for buildings destroyed
        //QString l_text = "Building destroyed";
        //m_parent->addText(l_text);
    }
    m_known_items = l_num_items;
}
