/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

static int topbar = 1;                      /* -b  option; if 0, dmenu appears at bottom     */
static int centered = 1;                    /* -c option; centers dmenu on screen */
static int min_width = 500;                    /* minimum width when centered */
static const float menu_height_ratio = 4.0f;  /* This is the ratio used in the original calculation */
static int fuzzy  = 1;                      /* -F  option; if 0, dmenu doesn't use fuzzy matching */
/* -fn option overrides fonts[0]; default X11 font or font set */
static const char *fonts[] = {
	"Iosevka:pixelsize=16:antialias=true:autohint=true"
};
static const char *prompt      = "Run:";      /* -p  option; prompt to the left of input field */
static const char *colors[SchemeLast][2] = {
	/* fg         bg       */
	[SchemeNorm] = { "#bbbbbb", "#000000" }, /* Gray text on Black bg */
	[SchemeSel]  = { "#000000", "#ffffff" }, /* Black text on White bg (Highlight) */
	[SchemeOut]  = { "#000000", "#ffffff" }, /* Matching highlight for multi-select */
};
/* -l option; if nonzero, dmenu uses vertical list with given number of lines */
static unsigned int lines      = 10;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";

/* Size of the window border */
static unsigned int border_width = 2;
