/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file priqueue.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/22 15:20:32
 * @brief 
 *  
 **/

#include "priqueue.h"
#include <queue>

using namespace std;

PriQueue::PriQueue()
{}

PriQueue::~PriQueue()
{
    reVo.clear();
}

void PriQueue::put(std::vector<HitDoc> hitVo)
{
    if (hitVo.size() > 0)
        reVo.push_back(hitVo);
}

void PriQueue::getResult(std::vector<HitDoc> &hitVo)
{
    std::priority_queue<int, vector<HitDoc> > Pqueue;
    if (reVo.size() == 0)
                return;
    else if (reVo.size() == 1) {
        vector<HitDoc> re = reVo.front();
        for (int i = 0; i < (int)re.size(); i++)
            Pqueue.push(re[i]);
    } else {

        sort();
        std::vector<HitDoc> firstVo = reVo.front();
        HitDoc hdoc = firstVo.front();

        std::vector<HitDoc> &inVo = reVo.front();
        inVo.erase(inVo.begin());

        bool isEnd = true;
        while (isEnd) {
            bool isDoc = true;
            
            for (vector<vector<HitDoc> >::iterator it = reVo.begin() + 1;
                    it != reVo.end(); it++) {
                vector<HitDoc> &tempVo = (*it);
                HitDoc tempdoc = tempVo.front();
                if (hdoc.getDocid() == tempdoc.getDocid()) {
                    hdoc.setHits(hdoc.getHits() + tempdoc.getHits());
                    tempVo.erase(tempVo.begin());
                    if (tempVo.size() == 0 || reVo.front().size() == 0) {
                        isEnd = false;
                    }
                } else {
                    isDoc = false;
                    if (tempVo.size() == 0 || reVo.front().size() == 0) {
                            isEnd = false;
                    }
                }
            }

            if (isDoc) {
                Pqueue.push(hdoc);
            }

            if (!isEnd)
                break;

            sort();
            firstVo = (*reVo.begin());
            hdoc = firstVo.front();
            vector<HitDoc> &inVo1 = reVo.front();
            inVo1.erase(inVo1.begin());
        }
    }

    int size = Pqueue.size();
    for (int i = 0; i < size; i++) {
        HitDoc hdocx = Pqueue.top();
        hitVo.push_back(hdocx);
        Pqueue.pop();
    }
}

void PriQueue::sort()
{
    vector<HitDoc> firstVo = reVo.front();
    HitDoc hdoc = firstVo.front();
    reVo.erase(reVo.begin());
    vector<vector<HitDoc> > tempReVo;

    for (vector<vector<HitDoc> >::iterator it = reVo.begin(); it != reVo.end();
            it++) {
        vector<HitDoc> tempVo = (*it);
        HitDoc tempdoc = (*tempVo.begin());
        if (hdoc.getDocid() < tempdoc.getDocid()) {
            tempReVo.push_back(tempVo);
        } else {
            hdoc = tempVo.front();
            vector<HitDoc> tempVo1 = firstVo;
            firstVo = tempVo;
            tempVo = tempVo1;
            tempReVo.push_back(tempVo);
        }
    }

    reVo.clear();
    reVo.push_back(firstVo);

    for (vector<vector<HitDoc> >::iterator it = (tempReVo.begin());
            it != tempReVo.end(); it++) {
        reVo.push_back(*it);
    }

    tempReVo.clear();
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
