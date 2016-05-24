/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file scw.cpp
 * @author qiangsheng(com@baidu.com)
 * @date 2010/11/12 11:05:08
 * @brief 
 *  
 **/

#include "scw.h"

char* Scw::kBufferName[5] = {"basic", "word-phrase", "subphrase", "human", "book"};

Scw::Scw()
{
    pwdict  = NULL;
    pout    = NULL;
}

Scw::~Scw()
{
    scw_destroy_worddict(pwdict);

    scw_destroy_out(pout);
}

bool Scw::LoadDict(const char* word_dict_path)
{
    pwdict = scw_load_worddict(word_dict_path);
    return (pwdict!=NULL);
}

bool Scw::InitBuffer(int bytes, int flag)
{
    pout = scw_create_out(bytes, flag);
    if (pout!=NULL)
    {
        term_count_[0] = &pout->wsbtermcount;
        term_count_[1] = &pout->wpbtermcount;
        term_count_[2] = &pout->spbtermcount;
        term_count_[3] = &pout->namebtermcount;
        term_count_[4] = &pout->bnbtermcount;

        term_pos_[0] = &pout->wsbtermpos;
        term_pos_[1] = &pout->wpbtermpos;
        term_pos_[2] = &pout->spbtermpos;
        term_pos_[3] = &pout->namebtermpos;
        term_pos_[4] = &pout->bnbtermpos;

        term_buf_[0] = &pout->wordsepbuf;
        term_buf_[1] = &pout->wpcompbuf;
        term_buf_[2] = &pout->subphrbuf;
        term_buf_[3] = &pout->namebuf;
        term_buf_[4] = &pout->booknamebuf;
        return true;
    }
    return false;
}

void Scw::PrintVersion()
{
    // 打印切词版本信息(仅新版本支持)
    char version[100];
    scw_get_wordseg_version(version, 100);
    fprintf(gLog, "worddict version is :%s\n", pwdict->version);
    fprintf(gLog, "wordseg version is :%s \n", version);
}

void Scw::SegmentWord(char* string, int len)
{
    // 拷贝原始字符串
    string_ = string;

    // 切词
    scw_segment_words(pwdict, pout, string, len);
}

int Scw::GetTermCount(int buf_id)
{
    return (int)(*(term_count_[buf_id]));
}

void Scw::ReadTerm(int bufid, int termid, char* out, int out_len)
{
    int pos = GET_TERM_POS((*term_pos_[bufid])[termid]);
    int len = GET_TERM_LEN((*term_pos_[bufid])[termid]);
    len = min(len, out_len-1);
    strncpy(out, (*term_buf_[bufid])+pos, len);
    out[len]= 0;
}

int chmop(char* line)
{
    int len = strlen(line);
    while (len > 0 && (line[len-1]=='\r') ||(line[len-1]=='\n'))
    {
      line[--len]=0;
    }

    return len;
}

void PrintScw(Scw* scw)
{
  char word[256];

  for (int bufid = 0; bufid < Scw::BUFFER_NUM; bufid++)
  {
      fprintf(gLog, "%15s: ", Scw::kBufferName[bufid]);
      for (int termid=0; termid < scw->GetTermCount(bufid); termid++)
      {
          scw->ReadTerm(bufid, termid, word, sizeof(word));
          fprintf(gLog, "%s/", word);
      }
      fprintf(gLog, "\n");
  }
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
