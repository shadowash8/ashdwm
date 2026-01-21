/*
* PATCHES
* dwm-statuscmd-20210405-67d76bd - for dwmblocks async
* dwm-hide_vacant_tags-6.4 - hide empty workspaces
* dwm-sticky-6.5 - sticky windows
* dwm-attachaside-6.6 - change the layout of new window added 
* dwm-resetnmaster-6.3 - reset master
* dwm-alwayscenter-20200625-f04cac6 - center floating windows
* dwm-pertag-6.2 - unique layout per tag
* shiftview - switch tags
* dwm-resizecorners-6.5 - resize from the nearest corner
* dwm-fullscreen-6.2 - real fullscreen function
* dwm-preserveonrestart-6.3 - preserve window location on restart
* dwm-barpadding-6.6 - add padding for the dwm bar
* dwm-cool_autostart-6.5 - use autostart inside dwm itself!
* dwm-systray-6.6 - add systray in dwm bar
* dwm-exitmenu-6.3 - add a exit menu in dwm
* dwm-attachbelow-toggleable-6.2 - attach new window below the current stack
* dwm-taglayouts-6.4 - define layout per tag
* dwm-colorbar-6.3 - fine color control over the bar
*/

#include <X11/XF86keysym.h>
#include "exitdwm.c"

/* autostart */
static const char *const autostart[] = {
	/* 1. Portal & Policykit */
	"/usr/libexec/xdg-desktop-portal", NULL,
	"/usr/libexec/xdg-desktop-portal-lxqt", NULL,
	"/usr/libexec/lxqt-policykit-agent", NULL,

	/* 2. Appearance & Desktop */
    "sh", "-c", "cat ~/.cache/wal/sequences", NULL,
	"sh", "-c", "feh --bg-fill $(cat ~/.cache/wal/wal)", NULL,
    "sh", "-c", "xrdb -merge ~/.Xresources", NULL,
	"picom", NULL,
	"unclutter", NULL,

	/* 3. Daemons & Services */
	"dunst", NULL,
	"dwmblocks", NULL,
	"greenclip", "daemon", NULL,
	"emacs-lucid", "--daemon", NULL,
	"kdeconnectd", NULL,
	"kdeconnect-indicator", NULL,
	"libinput-gestures-setup", "start", NULL,

	/* 4. Screen Lock & Power */
    "xset", "s", "300", NULL,
    /* 2. Lock the screen AND turn off the monitor when the timer hits */
    "xss-lock", "-l", "--", "sh", "-c", "i3lock && xset dpms force off", NULL,

	/* 5. Gsettings (Theme forcing) */
	"sh", "-c", "gsettings set org.gnome.desktop.interface gtk-theme 'Adwaita'", NULL,
	"sh", "-c", "gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark'", NULL,

    "sh", "-c", "xrdb ~/.Xresources && xdotool key --clearmodifiers super+control+shift+q && pidof st | xargs -r kill -USR1", NULL,

	NULL /* terminate the entire array */
};

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 7;        /* horiz inner gap between windows */
static const unsigned int gappiv    = 7;        /* vert inner gap between windows */
static const unsigned int gappoh    = 7;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 7;        /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 7;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 0;        /* vertical padding of bar */
static const int sidepad            = 0;        /* horizontal padding of bar */
static char font[]            = "Iosevka:pixelsize=16:antialias=true:autohint=true";
static const char *fonts[]          = { font };
static char dmenufont[]       = "Iosevka:pixelsize=16:antialias=true:autohint=true";

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
       [SchemeStatus]  = { normfgcolor, normbgcolor,  "#000000"  },
       [SchemeTagsSel]  = { selfgcolor, selbgcolor,  "#000000"  },
       [SchemeTagsNorm]  = { normfgcolor, normbgcolor,  "#000000"  },
       [SchemeInfoSel]  = { selfgcolor, selbgcolor,  "#000000"  },
       [SchemeInfoNorm]  = { normfgcolor, normbgcolor,  "#000000"  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int taglayouts[] = { 0, 2, 5, 1, 11, 9, 0, 0, 0 };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "mpv",      NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 60;  /* refresh rate (per second) for client move/resize */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
static int attachbelow       = 1;    /* 1 means attach after the currently active window */

#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },   
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "|M|",      centeredmaster },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "[\\]",     dwindle },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
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
#define STATUSBAR "dwmblocks"

