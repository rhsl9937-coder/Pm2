#ifndef GFX_H
#define GFX_H

#define GFX_SCENE_COUNT 6
#define GFX_HOME  0
#define GFX_STUDY 1
#define GFX_WORK  2
#define GFX_REST  3
#define GFX_GOOUT 4
#define GFX_TITLE 5

/* pictures (top screen) */
void gfx_init(void);
void gfx_show(int scene);
int  gfx_princess_x(int scene);

/* effects: call fx_init() after gfx_init(); call fx_frame() once per frame */
void fx_init(void);
void fx_frame(void);
void fx_fade_out(int screens, int frames);   /* screens: 1 = top, 2 = bottom, 3 = both */
void fx_fade_in(int screens, int frames);
void fx_flash(int screens, int frames);
void fx_burst(int kind, int x, int y, int count);  /* kind: 0 star, 1 heart, 2 coin, 3 blue star */
void fx_sparkle_at(int x, int y);
void fx_twinkle(int every);
void fx_rain(void);

#endif
