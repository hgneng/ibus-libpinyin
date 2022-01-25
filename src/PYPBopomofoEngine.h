/* vim:set et ts=4 sts=4:
 *
 * ibus-libpinyin - Intelligent Pinyin engine based on libpinyin for IBus
 *
 * Copyright (c) 2008-2010 Peng Huang <shawn.p.huang@gmail.com>
 * Copyright (c) 2010 BYVoid <byvoid1@gmail.com>
 * Copyright (c) 2011 Peng Wu <alexepico@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __PY_LIB_PINYIN_BOPOMOFO_ENGINE_H_
#define __PY_LIB_PINYIN_BOPOMOFO_ENGINE_H_

#include "PYEngine.h"
#include "PYPinyinProperties.h"

namespace PY {

class BopomofoEngine : public Engine {
public:
    BopomofoEngine (IBusEngine *engine);
    ~BopomofoEngine (void);

    // virtual functions
    gboolean processAccelKeyEvent (guint keyval, guint keycode,
                                   guint modifiers);
    gboolean processKeyEvent (guint keyval, guint keycode, guint modifiers);
    void focusIn (void);
    void focusOut (void);
    void reset (void);
    void enable (void);
    void disable (void);
    void pageUp (void);
    void pageDown (void);
    void cursorUp (void);
    void cursorDown (void);
    gboolean propertyActivate (const gchar *prop_name, guint prop_state);
    void candidateClicked (guint index, guint button, guint state);

private:
    gboolean processPunct (guint keyval, guint keycode, guint modifiers);

private:
    void showSetupDialog (void);
    void connectEditorSignals (EditorPtr editor);

private:
    void commitText (Text & text);

private:
    PinyinProperties m_props;

    guint m_prev_pressed_key;

    enum {
        MODE_INIT = 0,          // init mode
        MODE_PUNCT,             // punct mode
#if 0
        MODE_RAW,               // raw mode
        MODE_ENGLISH,           // press v into English input mode
        MODE_STROKE,            // press u into stroke input mode
        MODE_EXTENSION,         // press i into extension input mode
#endif
        MODE_SUGGESTION,        // suggestion mode
        MODE_LAST,
    } m_input_mode;

    gboolean m_need_update;

    EditorPtr m_editors[MODE_LAST];
    EditorPtr m_fallback_editor;
};

};


#endif
