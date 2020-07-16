#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char *argv[])
{
    FILE *f;
    int line_total = 0,word_total=0,character_total=0;
    for(int i = 1;i<argc;i++){
            int line = 0, word = 0, character = 0;
        f = fopen(argv[i], "r");
        if(f==NULL){
            printf("Unable to open file \n");
            return 0;
        }
        char c;
        int tmp = 0;
        while ((c = getc(f)) != EOF)
        {
            if (c == '\n')
            {
                line++;
            }

            if (c != '\n' && c != '\t' && c != ' ')
            {
                tmp = 1;
            }
            else if (tmp == 1)
            {
                word++;
                tmp = 0;
            }
            character++;
        }
        if(tmp==1){
            word++;
        }
        printf("%d %d %d %s\n", line, word, character, argv[i]);
        line_total+=line;
        word_total+=word;
        character_total+=character;
        fclose(f);
    }
    if(argc>2){
        printf("%d %d %d total\n", line_total, word_total, character_total);
    }
    return 0;
}