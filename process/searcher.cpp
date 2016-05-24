/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file searcher.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/22 16:57:27
 * @brief 
 *  
 **/


#include "search.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stdout, "Usage: %s indexpath dictpath\n", argv[0]);
        exit(-1);
    }

    Search sc(argv[1], argv[2]);

    std::vector<HitDoc> hitdoc;
    while (1) {
        char line[256];
        if (fgets(line, 256, stdin)) {
            int len = strlen(line);
            while (line[len - 1] == '\r' || line[len - 1] == '\n')
                len--;
            line[len] = 0;
            hitdoc.clear();
            fprintf(stdout, "Query: \"%s\"\n", line);
            sc.searchAndString(line, hitdoc);
            sc.printResult(hitdoc);
        }
    }

    return 0;
}
        


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
