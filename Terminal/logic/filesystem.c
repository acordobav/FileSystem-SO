#include <stdio.h>
#include <math.h>

#include "ls_function.c"

int *usedBlocks;
int min_block;

void updateTree();
void loadUsedBlocks();

/**
 * Funcion para iniciar el FileSystem
 * b_size: tamano de bloque
 * b_amount: cantidad de bloques
**/
void startFileSystem(int b_size, int b_amount)
{
    tree = malloc(sizeof(*tree));

    FILE *fptr = fopen(DISKMETADATA, "r");
    if (fptr == NULL)
    {
        //Disco de metadata no existe
        min_block = 0;
        tree->blocksize = b_size;
        tree->blocks = b_amount;

        createDisk(b_size, b_amount); // Creacion del disco
        tree->root = createRoot();    // Creacion de la raiz del arbol
        updateTree();                 // Creacion del archivo de metadata
    }
    else
    {
        // Disco de metadata encontrado
        fclose(fptr);
        json_to_tree(DISKMETADATA); // Reconstruccion de datos
    }

    usedBlocks = malloc(tree->blocks * sizeof(int));
    memset(usedBlocks, 0, tree->blocks * sizeof(int));
    loadUsedBlocks(tree->root); // Carga de bloques utilizados
}

/**
 * Funcion para cargar en la lista de bloques cuales estan
 * siendo utilizados por el filesystem
 * node: nodo al cual verificarle los bloques
**/
void loadUsedBlocks(Node *node)
{
    // Se recorre el arbol buscando hijos y hermanos
    if (node->kid != NULL)
        loadUsedBlocks(node->kid);
    if (node->brother != NULL)
        loadUsedBlocks(node->brother);

    // Se recorren los bloques del filedata
    Block *current = node->filedata->blocks;
    while (current != NULL)
    {
        usedBlocks[current->number] = 1;
        current = current->next;
    }
}

/**
 * Funcion para actualizar el archivo de texto que almacena
 * la metadata del disco, guarda el arbol en formato json
**/
void updateTree()
{
    tree_to_json(tree, DISKMETADATA);
}

/**
 * Funcion para obtener el FileData de un nodo
 * ref: puntero del nodo
 * return: FileData del nodo
**/
FileData *getFileData(void *ref)
{
    Node *node = (Node *)ref;
    return node->filedata;
}

/**
 * Funcion para buscar un elemento utilizando un nodo de referencia,
 * busca en un nivel mas abajo de la referencia
 * root: nodo de referencia
 * name: identificador del nodo
 * return: NULL -> elemento no encontrado
 *         puntero -> elemento encontrado
 **/
