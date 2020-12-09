#include <stdio.h>
#include <json-c/json.h>

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

/**
 * Funcion para convertir la lista de bloques a un json array
 * block: bloque inicial de la lista
 * return: json array
**/
json_object* block_to_json(Block* block) {
    // Creacion del json array
    json_object *j_array = json_object_new_array();
    
    Block* current = block;
    while (current != NULL)
    {
        // Agregar numero de bloque al array
        json_object_array_add(j_array, json_object_new_int(current->number));

        // Avanzar al siguiente bloque
        current = current->next;
    }

    return j_array;
}

/**
 * Funcion para obtener una lista de bloques a partir de un
 * array en formato json
 * json_blocks: objeto json con el array de numeros
 * return: puntero al primer bloque de la lista
**/
Block* json_to_block(json_object* json_blocks) {
    int lenght = json_object_array_length(json_blocks);

    // Se verifica si la lista esta vacia
    if(lenght == 0) return NULL;

    // Obtener numeros del array del json
    int blocks[lenght];
    for (int i = 0; i < lenght; i++) {
        json_object *j_object = json_object_array_get_idx(json_blocks, i);
        blocks[i] = json_object_get_int(j_object);
        //json_object_put(j_object);
    }
    
    // Creacion de los bloques
    return createBlocks(lenght, blocks);
}