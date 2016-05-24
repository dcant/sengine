/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file docmap.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 11:45:37
 * @brief 
 *  
 **/




#ifndef  __DOCMAP_H_
#define  __DOCMAP_H_
#include <stdio.h>
#include "doc.h"

class Docmap {
private:
    FILE *docfd;
    Doc *doc;

public:
    Docmap(char *filepath);
    ~Docmap();
    void buildDoc();
    Docinfo* getDoc(char *docid);
};














#endif  //__DOCID_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
