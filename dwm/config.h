/*
* PATCHES
* dwm-statuscmd-20210405-67d76bd - for dwmblocks async
* dwm-hide_vacant_tags-6.4 - hide empty workspaces
* dwm-sticky-6.5 - sticky windows
* dwm-attachaside-6.6 - change the layout of new window added 
* dwm-resetnmaster-6.3 - reset master
* dwm-fibonacci-6.2 - fibonacci layouts
* dwm-alwayscenter-20200625-f04cac6 - center floating windows
* dwm-pertag-6.2 - unique layout per tag
* shiftview - switch tags
* dwm-resizecorners-6.5 - resize from the nearest corner
* dwm-fullscreen-6.2 - real fullscreen function
* dwm-preserveonrestart-6.3 - preserve window location on restart
*/
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 0;       /* vertical padding of bar */
static const int sidepad            = 0;       /* horizontal padding of bar */
static const char *fonts[] = { "Pragmasevka:size=11" };
static const char dmenufont[]       = "Pragmasevka:size=11";

/* Monochrome Palette */
static const char col_black[]       = "#000000"; // Background
static const char col_dark_gray[]   = "#222222"; // Inactive border
static const char col_gray[]        = "#bbbbbb"; // Inactive text
static const char col_white[]       = "#ffffff"; // Active text / Active border

static const char *colors[][3]      = {
	/* fg         bg         border   */
	[SchemeNorm] = { col_gray,  col_black, col_dark_gray },
	[SchemeSel]  = { col_white, col_black, col_white },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 60;  /* refresh rate (per second) for client move/resize */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "sh", "-c", "j4-dmenu-desktop --dmenu=dmenu --term=kitty", NULL };
static const char *clipcmd[] = { "sh", "-c", "greenclip print | dmenu -s -l 20 -p 'Clipboard' | xargs -r -d'\\n' -I '{}' greenclip print '{}'", NULL };
static const char *browsercmd[] = { "qutebrowser-profile", "--menu", "dmenu", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *filescmd[]  = { "pcmanfm-qt", NULL };
static const char *emacscmd[]  = { "emacsclient", "-c", NULL };
static const char *phonecmd[]  = { "dmenu-connect", NULL };

/* screenshot commands */
static const char *shotcpycmd[]  = { "sh", "-c", "maim -s | xclip -selection clipboard -t image/png && notify-send 'Screenshot' 'Copied to Clipboard' -i camera-photo", NULL };
static const char *shotsavecmd[] = { "sh", "-c", "maim -s ~/Pictures/Screenshots/$(date +%s)_dwm.png && notify-send 'Screenshot' 'Saved to ~/Pictures/Screenshots' -i camera-photo", NULL };

/* hardware commands */
static const char *lockcmd[] = { "sh", "-c", "XSECURELOCK_SHOW_DATETIME=1 xsecurelock", NULL };
static const char *mutecmd[]    = { "sh", "-c", "wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle && pkill -RTMIN+4 dwmblocks", NULL };
static const char *volupcmd[]   = { "sh", "-c", "wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+ && pkill -RTMIN+4 dwmblocks", NULL };
static const char *voldowncmd[] = { "sh", "-c", "wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- && pkill -RTMIN+4 dwmblocks", NULL };
static const char *briup[]   = { "brightnessctl", "set", "+10%", NULL };
static const char *bridown[] = { "brightnessctl", "set", "10%-", NULL };

#include "exitdwm.c"
/* keybinds */
/* Layout Layer */
static Key keyseq_layout[] = {
	/* modifier      key        function        argument */
	{ 0,             XK_1,      setlayout,      {.v = &layouts[0]} }, // Tile
	{ 0,             XK_2,      setlayout,      {.v = &layouts[1]} }, // Floating
	{ 0,             XK_3,      setlayout,      {.v = &layouts[2]} }, // Monocle
	{ 0,             XK_4,      setlayout,      {.v = &layouts[3]} }, // Spiral
	{ 0,             XK_5,      setlayout,      {.v = &layouts[4]} }, // Dwindle
	{ 0,             XK_g,      setlayout,      {0} },                // Toggle Last
	{0} 
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	/* --- Launchers --- */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = clipcmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = filescmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = emacscmd } },
	{ MODKEY,                       XK_l,      spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = phonecmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	
	/* --- Navigation --- */
	{ MODKEY,                       XK_n,      focusstack,     {.i = +1 } }, // Next window
	{ MODKEY,                       XK_o,      focusstack,     {.i = -1 } }, // Prev window
    { MODKEY|ControlMask,           XK_n,      shiftview,      {.i = -1 } }, // Next tag
    { MODKEY|ControlMask,           XK_o,      shiftview,      {.i = +1 } }, // Prev tag
 
    /* --- Resizing --- */
	{ MODKEY,                       XK_e,      setmfact,       {.f = -0.05} }, // Shrink master
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} }, // Grow master

	/* --- Master Area --- */
	{ MODKEY,                       XK_m,      incnmaster,     {.i = +1 } }, // More windows in master
	{ MODKEY|ControlMask,           XK_m,      incnmaster,     {.i = -1 } }, // Fewer windows in master
	{ MODKEY,                       XK_z,      zoom,           {0} },        // Push window to master
    { MODKEY,                       XK_r,      resetnmaster,   {0}},  // Reset master

	/* --- Layout Management --- */
    { MODKEY|ShiftMask,             XK_s,      keypress_other, {.v = keyseq_layout} }, // Layout Layer
	{ MODKEY,                       XK_g,      togglefloating, {0} },
    { MODKEY,                       XK_a,      fullscreen,     {0} },
    { MODKEY,                       XK_s,      togglesticky,   {0} },
	
	/* --- System Control --- */
	{ MODKEY|ShiftMask,             XK_q,      exitdwm,        {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY|ControlMask,           XK_b,      togglebar,      {0} },

	/* --- Monitor Focus --- */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* --- Screenshot --- */
    { 0,                    XK_Print,  spawn,          {.v = shotcpycmd } },
    { ControlMask,          XK_Print,  spawn,          {.v = shotsavecmd } },

	/* --- Tag Keys --- */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

    /* --- Hardware Keys --- */
	{ 0,             XF86XK_AudioMute,         spawn,          {.v = mutecmd } },
	{ 0,             XF86XK_AudioLowerVolume,  spawn,          {.v = voldowncmd } },
	{ 0,             XF86XK_AudioRaiseVolume,  spawn,          {.v = volupcmd } },
    { 0,             XF86XK_MonBrightnessUp,   spawn,          {.v = briup } },
    { 0,             XF86XK_MonBrightnessDown, spawn,          {.v = bridown } },

    {0}
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
