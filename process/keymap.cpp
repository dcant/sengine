/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file keymap.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 10:27:08
 * @brief 
 *  
 **/


#include "keymap.h"
#include <string.h>
#include <string>
#include <stdlib.h>

unsigned long cryptTable[0x500];
int HashATable[TABLE_SIZE];
int HashBTable[TABLE_SIZE];
int HashValue[5] = {0};
char words[WORD_MAX_NUM][WORD_MAX_LEN] = {0};
char items[ITEM_MAX_NUM][WORD_MAX_LEN] = {0};

struct table HashTable[TABLE_SIZE] = {0};

key_list key_array[TABLE_SIZE];

Keymap::Keymap(char *indexpath)
{
    char filepath[WORD_MAX_LEN] = {0};
    strcpy(filepath, indexpath);
    strcat(filepath, "invert.ind");
    invfd = fopen(filepath, "r");
    if (!invfd) {
        printf("Failed to open file\n");
        exit(-1);
    }
    wordnum = 0;
}

Keymap::~Keymap()
{
    fclose(invfd);
}

void Keymap::init()
{
    char key[128] = {0};
    char line[256];
    int count = 0;
    prepareCryptTable();
    while (fgets(line, sizeof(line), invfd)) {
        char *p = strstr(line, "###");
        *p = 0;
        p += 3;
        count = atoi(p);
        strcpy(key, line);
        struct doc_node dn;
        for (int i = 0; i < count; i++) {
            char temp[256];
            fgets(temp, sizeof(temp), invfd);
            p = strstr(temp, " ");
            *p = 0;
            strcpy(dn.id, temp);
            dn.hits = atoi(p + 1);
            insert(key, dn);
        }
    }

}

void Keymap::destroy()
{
    HASHVALUE *hashvalue = (HASHVALUE *)malloc(sizeof(HASHVALUE));
    key_list keynode;
    doc_list doc, next;

    for (int i = 0; i < wordnum; i++) {
        initHashValue(words[i], hashvalue);
        keynode = searchByString(words[i]);
        doc = keynode->next;
        while (doc) {
            next = doc->next;
            free(doc);
            doc = next;
        }
    }
}

void Keymap::insert(char *key, struct doc_node &dn)
{
    key_list keynode;

    if (keynode = searchByString(key)) {
        doc_list infonode = (doc_list)malloc(sizeof(struct doc_node));
        strcpy(infonode->id, dn.id);
        infonode->hits = dn.hits;
        infonode->next = NULL;

        doc_list dp = keynode->next;

        if (atoi(dp->id) > atoi(infonode->id)) {
            infonode->next = dp;
            keynode->next = infonode;
        } else {
            doc_list pre = dp;
            dp = dp->next;

            while (dp != NULL && atoi(infonode->id) > atoi(dp->id)) {
                pre = dp;
                dp = dp->next;
            }
            infonode->next = dp;
            pre->next = infonode;
        }
/*
        if (dp->hits < infonode->hits || (dp->hits == infonode->hits &&
                    atoi(dp->id) > atoi(infonode->id))) {
            infonode->next = dp;
            keynode->next = infonode;
        }

        doc_list pre = dp;
        dp = dp->next;
        
        while (dp != NULL && dp->hits > infonode->hits) {
            pre = dp;
            dp = dp->next;
        }

        if (dp != NULL && dp->hits == infonode->hits) {
            while (dp != NULL && dp->hits == infonode->hits) {
                if (atoi(dp->id) < atoi(infonode->id)) {
                    pre = dp;
                    dp = dp->next;
                } else
                    break;
            }
        }

        infonode->next = pre->next;
        pre->next = infonode;
*/
    } else {
        int pos = insertString(key);
        keynode = key_array[pos];
        doc_list infonode = (doc_list)malloc(sizeof(struct doc_node));
        strcpy(infonode->id, dn.id);
        infonode->hits = dn.hits;
        infonode->next = NULL;
        keynode->next = infonode;
        if (pos != -1) {
            strcpy(words[wordnum++], key);
        }
    }
}


