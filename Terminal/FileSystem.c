#define __USE_XOPEN 700
#define _GNU_SOURCE

#include <time.h>
#include <regex.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <consts_regex.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#include "./logic/filesystem.c"

const float FPS = 30;
const int SCREEN_W = 608;
const int SCREEN_H = 588;

char SystemOwner[20] = "system";
Node *currentFolder;
Node *previousFolder;

int rne;
char *ffName;
int Shift = 0;
int REG_KEY = 0;
typedef int code;
char *directoryName = "";
char *new_ffName;

char text_terminal[70];
char text_terminal_output[255];
char aux_text_terminal[70];

bool ERROR_TERMINAL = false;
bool REDRAW_IS_READY = false;
bool TERMINAL_OUTPUT = false;

regex_t regex;
ALLEGRO_FONT *font;
ALLEGRO_DISPLAY *display;

#define REG_EXTENDED 1
#define XYZ_TOP 20, 60, 0
#define XYZ_TERMINAL 20, 20, 0
#define BLACK (al_map_rgb(0, 0, 0))

#define CODE_SUCCESS EXIT_SUCCESS
#define CODE_FAILURE EXIT_FAILURE
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CODE_CONTINUE (MIN(EXIT_SUCCESS, EXIT_FAILURE) - 1)

bool RedrawIsReady(void)
{
  switch (REDRAW_IS_READY)
  {
  case true:
    REDRAW_IS_READY = false;
    return true;
  default:
    return false;
  }
}

char *splitRegex(int index)
{
  strcpy(aux_text_terminal, text_terminal);
  char *token;
  char *rest = text_terminal;
  int i = 0;

  while ((token = strtok_r(rest, " ", &rest)))
  {
    if (i == index + 1)
      break;
    i++;
  }

  return token;
}

