/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file segment.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/18 16:32:01
 * @brief 
 *  
 **/




#ifndef  __SEGMENT_H_
#define  __SEGMENT_H_

#include <vector>
#include <string>
#include "scw.h"

class Segment {
private:
    Scw *scw;
    int index;
    int term_count;
    char word[256];
public:
    Segment(char *dictpath);
    ~Segment();
    void segment(char *str, int len);
    bool hasNext();
    char* next();
};














#endif  //__SEGMENT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
