//
//  PageScrollView.cpp
//  quickcocos2dx
//
//  Created by Tianze Zhao on 14-5-17.
//  Copyright (c) 2014年 qeeplay.com. All rights reserved.
//

#include "PageScrollView.h"
NS_CC_EXT_BEGIN
#define TURN_PAGE_SPEED		1.20f				//designPixl/ms
#define INVALID_PAGE		0xfff
#define TURN_PAGE_MIN_OFFSET_RATIO		0.4f

PageScrollView::PageScrollView()
: m_bPaged(false)
{
    CCLog("PageScrollView");
}

PageScrollView* PageScrollView::create(CCSize size, CCNode* container/* = NULL*/)
{
    PageScrollView *pRet = new PageScrollView();
    if(pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

PageScrollView* PageScrollView::create()
{
    PageScrollView* pRet = new PageScrollView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool PageScrollView::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return false;
    }
    
    CCRect frame = getViewRect();
    
    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved          ||
        !frame.containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(touch))))
    {
        return false;
    }
    
    if (!m_pTouches->containsObject(touch))
    {
        m_pTouches->addObject(touch);
    }
    
    if (m_pTouches->count() == 1)
    { // scrolling
        m_tTouchPoint     = this->convertTouchToNodeSpace(touch);
        m_bTouchMoved     = false;
        m_bDragging     = true; //dragging started
        m_tScrollDistance = ccp(0.0f, 0.0f);
        m_fTouchLength    = 0.0f;
        
        __pageTouchBegan();
    }
    else if (m_pTouches->count() == 2)
    {
        m_tTouchPoint  = ccpMidpoint(this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
                                     this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
        m_fTouchLength = ccpDistance(m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0)),
                                     m_pContainer->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(1)));
        m_bDragging  = false;
    }
    return true;
}

void PageScrollView::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }
    if (m_pTouches->containsObject(touch))
    {
        if (m_pTouches->count() == 1 && m_bTouchMoved)
        {
            if (__pageTouchEnd()) {
                __pageClearTouch();
            }else{
                this->schedule(schedule_selector(PageScrollView::deaccelerateScrolling));
            }
        }
        m_pTouches->removeObject(touch);
    }
    
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
}

void PageScrollView::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }
    m_pTouches->removeObject(touch);
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
    
    __pageTouchCancel();
}

void PageScrollView::__pageTouchBegan()
{
    //仅在设置了分页属性, 并且只有一个滑动方向的时候, 才支持分页.
    if( !m_bPaged || ( m_eDirection != kCCScrollViewDirectionHorizontal && m_eDirection != kCCScrollViewDirectionVertical )) return ;
    
    //记录初试时间和位置
    m_touchBeganTime = clock();
    m_touchBeganOffset = m_eDirection == kCCScrollViewDirectionHorizontal ? getContentOffset().x : getContentOffset().y;
}

bool PageScrollView::__pageTouchEnd()
{
    if( !m_bPaged || ( m_eDirection != kCCScrollViewDirectionHorizontal && m_eDirection != kCCScrollViewDirectionVertical )) return false ;
    
    //constant
    const float PAGE_DISTENCE = m_eDirection == kCCScrollViewDirectionHorizontal ? getViewSize().width : getViewSize().height ;
    if( PAGE_DISTENCE <= 0 ) return false;
    
    const float MAX_PAGE = ( m_eDirection == kCCScrollViewDirectionHorizontal ? getContentSize().width : getContentSize().height ) / PAGE_DISTENCE;
    const float MIN_PAGE = 0;
    
    float currOffset = m_eDirection == kCCScrollViewDirectionHorizontal ? getContentOffset().x : getContentOffset().y;
    float deltaOffset = -(currOffset - m_touchBeganOffset);
    clock_t currTime = clock();
    float speed =  currTime != m_touchBeganTime ? deltaOffset / ( currTime - m_touchBeganTime ) : 0;
    
    
    m_targetPage = m_currPage;
    if( fabs(deltaOffset) >= TURN_PAGE_MIN_OFFSET_RATIO*PAGE_DISTENCE )
    {//滑动距离大于某一阈值.
        
        if( deltaOffset > 0 )
        {
            m_targetPage = m_currPage + 1;
        }
        else if( deltaOffset < 0 )
        {
            m_targetPage = m_currPage - 1;
        }
    }
    else if( fabs(speed) >= TURN_PAGE_SPEED )
    {//速度大于某一阈值.
        if( speed > 0 )
        {
            m_targetPage = m_currPage + 1;
        }
        else if( speed < 0 )
        {
            m_targetPage = m_currPage - 1;
        }
    }
    
    if( m_targetPage > MAX_PAGE ) m_targetPage = MAX_PAGE;
    else if( m_targetPage < MIN_PAGE ) m_targetPage = MIN_PAGE;
    
    float targetOffset = -m_targetPage*( m_eDirection == kCCScrollViewDirectionHorizontal ? getViewSize().width : getViewSize().height );
    float pageDurateion = 0.5;
    CCPoint targetPointOffset = m_eDirection == kCCScrollViewDirectionHorizontal ? ccp( targetOffset, getContentOffset().y ) : ccp(getContentOffset().x, targetOffset );
    setContentOffsetInDuration(targetPointOffset, pageDurateion);
    
    m_currPage = m_targetPage;
    
    return true;
}
void PageScrollView::__pageTouchCancel()
{
    if( !m_bPaged || ( m_eDirection != kCCScrollViewDirectionHorizontal && m_eDirection != kCCScrollViewDirectionVertical )) return ;
    
    __pageClearTouch();
    
}
void PageScrollView::__pageClearTouch()
{
    //clear所有状态
    m_touchBeganOffset = 0;
    m_touchBeganTime = 0;
    m_targetPage = m_currPage;
}

NS_CC_EXT_END