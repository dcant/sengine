/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file index.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/19 16:19:48
 * @brief 
 *  
 **/

#include "index.h"
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

Index::Index(char *file, char *indexpath, char *worddictpath)
{
    seg = new Segment(worddictpath);
    ori = fopen(file, "r");
    char filepath[WORD_MAX_LEN] = {0};
    strcpy(filepath, indexpath);
    strcat(filepath, "docid.ind");
    docid = fopen(filepath, "w+");
    memset(filepath, 0, sizeof(filepath));
    strcpy(filepath, indexpath);
    strcat(filepath, "forward.ind");
    fwd = fopen(filepath, "w+");
    memset(filepath, 0, sizeof(filepath));
    strcpy(filepath, indexpath);
    strcat(filepath, "invert.ind");
    inv = fopen(filepath, "w+");
    if (!ori || !docid || !fwd || !inv) {
        printf("Failed to open file\n");
        exit(-1);
    }
}

Index::~Index()
{
    delete seg;
    fclose(ori);
    fclose(docid);
    fclose(fwd);
    fclose(inv);
}

void Index::forward()
{
    char s[1024000];
    unsigned i = 0;
    while (fgets(s, 1024000, ori)) {
        char *p = strstr(s, "~EOF!~BUF!");
        
        if (p) {
            *p++ = 0;
            while (*p)
                p++;
            if (++p) {
/*
                char *ind = p;
                while (*ind != '\r')
                    ind++;
                *ind = 0;
*/
                int len = strlen(s) - 1;
                while (len > 0 && (s[len] == (char)0x0d || s[len] == (char)0x0a
                        || s[len] == ' ' || s[len] == '\t'))
                    len--;
                if (len < 0)
                    continue;
                s[len + 1] = 0;

                seg->segment(s, strlen(s));
                fprintf(fwd, "%d", i);
                while (seg->hasNext()) {
                    char *word = seg->next();
                    len = strlen(word) - 1;
                    while (len > 0 && (word[len] == (char)0x0d || word[len]
                                == (char)0x0a || word[len] == ' ' ||
                                word[len] == '\t'))
                        len--;
                    if (len < 0)
                        continue;
                    word[len + 1] = 0;
                    fprintf(fwd, "###%s", word);
                }
                fprintf(fwd, "###\n");
                fflush(fwd);

                len = strlen(p) - 1;
                while (len > 0 && (p[len] == (char)0x0d || p[len] == (char)0x0a
                        || p[len] == ' ' || p[len] == '\t'))
                    len--;
                if (len < 0)
                    continue;
                p[len + 1] = 0;

                fprintf(docid, "%d%s%s%s%s\n", i, "###", s, "###", p + 6);
                i++;
            }
        }
    }
}

void Index::invert()
{
    key_list keynode;
    char *pbuf, *move;
    pbuf = (char *)malloc(BUF_MAX_LEN);
    memset(pbuf, 0, BUF_MAX_LEN);

    HASHVALUE *hashvalue = (HASHVALUE *)malloc(sizeof(HASHVALUE));
    prepareCryptTable();

    int wordnum = 0;
    fseek(fwd, 0, SEEK_SET);                                

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
                if (strcmp(infonode->id, p->id) < 0) {
                    infonode->next = p;
                    keynode->next = infonode;
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
            fprintf(inv, "%s %d\n", words[i], keynode->count);
            doc_list infolist = keynode->next;
            for (int j = 0; j < keynode->count; j++) {
                fprintf(inv, "%s %d\n", infolist->id, infolist->hits);
                infolist = infolist->next;
            }
        }
    }
    free(pbuf);
}

void Index::prepareCryptTable()
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

unsigned long Index::hashString(const char *str, unsigned long hashtype)
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

void Index::initHashValue(const char *str, HASHVALUE *hashvalue)
{
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    hashvalue->nHash = hashString(str, HASH_OFFSET);
    hashvalue->nHashA = hashString(str, HASH_A);
    hashvalue->nHashB = hashString(str, HASH_B);
    hashvalue->nHashStart = hashvalue->nHash % TABLE_SIZE;
    hashvalue->nHashPos = hashvalue->nHashStart;
}

key_list Index::searchByString(const char *str, HASHVALUE *hashvalue)
{
    unsigned int nHash = hashvalue->nHash;
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

key_list Index::searchByIndex(unsigned ind)
{
    unsigned int nHashPos = ind;
    if (ind < TABLE_SIZE) {       
        if(key_array[nHashPos] && strlen(key_array[nHashPos]->pkey)) {
            return key_array[nHashPos];
        }
    }

    return NULL;
}

int Index::insertString(const char *str, HASHVALUE *hashvalue)
{
    unsigned int nHash = hashvalue->nHash;
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

void Index::getItems(char *&move, int &count)
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

doc_list Index::saveItems()
{
    doc_list infolist = (doc_list)malloc(sizeof(DOCNODE));
    strcpy(infolist->id, items[0]);
    infolist->hits = 1;
    return infolist;
}


int Index::getRealString(char *pbuf)
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
