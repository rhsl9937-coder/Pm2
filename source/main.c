#include <nds.h>
#include <stdio.h>
#include <stdbool.h>
#include "gfx.h"

typedef struct {
    int month;
    int money;
    int strength;
    int intelligence;
    int charm;
    int stress;
    int reputation;
    int action_count;
    bool ended;
} GameState;

static GameState game = {1, 120, 30, 30, 30, 10, 0, 0, false};
static int selected = 0;
static int scene = GFX_HOME;   /* picture on the top screen */
static const char *last_msg = "Welcome, Princess!";

static const char *actions[] = {"Study", "Work", "Rest", "Go Out"};
static const int burst_kind[4] = {3, 2, 1, 0};   /* blue star, coin, heart, star */

static void draw_title(bool show_prompt) {
    consoleClear();
    iprintf("\n\n\n\n\n\n\n");
    iprintf("        PRINCESS LIFE DS\n\n");
    iprintf("      Original raising sim\n\n\n\n");
    if (show_prompt) iprintf("          PRESS START");
}

static void draw(void) {
    consoleClear();
    iprintf(" PRINCESS LIFE DS\n");
    iprintf(" Original raising sim\n");
    iprintf("----------------------\n");
    iprintf("Month: %d / 12\n", game.month);
    iprintf("Gold : %d G\n", game.money);
    iprintf("STR  : %d\n", game.strength);
    iprintf("INT  : %d\n", game.intelligence);
    iprintf("CHARM: %d\n", game.charm);
    iprintf("STRESS: %d\n", game.stress);
    iprintf("FAME : %d\n", game.reputation);
    iprintf("----------------------\n");
    if (!game.ended) {
        iprintf("Choose an action:\n\n");
        for (int i = 0; i < 4; i++) {
            iprintf("%s %s\n", (i == selected) ? ">" : " ", actions[i]);
        }
        iprintf("\n %s\n", last_msg);
        iprintf("\nUP/DOWN: select  A: go");
    } else {
        iprintf(" YEAR COMPLETE!\n\n");
        if (game.intelligence >= 55) iprintf("Ending: Scholar\n");
        else if (game.charm >= 55) iprintf("Ending: Star\n");
        else if (game.money >= 250) iprintf("Ending: Merchant\n");
        else iprintf("Ending: New Beginning\n");
        iprintf("\nPress START to go back\nto the title.");
    }
}

static int clamp_stat(int value) {
    if (value < 0) return 0;
    if (value > 99) return 99;
    return value;
}

static void take_action(void) {
    switch (selected) {
        case 0: /* Study */
            game.intelligence += 7;
            game.stress += 5;
            game.money -= 5;
            last_msg = "Study: INT+7 STRESS+5";
            break;
        case 1: /* Work */
            game.money += 35;
            game.strength += 2;
            game.stress += 6;
            last_msg = "Work: +35G STR+2";
            break;
        case 2: /* Rest */
            game.stress -= 12;
            game.strength += 2;
            last_msg = "Rest: STRESS-12";
            break;
        case 3: /* Go Out */
            game.charm += 7;
            game.reputation += 4;
            game.money -= 15;
            game.stress -= 2;
            last_msg = "Go out: CHARM+7 FAME+4";
            break;
    }
    game.strength = clamp_stat(game.strength);
    game.intelligence = clamp_stat(game.intelligence);
    game.charm = clamp_stat(game.charm);
    game.reputation = clamp_stat(game.reputation);
    game.stress = clamp_stat(game.stress);
    if (game.money < 0) game.money = 0;

    game.action_count++;
    if (game.action_count >= 4) {
        game.action_count = 0;
        game.month++;
        if (game.month > 12) game.ended = true;
    }

    scene = GFX_STUDY + selected;   /* study, work, rest, go-out pictures are in that order */
    draw();

    /* effect: fade the top screen, swap the picture, fade back in, burst of sparkles */
    fx_fade_out(1, 5);
    gfx_show(scene);
    fx_fade_in(1, 6);
    fx_burst(burst_kind[selected], gfx_princess_x(scene), 100, 14);
}

static void restart_game(void) {
    game.month = 1;
    game.money = 120;
    game.strength = 30;
    game.intelligence = 30;
    game.charm = 30;
    game.stress = 10;
    game.reputation = 0;
    game.action_count = 0;
    game.ended = false;
    selected = 0;
    scene = GFX_HOME;
    last_msg = "Welcome, Princess!";
}

static void run_title(void) {
    int t = 0;
    gfx_show(GFX_TITLE);
    draw_title(true);
    fx_fade_in(3, 30);

    while (1) {
        scanKeys();
        if (keysDown() & KEY_START) break;
        fx_twinkle(4);
        if (t % 30 == 0) draw_title(((t / 30) % 2) == 0);
        t++;
        fx_frame();
    }

    fx_burst(0, 128, 100, 16);
    fx_burst(1, 128, 100, 8);
    fx_flash(3, 10);
    fx_fade_out(3, 20);
}

static void play_game(void) {
    int t = 0;
    restart_game();
    gfx_show(scene);
    draw();
    fx_fade_in(3, 24);

    while (!game.ended) {
        scanKeys();
        int down = keysDown();

        if (down & KEY_UP) selected = (selected + 3) % 4;
        if (down & KEY_DOWN) selected = (selected + 1) % 4;
        if (down & KEY_A) take_action();
        if (down) draw();

        if (t % 45 == 0) fx_sparkle_at(gfx_princess_x(scene), 112);   /* ambient glitter */
        t++;
        fx_frame();
    }
}

static void run_ending(void) {
    fx_fade_out(3, 20);
    scene = GFX_TITLE;
    gfx_show(scene);
    draw();
    fx_fade_in(3, 24);

    while (1) {
        scanKeys();
        if (keysDown() & KEY_START) break;
        fx_rain();
        fx_frame();
    }
    fx_fade_out(3, 20);
}

int main(void) {
    consoleDemoInit();   /* text goes to the bottom screen */
    setBrightness(3, -16);   /* start from black so nothing flickers */
    gfx_init();          /* pictures go to the top screen */
    fx_init();           /* sparkle sprites, fades */

    while (1) {
        run_title();
        play_game();
        run_ending();
    }

    return 0;
}
