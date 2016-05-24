/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file segment.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/18 17:26:28
 * @brief 
 *  
 **/

#include "segment.h"


Segment::Segment()
{
    scw = new Scw();
    if (!scw->LoadDict("../worddict/")) {
        fprintf(stderr, "Failed to load dict\n");
        exit(-1);
    }
    u_int scw_out_flag = SCW_OUT_ALL | SCW_OUT_PROP;
    if (!scw->InitBuffer(512000, scw_out_flag)) {
        fprintf(stderr, "Init buffer error\n");
        exit(-1);
    }
    index = 0;
    term_count = 0;
}

Segment::~Segment()
{
    delete scw;
}

void Segment::segment(char *str, int len)
{
    scw->SegmentWord(str, len);
    index = 0;
    term_count = scw->GetTermCount(0);
}

bool Segment::hasNext()
{
    return index != term_count;
}

char* Segment::next()
{
    scw->ReadTerm(0, index, word, sizeof(word));
    index++;
    return word;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
