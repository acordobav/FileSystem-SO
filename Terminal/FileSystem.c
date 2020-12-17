#define __USE_XOPEN 700
#define _MAX_INPUT 1000
#define _MAX_L_CHAR 255
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

Node *searchedNode;
Node *currentFolder;

struct folder
{
  Node *treeNode;
  struct folder *next;
};

struct folder *start = NULL;
typedef int code;

int _exit_ = 0;
int _shift_ = 0;
int _isWriting_ = 0;

int rne;
int flag = -1;
int count = 0;
int regKey = 0;
int ttVertical = 30;

char *ffName;
char *param2;
char *nodeMsg;
char *directoryName = "";

char input[_MAX_INPUT];
char errorMsg[_MAX_L_CHAR];
char ttOutput[_MAX_L_CHAR];
char textTerminal[_MAX_INPUT];
char ffNameBackUp[_MAX_L_CHAR];
char auxTextTerminal[_MAX_INPUT];
char systemOwner[_MAX_L_CHAR] = "system";

bool LS_TERMINAL = false;
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

/**
 * @brief  List's length
 * @note   To Folder List (using by cd)
 * @retval length
 */
int length_list()
{
  struct folder *temp;
  temp = start;
  if (temp == NULL)
    return 0;

  int l = 1;
  while (temp->next != NULL)
  {
    temp = temp->next;
    l++;
  }
  return l;
}

/**
 * @brief  Last node of Folder List
 * @retval Last node
 */
Node *get_last()
{
  struct folder *temp;
  temp = start;
  while (temp->next != NULL)
    temp = temp->next;

  return temp->treeNode;
}

/**
 * @brief  Push new node to Folder List
 * @note   LIFO
 * @param  \*treeNode: new node
 * @retval None
 */
void insert_at_end(Node *treeNode)
{
  struct folder *t, *temp;

  t = (struct folder *)malloc(sizeof(struct folder));
  t->treeNode = treeNode;
  count++;

  if (start == NULL)
  {
    start = t;
    start->next = NULL;
    return;
  }

  temp = start;

  while (temp->next != NULL)
    temp = temp->next;

  temp->next = t;
  t->next = NULL;
}

/**
 * @brief  Remove last element of Folder List
 * @note   LIFO
 * @retval None
 */
void delete_from_end()
{
  struct folder *t, *u;

  if (start == NULL)
    return;

  count--;

  if (start->next == NULL)
  {
    free(start);
    start = NULL;
    return;
  }

  t = start;

  while (t->next != NULL)
  {
    u = t;
    t = t->next;
  }

  u->next = NULL;
  free(t);
}

/**
 * @brief  Check if UI can refresh
 * @note   Allegro
 * @retval Refresh screen
 */
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

/**
 * @brief  Split string
 * @note   Splitting textTerminal
 * @param  \index: index to split array
 * @retval Part of textTerminal
 */
char *splitRegex(int index)
{
  strcpy(auxTextTerminal, textTerminal);
  char *token;
  char *rest = textTerminal;
  int i = 0;

  while ((token = strtok_r(rest, " ", &rest)))
  {
    if (i == index + 1)
      break;
    i++;
  }

  return token;
}

/**
 * @brief  Print folders and files on UI
 * @param  \flag: by defaul or time
 * @retval None
 */
void ls_(int flag)
{
  int elem;
  FileData **fileList = filedataList(currentFolder, flag, &elem);

  int i;
  int v_line = 70;

  for (i = 0; i < elem; i++)
  {
    if (fileList[i]->isDirectory == 1) // Folders
      al_draw_text(font, al_map_rgb(5, 150, 220), 30, v_line, ALLEGRO_ALIGN_LEFT, fileList[i]->name);

    else // Files
      al_draw_text(font, al_map_rgb(220, 140, 245), 30, v_line, ALLEGRO_ALIGN_LEFT, fileList[i]->name);

    v_line = v_line + 15;
  }

  free(fileList);
}

/**
 * @brief  Executer of action -> FileSystem
 * @note   Important: High
 * @param  \regKey: Key to execute action
 * @retval None
 */
