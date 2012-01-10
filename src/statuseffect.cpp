/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "statuseffect.h"

#include "logger.h"
#include "sound.h"

#include "gui/widgets/chattab.h"

#include "utils/xml.h"

#include "configuration.h"

#include <map>

#include "debug.h"

#define STATUS_EFFECTS_FILE "status-effects.xml"

void unloadMap(std::map<int, StatusEffect *> &map);

bool StatusEffect::mLoaded = false;

StatusEffect::StatusEffect() :
    mPersistentParticleEffect(false)
{
}

StatusEffect::~StatusEffect()
{
}

void StatusEffect::playSFX()
{
    if (!mSFXEffect.empty())
        sound.playSfx(mSFXEffect);
}

void StatusEffect::deliverMessage()
{
    if (!mMessage.empty() && localChatTab)
        localChatTab->chatLog(mMessage, BY_SERVER);
}

Particle *StatusEffect::getParticle()
{
    if (!particleEngine || mParticleEffect.empty())
        return nullptr;
    else
        return particleEngine->addEffect(mParticleEffect, 0, 0);
}

AnimatedSprite *StatusEffect::getIcon()
{
    if (mIcon.empty())
    {
        return nullptr;
    }
    else
    {
        AnimatedSprite *sprite = AnimatedSprite::load(
            paths.getStringValue("sprites") + mIcon);
        if (false && sprite)
        {
            sprite->play(SpriteAction::DEFAULT);
            sprite->reset();
        }
        return sprite;
    }
}

std::string StatusEffect::getAction()
{
    if (mAction.empty())
        return SpriteAction::INVALID;
    else
        return mAction;
}


// -- initialisation and static parts --


typedef std::map<int, StatusEffect *> status_effect_map[2];

static status_effect_map statusEffects;
static status_effect_map stunEffects;
static std::map<int, int> blockEffectIndexMap;

int StatusEffect::blockEffectIndexToEffectIndex(int blockIndex)
{
    if (blockEffectIndexMap.find(blockIndex) == blockEffectIndexMap.end())
        return -1;
    return blockEffectIndexMap[blockIndex];
}

StatusEffect *StatusEffect::getStatusEffect(int index, bool enabling)
{
    return statusEffects[enabling][index];
}

StatusEffect *StatusEffect::getStunEffect(int index, bool enabling)
{
    return stunEffects[enabling][index];
}

void StatusEffect::load()
{
    if (mLoaded)
        unload();

    XML::Document doc(STATUS_EFFECTS_FILE);
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "status-effects"))
    {
        logger->log1("Error loading status effects file: "
                    STATUS_EFFECTS_FILE);
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        status_effect_map *the_map = nullptr;

        int index = atoi(XML::getProperty(node, "id", "-1").c_str());

        if (xmlStrEqual(node->name, BAD_CAST "status-effect"))
        {
            the_map = &statusEffects;
            int block_index = atoi(XML::getProperty(
                node, "block-id", "-1").c_str());

            if (index >= 0 && block_index >= 0)
                blockEffectIndexMap[block_index] = index;

        }
        else if (xmlStrEqual(node->name, BAD_CAST "stun-effect"))
            the_map = &stunEffects;

        if (the_map)
        {
            StatusEffect *startEffect = new StatusEffect;
            StatusEffect *endEffect = new StatusEffect;

            startEffect->mMessage = XML::getProperty(
                node, "start-message", "");
            startEffect->mSFXEffect = XML::getProperty(
                node, "start-audio", "");
            startEffect->mParticleEffect = XML::getProperty(
                node, "start-particle", "");

            startEffect->mIcon = XML::getProperty(node, "icon", "");
            startEffect->mAction = XML::getProperty(node, "action", "");
            startEffect->mPersistentParticleEffect = (XML::getProperty(
                node, "persistent-particle-effect", "no")) != "no";

            endEffect->mMessage = XML::getProperty(node, "end-message", "");
            endEffect->mSFXEffect = XML::getProperty(node, "end-audio", "");
            endEffect->mParticleEffect = XML::getProperty(
                node, "end-particle", "");

            (*the_map)[1][index] = startEffect;
            (*the_map)[0][index] = endEffect;
        }
    }
    mLoaded = true;
}

void unloadMap(std::map<int, StatusEffect *> &map)
{
    std::map<int, StatusEffect *>::iterator it;

    for (it = map.begin(); it != map.end(); ++it)
        delete (*it).second;

    map.clear();
}

void StatusEffect::unload()
{
    if (!mLoaded)
        return;

    unloadMap(statusEffects[0]);
    unloadMap(statusEffects[1]);
    unloadMap(stunEffects[0]);
    unloadMap(stunEffects[1]);

    mLoaded = false;
}
