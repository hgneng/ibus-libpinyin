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
#ifndef __PY_UTIL_H_
#define __PY_UTIL_H_

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include <cstdlib>
#include <string>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  include <memory>
#else
#  include <boost/shared_ptr.hpp>
#  include <boost/scoped_ptr.hpp>

namespace std {
    // import boost::shared_ptr to std namespace
    using boost::shared_ptr;
    // import boost::scoped_ptr to std namespace, and rename to unique_ptr
    // XXX: the unique_ptr can transfer the pointer ownership,
    //      but scoped_ptr cannot.
    template<typename T> class unique_ptr : public boost::scoped_ptr<T> {};
};

#endif

#include <ibus.h>

namespace PY {

// mask for Ctrl, Alt, Super, Hyper, Meta
const guint CMSHM_MASK = IBUS_CONTROL_MASK |
                         IBUS_MOD1_MASK |
                         IBUS_SUPER_MASK |
                         IBUS_HYPER_MASK |
                         IBUS_META_MASK;
// mask for Shift, Ctrl, Alt, Super, Hyper, Meta
const guint SCMSHM_MASK = CMSHM_MASK | IBUS_SHIFT_MASK;

inline guint
cmshm_filter (guint modifiers)
{
    return modifiers & CMSHM_MASK;
}

inline guint
scmshm_filter (guint modifiers)
{
    return modifiers & SCMSHM_MASK;
}

inline gboolean
cmshm_test (guint modifiers, guint mask)
{
    return cmshm_filter (modifiers) == mask;
}

inline gboolean
scmshm_test (guint modifiers, guint mask)
{
    return scmshm_filter (modifiers) == mask;
}

#if 0
class Uname {
public:
    Uname (void)
    {
        uname (&m_buf);
    }

    const gchar *hostname (void) const { return m_buf.nodename; }
private:
    struct utsname m_buf;
};

class Hostname : public Uname {
public:
    operator const gchar * (void) const
    {
        return hostname ();
    }
};

class Env : public std::string {
public:
    Env (const gchar *name)
    {
        gchar *str;
        str = std::getenv (name);
        assign (str != NULL ? str : "");
    }

    operator const gchar *(void) const
    {
        return c_str();
    }
};
#endif

};
#endif
