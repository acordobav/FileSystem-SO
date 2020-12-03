#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <consts_regex.h>

// ---------------------------------------------------------------------------
// Header of MAIN module
// ---------------------------------------------------------------------------
const float FPS = 30;
const int SCREEN_W = 608;
const int SCREEN_H = 588;

char text_terminal[70];
regex_t regex;

ALLEGRO_DISPLAY *display;
ALLEGRO_FONT *font;
#define BLACK (al_map_rgb(0, 0, 0))
#define XYZ_TOP 20, 20, 0
#define XYZ_TERMINAL 20, 540, 0

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CODE_CONTINUE (MIN(EXIT_SUCCESS, EXIT_FAILURE) - 1)
#define CODE_SUCCESS EXIT_SUCCESS
#define CODE_FAILURE EXIT_FAILURE

#define REG_EXTENDED 1

typedef int code;
code HandleEvent(ALLEGRO_EVENT ev);

bool REDRAW_IS_READY;

bool REDRAW_IS_READY = false;
void RedrawSetReady(void) { REDRAW_IS_READY = true; }
void RedrawClearReady(void) { REDRAW_IS_READY = false; }
bool RedrawIsReady(void)
{
  switch (REDRAW_IS_READY)
  {
  case true:
    RedrawClearReady();
    return true;
  default:
    return false;
  }
}

void callAction()
{

  // Folder
  int r_mkdir = regcomp(&regex, __r_mkdir, REG_EXTENDED);
  int r_rmdir = regcomp(&regex, __r_rmdir, REG_EXTENDED);
  int r_mv_folder = regcomp(&regex, __r_mv_folder, REG_EXTENDED);
  int r_mv_rename_folder = regcomp(&regex, __r_mv_rename_folder, REG_EXTENDED);

  // File
  int r_rm_unlink = regcomp(&regex, __r_rm_unlink, REG_EXTENDED);
  int r_mv_file = regcomp(&regex, __r_mv_file, REG_EXTENDED);
  int r_touch = regcomp(&regex, __r_touch, REG_EXTENDED);
  int r_cat = regcomp(&regex, __r_cat, REG_EXTENDED);

  // Other - Display
  int r_ls = regcomp(&regex, __r_ls, REG_EXTENDED);
  int r_ls_time = regcomp(&regex, __r_ls_time, REG_EXTENDED);

  if (!r_ls_time)
  {
    puts("Regular expression compiled successfully.");
  }
  else
  {
    puts("Compilation error.");
  }

  int reti = regexec(&regex, text_terminal, 0, NULL, 0);
  if (!reti)
  {
    puts("Match");
  }
  else
  {
    puts("NO Match");
  }

  // strcpy(text_terminal, "> ");
}

int regexTerminal()
{
}

void keyBoardController(int keycode)
{
  // Letters
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

  // DELETE
  else if (keycode == ALLEGRO_KEY_BACKSPACE && strlen(text_terminal) > 2)
    text_terminal[strlen(text_terminal) - 1] = '\0';
  // ENTER
  else if (keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_PAD_ENTER)
    callAction();
}

code HandleEvent(ALLEGRO_EVENT ev)
{
  switch (ev.type)
  {
  case ALLEGRO_EVENT_TIMER:
    RedrawSetReady();
    break;
  case ALLEGRO_EVENT_DISPLAY_CLOSE:
    return EXIT_SUCCESS;
  case ALLEGRO_EVENT_KEY_DOWN:
    if (strlen(text_terminal) < 68)
      keyBoardController(ev.keyboard.keycode);
    break;
  default:
    break;
  }
  return CODE_CONTINUE;
}

int main(int argc, char *argv[])
{
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
  ALLEGRO_BITMAP *TOP_img = al_load_bitmap("../src/TOP.png");           // Maze Image
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
    al_draw_bitmap(TOP_img, XYZ_TOP);
    al_draw_bitmap(TERMINAL_img, XYZ_TERMINAL);

    if (RedrawIsReady() && al_is_event_queue_empty(event_queue))
    {
      al_draw_text(font, al_map_rgb(255, 255, 220), 30, 552, ALLEGRO_ALIGN_LEFT, text_terminal);
      al_flip_display();
    }
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    code = HandleEvent(ev);
  }

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  al_destroy_bitmap(TOP_img);
  al_destroy_bitmap(TERMINAL_img);
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  return code;
}
