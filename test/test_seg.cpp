/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file test_seg.cpp
 * @author qiangsheng(com@baidu.com)
 * @date 2010/11/12 11:15:49
 * @brief 
 *  
 **/

#include "segment.h"

int main(int argc,char** argv)
{
  char  line[1024000];
  int   len = 0;

  Segment seg;

  FILE* out = stdout;

  FILE* temp = fopen("temp", "w+");

  while (fgets(line, sizeof(line), stdin))
  {
      len = chmop(line);
      if (len <= 0)
      {
          continue;
      }

      seg.segment(line, len);
      while (seg.hasNext()) {
          char *p = seg.next();
          fprintf(stdout, "%s\n", p);
          fprintf(temp, "%s\n", p);
      }
      fflush(temp);

  }

  fclose(temp);

  fclose(out);

  return 0;
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
