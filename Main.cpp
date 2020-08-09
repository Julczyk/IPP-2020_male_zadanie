
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define EXTEND_RATIO 2/3

#define SEPARATOR 1
#define LINE_END 2
#define FILE_END 3

#define SAME 0
#define ORDER 1
#define REVERSED 2

typedef struct BST
{
    char* name;     //nazwa
    int lenght;         //dlugosc nazwy
    struct BST* up;      //rodzic
    struct BST* sub;     //zawartosc
    struct BST* left;    //syn
    struct BST* right;   //syn
} BST;

BST* init_BST()
{
    BST* tree = malloc(sizeof(BST));
    tree->up = NULL;
    tree->sub = NULL;
    tree->left = NULL;
    tree->right = NULL;
    tree->lenght = 0;
    tree->name = malloc(sizeof(char));
    tree->name[0] = ' ';
    return tree;
}



char* write_word(int* lenght, int* end_type)
{
    *end_type = SEPARATOR; //czy koniec linii/tekstu
    char* text = NULL;
    int size = 0;
    int letter;
    int i = 0;
    for (i = 0; ((letter = getchar()) != '\n' && letter != EOF && letter != ' ' && letter != '\t' && letter != '\v' && letter != '\f' && letter != '\r'); i++)
    {
        printf("K");
        if (i == size)
        {
            printf("C");
            size = 10 + size * EXTEND_RATIO;
            printf("C");
            text = realloc(text, size * sizeof(*text));
            printf("C");
        }
        printf("X");
        text[i] = letter;
        printf("Y");
    }
    *lenght = i - 1;
    printf("A");
    if (letter == '\n')        *end_type = LINE_END;
    else if (letter == EOF)    *end_type = FILE_END;

    printf("\nRaport:\n - Dlugosc slowa - %d", *lenght);
    print_word(*text, lenght);

    return text;
}

void print_word(char* text, int lenght)
{
    for (int i = 0; i < lenght; i++)
        putchar(text[i]);
    printf("\n");
    return;
}

void delete_word(char* text, int lenght)
{
    for (int i = 0; i < lenght; i++)
        free(&text[i]);

    return;
}

int lexicographic(char* A, int lenA, char* B, int lenB)
{
    int i = 0;
    while (i < lenA && i < lenB && A[i] == B[i])
        i++;

    if (i == lenA && i == lenB)          return SAME;
    else if (i == lenA || A[i] < B[i])   return ORDER;
    else                            return REVERSED;
}

BST* create_node(char* name, int lenght, BST* parent)
{
    BST* node = malloc(sizeof(BST));
    node->name = name;
    node->lenght = lenght;
    node->left = NULL;
    node->right = NULL;
    node->sub = init_BST();

    return node;
}

void print_tree(BST* node)
{
    if (node == NULL)
        return;

    print_tree(node->left);
    print_word(node->name, node->lenght);
    putchar('\n');
    print_tree(node->right);

    return;
}

void clear_tree(BST* tree)
{
    if (tree == NULL)
        return;

    clear_tree(tree->left);
    clear_tree(tree->right);
    clear_tree(tree->sub);
    delete_word(tree->name, tree->lenght);
    free(tree);
    return;
}

void clear_node(BST* node)
{
    BST* parent = node->up;
    BST* tmp = node->left;

    clear_tree(node->sub);
    delete_word(node->name, node->lenght);

    if (node->left == NULL)
    {
        if (node == parent->left)
            parent->left = node->right;
        else
            parent->right = node->right;

        node->right->up = parent;
    }
    else
    {
        if (node == parent->left)
            parent->left = node->left;
        else
            parent->right = node->left;

        while (tmp->right != NULL)
            tmp = tmp->right;
        tmp->right = node->right;

        node->left->up = parent;
        node->right->up = tmp;
    }

    free(node);
    return;
}

void DEBUG_tree_print_whole(BST* tree, int step)
{
    if (tree == NULL)          return;
    if (tree->name <= 0)       return; //prawdopodobnie <0

    DEBUG_tree_print_whole(tree->left, step);

    for (int i = 0; i < step; i++)
        printf("    ");
    print_word(tree->name, tree->lenght);
    putchar(' ');

    DEBUG_tree_print_whole(tree->left, step + 1);

    DEBUG_tree_print_whole(tree->right, step);

    return;
}

