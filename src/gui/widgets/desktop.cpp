/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana World Development Team
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

#include "gui/widgets/desktop.h"

#include "configuration.h"

#include "gui/skin.h"

#include "gui/widgets/browserbox.h"

#include "input/inputmanager.h"

#include "render/opengldebug.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"
#include "resources/wallpaper.h"

#include "debug.h"

Desktop *desktop = nullptr;

Desktop::Desktop(const Widget2 *const widget) :
    Container(widget),
    LinkHandler(),
    WidgetListener(),
    mWallpaper(nullptr),
    mVersionLabel(new BrowserBox(this, BrowserBox::AUTO_WRAP, false,
        "browserbox.xml")),
    mSkin(nullptr),
    mBackgroundColor(getThemeColor(Theme::BACKGROUND, 128)),
    mBackgroundGrayColor(getThemeColor(Theme::BACKGROUND_GRAY)),
    mShowBackground(true)
{
    addWidgetListener(this);

    Wallpaper::loadWallpapers();

    if (theme)
        mSkin = theme->load("desktop.xml", "");

    if (mSkin)
        mShowBackground = mSkin->getOption("showBackground");

    const std::string appName = branding.getValue("appName", std::string());
    if (appName.empty())
    {
        mVersionLabel->addRow(FULL_VERSION);
    }
    else
    {
        mVersionLabel->addRow(strprintf("%s (%s)", FULL_VERSION,
            appName.c_str()));
    }
    mVersionLabel->addRow("copyright",
        "(C) ManaPlus developers, http://manaplus.org");
    mVersionLabel->setLinkHandler(this);
}

Desktop::~Desktop()
{
    if (mWallpaper)
    {
        mWallpaper->decRef();
        mWallpaper = nullptr;
    }
    if (theme)
        theme->unload(mSkin);
}

void Desktop::postInit()
{
    if (mSkin)
    {
        addXY(mVersionLabel,
            mSkin->getOption("versionX", 25),
            mSkin->getOption("versionY", 2));
    }
    else
    {
        addXY(mVersionLabel, 25, 2);
    }
}

void Desktop::reloadWallpaper()
{
    Wallpaper::loadWallpapers();
    setBestFittingWallpaper();
}

void Desktop::widgetResized(const Event &event A_UNUSED)
{
    mVersionLabel->setSize(getWidth(), getHeight());
    setBestFittingWallpaper();
}

void Desktop::draw(Graphics *graphics)
{
    BLOCK_START("Desktop::draw")
    GLDEBUG_START("Desktop::draw")

    const Rect &rect = mDimension;
    const int width = rect.width;
    const int height = rect.height;
    if (mWallpaper)
    {
        const int wallpWidth = mWallpaper->getWidth();
        const int wallpHeight = mWallpaper->getHeight();

        if (width > wallpWidth || height > wallpHeight)
        {
            graphics->setColor(mBackgroundGrayColor);
            graphics->fillRectangle(Rect(0, 0, width, height));
        }

        if (imageHelper->useOpenGL() == RENDER_SOFTWARE)
        {
            graphics->drawImage(mWallpaper,
                (width - wallpWidth) / 2,
                (height - wallpHeight) / 2);
        }
        else
        {
            graphics->drawRescaledImage(mWallpaper, 0, 0, width, height);
        }
    }
    else
    {
        graphics->setColor(mBackgroundGrayColor);
        graphics->fillRectangle(Rect(0, 0, width, height));
    }

    Container::draw(graphics);
    GLDEBUG_END()
    BLOCK_END("Desktop::draw")
}

void Desktop::setBestFittingWallpaper()
{
    if (!mShowBackground || !config.getBoolValue("showBackground"))
        return;

    const std::string wallpaperName =
        Wallpaper::getWallpaper(getWidth(), getHeight());

    Image *const nWallPaper = Theme::getImageFromTheme(wallpaperName);

    if (nWallPaper)
    {
        ResourceManager *const resman = ResourceManager::getInstance();
        if (mWallpaper)
        {
            resman->decRefDelete(mWallpaper);
            mWallpaper = nullptr;
        }

        const Rect &rect = mDimension;
        const int width = rect.width;
        const int height = rect.height;

        if (imageHelper->useOpenGL() == RENDER_SOFTWARE &&
            (nWallPaper->getWidth() != width
            || nWallPaper->getHeight() != height))
        {
            // We rescale to obtain a fullscreen wallpaper...
            Image *const newRsclWlPpr = resman->getRescaled(
                nWallPaper, width, height);

            if (newRsclWlPpr)
            {
                resman->decRefDelete(nWallPaper);
                // We replace the resource in the resource manager
                mWallpaper = newRsclWlPpr;
            }
            else
            {
                mWallpaper = nWallPaper;
            }
        }
        else
        {
            mWallpaper = nWallPaper;
        }
    }
    else
    {
        logger->log("Couldn't load %s as wallpaper", wallpaperName.c_str());
    }
}

void Desktop::handleLink(const std::string &link, MouseEvent *event A_UNUSED)
{
    if (link == "copyright")
        inputManager.executeAction(InputAction::WINDOW_ABOUT);
}
