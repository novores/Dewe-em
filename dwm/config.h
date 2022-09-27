/* See LICENSE file for copyright and license details. */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
static const double activeopacity   = 1.0f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.8f;   /* Window opacity when it's inactive (0 <= opacity <= 1) */
static       Bool bUseOpacity       = True;     /* Starts with opacity on any unfocused windows */
static const int horizpadbar        = 3;        /* horizontal padding for statusbar */
static const int vertpadbar         = 1;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font Bandit:style=Medium:size=9.5" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#1d2021";
static const char col_gray2[]       = "#282828";
static const char col_gray3[]       = "#504945";
static const char col_gray4[]       = "#ebdbb2";
static const char col_yellow[]      = "#e78a4e";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_gray1, col_gray3 },
	[SchemeSel]  = { col_yellow, col_gray1,  col_gray4 },
};

// autostart
static const char *const autostart[] = {
	"xrdb", "-merge", "/home/novores/.Xresources", NULL,
  "feh", "--bg-fill", "/home/novores/Gambar/wallpapers/minimalist_logo.jpg", NULL,
  "/usr/libexec/polkit-mate-authentication-agent-1", NULL,
  "xbanish", NULL,
  "pipewire-pulse", NULL,
  "/home/novores/Developments/deweem/xautolock.sh", NULL,
  "sh", "-c", "pkill bar.sh; /home/novores/Developments/deweem/dwmstatus/bar.sh", NULL,
  "xcompmgr", "-c", "-C", "-t-5", "-l-5", "-r4.2", "-o.55", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                    instance    title       tags mask     isfloating   monitor */
	{"Old School RuneScape",    NULL,       NULL,       1 << 7,       1,           -1 },
	{"net-runelite-client-RuneLite",        NULL,       NULL,       1 << 7,       1,           -1 },
  {"SimpleScreenRecorder",    NULL,       NULL,       0,            1,           -1},
  {"qutebrowser",             NULL,       NULL,       1 << 1,       0,           -1},
  {"Firefox",                 NULL,       NULL,       1 << 1,       0,           -1},
  {"mpv",                     NULL,       NULL,       0,            1,           -1},
  {"feh",                     NULL,       NULL,       0,            1,           -1},
  {"Lxappearance",            NULL,       NULL,       0,            1,           -1},
  {"qt5ct",                   NULL,       NULL,       0,            1,           -1},
  {"aft-linux-qt",            NULL,       NULL,       0,            1,           -1},
  {"KeePassXC",               NULL,       NULL,       0,            1,           -1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "    ",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

// library keyxf86
#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-s", "-m", dmenumon, "-c", "-bw", "3", "-l", "7", "-h", "35", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray4, "-sb", col_yellow, "-sf", col_gray2, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *volup[] = {"pamixer", "-i", "2", NULL};
static const char *voldown[] = {"pamixer", "-d", "2", NULL};
static const char *mute[] = {"pamixer", "-t", NULL};
static const char *brup[] = {"xbacklight", "-inc", "2", NULL};
static const char *brdown[] = {"xbacklight", "-dec", "2", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_a,      toggleopacity,  {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
  { MODKEY|ControlMask,           XK_q,        quit,         {0} }, 
  
  //launcher
  { MODKEY|ControlMask,           XK_l,       spawn,        SHCMD("xautolock -locknow -locker slock -killtime 10 -killer 'loginctl suspend' -detectsleep") }, 

  //command using xf86 keys
  {0,                             XF86XK_AudioMute,           spawn,  {.v = mute}},
  {0,                             XF86XK_AudioRaiseVolume,    spawn,  {.v = volup}},
  {0,                             XF86XK_AudioLowerVolume,    spawn,  {.v = voldown}},
  {0,                             XF86XK_MonBrightnessUp,     spawn,  {.v = brup}},
  {0,                             XF86XK_MonBrightnessDown,   spawn,  {.v = brdown}},

  // Misc
  {ALTKEY,                        XK_m,       spawn,        SHCMD("mpd && dunstify 'mpd' 'online'")},
  {ALTKEY|ShiftMask,              XK_m,       spawn,        SHCMD("pkill mpd && dunstify 'mpd' 'offline'")},
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

