#define __USE_XOPEN 700
#define _GNU_SOURCE

#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "logic/diskhandler.c"
#include "logic/node.c"

#define FONTSIZE 16
#define SPRITESIZE 20
#define ScreenWidth 600
#define ScreenHeight 600 
#define xOffset 20
#define yOffset 30

typedef struct Folder {
    Node* node;
    int isOpen;
} Folder;

int xMouse = 0;
int yMouse = 0;
int xStart = 20;
int yStart = 20;
int y;
int folderCount = 0;
Folder* folders;

const float FPS = 30.0;

void drawElements();

ALLEGRO_BITMAP *folderSprite;
ALLEGRO_BITMAP *fileSprite;
ALLEGRO_BITMAP *rightArrowSprite;
ALLEGRO_BITMAP *downArrowSprite;
ALLEGRO_FONT *font;

void visualizer_window() {
    // Inicio interfaz grafica
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    ALLEGRO_KEYBOARD_STATE keyState;

    // Carga de los sprites de la carpeta y el folder
    folderSprite = al_load_bitmap("../src/icons8-folder-20.png");
    fileSprite = al_load_bitmap("../src/icons8-file-20.png");
    rightArrowSprite = al_load_bitmap("../src/icons8-right-20.png");
    downArrowSprite = al_load_bitmap("../src/icons8-down-right-20.png");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(ScreenWidth, ScreenHeight);
    al_set_window_title(disp, "Visualizador");
    font = al_load_ttf_font("../src/ABeeZee-Regular.otf", FONTSIZE, ALLEGRO_TTF_MONOCHROME);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    int gameLoop = 1;
    while(gameLoop)
    {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyState);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:  // Mover ventana a la izquierda
                    xStart -= xOffset;
                    break;
                case ALLEGRO_KEY_RIGHT: // Mover ventana a la derecha
                    xStart += xOffset;
                    break;
                case ALLEGRO_KEY_UP:    // Mover ventana hacia arriba
                    yStart -= yOffset;
                    break;
                case ALLEGRO_KEY_DOWN:  // Mover ventana hacia abajo
                    yStart += yOffset;
                    break;
                default:
                    break;
            }
        } else if(event.type == ALLEGRO_EVENT_TIMER){
            if(event.timer.source == timer) {
                // Se despierta 30 veces en 1 segundo
                redraw = true;
            }
        } else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if(event.mouse.button & 1) {
                xMouse = event.mouse.x;
                yMouse = event.mouse.y;
            }
        }
        if(redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(217, 222, 219));
            
            y = yStart; // Restablecer valor de y
            drawElements(tree->root, xStart);

            al_flip_display();

            redraw = false;
        }
    }
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(folderSprite);
    al_destroy_bitmap(fileSprite);
    al_destroy_bitmap(rightArrowSprite);
    al_destroy_bitmap(downArrowSprite);
}

/**
 * Funcion para detectar si existe una colision entre dos objetos en el eje y
 * y1: posicion en y del objeto 1
 * y2: posicion en y del objeto 2
 * return: true si hay colision, false en caso contrario
**/ 
int isCollisioned(int y1, int y2) {
    if(y2 - SPRITESIZE <= y1 && y1 <= y2){
        return 1;
    }
    return 0;
}

/**
 * Funcion para contar la cantidad de folders totales en el arbol
**/
void countFolders(Node* root) {
    // Se recorre el arbol buscando carpetas
    if(root->kid != NULL) countFolders(root->kid);
    if(root->brother != NULL) countFolders(root->brother);

    // Se aumenta el contador si el nodo actual es un directorio
    if(root->filedata->isDirectory)
        folderCount++;
}

int findex = 0;
void createFolders(Node* root) {
    if(root->filedata->isDirectory) {
        // Se almacena elemento en la lista de carpetas
        folders[findex].node = root;
        folders[findex].isOpen = 0;
        findex++;
    }
    // Se recorre el arbol buscando carpetas
    if(root->kid != NULL) createFolders(root->kid);
    if(root->brother != NULL) createFolders(root->brother);
}

