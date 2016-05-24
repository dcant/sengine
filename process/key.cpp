/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file key.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 15:59:38
 * @brief 
 *  
 **/

#include "key.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef TABLE_SIZE
#define TABLE_SIZE 20000
#endif


Key::Key(char *filepath)
{
    char file[256] = {0};
    strcpy(file, filepath);
    strcat(file, "invert.ind");
    invfd = fopen(file, "r");
    if (!invfd) {
        fprintf(stderr, "Failed to open invert index file.\n");
        exit(-1);
    }
    init();
}

Key::~Key()
{
    destroy();
    fclose(invfd);
}

void Key::init()
{
    dict = (htable_t *)malloc(sizeof(htable_t));
    dict->size = TABLE_SIZE;
    dict->bucket = (hentry_t **)malloc(TABLE_SIZE * sizeof(hentry_t *));
    memset(dict->bucket, 0, TABLE_SIZE * sizeof(hentry_t *));
}

void Key::destroy()
{
    hentry_t *entry, *next;
    for (int pos = 0; pos < TABLE_SIZE; pos++) {
        entry = dict->bucket[pos];
        while (entry) {
            next = entry->next;
            free(entry->key);
            item_t *item = entry->item;
            item_t *nitem;
            while (item) {
                nitem = item->next;
                free(item);
                item = nitem;
            }
            free(entry->item);
            free(entry);
            entry = next;
        }
    }
    free(dict->bucket);
    free(dict);
}

void Key::buildKey()
{
    char key[128] = {0};
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), invfd)) {
        char *p = strstr(line, "###");
        *p = 0;
        p += 3;
        count = atoi(p);
        strcpy(key, line);
        fprintf(stdout, "%s %d\n", line, count);
        item_t it;
        for (int i = 0; i < count; i++) {
            char temp[256];
            fgets(temp, sizeof(temp), invfd);
            p = strstr(temp, " ");
            *p = 0;
            strcpy(it.docid, temp);
            it.hits = atoi(p + 1);
            insert(key, it);
        }
    }
}

void Key::insert(char *key, item_t &it)
{
    unsigned int hashcode = hashfunc(key);
    int pos = hashcode % TABLE_SIZE;

    hentry_t *hp = dict->bucket[pos];
    while (hp) {
        if (strcmp(hp->key, key) == 0) {
            item_t *ip = hp->item;
            item_t *temp = (item_t *)malloc(sizeof(item_t));
            if (temp) {
            strcpy(temp->docid, it.docid);
            temp->hits = it.hits;

            if (ip->hits < temp->hits || (ip->hits == temp->hits &&
                        atoi(ip->docid) > atoi(temp->docid))) {
                temp->next = ip;
                hp->item = temp;
                break;
            }

            item_t *pre = ip;
            ip = ip->next; 

            while (ip != NULL && ip ->hits > temp->hits) {
                pre = ip;
                ip = ip->next;
            }
    
            if (ip != NULL && ip->hits == temp->hits) {
                while (ip != NULL && ip->hits == temp->hits) {
                    if (atoi(ip->docid) < atoi(temp->docid)) {
                        pre = ip;
                        ip = ip->next;
                    } else
                        break;
                }
            }
            temp->next = pre->next;
            pre->next = temp;
            break;
            }
        } else 
            hp = hp->next;
    }
    
    if (hp == NULL) {
        hentry_t *entry = (hentry_t *)malloc(sizeof(hentry_t));
        if (entry) {
            entry->key = (char *)malloc(sizeof(key));
            strcpy(entry->key, key);
            entry->item = (item_t *)malloc(sizeof(item_t));
            strcpy(entry->item->docid, it.docid);
            entry->item->hits = it.hits;
            entry->item->next = NULL;

            entry->next = dict->bucket[pos];
            dict->bucket[pos] = entry;
        }
    }
}

item_t* Key::findByKey(char *key)
{
    unsigned int hashcode = hashfunc(key);
    int pos = hashcode % TABLE_SIZE;

    hentry_t *entry = dict->bucket[pos];
    while (entry) {
        if (strcmp(entry->key, key) == 0)
            break;
        else
            entry = entry->next;
    }

    if (entry)
        return entry->item;
    else
        return NULL;
}

unsigned Key::hashfunc(char *key)
{
    char *str = key;
    unsigned int hash = 5381;
    while (*str != 0) {
        hash += ((hash << 5) + hash) + *str;
        str++;
    }

    return hash;
}











/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
