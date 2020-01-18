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

#include "activities_channel.h"

#include <df/activity_entry.h>
#include <df/activity_event_conversationst.h>
#include <df/global_objects.h>
#include <df/world.h>

#include "../../DwidderApp.h"
#include "../../dwidder_utils.h"

#include <modules/Gui.h>
#include <modules/Items.h>
#include <modules/Units.h>

QString activity_event_subtype(df::activity_event* p_event)
{
    DFHack::virtual_identity* id = DFHack::virtual_identity::get(p_event);
    if (id != nullptr)
    {
        auto l_full_name = id->getFullName();
        if (l_full_name == "activity_event_training_sessionst")
            return "training session";
        if (l_full_name == "activity_event_combat_trainingst")
            return "combat training";
        if (l_full_name == "activity_event_skill_demonstrationst")
            return "skill demonstration";
        if (l_full_name == "activity_event_individual_skill_drillst")
            return "individual skill drill";
        if (l_full_name == "activity_event_sparringst")
            return "sparring";
        if (l_full_name == "activity_event_ranged_practicest")
            return "ranged practice";
        if (l_full_name == "activity_event_harassmentst")
            return "harassment";
        if (l_full_name == "activity_event_conversationst")
            return "conversation";
        if (l_full_name == "activity_event_conflictst")
            return "conflict";
        if (l_full_name == "activity_event_guardst")
            return "guard";
        if (l_full_name == "activity_event_reunionst")
            return "reunion";
        if (l_full_name == "activity_event_prayerst")
            return "prayer";
        if (l_full_name == "activity_event_socializest")
            return "socialize";
        if (l_full_name == "activity_event_worshipst")
            return "worship";
        if (l_full_name == "activity_event_performancest")
            return "performance";
        if (l_full_name == "activity_event_researchst")
            return "research";
        if (l_full_name == "activity_event_ponder_topicst")
            return "ponder topic";
        if (l_full_name == "activity_event_discuss_topicst")
            return "discuss_topic";
        if (l_full_name == "activity_event_readst")
            return "read";
        if (l_full_name == "activity_event_fill_service_orderst")
            return "fill service order";
        if (l_full_name == "activity_event_writest")
            return "write";
        if (l_full_name == "activity_event_copy_written_contentst")
            return "copy written content";
        if (l_full_name == "activity_event_teach_topicst")
            return "teach topic";
        if (l_full_name == "activity_event_playst")
            return "play";
        if (l_full_name == "activity_event_make_believest")
            return "make believe";
        if (l_full_name == "activity_event_play_with_toyst")
            return "play with toy";
        if (l_full_name == "activity_event_encounterst")
            return "encounter";
        if (l_full_name == "activity_event_store_objectst")
            return "store object";
    }
    return "";
}

activities_channel::activities_channel(DwidderApp* p_parent)
    : m_parent(p_parent)
{
}

void activities_channel::do_work()
{
    return;
    auto l_new_size = (df::global::world)->activities.all.size();
    for (size_t i = 0; i < l_new_size; i++)
    {
        df::activity_entry* l_activity = (df::global::world)->activities.all[i];
        if (m_id_set.find(l_activity->id) == m_id_set.end())
        {
            QString l_text = "ACTIVITY: type:" + QString::number(l_activity->type) + " " + QString::number(l_activity->events.size()) + " events:";
            m_parent->addText(l_text);
            if (l_activity->events.size() > 0)
            {
                for (size_t j = 0; j < l_activity->events.size(); j++)
                {
                    df::activity_event* l_event      = l_activity->events[j];
                    QString             l_event_text = activity_event_subtype(l_event);

                    if (l_event_text == "conversation")
                    {
                        df::activity_event_conversationst* l_conversation_event = (df::activity_event_conversationst*)l_event;
                        if (l_conversation_event)
                        {
                            for (size_t k = 0; k < l_conversation_event->participants.size(); k++)
                            {
                                auto l_participant = l_conversation_event->participants[k];
                                if (l_participant->unit_id != -1)
                                {
                                    l_event_text.append(" " + getUnitName(l_participant->unit_id));
                                }
                            }
                        }
                    }

                    l_event_text.append("");
                    m_parent->addText(l_event_text);
                }
            }
            m_id_set.insert(l_activity->id);
        }
    }
}

void activities_channel::init()
{
    auto l_new_size = (df::global::world)->activities.all.size();
    for (size_t i = 0; i < l_new_size; i++)
    {
        df::activity_entry* l_activity = (df::global::world)->activities.all[i];
        m_id_set.insert(l_activity->id);
    }
}
