/* Test driver for thwbrk
 */

#define MAXLINELENGTH 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thai/thbrk.h>
#include <thai/thwchar.h>
#include <thai/thwbrk.h>
#include <wchar.h>

int main (int argc, char* argv[])
{
  thchar_t str[MAXLINELENGTH];
  thwchar_t ustr[MAXLINELENGTH], uout[MAXLINELENGTH], unicodeCutCode[6];

  thchar_t out1[MAXLINELENGTH*2+1];
  thchar_t out2[MAXLINELENGTH];
  int pos[MAXLINELENGTH];
  int outputLength, unicodeCutCodeLength;
  int numCut, i;
  ThBrk *brk;

  brk = th_brk_new (NULL);
  if (!brk) {
    printf ("Unable to create word breaker!\n");
    exit (-1);
  }

  strcpy ((char *)str, "���ʴդ�Ѻ ��.���. ����繡�÷��ͺ����ͧ");
  printf ("Testing with input string: %s\n", str);

  printf ("Converting to Unicode...\n");
  th_tis2uni_line (str, ustr, MAXLINELENGTH);

  printf ("Calling th_brk_wbrk()...\n");
  numCut = th_brk_wbrk (brk, ustr, pos, MAXLINELENGTH);

  printf ("Total %d cut points.", numCut);
  if (numCut > 0) {
    printf ("Cut points list: %d", pos[0]);
    for (i = 1; i < numCut; i++) {
      printf (", %d", pos[i]);
    }
  }
  printf ("\n");
  if (numCut != 7) {
    printf ("Error! Should have 7 cut points.\n"
            "Test th_brk_wbrk() failed...\n");
    exit (-1);
  }

  unicodeCutCodeLength = th_tis2uni_line ((const thchar_t *) "<WBR>",
                                          (thwchar_t*) unicodeCutCode, 6);
  if (unicodeCutCodeLength != strlen ("<WBR>")) {
    printf ("Warning! Expect th_tis2uni_line() returned length %ld, got %d\n",
            (long)strlen ("<WBR>"), unicodeCutCodeLength);
  }

  printf ("Calling th_brk_wbrk_line() ....\n");
  outputLength = th_brk_wbrk_line (brk, ustr, (thwchar_t*) uout, MAXLINELENGTH,
                                   unicodeCutCode);

  printf ("Return value from th_brk_wbrk_line is %d\n", outputLength);
  printf ("Output string length is %ld\n", (long)wcslen(uout));
  if (outputLength != 75) {
    printf ("Error! Output string length != 75. "
            "Test th_brk_wbrk_line() failed...\n");
    exit (-1);
  }

  printf ("Compare with result from th_brk_line()..\n");
  th_brk_brk_line (brk, str, out1, MAXLINELENGTH*2+1, "<WBR>");
  th_uni2tis_line(uout, out2, MAXLINELENGTH);

  if (strcmp ((const char *)out1, (const char *)out2) == 0) {
    printf ("Correct! .. test th_brk_wbrk_line() passed...\n");
  } else {
    printf ("Error! Comparison of results from th_brk_brk_line() "
            "and th_brk_wbrk_line() failed.\n");
    printf ("th_brk_brk_line :\"%s\"\n", out1);
    printf ("th_brk_wbrk_line:\"%s\"\n", out2);
    printf ("Test th_brk_wbrk_line() failed...\n");
    exit (-1);
  }

  th_brk_delete (brk);

  return 0;
}
