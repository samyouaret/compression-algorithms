/**
 * Author: Sami Ouaret
 * https://github.com/samyouaret/compression-algorithms
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_HEIGHT 20

typedef struct bstnode
{
    unsigned int freq;
    char value;
    struct bstnode *left;
    struct bstnode *right;
} bstnode;

bstnode *createNewNode(int freq, char value);
void print_tree(bstnode *root);
bstnode *merge(bstnode *root1, bstnode *root2);
bstnode **allocate_tree_list(size_t size);
bstnode **build_tree_list(char *str);
int in_array(char *target, char ch);
int is_leaf(bstnode *node);
void swap(bstnode **trees, int size, int start);
void sort(bstnode **, int size);
void swap_pointers(bstnode **trees, int i, int j);
int left_compare(bstnode *node1, bstnode *node2);

typedef struct dict_node
{
    int code[MAX_HEIGHT];
    int top;
    char value;
} dict_node;

typedef struct dict_type
{
    int top;
    int size;
    dict_node **nodes;
} dict_type;

dict_type *allocate_dict(size_t size);
dict_node *create_dict_node(char ch, int code[MAX_HEIGHT], int code_size);
void push(dict_type *dict, char ch, int code[MAX_HEIGHT], int code_size);
int node_exists(dict_type *dict, int index);
dict_node *get_node(dict_type *dict, char ch);
void write_node_code(dict_node *node, int code[MAX_HEIGHT], int code_size);
void huffman_code(char *str, bstnode *root, int size);
void get_huffman_code(bstnode *root, int code[10], int top, dict_type *dict);
void print_code(dict_node *node);
void print_dict(dict_type *dict);

/**
 * build an array of bstnodes with their frequencies
  * @return array bstnode[size]
 * @arg target string to compress
**/
bstnode **build_tree_list(char *str)
{
    int length = strlen(str);
    bstnode **trees = allocate_tree_list(length);
    char visited[length];
    int j = 0;
    size_t freq;
    for (size_t i = 0; i < length; i++)
    {
        freq = 0;
        if (in_array(visited, str[i]))
        {
            continue;
        }
        for (int j = i; j < length; ++j)
        {
            if (str[i] == str[j])
                ++freq;
        }
        trees[j++] = createNewNode(freq, str[i]);
        visited[i] = str[i];
    }

    return trees;
}

/**
 * check if a char is in array
 * @return boolean 1/0
 * @arg target array to search in
 * @arg ch character to look for
**/
int in_array(char *target, char ch)
{
    for (size_t i = 0; i < sizeof(target); i++)
    {
        if (target[i] == ch)
        {
            return 1;
        }
    }

    return 0;
}

/**
 * @return array bstnode[size]
**/
bstnode **allocate_tree_list(size_t size)
{
    bstnode **trees = (bstnode **)malloc(sizeof(bstnode) * size);
    for (size_t i = 0; i < size; i++)
        trees[i] = NULL;

    return trees;
}

bstnode *huffman_compress(char *str)
{
    int size = strlen(str);
    // build chars frequencies
    bstnode **trees = build_tree_list(str);
    sort(trees, size);
    bstnode *root = NULL;
    for (size_t i = 0; i < size - 1 && trees[i + 1] != NULL; i++)
    {
        root = merge(trees[i], trees[i + 1]);
        trees[i + 1] = root;
        swap(trees, size, i + 1);
    }

    return root;
}

/**
 * create new bstnode
 * @return bstnode
**/
bstnode *createNewNode(int freq, char value)
{
    bstnode *newNode = (bstnode *)malloc(sizeof(bstnode));
    newNode->value = value;
    newNode->freq = freq;
    newNode->right = newNode->left = NULL;

    return newNode;
}

/**
 * check if a node is lead node(a node with no children)
 * @return boolean
**/
int is_leaf(bstnode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return node->left == NULL && node->right == NULL;
}

/**
 * merge two bstnodes by aggregating their frequencies(new node value is '$')
 * always left child has bigger freq than the right one
 * @return new bstnode
**/
bstnode *merge(bstnode *root1, bstnode *root2)
{
    bstnode *newRoot = createNewNode(root1->freq + root2->freq, '$');
    if (root1->freq > root2->freq)
    {
        newRoot->left = root1;
        newRoot->right = root2;
    }
    else
    {
        newRoot->left = root2;
        newRoot->right = root1;
    }

    return newRoot;
}

