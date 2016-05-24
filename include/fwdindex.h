/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file fwdindex.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 10:20:44
 * @brief 
 *  
 **/




#ifndef  __FWDINDEX_H_
#define  __FWDINDEX_H_

#include <stdio.h>
#include "segment.h"

class FwdIndex {
private:
    Segment *seg;
    FILE *ori;  //original file
    FILE *docid;    //map doc to docid
    FILE *fwd;  //forward index file

public:
    FwdIndex(char *file, char *filepath, char *worddictpath);
    ~FwdIndex();
    void index();
};
















#endif  //__FWDINDEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
