#include "LazyLynx.h"

#include <HTUtils.h>
#include <HTTP.h>
#include <HTAnchor.h>
#include <HTAccess.h>
#include <HTParse.h>
#include <LYCurses.h>
#include <GridText.h>
#include <LYGlobalDefs.h>
#include <LYUtils.h>
#include <LYCharSets.h>
#include <LYCharUtils.h>
#include <HTAlert.h>
#include <LYSignal.h>
#include <LYGetFile.h>
#include <LYPrint.h>
#include <LYOptions.h>
#include <LYStrings.h>
#include <LYClean.h>
#include <LYDownload.h>
#include <LYNews.h>
#include <LYMail.h>
#include <LYKeymap.h>
#include <LYBookmark.h>
#include <LYMap.h>
#include <LYList.h>
#include <LYReadCFG.h>
#include <LYHistory.h>
#include <LYPrettySrc.h>

#include <LYexit.h>
#include <LYLeaks.h>

int lazylynx_funny_base_52_decode(char *p);
int lazylynx_follow_link_base(int c, int cur, DocInfo *doc, int *num, BOOL want_go);

int lazylynx_follow_link_select(int c, int cur, DocInfo *doc, int *num) {
	return lazylynx_follow_link_base(c, cur, doc, num, 1);
}
int lazylynx_follow_link_click(int c, int cur, DocInfo *doc, int *num) {
	return lazylynx_follow_link_base(c, cur, doc, num, 0);
}

int lazylynx_funny_base_52_decode(char *p) {
	//retval = atoi(p);
	int retval = 0;
	int index;
	while (*p) {
		retval *= NUM_NUMBERERS;
		for (index=0; index<NUM_NUMBERERS; index++) {
			if (LINK_NUMBERERS[index] == *p) break;
		}
		if (index == NUM_NUMBERERS) return 0; //they entered something illegal, 0 is a number which is interpreted as do nothing by the other functions
		retval += index + 1; //its a funny base 52 number because this has 1 added to it. So aZ is actually 52 + 52, and the ones place has same value as tens. Pretty neat I think
		p++;
	}
	return retval;
}

void lazylynx_generate_numid(char *buffer, int line_number) {
	//sprintf(buffer, "(%d{", line_number);
	unsigned char i = 1;
	int guess_line = NUM_NUMBERERS;
	while (guess_line < line_number) {
		guess_line *= NUM_NUMBERERS;
		i++;
		if (i > 29) {
			endwin();
			fprintf(stderr, "Wtf you had more than %d^29 links on your website.\n", NUM_NUMBERERS);
			exit(1); //Cannot happen if NUM_NUMBERERS is bigger than 1
		}
	}
	buffer[0]=LINK_RICE[0];
	buffer[i+1]=LINK_RICE[1];
	buffer[i+2]='\0';
	while (i) {
		buffer[i--]=LINK_NUMBERERS[ (line_number - 1) % NUM_NUMBERERS];
		line_number = (line_number - 1) / NUM_NUMBERERS;
	}
}

/*
 * The user wants to select a link or page by number.
 *
 * If follow_link_number returns DO_LINK_STUFF do_link will be run immediately
 * following its execution.
 * If follow_link_number returns DO_GOTOLINK_STUFF it has updated the passed in
 * doc for positioning on a link.
 * If follow_link_number returns DO_GOTOPAGE_STUFF it has set doc->line to the
 * top line of the desired page for displaying that page.
 * If follow_link_number returns PRINT_ERROR an error message will be given to
 * the user.
 * If follow_link_number returns DO_FORMS_STUFF some forms stuff will be done. 
 * (Not yet implemented.)
 * If follow_link_number returns DO_NOTHING nothing special will run after it.
 */
int lazylynx_follow_link_base(int c, int cur, DocInfo *doc, int *num, BOOL want_go) {
	bstring *temp = NULL;
	char *p;
	int new_top, new_link;
	int code;

	CTRACE((tfp, "follow_link_number(%d,%d,...)\n", c, cur));
	BStrCopy0(temp, "?");
	temp->str[0] = (char) c;
	*num = -1;
	_statusline(FOLLOW_LINK_NUMBER);

	/*
	 * Get the number, which is really a base 52 funny number described in that method, from the user.
	 */
	if (LYgetBString(&temp, FALSE, 120, NORECALL) < 0 ||
	isBEmpty(temp)) {
	HTInfoMsg(CANCELLED);
	return (DO_NOTHING);
	}

	/*
	 * Calculate the number
	 */
	p = temp->str;
	*num = lazylynx_funny_base_52_decode(p);

	// implied c=0

	CTRACE((tfp, "  temp=%s, *num=%d, rel='%c'\n", temp->str, *num, 0));


	/*
	 * If we have a valid number, act on it.
	 */
	if (*num > 0) {
		int info;
		char *text = NULL;

		/*
		 * Get the lname, and hightext, directly from www structures and
		 * add it to the cur link so that we can pass it transparently on
		 * to getfile(), and load new_top and new_link if we instead want
		 * to make the link number current.  These things are done so that
		 * a link can be selected anywhere in the current document, whether
		 * it is displayed on the screen or not!
		 */
		info = HTGetLinkInfo(*num,
				 want_go,
				 &new_top,
				 &new_link,
				 &text,
				 &links[cur].lname);
		if (text != NULL)
		LYSetHilite(cur, text);

		if (info == WWW_INTERN_LINK_TYPE) {
		links[cur].type = WWW_INTERN_LINK_TYPE;
		code = DO_LINK_STUFF;
		} else if (info == LINK_LINE_FOUND) {
		doc->line = new_top + 1;
		doc->link = new_link;
		code = DO_GOTOLINK_STUFF;
		} else if (info) {
		links[cur].type = WWW_LINK_TYPE;
		code = DO_LINK_STUFF;
		} else {
		code = PRINT_ERROR;
		}
	} else {
		code = PRINT_ERROR;
	}
	BStrFree(temp);
	return code;
}
