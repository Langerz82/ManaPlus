/*
 *  The ManaPlus Client
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

#ifndef NET_TMWA_FAMILYHANDLER_H
#define NET_TMWA_FAMILYHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/familyhandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class FamilyHandler final : public MessageHandler,
                            public Net::FamilyHandler
{
    public:
        FamilyHandler();

        A_DELETE_COPY(FamilyHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void askForChild(const Being *const being) override final;

        void askForChildReply(const bool accept) override final;
};

}  // namespace TmwAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_TMWA_FAMILYHANDLER_H
