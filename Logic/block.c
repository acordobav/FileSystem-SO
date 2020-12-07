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
void appendBlocks(int number, int blocks[number], Block* block) {
    Block* current = block;
    // Busqueda del ultimo elemento
    while (current->next != NULL) 
        current = current->next;
    
    // Creacion y link de los demas elementos
    for (int i = 0; i < number; i++) {
        Block* new = malloc(sizeof(*new)); 
        new->number = blocks[i];
        current->next = new;
        current = new;
    }
    current->next = NULL;
}

/**
 * Funcion para eliminar una lista de bloques
 * block: primer elemento de la lista
**/
void deleteBlocks(Block* block) {
    if(block->next != NULL) deleteBlocks(block->next);
    free(block);
}

/**
 * Funcion que elimina todos los elementos de una lista
 * de bloques, sin borrar el primer elemento
 * block: primer elemento de la lista
**/
void removeBlocks(Block* block) {
    Block* next = block->next;
    block->next = NULL;
    deleteBlocks(next);
}