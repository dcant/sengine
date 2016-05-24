/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file invindex.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 10:27:08
 * @brief 
 *  
 **/


#include "invindex.h"
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

InvIndex::InvIndex(char *indexpath)
{
    char filepath[WORD_MAX_LEN] = {0};
    strcpy(filepath, indexpath);
    strcat(filepath, "forward.ind");
    fwd = fopen(filepath, "r");
    memset(filepath, 0, sizeof(filepath));
    strcpy(filepath, indexpath);
    strcat(filepath, "invert.ind");
    inv = fopen(filepath, "w+");
    if (!fwd || !inv) {
        printf("Failed to open file\n");
        exit(-1);
    }
}

InvIndex::~InvIndex()
{
    fclose(fwd);
    fclose(inv);
}

void InvIndex::index()
{
    key_list keynode;
    char *pbuf, *move;
    pbuf = (char *)malloc(BUF_MAX_LEN);
    memset(pbuf, 0, BUF_MAX_LEN);

    HASHVALUE *hashvalue = (HASHVALUE *)malloc(sizeof(HASHVALUE));
    prepareCryptTable();

    int wordnum = 0;

    while (fgets(pbuf, BUF_MAX_LEN, fwd)) {
        int count = 0;
        move = pbuf;
        if (getRealString(pbuf) <= 1)
            continue;
        while (move < pbuf + strlen(pbuf) - 1) {
            getItems(move, count);
        }
        for (int i = 1; i < count; i++) {
            initHashValue(items[i], hashvalue);
            if (keynode = searchByString(items[i], hashvalue)) {
                doc_list infonode = saveItems();
                doc_list p = keynode->next;
                if (atoi(infonode->id) < atoi(p->id)) {
                    infonode->next = p;
                    keynode->next = infonode;
                } else if (atoi(infonode->id) == atoi(p->id)) {
                    p->hits++;
                } else {
                    doc_list pre = p;
                    p = p->next;
                    while (p) {
                        if (atoi(infonode->id) > atoi(p->id)) {
                            p = p->next;
                            pre = pre->next;
                        } else if(atoi(infonode->id) == atoi(p->id)) {
                            p->hits++;
                            break;
                        } else {
                            infonode->next = p;
                            pre->next = infonode;
                            keynode->count++;
                            break;
                        }
                    }
                    if (p == NULL) {
                        infonode->next = p;
                        pre->next = infonode;
                        keynode->count++;
                    }
                }
            } else {
                int pos = insertString(items[i], hashvalue);
                keynode = key_array[pos];
                doc_list infolist = saveItems();
                infolist->next = NULL;
                keynode->next = infolist;
                if (pos != -1) {
                    strcpy(words[wordnum++], items[i]);
                }
            }
        }
    }

    for (int i = 0; i < wordnum; i++) {
        initHashValue(words[i], hashvalue);
        keynode = searchByString(words[i], hashvalue);
        if (keynode != NULL) {
            fprintf(inv, "%s###%d\n", words[i], keynode->count);
            doc_list infolist = keynode->next;
            for (int j = 0; j < keynode->count; j++) {
                fprintf(inv, "%s %d\n", infolist->id, infolist->hits);
                infolist = infolist->next;
            }
        }
    }
    free(pbuf);
}

void InvIndex::prepareCryptTable()
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

unsigned long InvIndex::hashString(const char *str, unsigned long hashtype)
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

void InvIndex::initHashValue(const char *str, HASHVALUE *&hashvalue)
{
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    hashvalue->nHash = hashString(str, HASH_OFFSET);
    hashvalue->nHashA = hashString(str, HASH_A);
    hashvalue->nHashB = hashString(str, HASH_B);
    hashvalue->nHashStart = hashvalue->nHash % TABLE_SIZE;
    hashvalue->nHashPos = hashvalue->nHashStart;
}

key_list InvIndex::searchByString(const char *str, HASHVALUE *hashvalue)
{
    //unsigned int nHash = hashvalue->nHash;
    unsigned int nHashA = hashvalue->nHashA;
    unsigned int nHashB = hashvalue->nHashB;
    unsigned int nHashStart = hashvalue->nHashStart;
    unsigned int nHashPos = hashvalue->nHashPos;

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

key_list InvIndex::searchByIndex(unsigned ind)
{
    unsigned int nHashPos = ind;
    if (ind < TABLE_SIZE) {       
        if(key_array[nHashPos] && strlen(key_array[nHashPos]->pkey)) {
            return key_array[nHashPos];
        }
    }

    return NULL;
}

int InvIndex::insertString(const char *str, HASHVALUE *hashvalue)
{
    unsigned int nHashA = hashvalue->nHashA;
    unsigned int nHashB = hashvalue->nHashB;
    unsigned int nHashStart = hashvalue->nHashStart;
    unsigned int nHashPos = hashvalue->nHashPos;

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

void InvIndex::getItems(char *&move, int &count)
{
    char *front = move;
    int len;
    move = strstr(move, "###");

    if (move) {
        len = move - front;
        strncpy(items[count], front, len);
    }
    items[count][len] = '\0';
    count++;

    move += 3;
}

doc_list InvIndex::saveItems()
{
    doc_list infolist = (doc_list)malloc(sizeof(DOCNODE));
    strcpy(infolist->id, items[0]);
    infolist->hits = 1;
    return infolist;
}


int InvIndex::getRealString(char *pbuf)
{
    int len = strlen(pbuf) - 1;
    while (len > 0 && (pbuf[len] == (char)0x0d || pbuf[len] == (char)0x0a
                || pbuf[len] == ' ' || pbuf[len] == '\t')) {
        len--;
    }

    if (len < 0) {
        *pbuf = 0;
        return len;
    }
    pbuf[len + 1] = 0;

    return len + 1;
}















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
