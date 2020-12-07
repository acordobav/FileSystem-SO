#include <stdio.h>
#include "filedata.c"

typedef struct Node {
    FileData* filedata;    // Informacion del archivo o directorio 
    struct Node* kid;      // Puntero al primer hijo
    struct Node* brother;  // Puntero al siguiente hermano  
} Node;

Node* createNode(FileData* filedata);
void backtrackigDelete(Node* root);

/**
 * Funcion para crear la raiz del arbol
*/
Node* createRoot() {
    FileData* filedata = createFileData("root", 1, "system", NULL, 0, NULL);
    return createNode(filedata);
}

/**
 * Funcion para crear un nuevo nodo
 * filedata: estructura del archivo o directorio del nodo
*/
Node* createNode(FileData* filedata) {
    Node* node = malloc(sizeof(*node)); 
    node->filedata = filedata;
    node->kid = NULL;
    node->brother = NULL;
}

/**
 * Funcion para eliminar un nodo
 * root: raiz del nodo que debe eliminarse
 * node: nodo a eliminar
*/
int deleteNode(Node* root, Node* node) {
    if (root->kid != node) {
        // Elemento a eliminar no es el hijo de la raiz
        Node* current = root->kid;

        // Recorre lista de hermanos hasta encontrar
        // el elemento que se desea eliminar
        while(current->brother != node) {
            // Verifica si elemento es parte de la raiz
            if(current->brother == NULL) return 1; 
            current = current->brother;
        }
        // Se coloca como referencia el hermano del nodo
        // que se va a eliminar
        current->brother = node->brother;

    } else {
        // Elemento a eliminar es el hijo de la raiz
        root->kid = node->brother;
    }

    // Limpieza de memoria
    node->brother = NULL;  // Eliminar referencia al hermano
    backtrackigDelete(node);
    return 0;
}

/**
 * Funcion que elimina todos los nodos descendientes de una raiz
 * root: nodo a partir del cual empezar a eliminar
*/
void backtrackigDelete(Node* root) {
    // Se recorre el arbol eliminando hijos y hermanos
    if(root->kid != NULL) backtrackigDelete(root->kid);
    if(root->brother != NULL) backtrackigDelete(root->brother);

    // Se elimina la raiz
    deleteFileData(root->filedata);
    free(root);
}

/**
 * Funcion para insertar un nodo en una raiz
 * root: raiz en la que insertar el nodo
 * newNode: nodo que debe ser insertado
*/
void insertNode(Node* root, Node* newNode) {
    // Si el root tiene un hijo, este se coloca
    // como hermano del nuevo elemento
    if (root->kid != NULL) {
        newNode->brother = root->kid;
    }
    // Se coloca el nuevo elmento como hijo del root 
    root->kid = newNode;
}