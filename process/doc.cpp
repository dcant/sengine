/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file doc.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 14:20:55
 * @brief 
 *  
 **/

#include "doc.h"
#include <string.h>
#include <stdlib.h>
#define TABLE_SIZE 20000


Doc::Doc()
{
    init();
}

Doc::~Doc()
{
    destroy();
}

void Doc::init()
{
    dict = (hashht_t *)malloc(sizeof(hashht_t));
    dict->size = TABLE_SIZE;
    dict->bucket = (hashentry_t **)malloc(TABLE_SIZE * sizeof(hashentry_t *));
    memset(dict->bucket, 0, TABLE_SIZE * sizeof(hashentry_t *));
}

void Doc::destroy()
{
    hashentry_t *entry, *next;
    for (int pos = 0; pos < TABLE_SIZE; pos++) {
        entry = dict->bucket[pos];
        while (entry) {
            next = entry->next;
            free(entry->key);
            free(entry->docinfo);
            free(entry);
            entry = next;
        }
    }
    free(dict->bucket);
    free(dict);
}

unsigned Doc::hashfunc(char *key)
{
    char *str = key;
    unsigned int hash = 5381;
    while (*str != 0) {
        hash += ((hash << 5) + hash) + *str;
        str++;
    }

    return hash;
}

void Doc::insert(char *key, Docinfo &docinfo)
{
    unsigned int hashcode = hashfunc(key);
    int pos = hashcode % TABLE_SIZE;

    hashentry_t *entry = (hashentry_t *)malloc(sizeof(hashentry_t));
    entry->key = (char *)malloc(strlen(key));
    strcpy(entry->key, key);
    entry->docinfo = (Docinfo *)malloc(sizeof(Docinfo));
    strcpy(entry->docinfo->title, docinfo.title);
    strcpy(entry->docinfo->url, docinfo.url);

    entry->next = dict->bucket[pos];
    dict->bucket[pos] = entry;
}

Docinfo* Doc::findById(char *docid)
{
    unsigned int hashcode = hashfunc(docid);
    int pos = hashcode % TABLE_SIZE;

    hashentry_t *entry = dict->bucket[pos];
    while (entry) {
        if (strcmp(entry->key, docid) == 0)
            break;
        else
            entry = entry->next;
    }

    if (entry)
        return entry->docinfo;
    else
        return NULL;
}















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