void regexTerminal(int regKey)
{
  LS_TERMINAL = false;
  ERROR_TERMINAL = false;
  TERMINAL_OUTPUT = false;

  strcpy(ttOutput, "");
  switch (regKey)
  {
  case 0: // EXIT
    _exit_ = 1;
    break;
  case 1: // MKDIR
    ffName = splitRegex(1);
    int mkdir_result = mkdir(currentFolder, ffName, systemOwner);
    if (mkdir_result == -1)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "mkdir: cannot create directory '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': File exists");
    }
    break;
  case 2: // RMDIR
    ffName = splitRegex(1);
    int rmdir_result = rmdir_(currentFolder, ffName);
    ERROR_TERMINAL = true;

    if (rmdir_result == 1)
      strcpy(errorMsg, "Error: cannot remove directory, please try again.!");

    else if (rmdir_result == 2)
    {
      strcpy(errorMsg, "rmdir: failed to remove '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': Not a directory");
    }
    else if (rmdir_result == 3)
    {
      strcpy(errorMsg, "rmdir: failed to remove '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': No such directory");
    }
    else
      ERROR_TERMINAL = false;

    break;
  case 3: // CD
    ffName = splitRegex(1);
    int r = strcmp(ffName, "..");
    if (r == 0)
    {
      if (length_list() == 1)
      {
        currentFolder = tree->root;
        directoryName = currentFolder->filedata->name;
      }
      else
      {
        delete_from_end();
        Node *temp = get_last();
        if (temp != NULL)
          currentFolder = temp;
        else
          currentFolder = tree->root;
        directoryName = currentFolder->filedata->name;
      }
    }
    else
    {
      void *ref = search(currentFolder, ffName);
      Node *temp = (Node *)ref;
      if (temp != NULL)
      {
        currentFolder = temp;
        directoryName = currentFolder->filedata->name;
        insert_at_end(currentFolder);
      }
      else
      {
        ERROR_TERMINAL = true;
        strcpy(errorMsg, "bash: cd: ");
        strcat(errorMsg, ffName);
        strcat(errorMsg, ": No such directory");
      }
    }
    break;
  case 4: // RM
    ffName = splitRegex(1);
    strcpy(textTerminal, auxTextTerminal);
    param2 = splitRegex(2);
    rne = renameElement(currentFolder, ffName, param2);
    if (rne != 0)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "mv: cannot stat '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': No such file or directory");
    }
    break;
  case 5: //  RM
    ffName = splitRegex(1);
    int rm_RM = rm(currentFolder, ffName);
    if (rm_RM == 1)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "rm: cannot remove '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': Is a directory");
    }
    else if (rm_RM == 2)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "rm: cannot remove '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': No such file");
    }
    break;
  case 55: // UNLINK
    ffName = splitRegex(1);
    int rm_UNLINK = rm(currentFolder, ffName);
    if (rm_UNLINK == 1)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "unlink: cannot unlink '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': Is a directory");
    }
    else if (rm_UNLINK == 2)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "unlink: cannot unlink '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': No such file");
    }
    break;
  case 6: // MV - file (Rename)
    ffName = splitRegex(1);
    strcpy(textTerminal, auxTextTerminal);
    param2 = splitRegex(2);
    rne = renameElement(currentFolder, ffName, param2);
    if (rne != 0)
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "mv: cannot stat '");
      strcat(errorMsg, ffName);
      strcat(errorMsg, "': No such file or directory");
    }
    break;
  case 7: // TOUCH
    ffName = splitRegex(1);
    Node *file1 = touch(currentFolder, ffName, systemOwner);
    if (file1 != NULL)
      writeFile(file1, "");
    break;
  case 8: // CAT
    ffName = splitRegex(1);
    searchedNode = search(currentFolder, ffName);
    if (searchedNode != NULL)
    {
      if (searchedNode->filedata->isDirectory == 1)
      {
        ERROR_TERMINAL = true;
        strcpy(errorMsg, "cat or less: ");
        strcat(errorMsg, ffName);
        strcat(errorMsg, ": Is a directory");
      }
      else
      {
        nodeMsg = readFile(searchedNode);
        strcat(ttOutput, "File Name: ");
        strcat(ttOutput, ffName);
        strcat(ttOutput, "\n\n\n");
        strcat(ttOutput, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");

        if (nodeMsg != NULL)
        {
          strcat(ttOutput, nodeMsg);
          free(nodeMsg);
        }
        TERMINAL_OUTPUT = true;
      }
    }
    else
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "cat or less: ");
      strcat(errorMsg, ffName);
      strcat(errorMsg, ": No such file or directory");
    }
    break;
  case 9: // GET
    ffName = splitRegex(1);
    searchedNode = searchElement(currentFolder, ffName);

    if (searchedNode != NULL)
    {
      FileData *FD = getFileData(searchedNode);

      if (FD != NULL)
      {
        if (FD->isDirectory == 1)
          strcat(ttOutput, "Folder Name: ");
        else
          strcat(ttOutput, "File Name: ");

        char snum[25];
        strcat(ttOutput, ffName);
        strcat(ttOutput, "\n\n\n");
        strcat(ttOutput, "Owner: ");
        strcat(ttOutput, FD->owner);
        strcat(ttOutput, "\n\n\n");
        strcat(ttOutput, "Created: ");
        strcat(ttOutput, FD->created);
        strcat(ttOutput, "\n\n");
        strcat(ttOutput, "Last Modified: ");
        strcat(ttOutput, FD->lastModified);
        strcat(ttOutput, "\n\n");
        strcat(ttOutput, "Size: ");
        sprintf(snum, "%d", FD->size);
        strcat(ttOutput, snum);

        TERMINAL_OUTPUT = true;
      }
    }
    else
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "get: ");
      strcat(errorMsg, ffName);
      strcat(errorMsg, ": No such file or directory");
    }

    break;
  case 10: // CAT >> (WRITE)
    ffName = splitRegex(2);
    strcpy(ffNameBackUp, ffName);
    searchedNode = search(currentFolder, ffName);

    if (searchedNode != NULL)
    {
      strcpy(textTerminal, "> ");

      if (searchedNode->filedata->isDirectory == 1)
      {
        ERROR_TERMINAL = true;
        strcpy(errorMsg, "bash: ");
        strcat(errorMsg, ffName);
        strcat(errorMsg, ": Is a directory");
      }
      else
      {
        _isWriting_ = 1;
        ttVertical = 70;
        nodeMsg = readFile(searchedNode);

        if (nodeMsg != NULL)
        {
          strcat(textTerminal, nodeMsg);
          free(nodeMsg);
        }
      }
    }
    else
    {
      Node *file1 = touch(currentFolder, ffName, systemOwner);
      if (file1 != NULL)
        writeFile(file1, "");
      strcpy(textTerminal, "> ");
      _isWriting_ = 1;
      ttVertical = 70;
    }
    break;
  case 13:
    flag = 0;
    LS_TERMINAL = true;
    break;
  case 14:
    flag = 1;
    LS_TERMINAL = true;
    break;
  case 15:
    showVisualicer();
    break;

  case 16: // set_owner
    ffName = splitRegex(1);
    strcpy(textTerminal, auxTextTerminal);
    char *flag = splitRegex(2);
    strcpy(textTerminal, auxTextTerminal);
    char *data = splitRegex(3);

    searchedNode = search(currentFolder, ffName);

    if (searchedNode != NULL)
      modifyOwner(searchedNode, data);
    else
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "set: ");
      strcat(errorMsg, ffName);
      strcat(errorMsg, ": No such file or directory");
    }
    break;
  case 17: // set_SIZE
    ffName = splitRegex(1);
    strcpy(textTerminal, auxTextTerminal);
    char *flag_ = splitRegex(2);
    strcpy(textTerminal, auxTextTerminal);
    char *data_ = splitRegex(3);

    searchedNode = search(currentFolder, ffName);

    if (searchedNode != NULL)
      modifySize(searchedNode, atoi(data_));
    else
    {
      ERROR_TERMINAL = true;
      strcpy(errorMsg, "set: ");
      strcat(errorMsg, ffName);
      strcat(errorMsg, ": No such file or directory");
    }

    break;
  default:
    break;
  }
}

