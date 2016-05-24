/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/19 16:47:30
 * @brief 
 *  
 **/

#include "index.h"

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stdout, "Usage: %s packpath indexpath dictpath\n", argv[0]);
        exit(0);
    }
    char filepath[256] = {0};
    strcpy(filepath, argv[1]);
    strcat(filepath, "reduce.pack");
    Index index(filepath, argv[2], argv[3]);
    index.forward();
    index.invert();
    return 0;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
