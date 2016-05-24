/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file priqueue.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/22 15:15:48
 * @brief 
 *  
 **/




#ifndef  __PRIQUEUE_H_
#define  __PRIQUEUE_H_

#include "hitdoc.h"
#include <vector>

class PriQueue {
private:
    std::vector<std::vector<HitDoc> > reVo;
    void sort();
public:
    PriQueue();
    ~PriQueue();
    void put(std::vector<HitDoc> hitVo);
    void getResult(std::vector<HitDoc> &hitvo);
};














#endif  //__PRIQUEUE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
