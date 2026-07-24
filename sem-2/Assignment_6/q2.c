#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char *str)
{
    char *kw[] = {"int","float","if","else","while","do","return","char","double","for","break","continue","void"};
    int n = sizeof(kw)/sizeof(kw[0]);
    for(int i = 0; i < n; i++)
        if(strcmp(str, kw[i]) == 0) return 1;
    return 0;
}

int isOperator(char c)
{
    char ops[] = "+-*/%=&|!<>^";
    for(int i = 0; ops[i]; i++)
        if(c == ops[i]) return 1;
    return 0;
}

int isPunct(char c)
{
    char p[] = "();{},[]";
    for(int i = 0; p[i]; i++)
        if(c == p[i]) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 2) return 0;

    FILE *fp = fopen(argv[1], "r");
    if(!fp) return 0;

    char line[1024];

    while(fgets(line, sizeof(line), fp))
    {
        int i = 0;

        while(line[i])
        {
            if(isspace(line[i]))
            {
                printf("whitespace\n");
                i++;
            }
            else if(line[i] == '/' && line[i+1] == '/')
            {
                printf("comment: %s", &line[i]);
                break;
            }
            else if(line[i] == '"')
            {
                int j = i + 1;
                while(line[j] && line[j] != '"') j++;
                char temp[256];
                strncpy(temp, &line[i], j - i + 1);
                temp[j - i + 1] = '\0';
                printf("string: %s\n", temp);
                i = j + 1;
            }
            else if(isalpha(line[i]) || line[i] == '_')
            {
                int j = i;
                while(isalnum(line[j]) || line[j] == '_') j++;
                char temp[256];
                strncpy(temp, &line[i], j - i);
                temp[j - i] = '\0';

                if(isKeyword(temp)) printf("keyword: %s\n", temp);
                else printf("identifier: %s\n", temp);

                i = j;
            }
            else if(isdigit(line[i]) || (line[i]=='-' && isdigit(line[i+1])))
            {
                int j = i;
                while(isdigit(line[j]) || line[j] == '.') j++;
                char temp[256];
                strncpy(temp, &line[i], j - i);
                temp[j - i] = '\0';
                printf("number: %s\n", temp);
                i = j;
            }
            else if(isOperator(line[i]))
            {
                printf("operator: %c\n", line[i]);
                i++;
            }
            else if(isPunct(line[i]))
            {
                printf("punctuation: %c\n", line[i]);
                i++;
            }
            else
            {
                printf("unknown: %c\n", line[i]);
                i++;
            }
        }
    }

    fclose(fp);
    return 0;
}
