/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NET_ADMINHANDLER_H
#define NET_ADMINHANDLER_H

#include "localconsts.h"

#include <string>

class Being;

namespace Net
{

class AdminHandler notfinal
{
    public:
        virtual ~AdminHandler()
        { }

        virtual void announce(const std::string &text) const = 0;

        virtual void localAnnounce(const std::string &text) const = 0;

        virtual void hide(const bool hide) const = 0;

        virtual void kick(const int playerId) const = 0;

        virtual void kickName(const std::string &name) const = 0;

        virtual void kickAll() const = 0;

        virtual void ban(const int playerId) const = 0;

        virtual void banName(const std::string &name) const = 0;

        virtual void unban(const int playerId) const = 0;

        virtual void unbanName(const std::string &name) const = 0;

        virtual void mute(const Being *const being,
                          const int type,
                          const int limit) const = 0;

        virtual void muteName(const std::string &name) const = 0;

        virtual void warp(const std::string &map,
                          const int x, const int y) const = 0;

        virtual void createItems(const int id, const int color,
                                 const int amount) const = 0;

        virtual void gotoName(const std::string &name) const = 0;

        virtual void recallName(const std::string &name) const = 0;

        virtual void reviveName(const std::string &name) const = 0;

        virtual void ipcheckName(const std::string &name) const = 0;

        virtual void resetStats() const = 0;

        virtual void resetSkills() const = 0;

        virtual void requestLogin(const Being *const being) const = 0;

        virtual void setTileType(const int x, const int y,
                                 const int type) const = 0;

        virtual void unequipAll(const Being *const being) const = 0;

        virtual void requestStats(const std::string &name) = 0;
};

}  // namespace Net

extern Net::AdminHandler *adminHandler;

#endif  // NET_ADMINHANDLER_H
