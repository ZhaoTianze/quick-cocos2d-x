//
//  CCBLocalizationManager.cpp
//  quickcocos2dx
//
//  Created by Tianze Zhao on 16/3/10.
//  Copyright © 2016年 qeeplay.com. All rights reserved.
//

#include "CCBLocalizationManager.hpp"
NS_CC_EXT_BEGIN
static CCBLocalizationManager* s_localizationInstance = NULL;

CCBLocalizationManager* CCBLocalizationManager::getInstance()
{
    if (! s_localizationInstance) {
        s_localizationInstance = new CCBLocalizationManager();
        s_localizationInstance->init();
    }
    
    return s_localizationInstance;
}

bool CCBLocalizationManager::init()
{
    mLocalizationDic = new CCDictionary();
    return true;
}

CCBLocalizationManager::CCBLocalizationManager()
: mLocalizationDic(NULL)
{
}

CCBLocalizationManager::~CCBLocalizationManager()
{
    CCLOGINFO("cocos2d: deallocing %p", this);
    CC_SAFE_RELEASE(mLocalizationDic);
}

void CCBLocalizationManager::setLocalizationFile(const char *pFileName)
{
    CCAssert(pFileName, "Invalid texture file name");
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    std::string fullPath = fileUtils->fullPathForFilename(pFileName);
    CCDictionary * dict = CCDictionary::createWithContentsOfFile(pFileName);
    CCAssert(dict, "CCBLocationManager: File Could not be found");
    CC_SAFE_RELEASE(mLocalizationDic);
    mLocalizationDic = dict;
    mLocalizationDic->retain();
}

const char* CCBLocalizationManager::getLocalizationString(const std::string &key){
    const CCString *value = mLocalizationDic->valueForKey(key);
    if (value->length() == 0) {
        return key.c_str();
    }
    return value->getCString();
}

NS_CC_EXT_END