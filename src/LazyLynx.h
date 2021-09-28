#ifndef LAZYLYNX
#define LAZYLYNX

#include "LYStructs.h"

#define LAZYLYNX_SCROLL_BY 12
static const char *LINK_NUMBERERS="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//static const char *LINK_NUMBERERS="123";
static const unsigned char NUM_NUMBERERS = 52; //This is at compile time so if you modify above string, count its length and put it here. If it is 0 or 1 you will have problems, don't be that guy
static const char *LINK_RICE="[]"; //There is no reason this couldn't be more than one prefix and one suffix if you wanted, however I'm too lazy to make it work like that out of the box so rice it yourself, brave coder. Just edit lazy_generate_number or whatever its called

int lazylynx_follow_link_click(int c, int cur, DocInfo *doc, int *num);
int lazylynx_follow_link_select(int c, int cur, DocInfo *doc, int *num);

void lazylynx_generate_numid(char *buffer, int line_number);
#endif
