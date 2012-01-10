/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "resources/spritedef.h"

#include "logger.h"

#include "resources/action.h"
#include "resources/animation.h"
#include "resources/dye.h"
#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/resourcemanager.h"

#include "configuration.h"

#include "debug.h"

SpriteReference *SpriteReference::Empty = nullptr;

Action *SpriteDef::getAction(std::string action, unsigned num) const
{
    Actions::const_iterator i = mActions.find(num);
    if (i == mActions.end() && num != 100)
        i = mActions.find(100);

    if (i == mActions.end() || !(*i).second)
        return nullptr;

    ActionMap::const_iterator it = ((*i).second)->find(action);

    if (it == ((*i).second)->end())
    {
        logger->log("Warning: no action \"%s\" defined!", action.c_str());
        return nullptr;
    }

    return (*it).second;
}

unsigned SpriteDef::findNumber(unsigned num) const
{
    unsigned min = 101;
    Actions::const_iterator it = mActions.begin();
    Actions::const_iterator it_end = mActions.end();
    for (; it != it_end; ++ it)
    {
        unsigned n = (*it).first;
        if (n >= num && n < min)
            min = n;
    }
    if (min == 101)
        return 0;
    return min;
}

SpriteDef *SpriteDef::load(const std::string &animationFile, int variant)
{
    std::string::size_type pos = animationFile.find('|');
    std::string palettes;
    if (pos != std::string::npos)
        palettes = animationFile.substr(pos + 1);

    XML::Document doc(animationFile.substr(0, pos));
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "sprite"))
    {
        logger->log("Error, failed to parse %s", animationFile.c_str());

        std::string errorFile = paths.getStringValue("sprites")
                                + paths.getStringValue("spriteErrorFile");
        if (animationFile != errorFile)
            return load(errorFile, 0);
        else
            return nullptr;
    }

    SpriteDef *def = new SpriteDef;
    def->mProcessedFiles.insert(animationFile);
    def->loadSprite(rootNode, variant, palettes);
    def->substituteActions();
    return def;
}

void SpriteDef::substituteAction(std::string complete, std::string with)
{
    Actions::const_iterator it = mActions.begin();
    Actions::const_iterator it_end = mActions.end();
    for (; it != it_end; ++ it)
    {
        ActionMap *d = (*it).second;
        if (!d)
            continue;
        if (d->find(complete) == d->end())
        {
            ActionMap::iterator i = d->find(with);
            if (i != d->end())
                (*d)[complete] = i->second;
        }
    }
}

void SpriteDef::substituteActions()
{
    substituteAction(SpriteAction::STAND, SpriteAction::DEFAULT);
    substituteAction(SpriteAction::MOVE, SpriteAction::STAND);
    substituteAction(SpriteAction::ATTACK, SpriteAction::STAND);
    substituteAction(SpriteAction::CAST_MAGIC, SpriteAction::ATTACK);
    substituteAction(SpriteAction::USE_ITEM, SpriteAction::CAST_MAGIC);
    substituteAction(SpriteAction::SIT, SpriteAction::STAND);
    substituteAction(SpriteAction::SLEEP, SpriteAction::SIT);
    substituteAction(SpriteAction::HURT, SpriteAction::STAND);
    substituteAction(SpriteAction::DEAD, SpriteAction::HURT);
    substituteAction(SpriteAction::SPAWN, SpriteAction::STAND);
}

void SpriteDef::loadSprite(XmlNodePtr spriteNode, int variant,
                           const std::string &palettes)
{
    // Get the variant
    const int variantCount = XML::getProperty(spriteNode, "variants", 0);
    int variant_offset = 0;

    if (variantCount > 0 && variant < variantCount)
    {
        variant_offset =
            variant * XML::getProperty(spriteNode, "variant_offset", 0);
    }

    for_each_xml_child_node(node, spriteNode)
    {
        if (xmlStrEqual(node->name, BAD_CAST "imageset"))
            loadImageSet(node, palettes);
        else if (xmlStrEqual(node->name, BAD_CAST "action"))
            loadAction(node, variant_offset);
        else if (xmlStrEqual(node->name, BAD_CAST "include"))
            includeSprite(node);
    }
}

