#include "DwidderApp.h"


#include <df/world.h>

/*
DF Calendar Months
01: Granite
02: Slate
03: Felsite
04: Hematite
05: Malachite
06: Galena
07: Limestone
08: Sandstone
09: Timber
10: Moonstone
11: Opal
12: Obsidian

Each month has 28 days
*/

DwidderApp::DwidderApp(MainWindow* p_parent, std::shared_ptr<EventProxy>&& p_proxy)
    : m_parent(p_parent),
      m_event_proxy(std::move(p_proxy)),
      m_core_suspender(nullptr),
      m_suspended(false)
{


    m_cur_year_tick = -1;
}

QString DateAsString(int p_year, int p_month, int p_day)
{
    QString dfDateString = QString::number(p_year);
    dfDateString         = dfDateString.append('-');
    dfDateString         = dfDateString.append(QString::number(p_month));
    dfDateString         = dfDateString.append('-');
    dfDateString         = dfDateString.append(QString::number(p_day));
    return dfDateString;
}

QString DateAsString(calendar_data& p_calendar_data)
{
    return DateAsString(p_calendar_data.m_year,
                        p_calendar_data.m_month,
                        p_calendar_data.m_day);
}

/**
 * @brief Handle the formatting of the Dwarf Fortress date
 *
 * Thanks to Kurik Amudnil for the date stuff
 * http://www.bay12forums.com/smf/index.php?PHPSESSID=669fc6cc7664043c4b34992a301abb0c&topic=91166.msg4247785#msg4247785
 * @return QString with the date inf format YYYY/M/D
 */
QString GetDFDate()
{
    //    -- Would it be useful to return a part of the DF date?
    //    -- local absTick = 1200*28*12*df.global.cur_year +
    //    df.global.cur_year_tick
    int32_t dfYear  = *df::global::cur_year;
    int32_t dfMonth = floor((*df::global::cur_year_tick / 33600) + 1);
    int32_t dfDay   = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;

    return DateAsString(dfYear, dfMonth, dfDay);
}

/**
 * @brief  Convert a df.coord (x,y,z) to a string
 *
 * @param p_coord df.coord object
 * @return QString string representation of the object
 */
QString coord_2_string(const df::coord& p_coord)
{
    QString l_result = "[";
    l_result.append(QString::number(p_coord.x));
    l_result.append(",");
    l_result.append(QString::number(p_coord.y));
    l_result.append(",");
    l_result.append(QString::number(p_coord.z));
    l_result.append(",");
    l_result.append("]");

    return l_result;
}

bool DwidderApp::process_announcements(int p_num_new_announcements)
{
    std::vector<std::unique_ptr<announcement_data>> l_initial_data_vector;
    std::vector<std::unique_ptr<announcement_data>> l_final_data_vector;

    for (int i = (df::global::world)->status.announcements.size() - p_num_new_announcements, j = 0; i < (df::global::world)->status.announcements.size(); i++)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        auto        l_ptr    = std::make_unique<announcement_data>(l_report);
        l_initial_data_vector.push_back(std::move(l_ptr));
    }

    for (int i = l_initial_data_vector.size() - 1; i >= 0; i--)
    {
        announcement_data* l_data = l_initial_data_vector[i].get();
        if (l_data->m_flags.whole & 0x1)
        {
            announcement_data* l_data_prev = l_initial_data_vector[i - 1].get();
            l_data_prev->m_text            = l_data_prev->m_text + " " + l_data->m_text;
        }
    }

    for (int i = l_initial_data_vector.size() - 1; i >= 0; i--)
    {
        announcement_data* l_data = l_initial_data_vector[i].get();
        if (l_data->m_flags.whole & 0x1)
            continue;
        l_final_data_vector.push_back(std::move(l_initial_data_vector[i]));
    }

    for (int i = 0; i < l_final_data_vector.size(); i++)
    {
        announcement_data* l_data   = l_final_data_vector[i].get();
        QString            l_result = process_announcement(*l_data);
        QString l_result2  = process_announcement(*l_data);
        m_parent->addText(l_result2);
    }
    return true;
}

QString DwidderApp::process_announcement(announcement_data& p_data)
{
    QString l_result = QString::number(m_cur_year_tick) + "/" + GetDFDate() + "-";
    QString l_pos    = coord_2_string(p_data.m_pos);

    // Center window
    DFHack::Gui::revealInDwarfmodeMap(p_data.m_pos, true);

    DFHack::Gui::setCursorCoords(p_data.m_pos.x,
                                 p_data.m_pos.y,
                                 p_data.m_pos.z);

    l_result.append(l_pos + " " + p_data.m_text);

    return l_result;
}