/**
 * sort an array of bstnodes based on their freq
 * @return void
**/
void sort(bstnode **trees, int size)
{
    int i, j;
    bstnode *temp;
    for (i = 0; i < size && trees[i] != NULL; i++)
    {
        for (j = 0; j < i; j++)
        {
            if (trees[i]->freq <= trees[j]->freq)
            {
                swap_pointers(trees, i, j);
            }
        }
    }
}

/**
 * re-order the tree list after merging two lowest nodes
 * @return void
**/
void swap(bstnode **trees, int size, int start)
{
    int i;
    bstnode *temp;
    for (i = start; i < size && trees[i + 1] != NULL; i++)
    {
        if (trees[i]->freq > trees[i + 1]->freq)
        {
            swap_pointers(trees, i, i + 1);
        }
        // swap if current alpha order is the lowest
        else if (trees[i]->freq == trees[i + 1]->freq)
        {
            if (left_compare(trees[i], trees[i + 1]))
            {
                swap_pointers(trees, i, i + 1);
            }
        }
    }
}

/**
 * swap two bstnode pointers in tree list
 * @return void
**/
void swap_pointers(bstnode **trees, int i, int j)
{
    bstnode *temp = trees[i];
    trees[i] = trees[j];
    trees[j] = temp;
}
/**
 * compare most left chars for two nodes to ensure Alpha order
 * @return void
**/
int left_compare(bstnode *node1, bstnode *node2)
{
    while (node1 != NULL && node1->value == '$')
    {
        node1 = node1->left;
    }
    while (node2 != NULL && node2->value == '$')
    {
        node2 = node2->left;
    }
    if (node1->value < node2->value)
    {
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("please enter text to compress\n");
        exit(1);
    }
    // bstnode *root = huffman_compress("BILLBEATSBEN");
    int size = strlen(argv[1]);
    bstnode *root = huffman_compress(argv[1]);
    printf("result of compressing: %s\n", argv[1]);
    huffman_code(argv[1], root, size);

    return 0;
}

dict_node *get_node(dict_type *dict, char ch)
{
    for (size_t i = 0; node_exists(dict, i); i++)
    {
        if (dict->nodes[i]->value == ch)
        {
            return dict->nodes[i];
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

dict_node *create_dict_node(char ch, int code[MAX_HEIGHT], int size)
{
    dict_node *node = (dict_node *)malloc(sizeof(dict_node));
    node->value = ch;
    node->top = 0;
    write_node_code(node, code, size);
    return node;
}

void write_node_code(dict_node *node, int code[MAX_HEIGHT], int size)
{
    node->top = 0;
    while (node->top < size)
    {
        node->code[node->top] = code[node->top];
        ++node->top;
    }
}

/**
 * @return array dict_node[size]
**/
void push(dict_type *dict, char ch, int code[MAX_HEIGHT], int code_size)
{
    if (dict->top == dict->size)
    {
        return;
    }
    dict->nodes[dict->top] = create_dict_node(ch, code, code_size);
    ++dict->top;
}

// print out the code for orginal string
void huffman_code(char *str, bstnode *root, int size)
{
    dict_type *dict = allocate_dict(size);
    int code[MAX_HEIGHT];
    get_huffman_code(root, code, 0, dict);
    dict_node *node;
    printf("\n");
    for (size_t i = 0; i < size; i++)
    {
        node = get_node(dict, str[i]);
        print_code(node);
        if (i != size - 1)
        {
            printf(",");
        }
    }
    printf("\n");
    print_dict(dict);
}
//traverse the tree to get huffman code for each letter
void get_huffman_code(bstnode *root, int code[10], int top, dict_type *dict)
{
    if (root == NULL)
    {
        return;
    }
    if (root->left)
    {
        code[top] = 0;
        get_huffman_code(root->left, code, top + 1, dict);
    }
    if (root->right)
    {
        code[top] = 1;
        get_huffman_code(root->right, code, top + 1, dict);
    }
    if (is_leaf(root))
    {
        push(dict, root->value, code, top);
    }
}

// printer functions

/**
 * print binary tree
 * @return void
**/
void print_tree(bstnode *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%c with freq %d\n", root->value, root->freq);
    print_tree(root->left);
    print_tree(root->right);
}

void print_dict(dict_type *dict)
{
    printf("\nletter\tcode\n\n");
    for (size_t i = 0; node_exists(dict, i); i++)
    {
        printf(" %c", dict->nodes[i]->value);
        printf("\t");
        print_code(dict->nodes[i]);
        printf("\n");
    }
}

void print_code(dict_node *node)
{
    if (node == NULL)
    {
        return;
    }

    for (size_t j = 0; j < node->top; j++)
    {
        printf("%d", node->code[j]);
    }
}