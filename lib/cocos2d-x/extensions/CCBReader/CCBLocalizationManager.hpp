//
//  CCBLocalizationManager.hpp
//  quickcocos2dx
//
//  Created by Tianze Zhao on 16/3/10.
//  Copyright © 2016年 qeeplay.com. All rights reserved.
//

#ifndef CCBLocalizationManager_hpp
#define CCBLocalizationManager_hpp

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class CC_EX_DLL CCBLocalizationManager : public CCObject
{
public:
    CCBLocalizationManager();
    ~CCBLocalizationManager();
    virtual bool init();
    //设置多语言plist文件
    void setLocalizationFile(const char *pFileName);
    //取值
    const char *getLocalizationString(const std::string &key);
    
public:
    static CCBLocalizationManager* getInstance();
private:
    CCDictionary *mLocalizationDic;
};

NS_CC_EXT_END

#endif /* CCBLocalizationManager_hpp */