/* helper for spawning shell commands */
#define SHCMD(cmd) { "sh", "-c", cmd, NULL }
#define TERMINAL "kitty"

/* commands */
static char dmenumon[2] = "0"; 
static const char *dmenucmd[] = { "sh", "-c", "j4-dmenu-desktop --dmenu='dmenu -m \"$0\"' --term=" TERMINAL, dmenumon, NULL };
static const char *clipcmd[]      = SHCMD("greenclip print | dmenu -s -l 20 -p 'Clipboard' | xargs -r -d'\\n' -I '{}' greenclip print '{}'");
static const char *browsercmd[]   = { "qbpm", "choose", NULL };
static const char *termcmd[]      = { TERMINAL, NULL };
static const char *filescmd[]     = { "pcmanfm-qt", NULL };
static const char *emacscmd[]     = { "emacsclient", "-c", NULL };
static const char *phonecmd[]     = SHCMD("connect --menu dmenu");
static const char *websearchcmd[] = SHCMD("websearch --menu dmenu");
static const char *notescmd[] = SHCMD("notes --menu dmenu");
static const char *musiccmd[]     = { TERMINAL, "-e", "rmpc", NULL };
static const char *wallpapercmd[]     = { "walmenu", NULL };

/* screenshot commands */
static const char *shotcpycmd[]  = SHCMD("maim -s | xclip -selection clipboard -t image/png && notify-send 'Screenshot' 'Copied to Clipboard' -i camera-photo");
static const char *shotsavecmd[] = SHCMD("screenshot");
static const char *ocrcmd[] = SHCMD("maim -s /tmp/ocr.png && tesseract /tmp/ocr.png - | xclip -selection clipboard && notify-send 'OCR' 'Text copied to clipboard' -i edit-paste && rm /tmp/ocr.png");
static const char *colpickcmd[] = SHCMD("screenshot color");

/* hardware commands */
static const char *lockcmd[]     = SHCMD("XSECURELOCK_SHOW_DATETIME=1 xsecurelock");
static const char *mutecmd[]     = SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; pkill -RTMIN+4 dwmblocks");
static const char *volupcmd[]    = SHCMD("wpctl set-volume -l 1 @DEFAULT_AUDIO_SINK@ 5%+; pkill -RTMIN+4 dwmblocks");
static const char *voldowncmd[]  = SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; pkill -RTMIN+4 dwmblocks");
static const char *briup[]       = SHCMD("brightnessctl set 10%+");
static const char *bridown[]     = SHCMD("brightnessctl set 10%-");

/* Layout Layer */
static Key keyseq_layout[] = {
	/* modifier      key        function        argument */
	{ 0,             XK_1,      setlayout,      {.v = &layouts[0]}  }, // Tile
	{ 0,             XK_2,      setlayout,      {.v = &layouts[1]}  }, // Floating
	{ 0,             XK_3,      setlayout,      {.v = &layouts[2]}  }, // Monocle
	{ 0,             XK_4,      setlayout,      {.v = &layouts[3]}  }, // Spiral
	{ 0,             XK_5,      setlayout,      {.v = &layouts[4]}  }, // Centeredmaster
	{ 0,             XK_6,      setlayout,      {.v = &layouts[5]}  }, // Deck
	{ 0,             XK_7,      setlayout,      {.v = &layouts[6]}  }, // Bstack
	{ 0,             XK_8,      setlayout,      {.v = &layouts[7]}  }, // Bstackhoriz
	{ 0,             XK_9,      setlayout,      {.v = &layouts[8]}  }, // Grid
	{ 0,             XK_0,      setlayout,      {.v = &layouts[9]}  }, // Nrowgrid
	{ 0,             XK_minus,  setlayout,      {.v = &layouts[10]} }, // Horizgrid
	{ 0,             XK_equal,  setlayout,      {.v = &layouts[11]} }, // Gaplessgrid
	{ 0,             XK_m,      setlayout,      {.v = &layouts[12]} }, // Dwindle 
	{ 0,             XK_f,      setlayout,      {.v = &layouts[13]} }, // Centeredfloating
	{ 0,             XK_g,      setlayout,      {0} },                 // Toggle Last
    { 0,             XK_Tab,    toggleAttachBelow,           {0} },    // ToggleAttachBelow
    { 0,             0,         NULL,           {0} } 
};

