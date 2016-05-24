/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file docmap.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 11:50:48
 * @brief 
 *  
 **/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "doc.h"
#include "docmap.h"

#define MAX_LINE_LENGTH 1024
#define MAX_WORD_LENGTH 128 

Docmap::Docmap(char *filepath)
{
    char file[256] = {0};
    strcpy(file, filepath);
    strcat(file, "docid.ind");
    docfd = fopen(file, "r");
    if (!docfd) {
        fprintf(stdout, "Failed to open file.\n");
        exit(-1);
    }
    doc = new Doc();
}

Docmap::~Docmap()
{
    delete doc;
    fclose(docfd);
}

void Docmap::buildDoc()
{
    char line[MAX_LINE_LENGTH] = {0};
    Docinfo docinfo;
    char docid[MAX_WORD_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, docfd)) {
        char *move = line;
        move = strstr(move, "###");
        *move = 0;
        strcpy(docid, line);

        move += 3;
        char *p = strstr(move, "###");
        *p = 0;
        strcpy(docinfo.title, move);

        move = p + 3;
        move[strlen(move) -2] = 0;
        strcpy(docinfo.url, move);
        doc->insert(docid, docinfo);
    }
}

Docinfo* Docmap::getDoc(char *docid)
{
    return doc->findById(docid);
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
