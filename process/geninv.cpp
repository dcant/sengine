/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file geninv.cpp
 * @author zangzhida(com@baidu.com)
 * @date 2016/04/21 11:07:21
 * @brief 
 *  
 **/

#include "invindex.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stdout, "Usage: %s indexpath\n", argv[0]);
        exit(0);
    }
    InvIndex index(argv[1]);
    index.index();
    return 0;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
