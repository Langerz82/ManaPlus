/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "gui/windows/tradewindow.h"

#include "configuration.h"
#include "item.h"
#include "units.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "enums/being/attributes.h"

#include "gui/gui.h"

#include "gui/fonts/font.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/itemamountwindow.h"
#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "net/tradehandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

TradeWindow *tradeWindow = nullptr;

// TRANSLATORS: trade window button
#define CAPTION_PROPOSE _("Propose trade")
// TRANSLATORS: trade window button
#define CAPTION_CONFIRMED _("Confirmed. Waiting...")
// TRANSLATORS: trade window button
#define CAPTION_ACCEPT _("Agree trade")
// TRANSLATORS: trade window button
#define CAPTION_ACCEPTED _("Agreed. Waiting...")

TradeWindow::TradeWindow() :
    // TRANSLATORS: trade window caption
    Window(_("Trade: You"), false, nullptr, "trade.xml"),
    ActionListener(),
    SelectionListener(),
    mMyInventory(new Inventory(InventoryType::TRADE)),
    mPartnerInventory(new Inventory(InventoryType::TRADE)),
    mMyItemContainer(new ItemContainer(this, mMyInventory)),
    mPartnerItemContainer(new ItemContainer(this, mPartnerInventory)),
    // TRANSLATORS: trade window money label
    mMoneyLabel(new Label(this, strprintf(_("You get %s"), ""))),
    // TRANSLATORS: trade window button
    mAddButton(new Button(this, _("Add"), "add", this)),
    mOkButton(new Button(this, "", "", this)),  // Will be filled in later
    // TRANSLATORS: trade window money change button
    mMoneyChangeButton(new Button(this, _("Change"), "money", this)),
    mMoneyField(new TextField(this)),
    mAutoAddItem(nullptr),
    mAutoAddToNick(""),
    mGotMoney(0),
    mGotMaxMoney(0),
    mAutoMoney(0),
    mAutoAddAmount(0),
    mStatus(PROPOSING),
    mOkOther(false),
    mOkMe(false)
{
    setWindowName("Trade");
    setResizable(true);
    setCloseButton(true);
    setStickyButtonLock(true);
    setDefaultSize(386, 180, ImageRect::CENTER);
    setMinWidth(310);
    setMinHeight(180);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    const Font *const fnt = mOkButton->getFont();
    int width = std::max(fnt->getWidth(CAPTION_PROPOSE),
        fnt->getWidth(CAPTION_CONFIRMED));
    width = std::max(width, fnt->getWidth(CAPTION_ACCEPT));
    width = std::max(width, fnt->getWidth(CAPTION_ACCEPTED));

    mOkButton->setWidth(8 + width);

    mMyItemContainer->addSelectionListener(this);

    ScrollArea *const myScroll = new ScrollArea(this, mMyItemContainer,
        true, "trade_background.xml");
    myScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    mPartnerItemContainer->addSelectionListener(this);

    ScrollArea *const partnerScroll = new ScrollArea(this,
        mPartnerItemContainer,
        true, "trade_background.xml");
    partnerScroll->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    // TRANSLATORS: trade window money label
    Label *const moneyLabel2 = new Label(this, _("You give:"));

    mMoneyField->setWidth(40);

    place(1, 0, mMoneyLabel);
    place(0, 1, myScroll).setPadding(3);
    place(1, 1, partnerScroll).setPadding(3);
    ContainerPlacer placer = getPlacer(0, 0);
    placer(0, 0, moneyLabel2);
    placer(1, 0, mMoneyField, 2);
    placer(3, 0, mMoneyChangeButton).setHAlign(LayoutCell::LEFT);
    placer = getPlacer(0, 2);
    placer(0, 0, mAddButton);
    placer(1, 0, mOkButton);
    Layout &layout = getLayout();
    layout.extend(0, 2, 2, 1);
    layout.setRowHeight(1, LayoutType::SET);
    layout.setRowHeight(2, 0);
    layout.setColWidth(0, LayoutType::SET);
    layout.setColWidth(1, LayoutType::SET);

    loadWindowState();
    enableVisibleSound(true);

    reset();
}

TradeWindow::~TradeWindow()
{
    delete2(mMyInventory);
    delete2(mPartnerInventory);
}

