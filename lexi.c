#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int isKeyword(char buffer[])
{
    char keywords[][15] = {
        "auto","break","case","char","const","continue",
        "default","do","double","else","enum","extern",
        "float","for","goto","if","int","long",
        "register","return","short","signed","sizeof",
        "static","struct","switch","typedef","union",
        "unsigned","void","volatile","while"
    };

    int n = sizeof(keywords) / sizeof(keywords[0]);

    for(int i=0;i<n;i++)
    {
        if(strcmp(buffer, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main()
{
    FILE *fp;
    char ch, next;
    char buffer[100];
    int j = 0;

    char operators[] = "+-*/%=<>!";
    char special[] = ",;(){}[]";

    fp = fopen("sum.txt","r");

    if(fp == NULL)
    {
        printf("Cannot open file.\n");
        return 0;
    }

    printf("----------- OUTPUT -----------\n");

    while((ch = fgetc(fp)) != EOF)
    {
        /* Skip Single Line Comments */
        if(ch == '/')
        {
            next = fgetc(fp);

            if(next == '/')
            {
                while((ch = fgetc(fp)) != '\n' && ch != EOF);
                continue;
            }
            else if(next == '*')
            {
                char prev = 0;
                while((ch = fgetc(fp)) != EOF)
                {
                    if(prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                continue;
            }
            else
            {
                ungetc(next, fp);
                printf("/ is Operator\n");
                continue;
            }
        }

        /* Preprocessor Directive */
        if(ch == '#')
        {
            printf("Preprocessor Directive : ");
            while(ch != '\n' && ch != EOF)
            {
                printf("%c", ch);
                ch = fgetc(fp);
            }
            printf("\n");
            continue;
        }

        /* String Constant */
        if(ch == '"')
        {
            printf("String Constant : \"");

            while((ch = fgetc(fp)) != '"' && ch != EOF)
            {
                printf("%c", ch);
            }

            printf("\"\n");
            continue;
        }

        /* Character Constant */
        if(ch == '\'')
        {
            printf("Character Constant : '");

            while((ch = fgetc(fp)) != '\'' && ch != EOF)
            {
                printf("%c", ch);
            }

            printf("'\n");
            continue;
        }

        /* Identifier or Keyword */
        if(isalpha(ch) || ch == '_')
        {
            j = 0;

            do
            {
                buffer[j++] = ch;
                ch = fgetc(fp);
            }
            while(isalnum(ch) || ch == '_');

            buffer[j] = '\0';

            if(isKeyword(buffer))
                printf("%s is Keyword\n", buffer);
            else
                printf("%s is Identifier\n", buffer);

            ungetc(ch, fp);
            continue;
        }

        /* Integer Constant */
        if(isdigit(ch))
        {
            j = 0;

            do
            {
                buffer[j++] = ch;
                ch = fgetc(fp);
            }
            while(isdigit(ch));

            buffer[j] = '\0';

            printf("%s is Constant\n", buffer);

            ungetc(ch, fp);
            continue;
        }

        /* Operators */
        if(strchr(operators, ch))
        {
            printf("%c is Operator\n", ch);
            continue;
        }

        /* Special Characters */
        if(strchr(special, ch))
        {
            printf("%c is Special Character\n", ch);
            continue;
        }
    }

    fclose(fp);

    return 0;
}
