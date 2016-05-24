/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file fwdindex.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 10:17:47
 * @brief 
 *  
 **/

#include "fwdindex.h"
#include <string.h>
#include <stdlib.h>




FwdIndex::FwdIndex(char *file, char *indexpath, char *worddictpath)
{
    seg = new Segment(worddictpath);
    ori = fopen(file, "r");
    char filepath[256] = {0};
    strcpy(filepath, indexpath);
    strcat(filepath, "docid.ind");
    docid = fopen(filepath, "w+");
    memset(filepath, 0, sizeof(filepath));
    strcpy(filepath, indexpath);
    strcat(filepath, "forward.ind");
    fwd = fopen(filepath, "w+");
    if (!ori || !docid || !fwd) {
        printf("Failed to open file\n");
        exit(-1);
    }
}

FwdIndex::~FwdIndex()
{
    delete seg;
    fclose(ori);
    fclose(docid);
    fclose(fwd);
}

void FwdIndex::index()
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
















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