void Keymap::prepareCryptTable()
{
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

    for (index1 = 0; index1 < 0x100; index1++) {
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {
            unsigned long temp1, temp2;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);
            cryptTable[index2] = (temp1 | temp2);
        }
    }
}

unsigned long Keymap::hashString(const char *str, unsigned long hashtype)
{
    unsigned char *key  = (unsigned char *)str;
    unsigned long seed1 = 0x7FED7FED;
    unsigned long seed2 = 0xEEEEEEEE;
    int ch;

    while( *key != 0 )
    {
        ch = *key++;
        seed1 = cryptTable[(hashtype<<8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2<<5) + 3;
    }
    return seed1;
}

void Keymap::initHashValue(const char *str, HASHVALUE *&hashvalue)
{
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    hashvalue->nHash = hashString(str, HASH_OFFSET);
    hashvalue->nHashA = hashString(str, HASH_A);
    hashvalue->nHashB = hashString(str, HASH_B);
    hashvalue->nHashStart = hashvalue->nHash % TABLE_SIZE;
    hashvalue->nHashPos = hashvalue->nHashStart;
}

key_list Keymap::searchByString(const char *str)
{
    HASHVALUE *hashvalue = (HASHVALUE *)malloc(sizeof(HASHVALUE));
    initHashValue(str, hashvalue);
    unsigned int nHashA = hashvalue->nHashA;
    unsigned int nHashB = hashvalue->nHashB;
    unsigned int nHashStart = hashvalue->nHashStart;
    unsigned int nHashPos = hashvalue->nHashPos;
    free(hashvalue);

    while (HashTable[nHashPos].bExists) {
        if (HashATable[nHashPos] == (int) nHashA && HashBTable[nHashPos] == (int) nHashB) {
            break;
        } else {
            nHashPos = (nHashPos + 1) % TABLE_SIZE;
        }

        if (nHashPos == nHashStart) {
            break;
        }
    }

    if( key_array[nHashPos] && strlen(key_array[nHashPos]->pkey)) {
        return key_array[nHashPos];
    } 

    return NULL;
}

int Keymap::insertString(const char *str)
{
    HASHVALUE *hashvalue = (HASHVALUE *)malloc(sizeof(HASHVALUE));
    initHashValue(str, hashvalue);
    unsigned int nHashA = hashvalue->nHashA;
    unsigned int nHashB = hashvalue->nHashB;
    unsigned int nHashStart = hashvalue->nHashStart;
    unsigned int nHashPos = hashvalue->nHashPos;
    free(hashvalue);

    while (HashTable[nHashPos].bExists) {
        nHashPos = (nHashPos + 1) % TABLE_SIZE;

        if (nHashPos == nHashStart)
            break;
    }

    int len = strlen(str);
    if (!HashTable[nHashPos].bExists && (len < WORD_MAX_LEN)) { 
        HashATable[nHashPos] = nHashA;
        HashBTable[nHashPos] = nHashB;

        key_array[nHashPos] = (KEYNODE *) malloc (sizeof(KEYNODE) * 1);
        if(key_array[nHashPos] == NULL) {
            printf("10000 EMS ERROR !!!!\n");
            return 0;
        }

        key_array[nHashPos]->pkey = (char *)malloc(len+1);
        if(key_array[nHashPos]->pkey == NULL) {
            printf("10000 EMS ERROR !!!!\n");
            return 0;
        }

        memset(key_array[nHashPos]->pkey, 0, len+1);
        strncpy(key_array[nHashPos]->pkey, str, len);
        *((key_array[nHashPos]->pkey)+len) = 0;
        key_array[nHashPos]->pos = nHashPos;
        key_array[nHashPos]->count = 1;
        key_array[nHashPos]->next = NULL;
        HashTable[nHashPos].bExists = 1;
        return nHashPos;
    }

    if(HashTable[nHashPos].bExists)
        printf("30000 in the hash table %s !!!\n", str);
    else
        printf("90000 strkey error !!!\n");
    return -1;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
