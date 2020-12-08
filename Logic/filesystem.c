#include <stdio.h>

#include "node.c"
#include "diskhandler.c"

void updateTree() {
    
}

/**
 * Funcion para obtener el FileData de un nodo
 * ref: puntero del nodo
 * return: FileData del nodo
**/
FileData* getFileData(void* ref) {
    Node* node = (Node*) ref;
    return node->filedata;
}

/**
 * Funcion para crear un nuevo directorio
 * ref: Puntero de referencia de la raiz del nuevo directorio
 * dirname: nombre del directorio
 * owner: nombre del dueno del directorio
**/
int mkdir(void* ref, char* dirname, char* owner) {
    Node* root = (Node*) ref;
    FileData* filedata = createFileData(dirname, 1, owner, NULL, 0, NULL);
    Node* newNode = createNode(filedata);
    insertNode(root, newNode);
    updateTree();
    return 0;
}

/**
 * Funcion para buscar un elemento utilizando un nodo de referencia,
 * busca en un nivel mas abajo de la referencia
 * root: nodo de referencia
 * name: identificador del nodo
 * return: NULL -> elemento no encontrado
 *         puntero -> elemento encontrado
 **/
Node* searchElement(Node* root, char* name) {
    Node* current = root->kid;

    // Se busca el nodo que se deseado
    while(current != NULL) {
        // Se compara el nombre del nodo con dirname
        if(strcmp(current->filedata->name, name) != 0) {
            // Nombres no coinciden
            current = current->brother;
        } else {
            return current;
        }
    }
    return NULL;
}

/**
 * Funcion para eliminar un directorio
 * ref: referencia al nodo raiz del directorio
 * dirname: nombre del directorio que debe borrarse
 * return: numero indicado el resultado de la operacion
 *          0 -> eliminado
 *          1 -> problema al borrar
 *          2 -> elemento no es un directorio
 *          3 -> elemento no encontrado
**/
int rmdir(void* ref, char* dirname) {
    Node* root = (Node*) ref;
    Node* current = root->kid;

    // Se busca el nodo que se desea eliminar
    while(current != NULL) {
        // Se compara el nombre del nodo con dirname
        if(strcmp(current->filedata->name, dirname) != 0) {
            // Nombres no coinciden
            current = current->brother;
        } else {
            // Nodo encontrado, se procede a borrarlo
            if(current->filedata->isDirectory) {
                // Nodo encontrado es un directorio
                int r = deleteNode(root, current);
                updateTree();
                return r;
            } else {
                // Nodo encontrado es un archivo
                return 2;
            }
        }
    }
    // Elemento no es parte de la raiz
    return 3;
}

/**
 * Funcion para buscar un elemento a partir de una referencia y una ruta
 * ref: nodo raiz (referencia) a partir de la cual buscar
 * route: string con la ruta que debe seguirse para encontar el elemento,
 *        no debe incluise el nodo de referencia
 * return: puntero al nodo obtenido, NULL en caso de no encontrarse
**/
void* search(void* ref, char* route) {
    Node* current = (Node*) ref;
    char* sroute = malloc((strlen(route)+1)*sizeof(char));
    strcpy(sroute, route);
    char* token = strtok(sroute, "/");

    // Busqueda a partir del nodo de referencia (current)
    while(token != NULL) {
        current = searchElement(current, token);
        if(current == NULL) {
            free(sroute);
            return NULL;
        }
        token = strtok(NULL, "/");
    }

    // Archivo/directorio encontrado
    free(sroute);
    return current;
}

/**
 * Funcion para cambiar el nombre de un elemento
 * ref: nodo raiz del elemento al cual cambiar el nombre
 * oldName: nombre actual del archivo/directorio
 * newName: nuevo nombre
 * return: 0 -> renombramiento exitoso
 *         1 -> archivo/directorio no encontrado
**/
int renameElement(void* ref, char* oldName, char* newName) {
    Node* root = (Node*) ref;

    // Busqueda del elemento
    Node* node = searchElement(root, oldName);
    if(node != NULL) {
        // Cambio de nombre
        node->filedata->name = newName;
        return 0;
    }
    // Elemento no encontrado
    return 1;
}


    