/**
 * @brief  Method to write text in files
 * @param  \*text: Message to write in file
 * @retval None
 */
void write_File(char *text)
{
  searchedNode = search(currentFolder, ffNameBackUp);

  if (searchedNode != NULL)
  {
    char *chopped_text = text + 2;
    writeFile(searchedNode, chopped_text);
  }
  else
  {
    puts(">>> Error! Code:550");
  }
  _isWriting_ = 0;
  TERMINAL_OUTPUT = false;
  ttVertical = 30;
  strcpy(textTerminal, "> ");
}

/**
 * @brief  Method to validate the input text (textTerminal)
 * @note   > regexTerminal
 * @note   Important: High
 * @retval None
 */
void validateInput()
{
  // Folder
  int r_mkdir = regcomp(&regex, __r_mkdir, REG_EXTENDED);
  int R_MKDIR = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_rmdir = regcomp(&regex, __r_rmdir, REG_EXTENDED);
  int R_RMDIR = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_cd = regcomp(&regex, __r_cd, REG_EXTENDED);
  int R_CD = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_mv_rename_folder = regcomp(&regex, __r_mv_rename_folder, REG_EXTENDED);
  int R_MV_RENAME_FOLDER = regexec(&regex, textTerminal, 0, NULL, 0);

  // File
  int r_rm = regcomp(&regex, __r_rm, REG_EXTENDED);
  int R_RM = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_unlink = regcomp(&regex, __r_unlink, REG_EXTENDED);
  int R_UNLINK = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_mv_file = regcomp(&regex, __r_mv_file, REG_EXTENDED);
  int R_MV_FILE = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_touch = regcomp(&regex, __r_touch, REG_EXTENDED);
  int R_TOUCH = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_cat = regcomp(&regex, __r_cat, REG_EXTENDED);
  int R_CAT = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_get = regcomp(&regex, __r_get, REG_EXTENDED);
  int R_GET = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_cat_write = regcomp(&regex, __r_cat_write, REG_EXTENDED);
  int R_CAT_WRITE = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_less = regcomp(&regex, __r_less, REG_EXTENDED);
  int R_LESS = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_close = regcomp(&regex, __r_close, REG_EXTENDED);
  int R_CLOSE = regexec(&regex, textTerminal, 0, NULL, 0);

  // Other - Display
  int r_ls = regcomp(&regex, __r_ls, REG_EXTENDED);
  int R_LS = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_ls_time = regcomp(&regex, __r_ls_time, REG_EXTENDED);
  int R_LS_TIME = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_vs = regcomp(&regex, __r_vs, REG_EXTENDED);
  int R_VS = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_exit = regcomp(&regex, __r_exit, REG_EXTENDED);
  int R_EXIT = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_set_owner = regcomp(&regex, __r_set_owner, REG_EXTENDED);
  int R_SET_OWNER = regexec(&regex, textTerminal, 0, NULL, 0);

  int r_set_size = regcomp(&regex, __r_set_size, REG_EXTENDED);
  int R_SET_SIZE = regexec(&regex, textTerminal, 0, NULL, 0);

  ERROR_TERMINAL = false;

  if (!R_EXIT) // EXIT
    regKey = 0;
  else if (!R_MKDIR) // crear
    regKey = 1;
  else if (!R_RMDIR) // eliminar
    regKey = 2;
  else if (!R_CD) // desplazarse
    regKey = 3;
  else if (!R_MV_RENAME_FOLDER) // renombrar
    regKey = 4;
  else if (!R_RM) // eliminar
    regKey = 5;
  else if (!R_UNLINK) // eliminar
    regKey = 55;
  else if (!R_MV_FILE) // renombrar
    regKey = 6;
  else if (!R_TOUCH) // crear
    regKey = 7;
  else if (!R_CAT) // abrir
    regKey = 8;
  else if (!R_GET) // obtener atributos
    regKey = 9;
  else if (!R_CAT_WRITE) // escribir
    regKey = 10;
  else if (!R_LESS) // leer
    regKey = 8;
  else if (!R_CLOSE) // cerrar
    regKey = 12;
  else if (!R_LS) // mostrar por defecto
    regKey = 13;
  else if (!R_LS_TIME) // mostrar por fechas
    regKey = 14;
  else if (!R_VS) // visualizador
    regKey = 15;
  else if (!R_SET_OWNER) // establecer dueno
    regKey = 16;
  else if (!R_SET_SIZE) // establecer tamano
    regKey = 17;
  else
  {
    strcpy(errorMsg, "Command '");
    strcat(errorMsg, textTerminal + 2);
    strcat(errorMsg, "' not found.");
    strcpy(textTerminal, "> ");
    ERROR_TERMINAL = true;
    regKey = -1;
  }
  if (regKey != -1)
  {
    regexTerminal(regKey);
    if (regKey != 10)
    {
      strcpy(textTerminal, "> ");
    }
  }
}

