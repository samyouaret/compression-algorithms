/**
 * Author: Sami Ouaret
 * https://github.com/samyouaret/compression-algorithms
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 100
#define HEADER_CHAR '$'
#define MIN_REPEAT 3

char *rle_compress(const char *str, size_t min_repeat, char header)
{
    char *result = malloc(sizeof(char) * BUFFER_SIZE);
    char current = '\0';
    int repeat = 0;
    int j = 0;
    result[j++] = header;
    for (size_t i = 0; i < strlen(str); i++)
    {
        repeat = 1;
        current = str[i];
        while (current == str[i + repeat])
        {
            ++repeat;
        }
        if (repeat >= min_repeat)
        {
            i += repeat - 1;            // repeat started from 1 i started form 0
            result[j++] = '$';          // convert it to char
            result[j++] = repeat + '0'; // convert it to char
        }
        result[j++] = current;
    }

    return result;
}

char *rle_uncompress(const char *str)
{
    char *result = malloc(sizeof(char) * BUFFER_SIZE);
    int repeat = 0;
    int j = 0;
    char header = str[0];
    for (size_t i = 1; i < strlen(str); i++)
    {
        if (str[i] == header)
        {
            ++i;
            // convert freq to integer
            repeat = str[i] - '0';
            //write result
            while (repeat-- > 0)
            {
                result[j++] = str[i + 1];
            }
            ++i;
        }
        else
        {
            result[j++] = str[i];
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    char *str = "AAAAABBBABBBBBCCCDDDCCCX";
    if (argc < 2)
    {
        printf("you didn't enter a text ,will use \"%s\" as example\n", str);
        // exit(1);
    }
    else
    {
        str = argv[1];
    }

    char *result = rle_compress(str, MIN_REPEAT, HEADER_CHAR);
    printf("result of compressing %s is %s\n", str, result);
    printf("result of uncompressing %s is %s\n", result, rle_uncompress(result));

    return 0;
}
