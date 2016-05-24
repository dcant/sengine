/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file key.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 16:00:28
 * @brief 
 *  
 **/




#ifndef  __KEY_H_
#define  __KEY_H_
#include <stdio.h>

typedef struct item {
    char docid[256];
    int hits;
    struct item *next;
} item_t;

typedef struct hentry {
    char *key;
    item_t *item;
    struct hentry *next;
} hentry_t;

typedef struct htable {
    unsigned int size;
    hentry_t **bucket;
} htable_t;


class Key {
private:
    FILE *invfd;
    htable_t *dict;
    void init();
    void destroy();
    unsigned int hashfunc(char *key);
    void insert(char *key, item_t &it);

public:
    Key(char *filepath);
    ~Key();
    void buildKey();
    item_t* findByKey(char *key);
};














#endif  //__KEY_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
