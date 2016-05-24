/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file process/search.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/20 17:31:32
 * @brief 
 *  
 **/

#include "search.h"
#include <string.h>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

Search::Search(char *indexpath, char *dictpath)
{
    seg = new Segment(dictpath);
    docmap = new Docmap(indexpath);
    docmap->buildDoc();
    keymap = new Keymap(indexpath);
    keymap->init();
}

Search::~Search()
{
    keymap->destroy();
    delete keymap;
    delete docmap;
    delete seg;
}

void Search::searchAndString(char *term, std::vector<HitDoc> &hitdoc)
{
    PriQueue pQueue;
    std::vector<string> keys;
    seg->segment(term, strlen(term));
    while (seg->hasNext()) {
        keys.push_back(string(seg->next()));
    }

/*
    if (keys.size() == 1) {
        searchKey(keys.front(), hitdoc);
        return;
    }
*/
    for (std::vector<string>::iterator it = keys.begin(); it != keys.end();
            it++) {
        std::vector<HitDoc> tempHits;
        getKeyDocs(*it, tempHits);
        pQueue.put(tempHits);
    }

    pQueue.getResult(hitdoc);
    this->hitcounts = hitdoc.size();
}

void Search::searchOrString(char *term, std::vector<HitDoc> &hitdoc)
{}

void Search::searchKey(string key, vector<HitDoc> &hitdoc)
{
    getKeyDocs(key, hitdoc);
    this->hitcounts = hitdoc.size();
}

void Search::getKeyDocs(string key, vector<HitDoc> &hitdoc)
{
    key_list kl = keymap->searchByString(key.c_str());
    if (kl) {
        doc_list p = kl->next;
        while (p) {
            HitDoc hit;
            hit.setDocid(atoi(p->id));
            hit.setHits(p->hits);
            hitdoc.push_back(hit);
            p = p->next;
        }
    }
}

void Search::printResult(vector<HitDoc> &hitdoc)
{
    char docid[32];
    fprintf(stdout, "\033[1;32;40mResults\033[0m: %d\n\n",
            this->hitcounts > 10 ? 10 : this->hitcounts);
    for (int i = 0; i < this->hitcounts && i < 10; i++) {
        HitDoc hit = hitdoc[i];
        sprintf(docid, "%d", hit.getDocid());
        Docinfo *docinfo = docmap->getDoc(docid);
        if (docinfo)
            fprintf(stdout, "[Title] %s\n[Url] %s\n\n",
                    docinfo->title, docinfo->url);
        else
            fprintf(stdout, "Docinfo get error!\n");
    }
    fprintf(stdout, "\n\n");
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
