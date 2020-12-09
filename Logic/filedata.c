#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "block.c"

#define STRING_MAX_LENGHT 30

typedef struct FileData {
    char* name;         // Nombre del directorio o archivo
    int isDirectory;    // Identifica si es directorio o archivo
    char* owner;        // Usuario creador
    char* created;      // Fecha de creacion
    char* lastModified; // Fecha de ultima modificacion
    int size;         // Peso del archivo en bytes
    Block* blocks;      // Lista con los bloques en disco
} FileData;

unsigned long max_lenght = STRING_MAX_LENGHT*sizeof(char);

/**
 * Funcion para crear una estructura que almacena la informacion de un archivo
 * name: nombre del archivo o directorio
 * isDirectory: 0 si es un archivo, 1 si es un directorio
 * owner: nombre del usuario que crea el archivo o directorio
 * size: peso del archivo o directorio
 * return: puntero al FileData creado
*/
FileData* createFileData(char* name, int isDirectory, char* owner, int size) {
    // Creacion de la estructura
    FileData* fdata = malloc(sizeof(*fdata)); 
    fdata->name = malloc(max_lenght);
    fdata->isDirectory = isDirectory;
    fdata->owner = malloc(max_lenght);
    fdata->created = malloc(max_lenght);
    fdata->lastModified = malloc(max_lenght);
    fdata->size = size;
    fdata->blocks = NULL;
    
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char* s = asctime(tm);

    // Guardar fecha de creacion y ultima modificacion
    strcpy(fdata->created, s);
    strcpy(fdata->lastModified, s);
    
    // Guardar nombre y propietario
    strcpy(fdata->name, name);
    strcpy(fdata->owner, owner);

    return fdata;   
}

/**
 * Funcion para crear una estructura que almacena la informacion de un archivo
 * name: nombre del archivo o directorio
 * isDirectory: 0 si es un archivo, 1 si es un directorio
 * owner: nombre del usuario que crea el archivo o directorio
 * size: peso del archivo o directorio
 * created: fecha de creacion
 * lastModified: fecha de ultima modificacion
 * blocks: bloques que ocupa el archivo
 * return: puntero al FileData creado
*/
FileData* createFileData2(char* name, int isDirectory, char* owner, int size,
                          char* created, char* lastModified, Block* blocks) {
    // Creacion de la estructura
    FileData* fdata = malloc(sizeof(*fdata)); 
    fdata->name = malloc(max_lenght);
    fdata->isDirectory = isDirectory;
    fdata->owner = malloc(max_lenght);
    fdata->created = malloc(max_lenght);
    fdata->lastModified = malloc(max_lenght);
    fdata->size = size;
    fdata->blocks = blocks;

    // Guardar fecha de creacion y ultima modificacion
    strcpy(fdata->created, created);
    strcpy(fdata->lastModified, lastModified);
    
    // Guardar nombre y propietario
    strcpy(fdata->name, name);
    strcpy(fdata->owner, owner);

    return fdata;   
}

/**
 * Funcion que se encarga de eliminar un FileData 
 * liberando la memoria solicitada con malloc
 * fdata: puntero a la estructura
*/
void deleteFileData(FileData* fdata) {
    //if(fdata->blocks != NULL) deleteBlocks(fdata->blocks);
    free(fdata->name);
    free(fdata->owner);
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
    memset(filedata->lastModified, 0, max_lenght);
    strcpy(filedata->lastModified, s);
}

/**
 * Funcion para modificar el dueno de un archivo/directorio
 * filedata: FileData que debe ser actualizado
 * newName: nuevo nombre
**/
void changeOwner(FileData* filedata, char* newName) {
    // Limpiar y guardar nuevo nombre
    memset(filedata->owner, 0, max_lenght);
    strcpy(filedata->owner, newName);
    updateLastModified(filedata);
}

/**
 * Funcion para cambiar el nombre de un FileData
 * filedata: FileData que debe ser actualizado
 * newName: nuevo nombre
**/
void changeName(FileData* filedata, char* newName) {
    // Limpiar y guardar nuevo nombre
    memset(filedata->name, 0, max_lenght);
    strcpy(filedata->name, newName);
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

/**
 * Funcion para convertir un string en formato json a un filedata
 * json_filedata: objeto json que debe ser convertido
 * return: filedata con la informacion del json_filedata ingresado
**/
FileData* json_to_filedata(json_object* json_filedata) {
    // Nombre del filedata
    json_object* json_name;
    json_object_object_get_ex(json_filedata, "name", &json_name);

    // isDirectory del filedata
    json_object* json_isDirectory;
    json_object_object_get_ex(json_filedata, "isDirectory", &json_isDirectory);

    // Owner del filedata
    json_object* json_owner;
    json_object_object_get_ex(json_filedata, "owner", &json_owner);

    // Created del filedata
    json_object* json_created;
    json_object_object_get_ex(json_filedata, "created", &json_created);

    // LastModified del filedata
    json_object* json_lastModified;
    json_object_object_get_ex(json_filedata, "lastModified", &json_lastModified);

    // Size del filedata
    json_object* json_size;
    json_object_object_get_ex(json_filedata, "size", &json_size);

    // Blocks del filedata
    json_object* json_blocks;
    json_object_object_get_ex(json_filedata, "blocks", &json_blocks);

    // Conversion de los datos
    const char* name = json_object_get_string(json_name);
    int isDirectory = json_object_get_int(json_isDirectory);
    const char* owner = json_object_get_string(json_owner);
    const char* created = json_object_get_string(json_created);
    const char* lastModified = json_object_get_string(json_lastModified);
    int size = json_object_get_int(json_size);
    Block* blocks = json_to_block(json_blocks);

    // Creacion de la estructura
    FileData* filedata = createFileData2((char*) name, 
                                         isDirectory, 
                                         (char*) owner, 
                                         size, 
                                         (char*) created, 
                                         (char*) lastModified, 
                                         blocks);
    // Liberar memoria
    /*json_object_put(json_name);
    json_object_put(json_isDirectory);
    json_object_put(json_owner);
    json_object_put(json_created);
    json_object_put(json_lastModified);
    json_object_put(json_size);*/
    //json_object_put(json_blocks);

    return filedata;
}