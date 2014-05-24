//
//  PageScrollView.h
//  quickcocos2dx
//
//  Created by Tianze Zhao on 14-5-17.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#ifndef __quickcocos2dx__PageScrollView__
#define __quickcocos2dx__PageScrollView__

#include <iostream>
#include "CCScrollView.h"
#endif /* defined(__quickcocos2dx__PageScrollView__) */
NS_CC_EXT_BEGIN
class PageScrollView:public CCScrollView
{
public:
    PageScrollView();
    static PageScrollView* create(CCSize size, CCNode* container = NULL);
    static PageScrollView* create();
    /** override functions */
    // optional
    virtual int ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    bool isPaged() {return m_bPaged;}
    void setPaged(bool value){m_bPaged = value;}
protected: // page
    clock_t m_touchBeganTime;
    int m_touchBeganOffset;
    int m_targetPage;
    int m_currPage;
    float m_pageAccSpeed;
    bool m_bPaged;
    
    void __pageTouchBegan();
    bool __pageTouchEnd();
    void __pageTouchCancel();
    void __pageClearTouch();
};
NS_CC_EXT_END