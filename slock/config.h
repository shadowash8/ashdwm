/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nobody";

static const char *colorname[NUMCOLS] = {
	[BACKGROUND] = "black",   /* background color */
	[INIT] =   "#2d2d2d",     /* color before typing */
	[INPUT] =  "#005577",     /* color while typing */
	[FAILED] = "#CC3333",     /* color on wrong password */
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{ "color0", STRING, &colorname[INIT] },
	{ "color4", STRING, &colorname[INPUT] },
	{ "color1", STRING, &colorname[FAILED] },
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;

/* logo settings */
static const int logosize = 75;
static const int logow = 12;	/* grid width and height */
static const int logoh = 6;

static XRectangle rectangles[9] = {
	/* x	y	w	h */
	{ 0,	3,	1,	3 },
	{ 1,	3,	2,	1 },
	{ 0,	5,	8,	1 },
	{ 3,	0,	1,	5 },
	{ 5,	3,	1,	2 },
	{ 7,	3,	1,	2 },
	{ 8,	3,	4,	1 },
	{ 9,	4,	1,	2 },
	{ 11,	4,	1,	2 },
};