/**
 * Funcion para buscar un folder a partir del nodo
 * node: node que coincide con el folder que se busca
 * return: puntero al folder encontrado, NULL si no se encontro
**/
int getFolderElement(Node* node) {
    int element = -1;
    
    // Se recorre la lista de folders buscando la coincidencia
    for (int i = 0; i < folderCount; i++) {
        Folder current = folders[i];

        // Si el puntero del nodo ingresado coincide con el puntero
        // del nodo que contiene el folder actual, se encontro el elemento
        if(node == current.node){
            element = i;
            break;
        }
    }
    
    return element;
}

/**
 * Funcion para dibujar las lineas para representar 
 * profundidad en el arbol de archivos
 * x: posicion x en la que se dibuja el icono del elemento
**/
void drawLines(int x) {
    int lines = (x - xStart) / xOffset;
    int start = xStart;
    for (int i = 0; i < lines; i++)
    {
        int xpos = start + xOffset/2;
        al_draw_line(xpos, y, xpos, y + yOffset, al_map_rgb(0,0,0), 2);
        start += xOffset;
    }
}

/**
 * Funcion para dibujar el icono y nombre del folder
 * name: nombre del folder
 * x: posicion en x en la que se debe dibujar el icono
**/
void drawFolder(char* name, int x, int isOpen) {
    if(isOpen) al_draw_bitmap(downArrowSprite, x, y, 0);
    else al_draw_bitmap(rightArrowSprite, x, y, 0);
    al_draw_bitmap(folderSprite, x + SPRITESIZE, y, 0);
    al_draw_text(font, al_map_rgb(232, 141, 50), x + 2*SPRITESIZE + 5, y + 2, 0, name);
    drawLines(x);
    y += yOffset;
}

/**
 * Funcion para dibujar el icono y nombre de un archivo
 * name: nombre del folder
 * x: posicion en x en la que se debe dibujar el icono
**/
void drawFile(char* name, int x) {
    al_draw_bitmap(fileSprite, x, y, 0);
    al_draw_text(font, al_map_rgb(0, 0, 0), x + SPRITESIZE + 5, y + 2, 0, name);
    drawLines(x);
    y += yOffset;
}

/**
 * Funcion para dibujar los elementos que componen
 * el arbol de archivos
 * root: nodo que se debe dibujar
 * x: posicion en x en la que se debe dibujar el elemento
**/
void drawElements(Node* root, int x) {
    if(root->filedata->isDirectory) {
        // Se obtiene el folder correspondiente al nodo
        int folderIndex = getFolderElement(root);
        int isOpen = folders[folderIndex].isOpen;
        
        // Dibujado del folder
        drawFolder(root->filedata->name, x, isOpen);

        // Se verifica si el usuario hizo click en el folder
        if(isCollisioned(yMouse,y)) {
            // Se cambia el estado del folder al valor contrario
            xMouse = 0; yMouse = 0;
            folders[folderIndex].isOpen = !isOpen;
        }

        // Se verifica si el folder se encuentra abierto
        if(isOpen && root->kid != NULL) 
            drawElements(root->kid, x + xOffset);
    }  
    else
        // Dibujado del archivo
        drawFile(root->filedata->name, x);

    // Dibujado de los elementos hermanos
    if(root->brother != NULL) drawElements(root->brother, x);
}




int main() {
    // Verificar si archivo de metadata existe
    FILE *fptr = fopen(DISKMETADATA, "r");
    if (fptr == NULL) {
        perror("Metadata disk file: ");
        return EXIT_FAILURE;
    }
    fclose(fptr);

    // Carga del arbol
    tree = malloc(sizeof(*tree));
    json_to_tree(DISKMETADATA);

    // Contar cantidad de carpetas
    countFolders(tree->root);

    // Crear lista de folders
    folders = (Folder*) malloc(sizeof(*folders)*folderCount);
    createFolders(tree->root);

    // Interfaz grafica
    visualizer_window();
    return 0;
}