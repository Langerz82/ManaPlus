/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef GUI_POPUPMANAGER_H
#define GUI_POPUPMANAGER_H

#include "being/actortype.h"

#include <string>
#include <vector>

#include "localconsts.h"

class ActorSprite;
class Button;
class Being;
class BeingPopup;
class ChatTab;
class FloorItem;
class Item;
class MapItem;
class PopupMenu;
class ProgressBar;
class TextCommand;
class TextField;
class TextPopup;
class Window;

class PopupManager final
{
    public:
        /**
         * Constructor.
         */
        PopupManager();

        A_DELETE_COPY(PopupManager)

        /**
         * Destructor.
         */
        ~PopupManager();

        bool isBeingPopupVisible() const;

        bool isTextPopupVisible() const;

        /**
         * Closes the popup menu. Needed for when the player dies or switching
         * maps.
         */
        void closePopupMenu();

        /**
         * Hides the BeingPopup.
         */
        void hideBeingPopup();

        void hideTextPopup();

        bool isPopupMenuVisible() const A_WARN_UNUSED;

        void clearPopup();

        void hidePopupMenu();
};

extern PopupManager *popupManager;

#endif  // GUI_POPUPMANAGER_H