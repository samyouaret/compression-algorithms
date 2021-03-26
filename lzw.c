/**
 * Author: Sami Ouaret
 * https://github.com/samyouaret/compression-algorithms
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define BUFFER_SIZE 100
typedef struct dict_node
{
    int index;
    char str[10];
} dict_node;

typedef struct dict_type
{
    int top;
    int size;
    dict_node **nodes;
} dict_type;

void lzw_compress(const char *str);
void lzw_decompress(int sequence[], int size);
char *get_str_by_index(dict_type *dict, int index);
int in_dict(dict_type *dict, char *str);
void print_dict(dict_type *dict);
dict_type *allocate_dict(size_t size);
dict_node *create_dict_node(char *str, int index);
int node_exists(dict_type *dict, int index);
void push(dict_type *dict, char *str, int index);
int get_index(dict_type *dict, char *str);
void print_address(char *str, dict_type *dict);
void print_array(int arr[], int size);

void lzw_compress(const char *str)
{
    char *current = malloc(sizeof(char) * 20);
    char next;
    char concat[10];
    int current_index = 256;
    int size = strlen(str);
    dict_type *dict = allocate_dict(size);
    // set current to first character
    strncpy(current, &str[0], 1);
    printf("output\n\n");
    int seq_index = 0;
    for (size_t i = 0; i < size - 1; i++)
    {
        next = str[i + 1];
        // concatenate current text with next char
        strcpy(concat, current);
        strncat(concat, &next, 1);
        // check if the new concatenated text in dictionary
        if (in_dict(dict, concat))
        {
            // copy concat text to current
            strcpy(current, concat);
        }
        else
        {
            // push new text to dictionary
            push(dict, concat, current_index++);
            print_address(current, dict);
            // set current text to next char
            sprintf(current, "%c", next);
        }
        printf("--------\n");
    }
    print_address(current, dict);
    printf("\n\n");
    print_dict(dict);
}
void lzw_decompress(int sequence[], int size)
{
    char element[10];
    int index = 256;
    dict_type *dict = allocate_dict(size);
    char word[10];
    printf("%c", sequence[0]);
    sprintf(element, "%c", sequence[1]);
    sprintf(word, "%c", sequence[0]);
    for (size_t i = 1; i < size; i++)
    {
        // find current char in dictionary or just get ascii code
        if (sequence[i] < 256)
        {
            sprintf(element, "%c", sequence[i]);
        }
        else
        {
            // printf("entered....\n");
            sprintf(element, "%s", get_str_by_index(dict, sequence[i]));
        }
        printf("%s", element);
        strncat(word, element, 1);
        if (in_dict(dict, word) == 0)
        {
            push(dict, word, index++);
        }
        strcpy(word, element);
    }
}

int in_dict(dict_type *dict, char *str)
{
    for (size_t i = 0; node_exists(dict, i); i++)
    {
        // printf("\n***** in node %s, needle %s \n", dict->nodes[i]->str, str);
        if (strcmp(str, dict->nodes[i]->str) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int get_index(dict_type *dict, char *str)
{
    for (size_t i = 0; node_exists(dict, i); i++)
    {
        if (strcmp(str, dict->nodes[i]->str) == 0)
        {
            return dict->nodes[i]->index;
        }
    }

    return -1;
}

char *get_str_by_index(dict_type *dict, int index)
{
    for (size_t i = 0; node_exists(dict, i); i++)
    {
        if (dict->nodes[i]->index == index)
        {
            return dict->nodes[i]->str;
        }
    }

    return NULL;
}

int node_exists(dict_type *dict, int index)
{
    return index < dict->top && index < dict->size;
}

/**
 * @return array dict_node[size]
**/
dict_type *allocate_dict(size_t size)
{
    dict_type *dict = (dict_type *)malloc(sizeof(dict_type));
    dict->nodes = (dict_node **)malloc(sizeof(dict_node *) * size);
    dict->top = 0;
    dict->size = size;

    return dict;
}

dict_node *create_dict_node(char *str, int index)
{
    dict_node *node = (dict_node *)malloc(sizeof(dict_node));
    strcpy(node->str, str);
    node->index = index;

    return node;
}

/**
 * @return array dict_node[size]
**/
void push(dict_type *dict, char *str, int index)
{
    if (dict->top == dict->size)
    {
        return;
    }
    dict->nodes[dict->top] = create_dict_node(str, index);
    dict->nodes[dict->top]->index = index;
    ++dict->top;
}

void print_address(char *str, dict_type *dict)
{
    int index = get_index(dict, str);
    if (index > -1)
    {
        printf(" @%s=%d\n", str, index);
    }
    else
    {
        printf(" @%c=%d\n", str[0], str[0]);
    }
}

void print_dict(dict_type *dict)
{
    printf("Dictionary new entries\n");
    printf("\ntext\tindex\n\n");
    for (size_t i = 0; node_exists(dict, i); i++)
    {
        printf(" %s\t %d\n", dict->nodes[i]->str, dict->nodes[i]->index);
    }
}

void print_array(int arr[], int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i != size - 1)
        {
            printf(", ");
        }
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    char *str = "SISI-ET-ISIS";
    if (argc < 2)
    {
        printf("you didn't enter a text ,will use \"%s\" as example\n", str);
        // exit(1);
    }
    else
    {
        str = argv[1];
    }
    printf("\nresult of compressing\n");
    lzw_compress(str);
    int sequence[] = {83, 73, 256, 45, 69, 84, 45, 257, 257};
    int size = ARRAY_SIZE(sequence);
    printf("\nexample of decompressing: ");
    print_array(sequence, size);
    lzw_decompress(sequence, size);
    printf("\n");

    return 0;
}