void regexTerminal(int REG_KEY)
{
  TERMINAL_OUTPUT = false;
  strcpy(text_terminal_output, "");
  switch (REG_KEY)
  {
  case 1:
    ffName = splitRegex(1);
    printf("--> currentFolder : %s \t ffName : %s \t SystemOwner : %s\n", currentFolder->filedata->name, ffName, SystemOwner);
    mkdir(currentFolder, ffName, SystemOwner);
    break;
  case 2:
    ffName = splitRegex(1);
    printf("--> currentFolder : %s \t ffName : %s \n", currentFolder->filedata->name, ffName);
    rmdir_(currentFolder, ffName);
    break;
  case 3:
    ffName = splitRegex(1);
    int r = strcmp(ffName, "..");

    if (r == 0 && previousFolder != NULL)
    {
      directoryName = "";
      currentFolder = previousFolder;
      directoryName = currentFolder->filedata->name;
      printf("--> currentFolder : %s \n", currentFolder->filedata->name);
    }
    else
    {
      void *ref = search(currentFolder, ffName);
      Node *temp = (Node *)ref;
      if (temp != NULL)
      {
        directoryName = "";
        previousFolder = currentFolder;
        currentFolder = temp;
        directoryName = currentFolder->filedata->name;
        printf("--> currentFolder : %s \n", currentFolder->filedata->name);
      }
      else
      {
        puts("Error..........");
      }
    }
    break;
  case 4:
    ffName = splitRegex(1);
    strcpy(text_terminal, aux_text_terminal);
    new_ffName = splitRegex(2);
    printf("rename FOLDER> %s -> %s\n", ffName, new_ffName);

    rne = renameElement(currentFolder, ffName, new_ffName);
    if (rne == 0)
    {
      puts("Exito");
    }
    else
    {
      puts("error");
    }
    break;
  case 5:
    ffName = splitRegex(1);
    printf("remove file--> currentFolder : %s \t ffName : %s\n", currentFolder->filedata->name, ffName);
    int rm_ = rm(currentFolder, ffName);
    if (rm_ == 0)
    {
      puts("OK");
    }
    else if (rm_ == 1)
    {
      puts("Error, es un folder");
    }
    else
    {
      puts("Archivo no encontrado");
    }

    break;
  case 6:
    ffName = splitRegex(1);
    strcpy(text_terminal, aux_text_terminal);
    new_ffName = splitRegex(2);
    printf("rename FILE > %s -> %s\n", ffName, new_ffName);

    rne = renameElement(currentFolder, ffName, new_ffName);
    if (rne == 0)
    {
      puts("Exito");
    }
    else
    {
      puts("error");
    }
    break;
  case 7:
    ffName = splitRegex(1);
    printf("--> %s\n", ffName);
    Node *file1 = touch(currentFolder, ffName, SystemOwner);
    char *msg = "";
    writeFile(file1, msg);
    break;
  case 8:
    puts("8");
    break;
  case 9:
    ffName = splitRegex(1);
    printf("--> currentFolder : %s \t ffName : %s \n", currentFolder->filedata->name, ffName);
    Node *N_FD = searchElement(currentFolder, ffName);
    FileData *FD = getFileData(N_FD);

    strcat(text_terminal_output, "File Name: ");
    strcat(text_terminal_output, ffName);
    strcat(text_terminal_output, "\n\n\n");
    strcat(text_terminal_output, "Owner: ");
    strcat(text_terminal_output, FD->owner);
    strcat(text_terminal_output, "\n\n\n");
    strcat(text_terminal_output, "Created: ");
    strcat(text_terminal_output, FD->created);
    strcat(text_terminal_output, "\n\n");
    strcat(text_terminal_output, "Last Modified: ");
    strcat(text_terminal_output, FD->lastModified);
    strcat(text_terminal_output, "\n\n");
    strcat(text_terminal_output, "Size: ");
    char snum[25];
    sprintf(snum, "%d", FD->size);
    strcat(text_terminal_output, snum);

    TERMINAL_OUTPUT = true;

    break;
  case 10:
    puts("10");
    break;
  case 11:
    puts("11");
    break;
  case 12:
    puts("12");
    break;
  case 13:
    puts("13");
    break;
  case 14:
    puts("14");
    break;
  case 15:
    showVisualicer();
  case 16:
    puts("16");
    break;
  default:
    break;
  }
}

