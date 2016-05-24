/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file scw.h
 * @author qiangsheng(com@baidu.com)
 * @date 2010/11/12 11:03:32
 * @brief 
 *  
 **/

#ifndef  __SCW_H_
#define  __SCW_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <string>
using namespace std;

#include "scwdef.h"

class Scw
{
public:
    enum
    {
        SCW_BASIC  = 0,
        SCW_WP     = 1,
        SCW_SP     = 2,
        SCW_HUMAN  = 3,
        SCW_BOOK   = 4,
        BUFFER_NUM = 5
    };

    static char* kBufferName[BUFFER_NUM];

    Scw();
    ~Scw();

    bool LoadDict(const char* word_dict_path);

    bool InitBuffer(int bytes, int flag);

    void PrintVersion();

    void SegmentWord(char* string, int len);

    string& GetString() { return string_; }

    //---------------------------------------------
    //-------------- 获取切词结果 -----------------
    int  GetTermCount(int bufid);

    void ReadTerm(int bufid, int termid, char* out, int len);

    //----------------------------------------------
    //------------- 暴力访问函数 -------------------
    scw_out_t*      get_scw_out()      { return pout;   }
    scw_worddict_t* get_scw_worddict() { return pwdict; }

private:
    scw_worddict_t* pwdict;
    scw_out_t*      pout;

    string   string_;
    u_int*   term_count_[BUFFER_NUM];
    u_int**  term_pos_[BUFFER_NUM];
    char**   term_buf_[BUFFER_NUM];
};

static FILE* gLog = stdout;

int chmop(char* line);
void PrintScw(Scw* scw);

#endif  //__SCW_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
