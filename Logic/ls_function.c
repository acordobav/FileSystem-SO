//#define __USE_XOPEN 700
//#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "node.c"
#include "diskhandler.c"

typedef struct Bubble {
    Node* node;
    double order;
} Bubble;

/**
 * Funcion para crear una burbuja
 * node: copia del nodo original
 * order: orden de tiempo
*/
Bubble createBubble(Node* node, double order){
    Bubble bubble;
    bubble.node = node;
    bubble.order = order;
    return bubble;
}

/**
 * Funcion para crear la lista de burbujas
 * root: nodo que esta en la raiz
 * elem: cantidad de hijos de esa raiz
*/
Bubble* createBubbleList(Node* root, int elem){

    Bubble* list = malloc(sizeof(*list)*elem);
    Node* child = root->kid;
    
    //Obtener la fecha y hora actual
    time_t t1 = time(NULL);
    struct tm *tm1 = localtime(&t1);
    char* s1 = asctime(tm1);
    struct tm result1;
    strptime(s1, "%a %b %d %H:%M:%S %Y", &result1);
    time_t now = mktime(&result1);

    int i;
    for(i = 0; i < elem; i++){
        
        /*
        printf("Actual Time: %s\n", s1);
        printf("Child Time: %s\n", child->filedata->lastModified);
        */
        //Obtener fecha y hora del nodo hijo
        struct tm result2;
        strptime(child->filedata->lastModified, "%a %b %d %H:%M:%S %Y", &result2);
        time_t child_time = mktime(&result2);
        
        /*
        char buf1[30];
        strftime(buf1, sizeof(buf1), "%a %b %d %H:%M:%S %Y", localtime(&now));
        printf("%s\n", buf1);
        char buf[30];
        strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", localtime(&child_time));
        printf("%s\n", buf);
        */
        
        //Obtener diferencia de tiempo
        double order = difftime(now, child_time);
        //printf("Diff time = %.2lf\n\n", order);

        //Crear burbuja
        Bubble bubble = createBubble(child, order);
        list[i] = bubble;
        child = child->brother;

    }
    return list;
}

/**
 * Funcion para intercambiar orden las burbujas en la lista
 * bubble1: primer burbuja a intercambiar
 * bubble2: segunda burbuja a intercambiar
*/
void swap(Bubble *bubble1, Bubble *bubble2){
    Bubble temp = *bubble1;
    *bubble1 = *bubble2;
    *bubble2 = temp;
}

/**
 * Funcion para ordenar las burbujas alfabeticamente
 * root: raiz de la lista de burbujas
 * n: cantidad de elementos en la lista de burbujas
*/
void alphabeticalSort(Bubble* list, int n){
    int i, j, compare;
    for (i = 0; i < n-1; i++){
        for (j = 0; j < n-i-1; j++){
            compare = strcmp(list[j].node->filedata->name, 
                             list[j+1].node->filedata->name);
            if(compare > 0){
                swap(&list[j], &list[j+1]);
            }
        }
    }
}

/**
 * Funcion para ordernar las burbujas segun fecha modificacion
 * root: raiz de la lista de burbujas
 * n: cantidad de elementos en la lista de burbujas
*/
void timeSort(Bubble* list, int n){
    int i, j;
    for (i = 0; i < n-1; i++){
        for (j = 0; j < n-i-1; j++){
            if(list[j].order > list[j+1].order){
                swap(&list[j], &list[j+1]);
            }
        }
    }
}

/**
 * Funcion que devuelve una lista de punteros de filedatas para el
 * comando ls
 * root: nodo raiz que contiene los archivos y directorios a listar
 * flag: bandera de ordenamiento del comando, 0 = por defecto, 1 = por tiempo
*/
FileData** filedataList(void* ref, int flag, int* elem){
    Node* root = (Node*) ref;
    *elem = countNodeChilds(root);
    int n = *elem;
    //No tiene hijos
    if (n == 0){
        return NULL;
    }

    Bubble* blist = createBubbleList(root, n);
    if (flag == 1) timeSort(blist, n);
    else if (flag == 0) alphabeticalSort(blist, n);

    FileData** fileList = malloc(sizeof(*fileList)*n);
    int i;
    for(i = 0; i < n; i++){
        fileList[i] = blist[i].node->filedata;
    }
    free(blist);
    return fileList;
    
}

/*
//Pruebas
int main(void){
    
    FileData* filedata1 = createFileData("directorio", 1, "home");
    sleep(2);
    FileData* filedata2 = createFileData("archivo1", 0, "directorio");
    sleep(2);
    FileData* filedata3 = createFileData("archivo2", 0, "directorio");
    sleep(2);
    FileData* filedata4 = createFileData("archivo3", 0, "directorio");
    sleep(2);

    Node* root = createRoot();
    Node* node1 = createNode(filedata1);
    Node* node2 = createNode(filedata2);
    Node* node3 = createNode(filedata3);
    Node* node4 = createNode(filedata4);
    
    insertNode(root, node1);
    insertNode(root, node3);
    insertNode(root, node2);
    insertNode(root, node4);

    int elem;
    FileData** fileList = filedataList(root, 1, &elem);

    
    int i;
    for (i = 0; i < elem; i++)
    {
        printf("Name: %s, Date: %s\n", fileList[i]->name, fileList[i]->lastModified);
    }
    
    free(fileList);

}
*/