/* Gaps & Resizing Layer */
static Key keyseq_gaps[] = {
	/* modifier      key        function        argument */
	/* Total Gaps (Overall) */
	{ 0,             XK_n,      incrgaps,       {.i = +3 } },
	{ 0,             XK_e,      incrgaps,       {.i = -3 } },

	/* Inner Gaps (Horizontal/Vertical) */
	{ 0,             XK_i,      incrigaps,      {.i = +3 } },
	{ 0,             XK_o,      incrogaps,      {.i = +3 } },
	{ ShiftMask,     XK_i,      incrihgaps,     {.i = +3 } }, // Inner Horiz
	{ ShiftMask,     XK_o,      incrivgaps,     {.i = +3 } }, // Inner Vert

	/* Outer Gaps (Horizontal/Vertical) */
	{ 0,             XK_h,      incrohgaps,     {.i = +3 } }, // Outer Horiz
	{ 0,             XK_l,      incrovgaps,     {.i = +3 } }, // Outer Vert

	/* Cfacts (Window Weight / Size) */
    { ControlMask,   XK_n,      setcfact,       {.f = +0.25} },
	{ ControlMask,   XK_e,      setcfact,       {.f = -0.25} },
	{ ControlMask,   XK_r,      setcfact,       {.f =  0.00} },

	/* Control & Reset */
	{ 0,             XK_r,      defaultgaps,    {0} },
	{ 0,             XK_g,      togglegaps,     {0} },
	{ 0,             XK_Return, zoom,           {0} },
	
	{ 0,             0,         NULL,           {0} }
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
	{ MODKEY,                       XK_m,      spawn,          {.v = musiccmd } },
	{ MODKEY|ControlMask,           XK_r,      spawn,          {.v = websearchcmd } },
	{ MODKEY|ShiftMask,             XK_space,  spawn,          {.v = notescmd } },
	{ MODKEY|ControlMask,           XK_space,  spawn,          {.v = wallpapercmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,     {0} },
	
	/* --- Navigation --- */
	{ MODKEY,                       XK_n,      focusstack,     {.i = +1 } }, // Next window
	{ MODKEY,                       XK_o,      focusstack,     {.i = -1 } }, // Prev window
    { MODKEY|ControlMask,           XK_n,      shiftview,      {.i = -1 } }, // Next tag
    { MODKEY|ControlMask,           XK_o,      shiftview,      {.i = +1 } }, // Prev tag
 
    /* --- Resizing --- */
	{ MODKEY,                       XK_e,      setmfact,       {.f = -0.05} }, // Shrink master
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} }, // Grow master

	/* --- Master Area --- */
	{ MODKEY,                       XK_z,      incnmaster,     {.i = +1 } }, // More windows in master
	{ MODKEY|ControlMask,           XK_z,      incnmaster,     {.i = -1 } }, // Fewer windows in master
	{ MODKEY|ShiftMask,             XK_z,      zoom,           {0} },        // Push window to master
    { MODKEY,                       XK_r,      resetnmaster,   {0}},  // Reset master

	/* --- Layout Management --- */
    { MODKEY|ShiftMask,             XK_s,      keypress_other, {.v = keyseq_layout} }, // Layout Layer
    { MODKEY|ShiftMask,             XK_f,      keypress_other, {.v = keyseq_gaps}   }, // Gaps Layer
	{ MODKEY,                       XK_g,      togglefloating, {0} },
	{ MODKEY,                       XK_minus,  incrgaps,       {.i = -3 } },
	{ MODKEY,                       XK_equal,  incrgaps,       {.i = +3 } },
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
    { ShiftMask,            XK_Print,  spawn,          {.v = ocrcmd } },
    { ControlMask|ShiftMask,XK_Print,  spawn,          {.v = colpickcmd } },

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