/**
 * @brief  Method to concatenate character to textTerminal and call validateInput
 * @note   > validateInput
 * @param  \keyType: Allegro keyboard type
 * @param  \keycode: Allegro keyboard code
 * @retval None
 */
void keyBoardController(ALLEGRO_EVENT_TYPE keyType, int keycode)
{
  LS_TERMINAL = false;

  if (keyType == ALLEGRO_EVENT_KEY_DOWN && (keycode == ALLEGRO_KEY_FULLSTOP || keycode == ALLEGRO_KEY_PAD_DELETE) && _shift_ == 100)
  {
    _shift_ = 0;
    strcat(textTerminal, ">");
  }
  else
  {
    _shift_ = 0;
    if (keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_PAD_ENTER)
    {
      TERMINAL_OUTPUT = false;
      ERROR_TERMINAL = false;
      LS_TERMINAL = false;
    }

    if (keycode == ALLEGRO_KEY_A)
      strcat(textTerminal, "a");
    else if (keycode == ALLEGRO_KEY_B)
      strcat(textTerminal, "b");
    else if (keycode == ALLEGRO_KEY_C)
      strcat(textTerminal, "c");
    else if (keycode == ALLEGRO_KEY_D)
      strcat(textTerminal, "d");
    else if (keycode == ALLEGRO_KEY_E)
      strcat(textTerminal, "e");
    else if (keycode == ALLEGRO_KEY_F)
      strcat(textTerminal, "f");
    else if (keycode == ALLEGRO_KEY_G)
      strcat(textTerminal, "g");
    else if (keycode == ALLEGRO_KEY_H)
      strcat(textTerminal, "h");
    else if (keycode == ALLEGRO_KEY_I)
      strcat(textTerminal, "i");
    else if (keycode == ALLEGRO_KEY_J)
      strcat(textTerminal, "j");
    else if (keycode == ALLEGRO_KEY_K)
      strcat(textTerminal, "k");
    else if (keycode == ALLEGRO_KEY_L)
      strcat(textTerminal, "l");
    else if (keycode == ALLEGRO_KEY_M)
      strcat(textTerminal, "m");
    else if (keycode == ALLEGRO_KEY_N)
      strcat(textTerminal, "n");
    else if (keycode == ALLEGRO_KEY_O)
      strcat(textTerminal, "o");
    else if (keycode == ALLEGRO_KEY_P)
      strcat(textTerminal, "p");
    else if (keycode == ALLEGRO_KEY_Q)
      strcat(textTerminal, "q");
    else if (keycode == ALLEGRO_KEY_R)
      strcat(textTerminal, "r");
    else if (keycode == ALLEGRO_KEY_S)
      strcat(textTerminal, "s");
    else if (keycode == ALLEGRO_KEY_T)
      strcat(textTerminal, "t");
    else if (keycode == ALLEGRO_KEY_U)
      strcat(textTerminal, "u");
    else if (keycode == ALLEGRO_KEY_V)
      strcat(textTerminal, "v");
    else if (keycode == ALLEGRO_KEY_W)
      strcat(textTerminal, "w");
    else if (keycode == ALLEGRO_KEY_X)
      strcat(textTerminal, "x");
    else if (keycode == ALLEGRO_KEY_Y)
      strcat(textTerminal, "y");
    else if (keycode == ALLEGRO_KEY_Z)
      strcat(textTerminal, "z");

    // Numbers
    else if (keycode == ALLEGRO_KEY_0 || keycode == ALLEGRO_KEY_PAD_0)
      strcat(textTerminal, "0");
    else if (keycode == ALLEGRO_KEY_1 || keycode == ALLEGRO_KEY_PAD_1)
      strcat(textTerminal, "1");
    else if (keycode == ALLEGRO_KEY_2 || keycode == ALLEGRO_KEY_PAD_2)
      strcat(textTerminal, "2");
    else if (keycode == ALLEGRO_KEY_3 || keycode == ALLEGRO_KEY_PAD_3)
      strcat(textTerminal, "3");
    else if (keycode == ALLEGRO_KEY_4 || keycode == ALLEGRO_KEY_PAD_4)
      strcat(textTerminal, "4");
    else if (keycode == ALLEGRO_KEY_5 || keycode == ALLEGRO_KEY_PAD_5)
      strcat(textTerminal, "5");
    else if (keycode == ALLEGRO_KEY_6 || keycode == ALLEGRO_KEY_PAD_6)
      strcat(textTerminal, "6");
    else if (keycode == ALLEGRO_KEY_7 || keycode == ALLEGRO_KEY_PAD_7)
      strcat(textTerminal, "7");
    else if (keycode == ALLEGRO_KEY_8 || keycode == ALLEGRO_KEY_PAD_8)
      strcat(textTerminal, "8");
    else if (keycode == ALLEGRO_KEY_9 || keycode == ALLEGRO_KEY_PAD_9)
      strcat(textTerminal, "9");

    // Others
    else if (keycode == ALLEGRO_KEY_SPACE)
      strcat(textTerminal, " ");

    else if (keycode == ALLEGRO_KEY_SLASH || keycode == ALLEGRO_KEY_PAD_SLASH)
      strcat(textTerminal, "/");

    else if (keycode == ALLEGRO_KEY_FULLSTOP || keycode == ALLEGRO_KEY_PAD_DELETE)
      strcat(textTerminal, ".");

    else if (keycode == ALLEGRO_KEY_MINUS || keycode == ALLEGRO_KEY_PAD_MINUS)
      strcat(textTerminal, "-");

    else if (keycode == ALLEGRO_KEY_RSHIFT || keycode == ALLEGRO_KEY_LSHIFT)
      _shift_ = _shift_ + 100;

    else if (keycode == ALLEGRO_KEY_BACKSLASH || keycode == ALLEGRO_KEY_BACKSLASH2)
      strcat(textTerminal, "\n\n");
    // DELETE
    else if (keycode == ALLEGRO_KEY_BACKSPACE && strlen(textTerminal) > 2)
      textTerminal[strlen(textTerminal) - 1] = '\0';
    // ENTER
    else if ((keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_PAD_ENTER) && _isWriting_ == 0)
      validateInput();
    // ENTER
    else if ((keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_PAD_ENTER) && _isWriting_ == 1)
      write_File(textTerminal);
  }
}