void TradeWindow::setMoney(const int amount)
{
    if (amount < 0 || amount < mGotMaxMoney)
    {
        if (config.getBoolValue("securetrades"))
        {
            close();
            return;
        }
        else
        {
            mMoneyLabel->setForegroundColorAll(getThemeColor(
                static_cast<int>(Theme::WARNING)), getThemeColor(
                static_cast<int>(Theme::WARNING_OUTLINE)));
        }
    }
    else
    {
        mMoneyLabel->setForegroundColorAll(getThemeColor(
            static_cast<int>(Theme::LABEL)), getThemeColor(
            static_cast<int>(Theme::LABEL_OUTLINE)));
        mGotMaxMoney = amount;
    }

    mGotMoney = amount;
    // TRANSLATORS: trade window money label
    mMoneyLabel->setCaption(strprintf(_("You get %s"),
        Units::formatCurrency(amount).c_str()));
    mMoneyLabel->adjustSize();
}

void TradeWindow::addItem(const int id,
                          const int type,
                          const bool own,
                          const int quantity,
                          const uint8_t refine,
                          const unsigned char color,
                          const Identified identified,
                          const Damaged damaged,
                          const Favorite favorite) const
{
    Inventory *inv = own ? mMyInventory : mPartnerInventory;
    inv->addItem(id,
        type,
        quantity,
        refine,
        color,
        identified,
        damaged,
        favorite,
        Equipm_false,
        Equipped_false);
}

void TradeWindow::addItem2(const int id,
                           const int type,
                           const int *const cards,
                           const int sz,
                           const bool own,
                           const int quantity,
                           const uint8_t refine,
                           const unsigned char color,
                           const Identified identified,
                           const Damaged damaged,
                           const Favorite favorite,
                           const Equipm equipment) const
{
    Inventory *inv = own ? mMyInventory : mPartnerInventory;
    const int slot = inv->addItem(id,
        type,
        quantity,
        refine,
        color,
        identified,
        damaged,
        favorite,
        equipment,
        Equipped_false);
    if (slot >= 0)
        inv->setCards(slot, cards, sz);
}

void TradeWindow::changeQuantity(const int index, const bool own,
                                 const int quantity) const
{
    Item *item;
    if (own)
        item = mMyInventory->getItem(index);
    else
        item = mPartnerInventory->getItem(index);
    if (item)
        item->setQuantity(quantity);
}

void TradeWindow::increaseQuantity(const int index, const bool own,
                                   const int quantity) const
{
    Item *item;
    if (own)
        item = mMyInventory->getItem(index);
    else
        item = mPartnerInventory->getItem(index);
    if (item)
        item->increaseQuantity(quantity);
}

void TradeWindow::reset()
{
    mMyInventory->clear();
    mPartnerInventory->clear();
    mOkOther = false;
    mOkMe = false;
    setMoney(0);
    mMoneyField->setEnabled(true);
    mMoneyField->setText("");
    mMoneyLabel->setForegroundColorAll(getThemeColor(
        static_cast<int>(Theme::LABEL)), getThemeColor(
        static_cast<int>(Theme::LABEL_OUTLINE)));
    mAddButton->setEnabled(true);
    mMoneyChangeButton->setEnabled(true);
    mGotMoney = 0;
    mGotMaxMoney = 0;
    setStatus(PREPARING);
}

void TradeWindow::receivedOk(const bool own)
{
    if (own)
        mOkMe = true;
    else
        mOkOther = true;

    if (mOkMe && mOkOther)
        setStatus(ACCEPTING);
}

void TradeWindow::tradeItem(const Item *const item, const int quantity,
                            const bool check) const
{
    if (check && !checkItem(item))
        return;

    tradeHandler->addItem(item, quantity);
}

void TradeWindow::valueChanged(const SelectionEvent &event)
{
    if (!mMyItemContainer || !mPartnerItemContainer)
        return;

    /* If an item is selected in one container, make sure no item is selected
     * in the other container.
     */
    if (event.getSource() == mMyItemContainer &&
        mMyItemContainer->getSelectedItem())
    {
        mPartnerItemContainer->selectNone();
    }
    else if (mPartnerItemContainer->getSelectedItem())
    {
        mMyItemContainer->selectNone();
    }
}

void TradeWindow::setStatus(const Status s)
{
    if (s == mStatus)
        return;
    mStatus = s;

    switch (s)
    {
        case PREPARING:
            mOkButton->setCaption(CAPTION_PROPOSE);
            mOkButton->setActionEventId("ok");
            break;
        case PROPOSING:
            mOkButton->setCaption(CAPTION_CONFIRMED);
            mOkButton->setActionEventId("");
            break;
        case ACCEPTING:
            mOkButton->setCaption(CAPTION_ACCEPT);
            mOkButton->setActionEventId("trade");
            break;
        case ACCEPTED:
            mOkButton->setCaption(CAPTION_ACCEPTED);
            mOkButton->setActionEventId("");
            break;
        default:
            break;
    }

    mOkButton->setEnabled((s != PROPOSING && s != ACCEPTED));
}

