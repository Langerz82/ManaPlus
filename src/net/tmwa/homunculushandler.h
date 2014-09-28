/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef NET_TMWA_HOMUNCULUSHANDLER_H
#define NET_TMWA_HOMUNCULUSHANDLER_H

#include "net/homunculushandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class HomunculusHandler final : public MessageHandler,
                                public Net::HomunculusHandler
{
    public:
        HomunculusHandler();

        A_DELETE_COPY(HomunculusHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void setName(const std::string &name) const override final;

        void moveToMaster() const override final;

        void move(const int x, const int y) const override final;

        void attack(const int targetId, const bool keep) const override final;

        void feed() const override final;

        void fire() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_HOMUNCULUSHANDLER_H