/************************************************************************
 *                                                                      *
 * Copyright 2020 AvatarIn Inc. Tokyo, Japan. All Rights Reserved       *
 *                                                                      *
 *       _____               __               .___                      *
 *      /  _  \___  ______ _/  |______ _______|   | ____                *
 *     /  /_\  \  \/ |__  \\   __\__  \\_  __ \   |/    \               *
 *    /    |    \   / / __ \|  |  / __ \|  | \/   |   |  \              *
 *    \____|__  /\_/ (____  /__| (____  /__|  |___|___|  /              *
 *            \/          \/          \/               \/               *
 *                                                                      *
 * Developed by: Yamen Saraiji                                          *
 ************************************************************************/

#ifndef __IOBJECT__
#define __IOBJECT__

#include "IDelegateContainer.h"
#include "PropertySet.h"
#include "RTTI.h"
#include "avatarutils_config.h"

namespace utils {

class AvaUtils_API IObject : public PropertySet {
    DECLARE_RTTI
   public:
    IObject();
    virtual ~IObject();

    virtual std::string ToString() const;
    virtual void Parse(const std::string &str);

    virtual void FillProperties();

    // virtual xml::XMLElement *loadXMLSettings(xml::XMLElement *elem);
    // virtual xml::XMLElement *exportXMLSettings(xml::XMLElement *elem);

    virtual void OnVisit(IPropertyVistor *visitor);
};

}  // namespace utils

#endif  //___IObject___
