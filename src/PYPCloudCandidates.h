/* vim:set et ts=4 sts=4:
 *
 * ibus-libpinyin - Intelligent Pinyin engine based on libpinyin for IBus
 *
 * Copyright (c) 2018 linyu Xu <liannaxu07@gmail.com>
 * Copyright (c) 2020 Weixuan XIAO <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __PY_LIB_PINYIN_ClOUD_CANDIDATES_H_
#define __PY_LIB_PINYIN_ClOUD_CANDIDATES_H_

#include "PYString.h"
#include "PYPointer.h"
#include "PYPEnhancedCandidates.h"
#include <string>
#include <vector>
#include <set>
#include <glib.h>
#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include "PYConfig.h"



class CloudCandidatesResponseParser;

namespace PY {

#define BUFFERLENGTH 2048
#define CLOUD_MINIMUM_UTF8_TRIGGER_LENGTH 2

enum InputMode {
    FullPinyin = 0,
    DoublePinyin,
    Bopomofo
};

class PhoneticEditor;

class CloudCandidates : public EnhancedCandidates<PhoneticEditor>
{
public:

    CloudCandidates (PhoneticEditor *editor);
    ~CloudCandidates();

    void setInputMode (InputMode mode) { m_input_mode = mode; }

    gboolean processCandidates (std::vector<EnhancedCandidate> & candidates);

    int selectCandidate (EnhancedCandidate & enhanced);

    void cloudAsyncRequest (gpointer user_data);
    void cloudSyncRequest (const gchar* pinyin, std::vector<EnhancedCandidate> & candidates);

    void delayedCloudAsyncRequest (const gchar* pinyin);

    void updateLookupTable ();

    guint m_source_event_id;
    SoupMessage *m_message;
    std::string m_last_requested_pinyin;

private:
    static gboolean delayedCloudAsyncRequestCallBack (gpointer user_data);
    static void cloudResponseCallBack (GObject *object, GAsyncResult *result, gpointer user_data);

    gboolean processCloudResponse (GInputStream *stream, std::vector<EnhancedCandidate> & candidates, const gchar *pinyin);

    /* get internal full pinyin representation */
    String getFullPinyin ();

    void resetCloudResponseParser ();

private:
    SoupSession *m_session;
    InputMode m_input_mode;

    CloudInputSource m_input_source;
    CloudCandidatesResponseParser *m_parser;
    GTimer *m_timer;

protected:
    std::vector<EnhancedCandidate> m_candidates;

    /* The candidate cache contains some candidates from libpinyin,
       use this cache to remove duplicated cloud candidates. */
    std::set<std::string> m_candidate_cache;
};

};

#endif