BST* find_node(BST* tree, char* name, int lenght, bool make_new)
{
    BST* final = tree;
    int lex = lexicographic(name, lenght, tree->name, tree->lenght);
    while (final != NULL && lex != SAME)
    {

        if (lex == ORDER)
        {
            if (final->left == NULL && make_new)
                final->left = create_node(name, lenght, final);

            final = final->left;
        }
        else if (lex == REVERSED)
        {
            if (final->right == NULL && make_new)
                final->right = create_node(name, lenght, final);

            final = final->right;
        }
        lex = lexicographic(name, lenght, final->name, final->lenght);
    }
    //assert(final==NULL);

    return final;
}

void tADD(BST* world, char* forest, int forest_lenght, char* tree, int tree_lenght, char* animal, int animal_lenght)
{
    BST* this_forest;
    BST* this_tree;
    BST* this_animal;

    this_forest = find_node(world, forest, forest_lenght, true);

    if (tree_lenght > -1)
        this_tree = find_node(this_forest, tree, tree_lenght, true);

    if (animal_lenght > -1)
        this_animal = find_node(this_tree, animal, animal_lenght, true);

    printf("OK\n");

    return;
}

void tREM(BST* world, char* forest, int forest_lenght, char* tree, int tree_lenght, char* animal, int animal_lenght)
{
    BST* this_forest;
    BST* this_tree;
    BST* this_animal;

    this_forest = find_node(world, forest, forest_lenght, true);

    if (tree_lenght == 1)
    {
        clear_node(this_forest);
        return;
    }

    this_tree = find_node(this_forest, tree, tree_lenght, true);

    if (animal_lenght == 1)
    {
        clear_node(this_tree);
        return;
    }

    this_animal = find_node(this_tree, animal, animal_lenght, true);
    clear_node(this_animal);
    return;
}

void tPRINT(BST* world, char* forest, int forest_lenght, char* tree, int tree_lenght, char* animal, int animal_lenght)
{

}

void tCHECK(BST* world, char* forest, int forest_lenght, char* tree, int tree_lenght, char* animal, int animal_lenght)
{

}

void task_giver(BST* world)
{
    int end_type = 0;
    while (end_type != FILE_END)
    {
        int task_lenght = -1;
        int forest_lenght = -1;
        int tree_lenght = -1;
        int animal_lenght = -1;

        char* task = NULL;
        char* forest = NULL;
        char* tree = NULL;
        char* animal = NULL;
        printf("YE0T\n");
        task = write_word(&task_lenght, &end_type);
        printf("YE1T\n");
        if (end_type == SEPARATOR)
            forest = write_word(&forest_lenght, &end_type);
        printf("YE2T\n");
        if (end_type == SEPARATOR)
            tree = write_word(&tree_lenght, &end_type);
        printf("YE3T\n");
        if (end_type == SEPARATOR)
            animal = write_word(&animal_lenght, &end_type);


        if (task == "ADD" && forest_lenght > -1)
        {
            tADD(world, forest, forest_lenght, tree, tree_lenght, animal, animal_lenght);
            printf("OK\n");
        }
        else if (task == "DEL" && forest_lenght > -1)
        {
            tREM(world, forest, forest_lenght, tree, tree_lenght, animal, animal_lenght);
            printf("OK\n");
        }
        else if (task == "PRINT" && forest_lenght > -1)
        {
            tPRINT(world, forest, forest_lenght, tree, tree_lenght, animal, animal_lenght);
        }
        else if (task == "CHECK" && forest_lenght > -1)
        {
            tCHECK(world, forest, forest_lenght, tree, tree_lenght, animal, animal_lenght);
        }
        else
            printf("ERROR\n");
    }
    return;
}

int main()
{
    BST* world = init_BST();
    task_giver(world);
    DEBUG_tree_print_whole(world, 0);
    return 0;
}