void validateInput()
{
  // Folder
  int r_mkdir = regcomp(&regex, __r_mkdir, REG_EXTENDED);
  int R_MKDIR = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_rmdir = regcomp(&regex, __r_rmdir, REG_EXTENDED);
  int R_RMDIR = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_cd = regcomp(&regex, __r_cd, REG_EXTENDED);
  int R_CD = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_mv_rename_folder = regcomp(&regex, __r_mv_rename_folder, REG_EXTENDED);
  int R_MV_RENAME_FOLDER = regexec(&regex, text_terminal, 0, NULL, 0);

  // File
  int r_rm_unlink = regcomp(&regex, __r_rm_unlink, REG_EXTENDED);
  int R_RM_UNLINK = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_mv_file = regcomp(&regex, __r_mv_file, REG_EXTENDED);
  int R_MV_FILE = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_touch = regcomp(&regex, __r_touch, REG_EXTENDED);
  int R_TOUCH = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_cat = regcomp(&regex, __r_cat, REG_EXTENDED);
  int R_CAT = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_get = regcomp(&regex, __r_get, REG_EXTENDED);
  int R_GET = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_cat_write = regcomp(&regex, __r_cat_write, REG_EXTENDED);
  int R_CAT_WRITE = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_less = regcomp(&regex, __r_less, REG_EXTENDED);
  int R_LESS = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_close = regcomp(&regex, __r_close, REG_EXTENDED);
  int R_CLOSE = regexec(&regex, text_terminal, 0, NULL, 0);

  // Other - Display
  int r_ls = regcomp(&regex, __r_ls, REG_EXTENDED);
  int R_LS = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_ls_time = regcomp(&regex, __r_ls_time, REG_EXTENDED);
  int R_LS_TIME = regexec(&regex, text_terminal, 0, NULL, 0);

  int r_vs = regcomp(&regex, __r_vs, REG_EXTENDED);
  int R_VS = regexec(&regex, text_terminal, 0, NULL, 0);

  if (!r_close && !r_mkdir && !r_rmdir && !r_cd && !r_mv_rename_folder && !r_rm_unlink && !r_mv_file && !r_touch && !r_cat && !r_get && !r_cat_write && !r_less && !r_close && !r_ls && !r_ls_time && !r_vs)
    puts("");
  else
    puts("Compilation error.");

  ERROR_TERMINAL = false;

  if (!R_MKDIR) // crear
    REG_KEY = 1;
  else if (!R_RMDIR) // eliminar
    REG_KEY = 2;
  else if (!R_CD) // desplazarse
    REG_KEY = 3;
  else if (!R_MV_RENAME_FOLDER) // renombrar
    REG_KEY = 4;
  else if (!R_RM_UNLINK) // eliminar
    REG_KEY = 5;
  else if (!R_MV_FILE) // renombrar
    REG_KEY = 6;
  else if (!R_TOUCH) // crear
    REG_KEY = 7;
  else if (!R_CAT) // abrir
    REG_KEY = 8;
  else if (!R_GET) // obtener atributos
    REG_KEY = 9;
  else if (!R_CAT_WRITE) // escribir
    REG_KEY = 10;
  else if (!R_LESS) // leer
    REG_KEY = 11;
  else if (!R_CLOSE) // cerrar
    REG_KEY = 12;
  else if (!R_LS)
    REG_KEY = 13;
  else if (!R_LS_TIME)
    REG_KEY = 14;
  else if (!R_VS)
    REG_KEY = 15;
  // TODO establecer atributos.
  else
  {
    puts("---- ERROR ---");
    ERROR_TERMINAL = true;
    REG_KEY = 0;
  }
  if (REG_KEY != 0)
  {
    regexTerminal(REG_KEY);
    strcpy(text_terminal, "> ");
  }
}

