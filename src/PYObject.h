/* vim:set et ts=4 sts=4:
 *
 * ibus-libpinyin - Intelligent Pinyin engine based on libpinyin for IBus
 *
 * Copyright (c) 2008-2010 Peng Huang <shawn.p.huang@gmail.com>
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
#ifndef __PY_OBJECT_H_
#define __PY_OBJECT_H_

#include <glib-object.h>
#include "PYPointer.h"

namespace PY {

class Object {
protected:
    template <typename T>
    Object (T *p) : m_p ((GObject *)p)
    {
        g_assert (get <GObject *>() != NULL);
    }

    operator GObject * (void) const
    {
        return m_p;
    }

    template <typename T>
    T * get (void) const
    {
        return (T *) (GObject *) m_p;
    }

private:
    Pointer<GObject> m_p;
};

};

#endif
