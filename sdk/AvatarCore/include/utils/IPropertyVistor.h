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

#ifndef __IPROPERTYVISTOR__
#define __IPROPERTYVISTOR__

namespace utils {
class IObject;
class IProperty;
class PropertySet;
class PropertieDictionary;
class IPropertyVistor {
   private:
   protected:
   public:
    IPropertyVistor() {}
    virtual ~IPropertyVistor() {}

    virtual void Visit(IObject *prop) = 0;
    virtual void Visit(IProperty *prop) = 0;
    virtual void Visit(PropertySet *prop) = 0;
    virtual void Visit(PropertieDictionary *prop) = 0;
};

}  // namespace utils

#endif
