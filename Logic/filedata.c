#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "block.c"

typedef struct FileData {
    char* name;         // Nombre del directorio o archivo
    int isDirectory;    // Identifica si es directorio o archivo
    char* owner;        // Usuario creador
    char* created;      // Fecha de creacion
    char* lastModified; // Fecha de ultima modificacion
    char* size;         // Peso del archivo
    Block* blocks;      // Lista con los bloques en disco
} FileData;

/**
 * Funcion para crear una estructura que almacena la informacion de un archivo
 * name: nombre del archivo o directorio
 * isDirectory: 0 si es un archivo, 1 si es un directorio
 * owner: nombre del usuario que crea el archivo o directorio
 * size: peso del archivo o directorio
 * numBlocks: numero de bloques totales
 * blocks: bloques que ocupa el archivo
*/
FileData* createFileData(char* name, int isDirectory, char* owner, char* size,
                         int numBlocks, int blocks[numBlocks]) {
    // Creacion de la estructura
    FileData* fdata = malloc(sizeof(*fdata)); 
    fdata->name = name;
    fdata->isDirectory = isDirectory;
    fdata->owner = owner;
    fdata->created = malloc(30*sizeof(char));
    fdata->lastModified = malloc(30*sizeof(char));
    fdata->size = size;
    fdata->blocks = createBlocks(numBlocks, blocks);
    
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char* s = asctime(tm);

    // Guardar fecha de creacion y ultima modificacion
    strcpy(fdata->created, s);
    strcpy(fdata->lastModified, s);

    return fdata;   
}

/**
 * Funcion que se encarga de eliminar un FileData 
 * liberando la memoria solicitada con malloc
 * fdata: puntero a la estructura
*/
void deleteFileData(FileData* fdata) {
    //if(fdata->blocks != NULL) deleteBlocks(fdata->blocks);
    free(fdata->created);
    free(fdata->lastModified);
    free(fdata);
}