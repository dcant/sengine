/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file invindex.h
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 10:22:39
 * @brief 
 *  
 **/




#ifndef  __INVINDEX_H_
#define  __INVINDEX_H_


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

class InvIndex {
private:
    FILE *fwd;  //forward index file
    FILE *inv;  //inverted index file

    void prepareCryptTable();
    unsigned long hashString(const char *str, unsigned long hashtype);
    void initHashValue(const char *str, HASHVALUE *&hashvalue);
    key_list searchByString(const char *str, HASHVALUE*hashvalue);
    key_list searchByIndex(unsigned ind);
    int insertString(const char *str, HASHVALUE *hashvalue);
    void getItems(char *&move, int &count);
    doc_list saveItems();
    int getRealString(char *str);

public:
    InvIndex(char *filepath);
    ~InvIndex();
    void index();
};














#endif  //__INVINDEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