void SpriteDef::loadImageSet(XmlNodePtr node, const std::string &palettes)
{
    const std::string name = XML::getProperty(node, "name", "");

    // We don't allow redefining image sets. This way, an included sprite
    // definition will use the already loaded image set with the same name.
    if (mImageSets.find(name) != mImageSets.end())
        return;

    const int width = XML::getProperty(node, "width", 0);
    const int height = XML::getProperty(node, "height", 0);
    std::string imageSrc = XML::getProperty(node, "src", "");
    Dye::instantiate(imageSrc, palettes);

    ResourceManager *resman = ResourceManager::getInstance();
    ImageSet *imageSet = resman->getImageSet(imageSrc, width, height);

    if (!imageSet)
    {
        logger->log1("Couldn't load imageset!");
        return;
    }

    imageSet->setOffsetX(XML::getProperty(node, "offsetX", 0));
    imageSet->setOffsetY(XML::getProperty(node, "offsetY", 0));
    mImageSets[name] = imageSet;
}

void SpriteDef::loadAction(XmlNodePtr node, int variant_offset)
{
    const std::string actionName = XML::getProperty(node, "name", "");
    const std::string imageSetName = XML::getProperty(node, "imageset", "");
    const unsigned hp = XML::getProperty(node, "hp", 100);

    ImageSetIterator si = mImageSets.find(imageSetName);
    if (si == mImageSets.end())
    {
        logger->log("Warning: imageset \"%s\" not defined in %s",
            imageSetName.c_str(), getIdPath().c_str());
        return;
    }
    ImageSet *imageSet = si->second;

    if (actionName == SpriteAction::INVALID)
    {
        logger->log("Warning: Unknown action \"%s\" defined in %s",
            actionName.c_str(), getIdPath().c_str());
        return;
    }
    Action *action = new Action;
    action->setNumber(hp);
    addAction(hp, actionName, action);

    // dirty hack to fix bad resources in tmw server
    if (actionName == "attack_stab")
        addAction(hp, "attack", action);

    // When first action set it as default direction
    Actions::const_iterator i = mActions.find(hp);
    if ((*i).second->size() == 1)
        addAction(hp, SpriteAction::DEFAULT, action);

    // Load animations
    for_each_xml_child_node(animationNode, node)
    {
        if (xmlStrEqual(animationNode->name, BAD_CAST "animation"))
            loadAnimation(animationNode, action, imageSet, variant_offset);
    }
}

void SpriteDef::loadAnimation(XmlNodePtr animationNode,
                              Action *action, ImageSet *imageSet,
                              int variant_offset)
{
    if (!action || !imageSet)
        return;

    const std::string directionName =
        XML::getProperty(animationNode, "direction", "");
    const SpriteDirection directionType = makeSpriteDirection(directionName);

    if (directionType == DIRECTION_INVALID)
    {
        logger->log("Warning: Unknown direction \"%s\" used in %s",
                directionName.c_str(), getIdPath().c_str());
        return;
    }

    Animation *animation = new Animation;
    action->setAnimation(directionType, animation);

    // Get animation frames
    for_each_xml_child_node(frameNode, animationNode)
    {
        const int delay = XML::getProperty(frameNode, "delay", 0);
        int offsetX = XML::getProperty(frameNode, "offsetX", 0) +
            imageSet->getOffsetX();
        int offsetY = XML::getProperty(frameNode, "offsetY", 0) +
            imageSet->getOffsetY();
        int rand = XML::getProperty(frameNode, "rand", 100);

        offsetY -= imageSet->getHeight() - 32;
        offsetX -= imageSet->getWidth() / 2 - 16;

        if (xmlStrEqual(frameNode->name, BAD_CAST "frame"))
        {
            const int index = XML::getProperty(frameNode, "index", -1);

            if (index < 0)
            {
                logger->log1("No valid value for 'index'");
                continue;
            }

            Image *img = imageSet->get(index + variant_offset);

            if (!img)
            {
                logger->log("No image at index %d", index + variant_offset);
                continue;
            }

            animation->addFrame(img, delay, offsetX, offsetY, rand);
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "sequence"))
        {
            const int start = XML::getProperty(frameNode, "start", -1);
            const int end = XML::getProperty(frameNode, "end", -1);

            if (start < 0 || end < 0)
            {
                logger->log1("No valid value for 'start' or 'end'");
                continue;
            }

            int repeat = XML::getProperty(frameNode, "repeat", 1);

            if (repeat < 1)
            {
                logger->log1("No valid value for 'repeat'");
                continue;
            }

            while (repeat > 0)
            {
                int pos = start;
                while (end >= pos)
                {
                    Image *img = imageSet->get(pos + variant_offset);

                    if (!img)
                    {
                        logger->log("No image at index %d",
                            pos + variant_offset);
                        pos ++;
                        continue;
                    }

                    animation->addFrame(img, delay, offsetX, offsetY, rand);
                    pos ++;
                }
                repeat --;
            }
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "end"))
        {
            animation->addTerminator(rand);
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "jump"))
        {
            animation->addJump(XML::getProperty(
                frameNode, "action", ""), rand);
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "label"))
        {
            std::string name = XML::getProperty(frameNode, "name", "");
            if (!name.empty())
                animation->addLabel(name);
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "goto"))
        {
            std::string name = XML::getProperty(frameNode, "label", "");
            if (!name.empty())
                animation->addGoto(name, rand);
        }
    } // for frameNode
}