void keyBoardController(ALLEGRO_EVENT_TYPE keyType, int keycode)
{
  if (keyType == ALLEGRO_EVENT_KEY_DOWN && (keycode == ALLEGRO_KEY_FULLSTOP || keycode == ALLEGRO_KEY_PAD_DELETE) && Shift == 100)
  {
    Shift = 0;
    strcat(text_terminal, ">");
  }
  else
  {
    Shift = 0;
    if (keycode == ALLEGRO_KEY_A)
      strcat(text_terminal, "a");
    else if (keycode == ALLEGRO_KEY_B)
      strcat(text_terminal, "b");
    else if (keycode == ALLEGRO_KEY_C)
      strcat(text_terminal, "c");
    else if (keycode == ALLEGRO_KEY_D)
      strcat(text_terminal, "d");
    else if (keycode == ALLEGRO_KEY_E)
      strcat(text_terminal, "e");
    else if (keycode == ALLEGRO_KEY_F)
      strcat(text_terminal, "f");
    else if (keycode == ALLEGRO_KEY_G)
      strcat(text_terminal, "g");
    else if (keycode == ALLEGRO_KEY_H)
      strcat(text_terminal, "h");
    else if (keycode == ALLEGRO_KEY_I)
      strcat(text_terminal, "i");
    else if (keycode == ALLEGRO_KEY_J)
      strcat(text_terminal, "j");
    else if (keycode == ALLEGRO_KEY_K)
      strcat(text_terminal, "k");
    else if (keycode == ALLEGRO_KEY_L)
      strcat(text_terminal, "l");
    else if (keycode == ALLEGRO_KEY_M)
      strcat(text_terminal, "m");
    else if (keycode == ALLEGRO_KEY_N)
      strcat(text_terminal, "n");
    else if (keycode == ALLEGRO_KEY_O)
      strcat(text_terminal, "o");
    else if (keycode == ALLEGRO_KEY_P)
      strcat(text_terminal, "p");
    else if (keycode == ALLEGRO_KEY_Q)
      strcat(text_terminal, "q");
    else if (keycode == ALLEGRO_KEY_R)
      strcat(text_terminal, "r");
    else if (keycode == ALLEGRO_KEY_S)
      strcat(text_terminal, "s");
    else if (keycode == ALLEGRO_KEY_T)
      strcat(text_terminal, "t");
    else if (keycode == ALLEGRO_KEY_U)
      strcat(text_terminal, "u");
    else if (keycode == ALLEGRO_KEY_V)
      strcat(text_terminal, "v");
    else if (keycode == ALLEGRO_KEY_W)
      strcat(text_terminal, "w");
    else if (keycode == ALLEGRO_KEY_X)
      strcat(text_terminal, "x");
    else if (keycode == ALLEGRO_KEY_Y)
      strcat(text_terminal, "y");
    else if (keycode == ALLEGRO_KEY_Z)
      strcat(text_terminal, "z");

    // Numbers
    else if (keycode == ALLEGRO_KEY_0 || keycode == ALLEGRO_KEY_PAD_0)
      strcat(text_terminal, "0");
    else if (keycode == ALLEGRO_KEY_1 || keycode == ALLEGRO_KEY_PAD_1)
      strcat(text_terminal, "1");
    else if (keycode == ALLEGRO_KEY_2 || keycode == ALLEGRO_KEY_PAD_2)
      strcat(text_terminal, "2");
    else if (keycode == ALLEGRO_KEY_3 || keycode == ALLEGRO_KEY_PAD_3)
      strcat(text_terminal, "3");
    else if (keycode == ALLEGRO_KEY_4 || keycode == ALLEGRO_KEY_PAD_4)
      strcat(text_terminal, "4");
    else if (keycode == ALLEGRO_KEY_5 || keycode == ALLEGRO_KEY_PAD_5)
      strcat(text_terminal, "5");
    else if (keycode == ALLEGRO_KEY_6 || keycode == ALLEGRO_KEY_PAD_6)
      strcat(text_terminal, "6");
    else if (keycode == ALLEGRO_KEY_7 || keycode == ALLEGRO_KEY_PAD_7)
      strcat(text_terminal, "7");
    else if (keycode == ALLEGRO_KEY_8 || keycode == ALLEGRO_KEY_PAD_8)
      strcat(text_terminal, "8");
    else if (keycode == ALLEGRO_KEY_9 || keycode == ALLEGRO_KEY_PAD_9)
      strcat(text_terminal, "9");

    // Others
    else if (keycode == ALLEGRO_KEY_SPACE)
      strcat(text_terminal, " ");

    else if (keycode == ALLEGRO_KEY_SLASH || keycode == ALLEGRO_KEY_PAD_SLASH)
      strcat(text_terminal, "/");

    else if (keycode == ALLEGRO_KEY_FULLSTOP || keycode == ALLEGRO_KEY_PAD_DELETE)
      strcat(text_terminal, ".");

    else if (keycode == ALLEGRO_KEY_MINUS || keycode == ALLEGRO_KEY_PAD_MINUS)
      strcat(text_terminal, "-");

    else if (keycode == ALLEGRO_KEY_RSHIFT || keycode == ALLEGRO_KEY_LSHIFT)
      Shift = Shift + 100;

    // DELETE
    else if (keycode == ALLEGRO_KEY_BACKSPACE && strlen(text_terminal) > 2)
      text_terminal[strlen(text_terminal) - 1] = '\0';
    // ENTER
    else if (keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_PAD_ENTER)
      validateInput();
  }
}

