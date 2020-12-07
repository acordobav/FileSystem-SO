#include <stdio.h>
#include <stdlib.h>

#define DISKFILENAME "disk"

/**
 * Funcion para crear un nuevo archivo de disco
 * blocksize: tamano del bloque
 * blocks: numero de bloques
**/
void createDisk(int blocksize, int blocks) {
    // Apertura del archivo
    FILE* fptr = fopen(DISKFILENAME, "wb");

    // Creacion de un buffer de ceros
    char buffer[blocksize];
    memset(buffer, 0, blocksize);

    // Escritura de ceros en el archivo
    if(fwrite(buffer,blocksize,blocks,fptr) != blocks){
        perror("Error creating the disk file");
        exit(EXIT_FAILURE);
    }

    fclose(fptr);
}

/**
 * Funcion para escribir en un bloque del disco
 * data: string de tamano blocksize que debe ser escrito
 * block: numero de bloque
 * blocksize: tamano del bloque
**/
void dwrite(char* data, int block, int blocksize) {
    FILE* fptr = fopen(DISKFILENAME, "r+");  // Apertura del archivo
    fseek(fptr, block*blocksize, SEEK_SET); // Mover puntero a bloque 
    if(fwrite(data, blocksize, 1, fptr) != 1) {  // Escritura en disco
        perror("Error writing on the disk file");
        exit(EXIT_FAILURE);
    }
    fclose(fptr);
}

/**
 * Funcion para leer de disco
 * block: numero de bloque
 * blocksize: tamano del bloque
 * return: informacion almacenada en el bloque solicitado
*/
char* dread(int block, int blocksize) {
    FILE* fptr = fopen(DISKFILENAME, "r");  // Apertura del archivo
    fseek(fptr, block*blocksize, SEEK_SET); // Mover puntero a bloque
    char* data = malloc(blocksize*sizeof(char));
    if(fread(data, blocksize, 1, fptr) != 1) {  // Lectura del disco
        perror("Error writing on the disk file");
        exit(EXIT_FAILURE);
    }
    fclose(fptr);
    return data;
}