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
#include "key.h"
#include <stdio.h>

int main()
{
    Docmap docmap("../index/");
    docmap.buildDoc();
    char *docid = "5100";
    Docinfo *docinfo = docmap.getDoc(docid);
    fprintf(stdout, "%s: %s\n", docinfo->title, docinfo->url);
    Key k("../index/");
    k.buildKey();
    item_t *it = k.findByKey("ÐÈ");
    if (it) {
        Docinfo *docinfo = docmap.getDoc(it->docid);
        fprintf(stdout, "%s: %s\n", docinfo->title, docinfo->url);
    } else {
        fprintf(stdout, "No key.\n");
    }

    FILE *fp = fopen("../index/invert.ind", "r");
    char temp[256];
    int i = 0;
    while (fgets(temp, 256, fp) && i++ < 50)
        fprintf(stdout, temp);
    fclose(fp);

    return 0;
}




















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
