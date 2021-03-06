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

#include "gui/windows/bankwindow.h"

#include "units.h"

#include "net/bankhandler.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

BankWindow *bankWindow = nullptr;

BankWindow::BankWindow() :
    // TRANSLATORS: bank window name
    Window(_("Bank"), false, nullptr, "bank.xml"),
    ActionListener(),
    BankListener(),
    // TRANSLATORS: bank window money label
    mBankMoneyLabel(new Label(this, strprintf(
        _("Money in bank: %s"), "            "))),
    mInputMoneyTextField(new IntTextField(this, 0, 0, 2147483647)),
    // TRANSLATORS: bank window button
    mWithdrawButton(new Button(this, _("Withdraw"), "withdraw", this)),
    // TRANSLATORS: bank window button
    mDepositButton(new Button(this, _("Deposit"), "deposit", this))
{
    setWindowName("Bank");
    setCloseButton(true);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    mBankMoneyLabel->adjustSize();
    ContainerPlacer placer = getPlacer(0, 0);
    placer(0, 0, mBankMoneyLabel, 7);
    placer(0, 1, mInputMoneyTextField, 10);
    placer(0, 2, mDepositButton, 5);
    placer(5, 2, mWithdrawButton, 5);

    setContentSize(300, 100);
    setDefaultSize(300, 100, ImageRect::CENTER, 0, 0);

    center();
    setDefaultSize();
    loadWindowState();
    reflowLayout(300);
    enableVisibleSound(true);
}

BankWindow::~BankWindow()
{
}

void BankWindow::widgetShown(const Event &event)
{
    if (event.getSource() == this)
        bankHandler->check();
}

void BankWindow::bankMoneyChanged(const int money)
{
    mBankMoneyLabel->setCaption(strprintf(_("Money in bank: %s"),
        Units::formatCurrency(money).c_str()));
}

void BankWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "deposit")
        bankHandler->deposit(mInputMoneyTextField->getValue());
    else if (eventId == "withdraw")
        bankHandler->withdraw(mInputMoneyTextField->getValue());
}
