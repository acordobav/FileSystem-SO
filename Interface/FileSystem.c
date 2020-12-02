#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

// ---------------------------------------------------------------------------
// Header of MAIN module
// ---------------------------------------------------------------------------
const float FPS = 30;
const int SCREEN_W = 608;
const int SCREEN_H = 588;

char energyLine[10] = "";

ALLEGRO_BITMAP *martian_img;
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

typedef int code;
code HandleEvent(ALLEGRO_EVENT ev);

bool REDRAW_IS_READY;
void RedrawSetReady(void);
void RedrawClearReady(void);
bool RedrawIsReady(void);
void RedrawDo(); // to be defined in Implementation of EVENT HANDLER Module

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

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);            // Clock : Timer - Allegro display screen
  display = al_create_display(SCREEN_W, SCREEN_H);              // Screen
  martian_img = al_load_bitmap("../src/S2.png");           // Martian Image
  ALLEGRO_BITMAP *TOP_img = al_load_bitmap("../src/TOP.png"); // Maze Image
  ALLEGRO_BITMAP *TERMINAL_img = al_load_bitmap("../src/TERMINAL.png"); // TERMINAL Image


  al_set_target_bitmap(martian_img);
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
  char snum[5];
  char senergy[5];
  char speriod[5];
  char smode[10];
  char smmove[10];

  int s30 = 0;
  int sGame = 0;
  int martianID = -1;
  int preview_martianID = -1;

  char ch = '>';
  int current_seconds = 0;
  float u = 0;

  ALLEGRO_THREAD *current_thread = NULL;

  while (code == CODE_CONTINUE)
  {
    al_draw_bitmap(TOP_img, XYZ_TOP);
    al_draw_bitmap(TERMINAL_img, XYZ_TERMINAL);

    if (RedrawIsReady() && al_is_event_queue_empty(event_queue))
    {

      al_draw_text(font, al_map_rgb(70, 70, 70), 10, 590, 0, "Mode: ");
      // sprintf(senergy, "%d", energy);
      al_draw_text(font, al_map_rgb(255, 255, 220), 45, 552, 10, "> ");
      /* al_draw_text(font, al_map_rgb(255, 255, 255), 70, 10, 0, senergy); */

      al_flip_display();
    }
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    code = HandleEvent(ev);
  }

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  al_destroy_bitmap(martian_img);
  al_destroy_bitmap(TOP_img);
  al_destroy_bitmap(TERMINAL_img);
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  return code;
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
    if (ev.keyboard.keycode == ALLEGRO_KEY_X)
    {
      printf("FIN\n");
      return EXIT_SUCCESS;
    }
    else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
    {
     printf("ENTER");
    }
    break;
  default:
    break;
  }
  return CODE_CONTINUE;
}

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