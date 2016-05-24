/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file doc.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/20 20:09:38
 * @brief 
 *  
 **/

#ifndef  __DOC_H_
#define  __DOC_H_


typedef struct Docinfo {
    char title[256];
    char url[256];
} Docinfo;

typedef struct hashentry {
    char *key;
    Docinfo *docinfo;
    struct hashentry *next;
} hashentry_t;

typedef struct hashht {
    unsigned int size;
    hashentry_t **bucket;
} hashht_t;

class Doc {
private:
    hashht_t *dict;
    void init();
    void destroy();
    unsigned int hashfunc(char *key);

public:
    Doc();
    ~Doc();
    void insert(char *key, Docinfo &docinfo);
    Docinfo* findById(char *docid);
};
















#endif  //__DOC_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
