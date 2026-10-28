#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

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
static const char *actions[] = {"Study", "Work", "Rest", "Go Out"};

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
        iprintf("\nUP/DOWN: select\nA: confirm  B: help");
    } else {
        iprintf(" YEAR COMPLETE!\n\n");
        if (game.intelligence >= 55) iprintf("Ending: Scholar\n");
        else if (game.charm >= 55) iprintf("Ending: Star\n");
        else if (game.money >= 250) iprintf("Ending: Merchant\n");
        else iprintf("Ending: New Beginning\n");
        iprintf("\nPress START to restart.");
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
            break;
        case 1: /* Work */
            game.money += 35;
            game.strength += 2;
            game.stress += 6;
            break;
        case 2: /* Rest */
            game.stress -= 12;
            game.strength += 2;
            break;
        case 3: /* Go Out */
            game.charm += 7;
            game.reputation += 4;
            game.money -= 15;
            game.stress -= 2;
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
}

int main(void) {
    consoleDemoInit();
    draw();

    while (1) {
        scanKeys();
        int down = keysDown();

        if (game.ended) {
            if (down & KEY_START) {
                restart_game();
                draw();
            }
        } else {
            if (down & KEY_UP) selected = (selected + 3) % 4;
            if (down & KEY_DOWN) selected = (selected + 1) % 4;
            if (down & KEY_A) take_action();
            if (down) draw();
        }
        swiWaitForVBlank();
    }

    return 0;
}