/**
 * @brief  Method to handle the UI events
 * @note   Allegro API
 * @param  \ev: Allegro Event
 * @retval code
 */
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
    /* if (strlen(textTerminal) < 68) */
    keyBoardController(ev.type, ev.keyboard.keycode);
    break;
  default:
    break;
  }
  return CODE_CONTINUE;
}

/**
 * @brief  Main Methgod
 * @retval progran status
 */
int main()
{
  int block_size = 0;
  printf("Username: ");
  scanf("%s", systemOwner);

  int block_length = 0;

  while (block_length == 0)
  {
    printf("Blocks: ");
    scanf("%s", input);
    block_length = atoi(input);
  }

  while (block_size == 0)
  {
    printf("Block Size: ");
    scanf("%s", input);
    block_size = atoi(input);
  }

  startFileSystem(block_size, block_length);
  insert_at_end(tree->root);

  currentFolder = tree->root;
  directoryName = currentFolder->filedata->name;

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

  strcpy(textTerminal, "> ");

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

  while (code == CODE_CONTINUE && _exit_ == 0)
  {
    al_clear_to_color(BLACK);
    al_draw_bitmap(TOP_img, XYZ_TOP);
    al_draw_bitmap(TERMINAL_img, XYZ_TERMINAL);

    if (RedrawIsReady() && al_is_event_queue_empty(event_queue))
    {
      al_draw_multiline_text(font, al_map_rgb(255, 255, 220), 30, ttVertical, SCREEN_W - 50, ALLEGRO_ALIGN_LEFT, 0, textTerminal);

      al_draw_text(font, al_map_rgb(255, 255, 220), 20, 10, ALLEGRO_ALIGN_LEFT, "~");
      al_draw_text(font, al_map_rgb(255, 255, 220), 30, 7, ALLEGRO_ALIGN_LEFT, directoryName);

      if (TERMINAL_OUTPUT)
        al_draw_multiline_text(font, al_map_rgb(0, 206, 80), 30, 70, SCREEN_W - 50, ALLEGRO_ALIGN_LEFT, 0, ttOutput);

      if (ERROR_TERMINAL)
        al_draw_multiline_text(font, al_map_rgb(215, 35, 35), 30, 70, SCREEN_W - 50, ALLEGRO_ALIGN_LEFT, 0, errorMsg);

      if (LS_TERMINAL)
        ls_(flag);

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

  // Free REGEX
  regfree(&regex);

  // Free Pointer
  free(start);
  start = NULL;
  free(searchedNode);
  searchedNode = NULL;
  free(currentFolder);
  currentFolder = NULL;

  return code;
}
