#include <X11/XF86keysym.h>
#define SESSION_FILE "/tmp/dwm-session"
#define STATUSBAR "dwmblocks"

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "SF Mono:pixelsize=14:antialias=true:autohint=true";
static const char *fonts[]          = { font };
static char dmenufont[]       = "monospace:size=10";

static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
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
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 60;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands */
#define SHCMD(cmd) { "sh", "-c", cmd, NULL }
#define TERMINAL "st"

/* commands */
static char dmenumon[2] = "0"; 
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static const char *clipcmd[]      = SHCMD("greenclip print | rofi -dmenu | xargs -r -d'\\n' -I '{}' greenclip print '{}'");
static const char *browsercmd[]   = { "firefox", NULL };
static const char *termcmd[]      = { TERMINAL, NULL };
static const char *filescmd[]     = { "thunar", NULL };
static const char *emacscmd[]     = { "emacsclient", "-c", NULL };
static const char *phonecmd[]     = SHCMD("connect");
static const char *websearchcmd[] = SHCMD("websearch");
static const char *notescmd[] = SHCMD("notes");
static const char *musiccmd[]     = { TERMINAL, "-e", "rmpc", NULL };
static const char *wallpapercmd[]     = { "walmenu", NULL };
static const char *exitdwmcmd[]     = { "pkill", "Xorg", NULL };

/* screenshot commands */
static const char *shotcpycmd[]  = SHCMD("maim -s | xclip -selection clipboard -t image/png && notify-send 'Screenshot' 'Copied to Clipboard' -i camera-photo");
static const char *shotsavecmd[] = SHCMD("screenshot");
static const char *ocrcmd[] = SHCMD("maim -s /tmp/ocr.png && tesseract /tmp/ocr.png - | xclip -selection clipboard && notify-send 'OCR' 'Text copied to clipboard' -i edit-paste && rm /tmp/ocr.png");
static const char *colpickcmd[] = SHCMD("screenshot color");

/* hardware commands */
static const char *lockcmd[]     = SHCMD("slock");
static const char *mutecmd[]     = SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; pkill -RTMIN+4 dwmblocks");
static const char *volupcmd[]    = SHCMD("wpctl set-volume -l 1 @DEFAULT_AUDIO_SINK@ 5%+; pkill -RTMIN+4 dwmblocks");
static const char *voldowncmd[]  = SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; pkill -RTMIN+4 dwmblocks");
static const char *briup[]       = SHCMD("brightnessctl set 10%+");
static const char *bridown[]     = SHCMD("brightnessctl set 10%-");


static const Key keys[] = {
	/* modifier                     key        function        argument */
	/* --- Launchers --- */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = clipcmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = filescmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = emacscmd } },
	{ MODKEY,                       XK_l,      spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = phonecmd } },
	{ MODKEY,                       XK_m,      spawn,          {.v = musiccmd } },
	{ MODKEY|ControlMask,           XK_r,      spawn,          {.v = websearchcmd } },
	{ MODKEY|ShiftMask,             XK_space,  spawn,          {.v = notescmd } },
	{ MODKEY|ControlMask,           XK_space,  spawn,          {.v = wallpapercmd } },
    { MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = exitdwmcmd } },

    /* --- Navigation --- */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, // Next window
	{ MODKEY,                       XK_semicolon,focusstack,     {.i = -1 } }, // Prev window
	{ MODKEY,                       XK_x,      togglebar,      {0} },
 
    /* --- Resizing --- */
	{ MODKEY,                       XK_k,      setmfact,       {.f = -0.05} }, // Shrink master
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} }, // Grow master
	{ MODKEY,                       XK_Tab,    view,           {0} },

	/* --- Master Area --- */
	{ MODKEY,                       XK_b,      zoom,           {0} },        // Push window to master
	{ MODKEY|ShiftMask,             XK_b,      incnmaster,     {.i = +1 } }, // More windows in master
	{ MODKEY|ControlMask,           XK_b,      incnmaster,     {.i = -1 } }, // Fewer windows in master

    /* --- Layouts --- */
	{ MODKEY|ShiftMask|ControlMask, XK_h,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_h,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_h,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_h,      setlayout,      {0} },
    { MODKEY,                       XK_g,      togglefloating, {0} },
    { MODKEY,                       XK_a,      togglefullscr,  {0} },
    { MODKEY,                       XK_d,      togglesticky,   {0} },

    /* --- Monitor Focus --- */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

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

    /* --- Screenshot --- */
    { 0,                            XK_Print,  spawn,          {.v = shotcpycmd } },
    { ControlMask,                  XK_Print,  spawn,          {.v = shotsavecmd } },
    { ShiftMask,                    XK_Print,  spawn,          {.v = ocrcmd } },
    { ControlMask|ShiftMask,        XK_Print,  spawn,          {.v = colpickcmd } },

    /* --- Hardware Keys --- */
	{ 0,             XF86XK_AudioMute,         spawn,          {.v = mutecmd } },
	{ 0,             XF86XK_AudioLowerVolume,  spawn,          {.v = voldowncmd } },
	{ 0,             XF86XK_AudioRaiseVolume,  spawn,          {.v = volupcmd } },
    { 0,             XF86XK_MonBrightnessUp,   spawn,          {.v = briup } },
    { 0,             XF86XK_MonBrightnessDown, spawn,          {.v = bridown } },
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