Node *searchElement(Node *root, char *name)
{
    Node *current = root->kid;

    // Se busca el nodo que se deseado
    while (current != NULL)
    {
        // Se compara el nombre del nodo con dirname
        if (strcmp(current->filedata->name, name) != 0)
        {
            // Nombres no coinciden
            current = current->brother;
        }
        else
        {
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
int rmdir_(void *ref, char *dirname)
{
    Node *root = (Node *)ref;
    Node *current = root->kid;

    // Se busca el nodo que se desea eliminar
    while (current != NULL)
    {
        // Se compara el nombre del nodo con dirname
        if (strcmp(current->filedata->name, dirname) != 0)
        {
            // Nombres no coinciden
            current = current->brother;
        }
        else
        {
            // Nodo encontrado, se procede a borrarlo
            if (current->filedata->isDirectory)
            {
                // Nodo encontrado es un directorio
                int r = deleteNode(root, current);
                updateTree();
                return r;
            }
            else
            {
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
void *search(void *ref, char *route)
{
    Node *current = (Node *)ref;
    char *sroute = malloc((strlen(route) + 1) * sizeof(char));
    strcpy(sroute, route);
    char *token = strtok(sroute, "/");

    // Busqueda a partir del nodo de referencia (current)
    while (token != NULL)
    {
        current = searchElement(current, token);
        if (current == NULL)
        {
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
 * Funcion para crear un nuevo directorio
 * ref: Puntero de referencia de la raiz del nuevo directorio
 * dirname: nombre del directorio
 * owner: nombre del dueno del directorio
 * return : 0: Exito | -1: Error
**/
int mkdir(void *ref, char *dirname, char *owner)
{
    Node *root = (Node *)ref;
    Node *temp = searchElement(root, dirname);

    if (temp == NULL)
    {
        FileData *filedata = createFileData(dirname, 1, owner);
        Node *newNode = createNode(filedata);
        insertNode(root, newNode);
        updateTree();
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * Funcion para cambiar el nombre de un elemento (archivo/directorio)
 * ref: nodo raiz del elemento al cual cambiar el nombre
 * oldName: nombre actual del archivo/directorio
 * newName: nuevo nombre
 * return: 0 -> renombramiento exitoso
 *         1 -> archivo/directorio no encontrado
**/
int renameElement(void *ref, char *oldName, char *newName)
{
    Node *root = (Node *)ref;

    // Busqueda del elemento
    Node *node = searchElement(root, oldName);
    if (node != NULL)
    {
        // Cambio de nombre
        changeName(node->filedata, newName);
        updateTree();
        return 0;
    }
    // Elemento no encontrado
    return 1;
}

/**
 * Funcion para crear un nuevo archivo
 * ref: nodo raiz en el cual sera insertado el elemento
 * name: nombre del archivo
 * owner: nombre del dueno
 * return: puntero a nuevo elemento
 *         NULL -> archivo ya existe
**/
void *touch(void *ref, char *name, char *owner)
{
    Node *root = (Node *)ref;
    Node *temp = searchElement(root, name);
    if(temp == NULL) {
        // Creacion del nuevo nodo
        FileData *filedata = createFileData(name, 0, owner);
        Node *newNode = createNode(filedata);

        // Insertar en el arbol
        insertNode(root, newNode);
        updateTree();
        return newNode;
    } else 
        return NULL;
}

/**
 * Funcion para eliminar una lista de bloques
 * block: primer elemento de la lista
**/
void deleteBlocks(Block *block)
{
    if (block->next != NULL)
        deleteBlocks(block->next);
    usedBlocks[block->number] = 0; // Se marca el bloque como disponible

    // Se actualiza el minimo bloque disponible
    if (min_block > block->number)
        min_block = block->number;

    freeBlock(block);
}

/**
 * Funcion que elimina los bloques que se encuentren despues
 * de los primeros neededBlocks de una lista de bloques
 * block: primer elemento de la lista
 * neededBlocks: numero de bloques que no se deben borrar
**/
void removeBlocks(Block *first, int neededBlocks)
{
    Block *block = first;
    Block *previous = NULL;
    for (int i = 0; i < neededBlocks; i++)
    {
        previous = block;
        block = block->next;
    }
    previous->next = NULL;
    deleteBlocks(block);
}

/**
 * Funcion para eliminar un archivo
 * ref: nodo de referencia (raiz) del elemento
 * name: nombre del archivo a eliminar
 * return: 0 -> borrado exitoso
 *         1 -> elemento es un directorio
 *         2 -> archivo no encontrado
**/
int rm(void *ref, char *name)
{
    Node *node = searchElement(ref, name);
    if (node != NULL)
    {
        if (!node->filedata->isDirectory)
        {   
            // Elemento es un archivo
            if(node->filedata->blocks != NULL) deleteBlocks(node->filedata->blocks);
            deleteNode(ref, node);
            updateTree();
            return 0;
        }
        // Elemento es un directorio
        return 1;
    }
    // Elemento no encontrado
    return 2;
}

/**
 * Funcion que encuentra una cantidad number de bloques disponibles
 * number: cantidad de bloques solicitados
 * return: puntero al primer elemento de la lista de bloques
**/
Block *getNewBlocks(int number)
{
    int newBlocks[number];
    int counter = 0;

    // Se recorre la lista en busca de bloques disponibles
    for (int i = min_block; i < tree->blocks; i++)
    {
        if (!usedBlocks[i])
        { //Bloque disponible
            newBlocks[counter] = i;
            counter++;
            usedBlocks[i] = 1;
            min_block = i;
            if (counter == number)
                break; // Condicion parada
        }
    }
    // Si counter es diferente de number, significa que no
    // hay suficiente espacio disponible en el disco
    if (counter != number)
        return NULL;

    // Creacion de la lista de bloques
    Block *new_blocks = createBlocks(number, newBlocks);
    return new_blocks;
}

/**
 * Funcion que le asigna los bloques necesarios a un nodo
 * node: nodo al que asignarle los bloques
 * blockNum: cantidad de bloques necesarios
 * return: 0 -> asignacion exitosa
 *         2 -> espacio insuficiente en disco
**/
int assignBlocks(Node *node, int blockNum)
{
    int assignedBlocks = countBlocks(node->filedata->blocks);
    int newBlocks = blockNum - assignedBlocks;

    if (newBlocks < 0)
    {
        // Es necesario liberar bloques
        int needed = assignedBlocks + newBlocks;
        removeBlocks(node->filedata->blocks, needed);
    }
    else if (newBlocks > 0)
    {
        // Reserva de bloques
        Block *block = getNewBlocks(newBlocks);
        if (block == NULL)
        {
            // No hay espacio suficiente
            return 2;
        }
        if (node->filedata->blocks != NULL)
            // Ya exiten bloques asignados
            appendBlocks(node->filedata->blocks, block);
        else
            // No existen nodos asignados
            node->filedata->blocks = block;
    }
    return 0;
}

/**
 * Funcion para escribir en un archivo
 * element: referencia del archivo en el que escribir
 * data: informacion que debe escribirse
 * return: 0 -> escritura exitosa
 *         1 -> elemento es un directorio 
 *         2 -> no hay espacio suficiente
**/
int writeFile(void *element, char *data)
{
    Node *node = (Node *)element;
    printf("Obtenido: %s %d\n", node->filedata->name, node->filedata->isDirectory);
    if (!node->filedata->isDirectory)
    {
        int length = strlen(data);
        int blocksize = tree->blocksize;

        // Calculo cantidad de bloques necesarios
        double blockNum = ceil((float)length / blocksize);
        // Asignacion de bloques
        if (assignBlocks(node, blockNum) == 2)
            return 2;

        // Copia de la informacion que debe ser escrita
        char *info = (char *)malloc(blockNum * blocksize);
        memset(info, 0, blockNum * blocksize);
        strcpy(info, data);

        // Escritura en disco
        char *current = info;
        char *buffer = (char *)malloc(blocksize);
        Block *block = node->filedata->blocks;
        for (int i = 0; i < blockNum; i++)
        {
            memset(buffer, 0, blocksize);       // Limpieza del buffer
            memcpy(buffer, current, blocksize); // Copia de la info en el buffer

            //Escritura en el bloque correspondiente
            dwrite(buffer, block->number, blocksize);

            current += blocksize;
            block = block->next;
        }
        free(buffer);
        free(info);
        updateLastModified(node->filedata); // Actualizacion fecha modificacion
        changeSize(node->filedata, length); // Actualizacion tamano del archivo
        updateTree();
        return 0;
    }
    // Se trata de un directorio
    return 1;
}

/**
 * Funcion para leer los datos en disco de un archivo
 * element: elemento del cual obtener los datos almacenados
 * return: string con la informacion almacenada
 *         NULL -> archivo vacio
**/
char *readFile(void *element)
{
    Node *node = (Node *)element;

    Block *block = node->filedata->blocks;
    int blockNum = countBlocks(block);
    if (blockNum == 0)
        return NULL; // Archivo vacio
    int blocksize = tree->blocksize;

    char *read = malloc(blockNum * blocksize);
    memset(read, 0, blockNum * blocksize);

    // Lectura de disco
    void *current = read;
    for (int i = 0; i < blockNum; i++)
    {
        char *data = dread(block->number, blocksize);
        memcpy(current, data, blocksize);
        current += blocksize;
        block = block->next;
        free(data);
    }

    return read;
}

/**
 * Funcion para cambiar el propietario de un archivo/directorio
 * ref: nodo que tiene el filedata que se tiene que cambiar
 * newName: nuevo nombre
**/
void modifyOwner(void *ref, char *newName)
{
    Node *node = (Node *)ref;
    changeOwner(node->filedata, newName);
}

/**
 * Funcion para cambiar el tamano de un archivo/directorio
 * ref: nodo que tiene el filedata que se tiene que cambiar
 * newSize: nuevo tamano
**/
void modifySize(void *ref, int newSize)
{
    Node *node = (Node *)ref;
    changeSize(node->filedata, newSize);
}

void showVisualicer()
{
    system("./visualizer");
}