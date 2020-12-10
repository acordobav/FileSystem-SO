#include <stdio.h>
#include <json-c/json.h>

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
    FileData* filedata = createFileData("root", 1, "system");
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
        if(current == NULL) return 1;

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

/**
 * Funcion para obtener la cantidad de bloques totales de una lista
 * first: primer elemento de la lista
**/
int countNodeChilds(Node* root) {
    Node* current = root->kid;
    int counter = 0;
    while(current != NULL) {
        counter++;
        current = current->brother;
    }
    return counter;
}

/**
 * Funcion para convertir un Node a un string en formato json
 * node: Node que debe ser convertido
 * return: json string con la informacion del node ingresado
**/
json_object* node_to_json(Node* node) {
    // Creacion objeto json
    json_object *j_object = json_object_new_object();

    // Conversion del FileData
    json_object *j_node_filedata = filedata_to_json(node->filedata);
    json_object_object_add(j_object, "filedata", j_node_filedata);
    
    // Conversion del nodo hijo
    json_object *j_node_kid = NULL;
    if(node->kid != NULL) 
        j_node_kid = node_to_json(node->kid);
    json_object_object_add(j_object, "kid", j_node_kid);

    // Conversion del nodo hermano
    json_object *j_node_brother = NULL;
    if(node->brother != NULL) 
        j_node_brother = node_to_json(node->brother);
    json_object_object_add(j_object, "brother", j_node_brother);
    
    return j_object;
}

/**
 * Funcion para convertir un string en formato json a un nodo
 * json_object: string en formato json con la informacion del nodo
 * return: puntero al nodo con la informacion del string
**/
Node* json_to_node(json_object* json_node) {
    // Conversion del filedata
    json_object* json_filedata;
    json_object_object_get_ex(json_node, "filedata", &json_filedata);
    FileData* filedata = json_to_filedata(json_filedata);
    //json_object_put(json_filedata);

    // Creacion del nodo
    Node* node = createNode(filedata);

    // Conversion del nodo hijo
    json_object* json_kid;
    json_object_object_get_ex(json_node, "kid", &json_kid);
    if(json_kid != NULL) node->kid = json_to_node(json_kid);
    //json_object_put(json_kid);

    // Conversion del nodo hermano
    json_object* json_brother;
    json_object_object_get_ex(json_node, "brother", &json_brother);
    if(json_brother != NULL) node->brother = json_to_node(json_brother);
    //json_object_put(json_brother);

    return node;
}

/**
 * Funcion para almacenar el arbol en un archivo .json
 * root: raiz del arbol
**/
void tree_to_json(Node* root) {
    json_object *j_object = json_object_new_object();

    // Creacion del json del arbol
    json_object *j_root = node_to_json(root);
    json_object_object_add(j_object, "root", j_root);

    // Escritura en un archivo
    json_object_to_file("tree.json", j_object);
    json_object_put(j_object);
}

/**
 * Funcion para almacenar el arbol en un archivo .json
 * root: raiz del arbol
**/
Node* json_to_tree() {
    // Apertura del archivo
    FILE *f = fopen("tree.json", "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET); 

    // Lectura del arbol
    char *buffer = malloc(fsize + 1);
    fread(buffer, 1, fsize, f);
    fclose(f);

    // Conversion del string a un arbol
    json_object* parsed_json = json_tokener_parse(buffer);
    json_object* json_root;
    json_object_object_get_ex(parsed_json, "root", &json_root);
    Node* node = json_to_node(json_root);
    json_object_put(parsed_json);

    free(buffer);
    return node;
}