#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>


enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct snakeNode
{
    int x;
    int y;
    SnakeNode* next;
} SnakeNode;



int main() {
    int size_x = 710;
    int size_y = 520;
    float speed = 3;

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1/speed);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(size_x, size_y);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool dead = false;
    int snakeLength = 3;

    SnakeNode* third = malloc(sizeof(SnakeNode));
    third->x = 80;
    third->y = 100;
    SnakeNode* second = malloc(sizeof(SnakeNode));
    second->x = 90;
    second->y = 100;
    second->next = third;
    SnakeNode* first = malloc(sizeof(SnakeNode));
    first->x = 100;
    first->y = 100;
    first->next = second;

    SnakeNode* head = first;
    SnakeNode* cur;
    SnakeNode* last;
    SnakeNode* newSnakeNode;

    srand(time(NULL));
    int apple_x = (rand() % 49 + 2) * 10;
    int apple_y = (rand() % 49 + 2) * 10;

    bool redraw = true;
    ALLEGRO_EVENT event;

    enum direction dir = RIGHT;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (1) {
        al_wait_for_event(queue, &event);

        // Key Presses
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                if (dead)
                    break;

                // ----- Game Logic -----
                // add next head node on every frame
                newSnakeNode = malloc(sizeof(SnakeNode));
                switch (dir) {
                    case UP:
                        newSnakeNode->x = head->x;
                        newSnakeNode->y = head->y - 10;
                        break;
                    case DOWN:
                        newSnakeNode->x = head->x;
                        newSnakeNode->y = head->y + 10;
                        break;
                    case LEFT:
                        newSnakeNode->x = head->x - 10;
                        newSnakeNode->y = head->y;
                        break;
                    case RIGHT:
                        newSnakeNode->x = head->x + 10;
                        newSnakeNode->y = head->y;
                        break;
                }
                newSnakeNode->next = head;
                head = newSnakeNode;

                // check for collision with apple
                if (apple_x == head->x && apple_y == head->y) {
                    snakeLength += 1;
                    srand(time(NULL));
                    apple_x = (rand() % 49 + 2) * 10;
                    apple_y = (rand() % 49 + 2) * 10;
                } else {
                    cur = head;
                    for (int i=0; i<snakeLength; i++)
                        cur = cur->next;
                    last = cur->next;
                    free(last);
                    cur->next = NULL;
                }

                // check for collision with snake
                cur = head;
                for (int i=0; i<snakeLength; i++) {
                    cur = cur->next;
                    if (head->x == cur->x && head->y == cur->y)
                        dead = true;
                }

                // check for collision with wall
                if (head->x <= 10 || head->x >= 510 || head->y <= 10 || head->y >= 510)
                    dead = true;
                
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                // key events
                if (event.keyboard.keycode == ALLEGRO_KEY_UP && dir > 1)
                    dir = UP;
                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && dir > 1)
                    dir = DOWN;
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && dir < 2)
                    dir = LEFT;
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && dir < 2)
                    dir = RIGHT;

                if (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    break;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done)
            break;

        // Rendering
        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // wall and score
            al_draw_rectangle(10, 10, 510, 510, al_map_rgb(255, 255, 255), 1);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 20, 0, "# ----- Stats ----- #");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 40, 0, "Speed %.0f", speed);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 50, 0, "Apple %d", (snakeLength - 3));
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 60, 0, "Score %.0f", (snakeLength - 3) * speed);
            if (dead)
                al_draw_textf(font, al_map_rgb(255, 255, 255), 600, 100, 0, "DEAD");

            // snake
            cur = head;
            for (int i=0; i<snakeLength; i++) {
                // al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 10*i+10, 0, "I: %d X: %d Y: %d H: %p P: %p", i, cur->x, cur->y, cur, cur->next);
                al_draw_filled_rectangle(cur->x-5, cur->y-5, cur->x+5, cur->y+5, al_map_rgb(0, 255, 0));
                cur = cur->next;
            }
            
            // apple
            // al_draw_textf(font, al_map_rgb(255, 255, 255), 5, 0, 0, "Apple x: %d y: %d Dead: %d", apple_x, apple_y, dead);
            al_draw_filled_rectangle(apple_x-5, apple_y-5, apple_x+5, apple_y+5, al_map_rgb(255, 0, 0));

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}