void SpriteDef::includeSprite(XmlNodePtr includeNode)
{
    std::string filename = XML::getProperty(includeNode, "file", "");

    if (filename.empty())
        return;
    filename = paths.getStringValue("sprites") + filename;

    if (mProcessedFiles.find(filename) != mProcessedFiles.end())
    {
        logger->log("Error, Tried to include %s which already is included.",
            filename.c_str());
        return;
    }
    mProcessedFiles.insert(filename);

    XML::Document doc(filename);
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "sprite"))
    {
        logger->log("Error, no sprite root node in %s", filename.c_str());
        return;
    }

    loadSprite(rootNode, 0);
}

SpriteDef::~SpriteDef()
{
    // Actions are shared, so ensure they are deleted only once.
    std::set<Action*> actions;
    for (Actions::iterator i = mActions.begin(),
         i_end = mActions.end(); i != i_end; ++i)
    {
        ActionMap::iterator it = (*i).second->begin();
        ActionMap::iterator it_end = (*i).second->end();
        for (; it != it_end; ++ it)
            actions.insert(it->second);
        delete (*i).second;
    }

    for (std::set<Action*>::const_iterator i = actions.begin(),
         i_end = actions.end(); i != i_end; ++i)
    {
        delete *i;
    }

    mActions.clear();

    for (ImageSetIterator i = mImageSets.begin();
         i != mImageSets.end(); ++i)
    {
        if (i->second)
        {
            i->second->decRef();
            i->second = nullptr;
        }
    }
}

SpriteDirection SpriteDef::makeSpriteDirection(const std::string &direction)
{
    if (direction.empty() || direction == "default")
        return DIRECTION_DEFAULT;
    else if (direction == "up")
        return DIRECTION_UP;
    else if (direction == "left")
        return DIRECTION_LEFT;
    else if (direction == "right")
        return DIRECTION_RIGHT;
    else if (direction == "down")
        return DIRECTION_DOWN;
    else if (direction == "upleft")
        return DIRECTION_UPLEFT;
    else if (direction == "upright")
        return DIRECTION_UPRIGHT;
    else if (direction == "downleft")
        return DIRECTION_DOWNLEFT;
    else if (direction == "downright")
        return DIRECTION_DOWNRIGHT;
    else
        return DIRECTION_INVALID;
}

void SpriteDef::addAction(unsigned hp, std::string name, Action *action)
{
    Actions::const_iterator i = mActions.find(hp);
    if (i == mActions.end())
        mActions[hp] = new ActionMap();

    (*mActions[hp])[name] = action;
}
