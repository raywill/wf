#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


extern char *optarg;

int debug = 0; // 调试模式

char *gDict = NULL;
int gDictPos = 0;
const int gDictMemSize = 1024 * 1024 * 50; // 50MB
int matchOutput = 1; // = 0 输出不匹配单词, = 1 输出匹配单词

void loadDict(FILE *fp) {
  char wordBuf[1024];
  while (!feof(fp)) {
    // 读入一行，应该是一个单词
    if (NULL == fgets(wordBuf, 1024, fp)) {
      break; // EOF
    }

    // 复制到全局字典
    int foundOneWord = 0;
    int idx = 0;

    while ((!foundOneWord) && gDictPos < gDictMemSize - 1) {

      char ch = wordBuf[idx];

      if (isspace(ch) || idx >= 1023) {

        gDict[gDictPos + idx] = '\0';

        // printf("found one word: %s\n", gDict +gDictPos);

        foundOneWord = 1;

        gDictPos += idx + 1; // move to next word

      } else {

        gDict[gDictPos + idx] = ch;

      }

      idx++;

    }

    // ensure last \0
    gDict[gDictPos] = '\0';

  } // end read file
}

void filterWord()
{
  const int bufSize = 1024;
  char buf[bufSize];
  while (NULL != fgets(buf, bufSize, stdin)) {
    // printf("read from stdin: %s\n", buf);
    // lookup in Dict
    int dictOffset = 0;
    size_t wordSize = strlen(buf);
    for (int i = 0; i < wordSize; ++i) {
      if (isspace(buf[i])) {
        buf[i] = '\0';
      }
    }
    wordSize = strlen(buf); // adjust

    int match = 0;
    while (dictOffset < gDictPos) {
      size_t dictWordSize = strlen(gDict + dictOffset);

      // fprintf(stderr, "%d %ld v.s. %ld, dict %s V.S. word %s\n", dictOffset, dictWordSize, wordSize,  gDict + dictOffset, buf); // MATCH, output it

      if (dictWordSize == wordSize && 0 == strcmp(gDict + dictOffset, buf)) {
        match = 1;
        break;
      }
      dictOffset += dictWordSize + 1;
    }

    if (matchOutput && match) {
      fprintf(stdout, "%s\n", buf); // MATCH, output it
    } else if ((!matchOutput) && (!match)) {
      fprintf(stdout, "%s\n", buf); // MATCH, output it
    }

  }
}

int main(int argc, char* const* argv)
{
     int ch;
     int level = 1; // default to 1
     while((ch = getopt(argc, argv, "l:v")) != -1) {
       switch(ch)
       {
         case 'l':
           level = atoi(optarg);
           // printf("level %d filter used\n", level);
           break;
        case 'v':
           matchOutput = 0;
           break;
         default:
           break;
       }
     }

    const char *homeDir = getenv("HOME");
    if (NULL == homeDir) {
      homeDir = "."; // point to current dir
    }
    char confPath[1024];
    char tmpPath[1024];
    snprintf(confPath, 1024, "%s%s", homeDir, "/.wf/");
    if (0 != access(confPath, R_OK)) {
       fprintf(stderr, "no word dict config found in %s\n", confPath);
       exit(-1);
     }

     gDict = malloc(gDictMemSize); // 50M，不够再realloc
     if (NULL == gDict) {
       fprintf(stderr, "not enough memory\n");
       exit(-1);
     }

     for (int i = 1; i <= level; ++i) {
       char fname[1024];
       snprintf(fname, 1024, "%slevel.%d", confPath, i);
       // fprintf(stderr, "reading %s\n", fname);
       FILE *fp = fopen(fname, "r");
       if (NULL == fp) {
         break; // no such dict file, just skip
       }
       loadDict(fp);
       fclose(fp);
     }

     for (int i = 0; i < gDictPos && debug; ++i) {
       printf("%c", gDict[i]);
       if (gDict[i] == '\0') printf("*");
     }

     filterWord();

     if (NULL != gDict) {
       free(gDict);
     }
     return 0;
}


