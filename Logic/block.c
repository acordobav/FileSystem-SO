#include <stdio.h>

typedef struct Block {
    int number;   // Block number
    struct Block* next;  // Next block
} Block;

/**
 * Funcion que crea una lista de bloques
 * number: numero de bloques, si es cero se retorna NULL
 * blocks: lista con los numeros de bloque
 * return: puntero al primer elemento de la lista
**/
Block* createBlocks(int number, int blocks[number]) {
    if(number > 0) {
        // Creacion del primer elemento de la lista
        Block* block = malloc(sizeof(*block)); 
        block->number = blocks[0];

        // Creacion y link de los demas elementos
        Block* current = block;
        for (int i = 1; i < number; i++) {
            Block* new = malloc(sizeof(*new)); 
            new->number = blocks[i];
            current->next = new;
            current = new;
        }
        
        current->next = NULL;
        return block;
    }

    return NULL;
}

/**
 * Funcion para agregar mas bloques al final de una lista
 * number: numero de bloques que seran agregados
 * blocks: lista con los numeros de bloque
 * block: Block al que seran agregados los elementos
**/
void appendBlocks(Block* block, Block* newBlock) {
    Block* current = block;
    // Busqueda del ultimo elemento
    while (current->next != NULL) 
        current = current->next;
    
    // Append del nuevo bloque al final de la lista
    current->next = newBlock;
}

/**
 * Funcion para obtener la cantidad de bloques totales de una lista
 * first: primer elemento de la lista
**/
int countBlocks(Block* first) {
    Block* current = first;
    int counter = 0;
    while(current != NULL) {
        counter++;
        current = current->next;
    }
    return counter;
}

/**
 * Funcion para eliminar un bloque
 * block: bloque que se debe eliminar
**/
void freeBlock(Block* block) {
    free(block);
}

void printBlocks(Block* first) {
    Block* current = first;
    while(current != NULL) {
        printf("%d ", current->number);
        current = current->next;
    }
    printf("\n");
}