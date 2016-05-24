/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 15:21:44
 * @brief 
 *  
 **/

#include "docmap.h"
#include "doc.h"
#include <stdio.h>

int main()
{
    Docmap docmap("../index/");
    docmap.buildDoc();
    char *docid = "5100";
    Docinfo *docinfo = docmap.getDoc(docid);
    fprintf(stdout, "%s: %s\n", docinfo->title, docinfo->url);

    return 0;
}




















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
