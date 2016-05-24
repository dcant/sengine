/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file search.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/20 17:23:00
 * @brief 
 *  
 **/



#ifndef  __SEARCH_H_
#define  __SEARCH_H_




#include <stdio.h>
#include <string>
#include <vector>
#include "priqueue.h"
#include "docmap.h"
#include "keymap.h"
#include "segment.h"
#include "hitdoc.h"

class Search {
private:
    Segment *seg;
    Docmap *docmap;
    Keymap *keymap;
    void searchKey(string key, std::vector<HitDoc> &hitdoc);
    void getKeyDocs(string key, std::vector<HitDoc> &hitdoc);

public:
    int hitcounts;
    Search(char *indexpath, char *dictpath);
    ~Search();
    void searchAndString(char *term, std::vector<HitDoc> &hitDoc);
    void searchOrString(char *term, std::vector<HitDoc> &hitDoc);
    void printResult(std::vector<HitDoc> &hitdoc);
};










#endif  //__SEARCH_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