code HandleEvent(ALLEGRO_EVENT ev)
{
  switch (ev.type)
  {
  case ALLEGRO_EVENT_TIMER:
    REDRAW_IS_READY = true;
    break;
  case ALLEGRO_EVENT_DISPLAY_CLOSE:
    return EXIT_SUCCESS;
  case ALLEGRO_EVENT_KEY_DOWN:
    if (strlen(text_terminal) < 68)
      keyBoardController(ev.type, ev.keyboard.keycode);
    break;
  default:
    break;
  }
  return CODE_CONTINUE;
}

int main(int argc, char *argv[])
{

  // strcpy(text_terminal, "mv   f1   folder1");

  printf("Username: ");
  scanf("%s", SystemOwner);

  startFileSystem(32, 10000);

  currentFolder = tree->root;
  directoryName = currentFolder->filedata->name;

  /*   Node *file1 = touch(currentFolder, "file1.txt", SystemOwner);
  char *msg = "";
  writeFile(file1, msg);

  mkdir(tree->root, "TEC", "Rogers");
  void *ref = search(tree->root, "TEC");
  void *file2 = touch(ref, "Algebra", "Rogers");
  writeFile(file2, msg);

  void *file3 = touch(tree->root, "Examen 2.pdf", "Arthur");
  writeFile(file3, msg); */

  //tree_to_json(tree, DISKMETADATA);

  al_init();
  if (!al_install_keyboard())
  {
    puts("couldn't initialize keyboard");
    return 1;
  }
  if (!al_init_image_addon())
  {
    puts("couldn't initialize image addon");
    return 1;
  }
  font = al_create_builtin_font(); // Font Styles
  if (!font)
  {
    puts("couldn't initialize font");
    return 1;
  }

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);                    // Clock : Timer - Allegro display screen
  display = al_create_display(SCREEN_W, SCREEN_H);                      // Screen
  ALLEGRO_BITMAP *TOP_img = al_load_bitmap("../src/TOP.png");           // Top Image
  ALLEGRO_BITMAP *TERMINAL_img = al_load_bitmap("../src/TERMINAL.png"); // TERMINAL Image

  strcpy(text_terminal, "> ");

  al_set_target_bitmap(al_get_backbuffer(display));
  ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_clear_to_color(BLACK);
  al_flip_display();

  // Start timer
  al_start_timer(timer);

  // Event loop
  int code = CODE_CONTINUE;

  while (code == CODE_CONTINUE)
  {
    al_clear_to_color(BLACK); // TODO: check if this line es necessary!! - showing directoryName

    al_draw_bitmap(TOP_img, XYZ_TOP);
    al_draw_bitmap(TERMINAL_img, XYZ_TERMINAL);

    if (RedrawIsReady() && al_is_event_queue_empty(event_queue))
    {
      al_draw_text(font, al_map_rgb(255, 255, 220), 30, 30, ALLEGRO_ALIGN_LEFT, text_terminal);

      al_draw_text(font, al_map_rgb(255, 255, 220), 20, 10, ALLEGRO_ALIGN_LEFT, "~");
      al_draw_text(font, al_map_rgb(255, 255, 220), 30, 7, ALLEGRO_ALIGN_LEFT, directoryName);

      if (TERMINAL_OUTPUT)
      {
        al_draw_multiline_text(font, al_map_rgb(0, 206, 80), 30, 70, SCREEN_W - 50, ALLEGRO_ALIGN_LEFT, 0, text_terminal_output);
      }

      if (ERROR_TERMINAL)
      {
        al_draw_text(font, al_map_rgb(255, 0, 0), 30, 70, ALLEGRO_ALIGN_LEFT, "Error...");
      }
      else
      {
        /* TODO ; Call funtion by REG_KEY */
      }

      al_flip_display();
    }
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    code = HandleEvent(ev);
  }

  // Reset Resources
  al_clear_to_color(BLACK);
  al_flip_display();

  // Free Resource Inteface
  al_destroy_bitmap(TOP_img);
  al_destroy_bitmap(TERMINAL_img);
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);

  //3. Free REGEX
  regfree(&regex);

  return code;
}
