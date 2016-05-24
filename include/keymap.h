/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file keymap.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 10:22:39
 * @brief 
 *  
 **/




#ifndef  __KEYMAP_H_
#define  __KEYMAP_H_


#include <stdio.h>
#include <string.h>

#define BUF_MAX_LEN 1024
#define WORD_MAX_LEN   128 
#define WORD_MAX_NUM    100000
#define ITEM_MAX_NUM   128 
#define TABLE_SIZE  200000

typedef struct table {
    char bExists;
};

typedef struct doc_node {
    char id[WORD_MAX_LEN];
    int hits;
    struct doc_node *next;
}DOCNODE, *doc_list;

typedef struct key_node {
    char *pkey;
    int count;
    int pos;
    struct doc_node *next;
} KEYNODE, *key_list;

typedef struct hash_value {
    unsigned nHash;
    unsigned nHashA;
    unsigned nHashB;
    unsigned nHashStart;
    unsigned nHashPos;
}HASHVALUE;

class Keymap {
private:
    FILE *invfd;  //inverted index file
    int wordnum;

    void prepareCryptTable();
    unsigned long hashString(const char *str, unsigned long hashtype);
    void initHashValue(const char *str, HASHVALUE *&hashvalue);
    int insertString(const char *str);
    void insert(char *key, struct doc_node &dn);

public:
    Keymap(char *filepath);
    ~Keymap();
    void init();
    void destroy();

    key_list searchByString(const char *str);
};














#endif  //__KEYMAP_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