void TradeWindow::action(const ActionEvent &event)
{
    if (!inventoryWindow)
        return;

    Item *const item = inventoryWindow->getSelectedItem();
    const std::string &eventId = event.getId();

    if (eventId == "add")
    {
        if (mStatus != PREPARING)
            return;

        if (!inventoryWindow->isWindowVisible())
        {
            inventoryWindow->setVisible(true);
            return;
        }

        if (!item)
            return;

        if (mMyInventory->getFreeSlot() == -1)
            return;

        if (!checkItem(item))
            return;

        // Choose amount of items to trade
        ItemAmountWindow::showWindow(ItemAmountWindow::TradeAdd, this, item);

        setStatus(PREPARING);
    }
    else if (eventId == "cancel")
    {
        setVisible(false);
        reset();
        PlayerInfo::setTrading(false);
        tradeHandler->cancel();
    }
    else if (eventId == "ok")
    {
        mMoneyField->setEnabled(false);
        mAddButton->setEnabled(false);
        mMoneyChangeButton->setEnabled(false);
        receivedOk(true);
        setStatus(PROPOSING);
        tradeHandler->confirm();
    }
    else if (eventId == "trade")
    {
        receivedOk(true);
        setStatus(ACCEPTED);
        tradeHandler->finish();
    }
    else if (eventId == "money")
    {
        if (mStatus != PREPARING)
            return;

        int v = atoi(mMoneyField->getText().c_str());
        const int curMoney = PlayerInfo::getAttribute(Attributes::MONEY);
        if (v > curMoney)
        {
            if (localChatTab)
            {
                // TRANSLATORS: trade error
                localChatTab->chatLog(_("You don't have enough money."),
                                      ChatMsgType::BY_SERVER);
            }
            v = curMoney;
        }
        tradeHandler->setMoney(v);
        mMoneyField->setText(strprintf("%d", v));
    }
}

void TradeWindow::close()
{
    tradeHandler->cancel();
    clear();
}

void TradeWindow::clear()
{
    mAutoAddItem = nullptr;
    mAutoAddToNick.clear();
    mAutoMoney = 0;
    mAutoAddAmount = 0;
    mGotMoney = 0;
    mGotMaxMoney = 0;
    mMoneyLabel->setForegroundColorAll(getThemeColor(
        static_cast<int>(Theme::LABEL)), getThemeColor(
        static_cast<int>(Theme::LABEL_OUTLINE)));
}

void TradeWindow::addAutoItem(const std::string &nick, Item* const item,
                              const int amount)
{
    mAutoAddToNick = nick;
    mAutoAddItem = item;
    mAutoAddAmount = amount;
}

void TradeWindow::addAutoMoney(const std::string &nick, const int money)
{
    mAutoAddToNick = nick;
    mAutoMoney = money;
}

void TradeWindow::initTrade(const std::string &nick)
{
    if (!localPlayer)
        return;

    if (!mAutoAddToNick.empty() && mAutoAddToNick == nick)
    {
        if (mAutoAddItem && mAutoAddItem->getQuantity())
        {
            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv)
            {
                const Item *const item = inv->findItem(mAutoAddItem->getId(),
                    mAutoAddItem->getColor());
                if (item)
                    tradeItem(item, mAutoAddAmount);
            }
        }
        if (mAutoMoney)
        {
            tradeHandler->setMoney(mAutoMoney);
            mMoneyField->setText(strprintf("%d", mAutoMoney));
        }
    }
    clear();
    if (!player_relations.isGoodName(nick))
        setCaptionFont(gui->getSecureFont());
}

bool TradeWindow::checkItem(const Item *const item) const
{
    const int itemId = item->getId();
    if (PlayerInfo::isItemProtected(itemId))
        return false;
    const Item *const tItem = mMyInventory->findItem(
        itemId, item->getColor());

    if (tItem && (tItem->getQuantity() > 1
        || item->getQuantity() > 1))
    {
        if (localChatTab)
        {
            // TRANSLATORS: trade error
            localChatTab->chatLog(_("Failed adding item. You can not "
                "overlap one kind of item on the window."),
                ChatMsgType::BY_SERVER);
        }
        return false;
    }
    return true;
}

bool TradeWindow::isInpupFocused() const
{
    return (mMoneyField && mMoneyField->isFocused());
}
