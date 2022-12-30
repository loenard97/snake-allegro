#include "snake.c"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>


int main() {
    int size_x = 770;
    int size_y = 520;
    int speed = 3;

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0/speed);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(size_x, size_y);
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_EVENT event;

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    bool done = false;
    bool pause = false;
    bool dead = false;
    bool redraw = true;

    Snake snake;
    initSnake(&snake, (Position) {100, 100}, (Position) {90, 100}, (Position) {80, 100});
    srand(time(NULL));
    Position apple = {(rand() % 49 + 2) * 10, (rand() % 49 + 2) * 10};
    Position snakePixel;

    al_start_timer(timer);
    while (!done) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                if (dead)
                    break;

                if (!pause)
                    dead = (bool) moveSnake(&snake, &apple);
                
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                // key events
                if (event.keyboard.keycode == ALLEGRO_KEY_UP && snake.direction > 1)
                    snake.direction = UP;
                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && snake.direction > 1)
                    snake.direction = DOWN;
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && snake.direction < 2)
                    snake.direction = LEFT;
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && snake.direction < 2)
                    snake.direction = RIGHT;

                if (event.keyboard.keycode == ALLEGRO_KEY_P)
                    pause = !pause;
                if (event.keyboard.keycode == ALLEGRO_KEY_J) {
                    speed += 1;
                    al_set_timer_speed(timer, 1.0/speed);
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_K && speed > 2) {
                    speed -= 1;
                    al_set_timer_speed(timer, 1.0/speed);
                }

                if (event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    break;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        // Rendering
        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // wall and score
            al_draw_rectangle(10, 10, 510, 510, al_map_rgb(255, 255, 255), 1);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 20, 0, "# ----- Stats ----- #");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 40, 0, "Speed %d", speed);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 50, 0, "Apple %d", (snake.length - 3));
            if (dead)
                al_draw_textf(font, al_map_rgb(255, 255, 255), 600, 100, 0, "DEAD");
            if (pause)
                al_draw_textf(font, al_map_rgb(255, 255, 255), 600, 120, 0, "PAUSE");
            
            // controls
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 200, 0, "# ----- Controls ----- #");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 220, 0, "Arrow Keys - Change Direction");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 230, 0, "P - Pause");
            al_draw_textf(font, al_map_rgb(255, 255, 255), 530, 240, 0, "J,K - Speed Up, Down");

            // snake and apple
            for (unsigned int i=0; i<snake.length; i++) {
                snakePixel = getSnakeNodePositions(&snake, i);
                al_draw_filled_rectangle(snakePixel.x-4, snakePixel.y-4, snakePixel.x+4, snakePixel.y+4, al_map_rgb(0, 255, 0));
            }
            al_draw_filled_rectangle(apple.x-4, apple.y-4, apple.x+4, apple.y+4, al_map_rgb(255, 0, 0));

            al_flip_display();

            redraw = false;
        }
    }

    deleteSnake(&snake);

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}