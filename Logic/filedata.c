#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "block.c"

#define DATELENGHT 30

typedef struct FileData {
    char* name;         // Nombre del directorio o archivo
    int isDirectory;    // Identifica si es directorio o archivo
    char* owner;        // Usuario creador
    char* created;      // Fecha de creacion
    char* lastModified; // Fecha de ultima modificacion
    int size;         // Peso del archivo en bytes
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
FileData* createFileData(char* name, int isDirectory, char* owner, int size) {
    // Creacion de la estructura
    FileData* fdata = malloc(sizeof(*fdata)); 
    fdata->name = name;
    fdata->isDirectory = isDirectory;
    fdata->owner = owner;
    fdata->created = malloc(DATELENGHT*sizeof(char));
    fdata->lastModified = malloc(DATELENGHT*sizeof(char));
    fdata->size = size;
    fdata->blocks = NULL;
    
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

/**
 * Funcion para actualizar la ultima fecha de modificacion
 * filedata: FileData que debe ser actualizado
**/
void updateLastModified(FileData* filedata) {
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char* s = asctime(tm);

    // Limpiar y guardar fecha de ultima modificacion
    memset(filedata->lastModified, 0, DATELENGHT);
    strcpy(filedata->lastModified, s);
}

/**
 * Funcion para modificar el dueno de un archivo/directorio
 * filedata: FileData que debe ser actualizado
 * newName: nuevo nombre
**/
void changeOwner(FileData* filedata, char* newName) {
    filedata->owner = newName;
    updateLastModified(filedata);
}

/**
 * Funcion para cambiar el nombre de un FileData
 * filedata: FileData que debe ser actualizado
 * newName: nuevo nombre
**/
void changeName(FileData* filedata, char* newName) {
    filedata->name = newName;
    updateLastModified(filedata);
}

/**
 * Funcion para cambiar el tamano de un FileData
 * filedata: FileData que debe ser actualizado
 * newSize: nuevo tamano
**/
void changeSize(FileData* filedata, int newSize) {
    filedata->size = newSize;
}

/**
 * Funcion para convertir un filedata a un string en formato json
 * filedata: FileData que debe ser convertido
 * return: json string con la informacion del filedata ingresado
**/
json_object* filedata_to_json(FileData* filedata) {
    // Creacion objeto json
    json_object *j_object = json_object_new_object();

    json_object_object_add(j_object, 
                           "name", 
                           json_object_new_string(filedata->name));
    
    json_object_object_add(j_object, 
                           "isDirectory", 
                           json_object_new_int(filedata->isDirectory));

    json_object_object_add(j_object, 
                           "owner", 
                           json_object_new_string(filedata->owner));

    json_object_object_add(j_object, 
                           "created", 
                           json_object_new_string(filedata->created));

    json_object_object_add(j_object, 
                           "lastModified", 
                           json_object_new_string(filedata->lastModified));
    
    json_object_object_add(j_object, 
                           "size", 
                           json_object_new_int(filedata->size));
    
    json_object* j_array = block_to_json(filedata->blocks);
    json_object_object_add(j_object, 
                           "blocks", 
                           j_array);

    return j_object;
}