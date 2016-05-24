/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file genfwd.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 11:06:20
 * @brief 
 *  
 **/

#include "fwdindex.h"

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stdout, "Usage: %s packpath indexpath dictpath\n", argv[0]);
        exit(0);
    }
    char filepath[256] = {0};
    strcpy(filepath, argv[1]);
    strcat(filepath, "reduce.pack");
    FwdIndex index(filepath, argv[2], argv[3]);
    index.index();
    return 0;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
