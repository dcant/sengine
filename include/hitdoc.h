/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file hitdoc.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/22 15:06:04
 * @brief 
 *  
 **/




#ifndef  __HITDOC_H_
#define  __HITDOC_H_


class HitDoc {
private:
    int docid;
    int hits;
public:
    HitDoc();
    ~HitDoc();
    int getDocid() {return docid;};
    void setDocid(int id) {docid = id;};
    int getHits() {return hits;};
    void setHits(int hit) {hits = hit;};
    bool operator>(const HitDoc &doc) const;
    bool operator<(const HitDoc &doc) const;
};













#endif  //__HITDOC_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
