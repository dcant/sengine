/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file hitdoc.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/22 15:11:49
 * @brief 
 *  
 **/


#include "hitdoc.h"

HitDoc::HitDoc()
{}

HitDoc::~HitDoc()
{}

bool HitDoc::operator>(const HitDoc &doc) const {
    if (hits != doc.hits)
        return hits > doc.hits;
    return docid < doc.docid;
}

bool HitDoc::operator<(const HitDoc &doc) const {
    if (hits != doc.hits)
        return hits < doc.hits;
    return docid > doc.docid;
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