int DwidderApp::check_for_new_announcements()
{
    int l_result = 0;
    for (int i = (df::global::world)->status.announcements.size() - 1; i >= 0; i--)
    {
        df::report* l_report = (df::global::world)->status.announcements[i];
        if (m_processed_announcements.find(l_report->id) != m_processed_announcements.end())
            break;
        l_result++;
        m_processed_announcements.insert(l_report->id);
    }
    return l_result;
}

announcement_data::announcement_data(df::report* p_df_announcement)
{
    m_type  = p_df_announcement->type;
    m_text  = DF2QT(p_df_announcement->text);
    m_flags = p_df_announcement->flags;
    m_pos   = p_df_announcement->pos;
    m_id    = p_df_announcement->id;
    m_year  = p_df_announcement->year;
    m_time  = p_df_announcement->time;

    bool process = false;
    m_unit_id    = -1;
    switch (m_type)
    {
        case df::announcement_type::MASTERFUL_IMPROVEMENT:
        case df::announcement_type::RECRUIT_PROMOTED:
        case df::announcement_type::CITIZEN_BECOMES_NONSOLDIER:
        case df::announcement_type::CITIZEN_BECOMES_SOLDIER:
        case df::announcement_type::ITEM_ATTACHMENT: //comma
        default:
            break;
    }

    if (process)
    {
        QStringList l_split = m_text.split(" ");
        if (l_split.size() >= 2)
        {
            QString l_unit_name = l_split.at(0) + " " + l_split.at(1);

            for (int i = 0; i < (df::global::world)->units.active.size(); i++)
            {
                df::unit*          l_unit          = (df::global::world)->units.active[i];
                df::language_name* l_lang          = &(l_unit->name);
                std::string        l_unit_name_std = DFHack::Translation::TranslateName(l_lang, false, false);
                QString            l_name          = QString::fromStdString(DF2UTF(l_unit_name_std));
                if (l_name == l_unit_name)
                {
                    m_unit_id = l_unit->id;
                    break;
                }
            }
        }
    }
}

QString DwidderApp::process_calendar()
{
    bool l_new_month = false;
    bool l_new_year  = false;

    auto l_year   = *df::global::cur_year;
    auto l__month = floor((*df::global::cur_year_tick / 33600) + 1);
    auto l_day    = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;

    if (l_day != m_calendar_data.m_day)
    {
        if (l_day == 1)
        {
            // new month
            l_new_month = true;
        }
    }

    if (l_year != m_calendar_data.m_year)
    {
        // new year
        l_new_year = true;
    }

    m_calendar_data.m_day   = l_day;
    m_calendar_data.m_month = l__month;
    m_calendar_data.m_year  = l_year;

    if (l_new_year)
    {
        return DateAsString(m_calendar_data) + " New Year!!";
    }
    else if (l_new_month)
    {
        return DateAsString(m_calendar_data) + " New month!!";
    }
    return "";
}

void calendar_data::update()
{
    m_year  = *df::global::cur_year;
    m_month = floor((*df::global::cur_year_tick / 33600) + 1);
    m_day   = floor((*df::global::cur_year_tick % 33600) / 1200) + 1;
}

void DwidderApp::tick()
{
    // Check if the core suspender was created or not. If not
    // create it. Then suspend DF
    if (m_core_suspender == nullptr)
        m_core_suspender = new DFHack::CoreSuspender;
    else
        m_core_suspender->lock();

    m_suspended = false;

    // First scan
    if (m_cur_year_tick == -1)
    {
        m_calendar_data.update();
        // Copy all the announcements ids to the set
        for (int i = 0; i < (df::global::world)->status.announcements.size(); i++)
        {
            df::report* l_report = (df::global::world)->status.announcements[i];
            m_processed_announcements.insert(l_report->id);
            //Update dwidder tick with th df one
            m_cur_year_tick = *df::global::cur_year_tick;
        }
    }

    // Normal scan
    // if the df tick is the same, the game will be probably paused
    if (m_cur_year_tick != *df::global::cur_year_tick)
    {
        //Update dwidder tick with th df one
        m_cur_year_tick = *df::global::cur_year_tick;

        // Check for calendar changes
        QString calendar_event = process_calendar();
        if (calendar_event.size() > 0)
        {
            m_parent->addText(calendar_event);
        }

        // Check if there are new announcements
        int l_num_new_announcements = check_for_new_announcements();
        if (l_num_new_announcements > 0)
        {
            process_announcements(l_num_new_announcements);
        }
    }

    m_core_suspender->unlock();
    m_suspended = false;
}
