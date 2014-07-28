/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_WINDOWS_WHOISONLINE_H
#define GUI_WINDOWS_WHOISONLINE_H

#include "listeners/configlistener.h"

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include <set>

#include "listeners/actionlistener.h"

class BrowserBox;
class Button;
class OnlinePlayer;
class ScrollArea;

struct SDL_Thread;

/**
 * Update progress window GUI
 *
 * \ingroup GUI
 */
class WhoIsOnline final : public Window,
                          public LinkHandler,
                          public ActionListener,
                          public ConfigListener
{
    public:
        /**
         * Constructor.
         */
        WhoIsOnline();

        A_DELETE_COPY(WhoIsOnline)

        /**
         * Destructor
         */
        ~WhoIsOnline();

        void postInit() override final;

        /**
         * Loads and display online list from the memory buffer.
         */
        void loadWebList();

        void loadList(const std::vector<OnlinePlayer*> &list);

        void handleLink(const std::string& link,
                        MouseEvent *event) override final;

        void logic() override final;

        void slowLogic();

        void action(const ActionEvent &event) override final;

        void widgetResized(const Event &event) override final;

        const std::set<OnlinePlayer*> &getOnlinePlayers() const A_WARN_UNUSED
        { return mOnlinePlayers; }

        const std::set<std::string> &getOnlineNicks() const A_WARN_UNUSED
        { return mOnlineNicks; }

        void setAllowUpdate(const bool n)
        { mAllowUpdate = n; }

        void optionChanged(const std::string &name) override final;

        void updateList(StringVect &list);

        void readFromWeb();

        static void setNeutralColor(OnlinePlayer *const player);

        void getPlayerNames(StringVect &names);

    private:
        void download();

        void updateSize();

        void handlerPlayerRelation(const std::string &nick,
                                   OnlinePlayer *const player);
        /**
         * The thread function that download the files.
         */
        static int downloadThread(void *ptr);

        /**
         * A libcurl callback for writing to memory.
         */
        static size_t memoryWrite(void *ptr, size_t size,
                                  size_t nmemb,
                                  FILE *stream);

        const std::string prepareNick(const std::string &restrict nick,
                                      const int level,
                                      const std::string &restrict color)
                                      const A_WARN_UNUSED;

        void updateWindow(size_t numOnline);

        enum DownloadStatus
        {
            UPDATE_ERROR = 0,
            UPDATE_COMPLETE,
            UPDATE_LIST
        };

        int mUpdateTimer;

        /** A thread that use libcurl to download updates. */
        SDL_Thread *mThread;

        /** Buffer for files downloaded to memory. */
        char *mMemoryBuffer;

        /** Buffer to handler human readable error provided by curl. */
        char *mCurlError;

        BrowserBox *mBrowserBox;
        ScrollArea *mScrollArea;
        std::set<OnlinePlayer*> mOnlinePlayers;
        std::set<std::string> mOnlineNicks;

        Button *mUpdateButton;
        std::vector<OnlinePlayer*> mFriends;
        std::vector<OnlinePlayer*> mNeutral;
        std::vector<OnlinePlayer*> mDisregard;
        std::vector<OnlinePlayer*> mEnemy;

        /** Byte count currently downloaded in mMemoryBuffer. */
        int mDownloadedBytes;

        /** Status of the current download. */
        DownloadStatus mDownloadStatus;

        /** Flag that show if current download is complete. */
        bool mDownloadComplete;
        bool mAllowUpdate;
        bool mShowLevel;
        bool mUpdateOnlineList;
        bool mGroupFriends;
};

extern WhoIsOnline *whoIsOnline;

#endif  // GUI_WINDOWS_WHOISONLINE_H
