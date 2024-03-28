/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMINAL2 "st" /* xfce4-terminal can be added as 2nd terminal */
/* added second terminal */
#define TERMCLASS "St"
#define BROWSER "chromium"
#define EDITOR "subl"
#define FILE_EXPLORER "thunar"

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 0;        /* horiz inner gap between windows */
static unsigned int gappiv    = 0;        /* vert inner gap between windows */
static unsigned int gappoh    = 0;        /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 0;        /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "InconsolataSemiCondensedBold:size=12", "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
static const char dmenufont[]       = "InconsolataSemiCondensedBold:size=12";
static char normbgcolor[]           = "#353535";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#F8F8F2";
static char selfgcolor[]            = "#F8F8F2";
static char selbordercolor[]        = "#616161";
static char selbgcolor[]            = "#616161";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
    */
    /* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
    { "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
    { TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
    { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
    { TERMCLASS,      "floatterm", NULL,       	    0,       1,           1,         0,        -1 },
    { TERMCLASS,      "bg",        NULL,       	    1 << 7,       0,           1,         0,        -1 },
    { TERMCLASS,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
    { TERMCLASS,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",	tile },			/* Default: Master on left, slaves on right */
    { "TTT",	bstack },		/* Master on top, slaves on bottom */

    { "[@]",	spiral },		/* Fibonacci spiral */
    { "[\\]",	dwindle },		/* Decreasing in size right and leftward */

    { "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
    { "[M]",	monocle },		/* All windows on top of eachother */

    { "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
    { ">M>",	centeredfloatingmaster },	/* Same but master floats */

    { "><>",	NULL },			/* no layout function means floating behavior */
    { NULL,		NULL },
};

/* key definitions */
/* Mod1Mask = Alt key; Mod4Mask = Win key */
/* ControlMask = Ctrl key; ShiftMask = Shift key */
#define MODKEY Mod4Mask // Win Key is Mod key.
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
    { MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
    { MOD,  XK_Tab,    ACTION##stack,  {.i = INC(+1) } }, \
    { MOD|ShiftMask,	XK_Tab,	ACTION##stack,	{.i = INC(-1) } }, \
    { MOD,    XK_k, ACTION##stack,  {.i = INC(-1) } }, \
    // { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL }; /* st */
static const char *termcmd2[]  = { TERMINAL2, NULL }; /* xfce4-terminal */

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
        { "color0",		STRING,	&normbordercolor },
        { "color8",		STRING,	&selbordercolor },
        { "color0",		STRING,	&normbgcolor },
        { "color4",		STRING,	&normfgcolor },
        { "color0",		STRING,	&selfgcolor },
        { "color4",		STRING,	&selbgcolor },
        { "borderpx",	INTEGER, &borderpx },
        { "snap",		INTEGER, &snap },
        { "showbar",	INTEGER, &showbar },
        { "topbar",		INTEGER, &topbar },
        { "nmaster",	INTEGER, &nmaster },
        { "resizehints",INTEGER, &resizehints },
        { "mfact",		FLOAT,	&mfact },
        { "gappih",		INTEGER, &gappih },
        { "gappiv",		INTEGER, &gappiv },
        { "gappoh",		INTEGER, &gappoh },
        { "gappov",		INTEGER, &gappov },
        { "swallowfloating",	INTEGER, &swallowfloating },
        { "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
    /* modifier                     key        function        argument */
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
    /* { MODKEY|ShiftMask,		XK_Escape,	spawn,	SHCMD("") }, */
    /* { MODKEY|ShiftMask,		XK_grave,	togglescratch,	SHCMD("") }, */
    TAGKEYS(			XK_1,		0)
    TAGKEYS(			XK_2,		1)
    TAGKEYS(			XK_3,		2)
    TAGKEYS(			XK_4,		3)
    TAGKEYS(			XK_5,		4)
    // TAGKEYS(			XK_6,		5)
    // TAGKEYS(			XK_7,		6)
    // TAGKEYS(			XK_8,		7)
    // TAGKEYS(			XK_9,		8)
    // Launching Programs Win + Numpad //

    /* use `xev` with numlock off, then see outputs of KP presses*/
    { MODKEY,		XK_q,		killclient,	{0} },
    { MODKEY,       XK_w,       spawn,      {.v = (const char*[]){ BROWSER, NULL } } }, /* Mod + w */
    { ControlMask,		XK_KP_Next,	spawn,		{.v = (const char*[]){ BROWSER, NULL } } }, /* Mod + KP3 */
    { ControlMask,       XK_KP_Up,   spawn,      {.v = (const char*[]){ "/home/dra/64g/Telegram", NULL } } }, /* Mod + KP8 */
    { ControlMask,       XK_KP_Home, spawn,      {.v = (const char*[]){ "firefox", NULL } } }, /* Mod + KP7 */
    { ControlMask,       XK_KP_End, spawn,      {.v = (const char*[]){ "audacious", NULL } } }, /* Mod + KP1 */
    { ControlMask,       XK_KP_Prior,spawn,      {.v = (const char*[]){ "google-chrome-stable", NULL } } }, /* Mod + KP9 */
    { ControlMask,       XK_KP_Right,spawn,      {.v = (const char*[]){ EDITOR, NULL } } }, /* Mod + KP6 */
    { MODKEY,       XK_e,       spawn,      {.v = (const char*[]){ FILE_EXPLORER, NULL } } },
    { ControlMask,       XK_KP_Down, spawn,      {.v = (const char*[]){ FILE_EXPLORER,"Downloads", NULL } } }, /* Mod + KP2 */
    { ControlMask,       XK_KP_Left, spawn,      {.v = (const char*[]){ FILE_EXPLORER,"Pictures", NULL } } }, /* Mod + KP4 */
    { MODKEY,		XK_r,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
    { MODKEY,       XK_n,       spawn,      {.v = (const char*[]){ TERMINAL, "-e", "micro", "~/clipsave.txt", NULL } } },
    { MODKEY,		XK_d,		spawn,      {.v = (const char*[]){ "dmenu_run", "-fn", dmenufont, NULL } } },
    { MODKEY,		XK_f,		togglefullscr,	{0} },
    { MODKEY,       XK_Left,    setmfact,   {.f = -0.05} },
    { MODKEY,       XK_Right,   setmfact,   {.f = +0.05} },
    { MODKEY,		XK_Return,	spawn,		{.v = termcmd2 } },
    { Mod1Mask,     XK_Return,  spawn,        {.v = termcmd } },
    // { MODKEY,		XK_c,		spawn,		    {.v = (const char*[]){ "clipmenu", NULL } } },
    { MODKEY,       XK_c,       spawn,          SHCMD("~/scripts/clipman.sh")}, /* clip manager */
    { MODKEY,       XK_v,       spawn,          SHCMD("~/scripts/ytdl.sh")}, /* query + download video */
    { MODKEY,       XK_x,       spawn,          SHCMD("~/scripts/ytx.sh")}, /* download audio */
    { MODKEY,       XK_F7,      spawn,          SHCMD("~/scripts/mountsd.sh")},
    { MODKEY|ShiftMask,  XK_F7, spawn,          SHCMD("~/scripts/umountsd.sh")},
    // { MODKEY|ShiftMask,  XK_q,  spawn,          {.v = (const char*[]){ TERMINAL, "-e", "sh", "~/scripts/qr.sh", NULL } } },
    { Mod1Mask,     XK_space,   spawn,          SHCMD("~/scripts/search.sh")},
    { MODKEY,			XK_Left,	focusmon,	{.i = -1 } },
    { MODKEY,			XK_Right,	focusmon,	{.i = +1 } },
    { MODKEY,			XK_F5,		xrdb,		{.v = NULL } },
    { MODKEY,			XK_space,	zoom,		{0} },
    // { MODKEY|ShiftMask,	XK_space,	togglefloating,	{0} }, /* you can move/resize with MODKEY + left/right mouse button */
    /* Win + Printscreen -> Insert; workaround for KeyBoard with FN keys, (DELL!!) */
    { MODKEY,           XK_Insert,  spawn,      SHCMD("scrot ~/Pictures/ss_$(date '+%Y%m%d%H%M%S').png") },
    
    { Mod1Mask,          XK_backslash,spawn,    SHCMD("amixer set Master toggle") },
    { Mod1Mask, XK_bracketright,    spawn,      SHCMD("amixer set Master 1+") },
    { Mod1Mask, XK_bracketleft,     spawn,      SHCMD("amixer set Master 1-") },
    
    { 0, XF86XK_AudioMute,		    spawn,		SHCMD("amixer set Master toggle") },
    { 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("amixer set Master 1+") },
    { 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("amixer set Master 1-") },
    /* PULSEAUDIO is nice for easy EQ and Audio redirection. */
    // { MODKEY,           XK_u,      spawn,      {.v = (const char*[]){ "pulseaudio-equalizer-gtk", NULL } } },
    // { 0, XF86XK_AudioMute,          spawn,      SHCMD("amixer -q -D pulse sset Master toggle") },
    // { 0, XF86XK_AudioRaiseVolume,   spawn,      SHCMD("amixer -q -D pulse sset Master 2%+") },
    // { 0, XF86XK_AudioLowerVolume,   spawn,      SHCMD("amixer -q -D pulse sset Master 2%-") },

    // { MODKEY,			XK_F11,		spawn,		SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
    // { MODKEY,			XK_F2,		spawn,		{.v = (const char*[]){ "tutorialvids", NULL } } },
    // { MODKEY,			XK_F3,		spawn,		{.v = (const char*[]){ "displayselect", NULL } } },
    // { MODKEY,			XK_F4,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
    // { MODKEY,			XK_F6,		spawn,		{.v = (const char*[]){ "torwrap", NULL } } },
    // { MODKEY,			XK_F7,		spawn,		{.v = (const char*[]){ "td-toggle", NULL } } },
    // { MODKEY,			XK_F8,		spawn,		{.v = (const char*[]){ "mailsync", NULL } } },
    // { MODKEY,			XK_F9,		spawn,		{.v = (const char*[]){ "mounter", NULL } } },
    // { MODKEY,			XK_F10,		spawn,		{.v = (const char*[]){ "unmounter", NULL } } },
    // { MODKEY,			XK_Print,	spawn,		{.v = (const char*[]){ "dmenurecord", NULL } } },
    // { MODKEY|ShiftMask,	XK_Print,	spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
    // { MODKEY,			XK_Delete,	spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
    // { MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },
    // { 0, XF86XK_AudioPrev,		spawn,		{.v = (const char*[]){ "mpc", "prev", NULL } } },
    // { 0, XF86XK_AudioNext,		spawn,		{.v = (const char*[]){ "mpc",  "next", NULL } } },
    // { 0, XF86XK_AudioPause,		spawn,		{.v = (const char*[]){ "mpc", "pause", NULL } } },
    // { 0, XF86XK_AudioPlay,		spawn,		{.v = (const char*[]){ "mpc", "play", NULL } } },
    // { 0, XF86XK_AudioStop,		spawn,		{.v = (const char*[]){ "mpc", "stop", NULL } } },
    // { 0, XF86XK_AudioRewind,	spawn,		{.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
    // { 0, XF86XK_AudioForward,	spawn,		{.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
    // { 0, XF86XK_AudioMedia,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
    // { 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
    /* { 0, XF86XK_PowerOff,		spawn,		{.v = (const char*[]){ "sysact", NULL } } }, */
    // { 0, XF86XK_Calculator,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },
    // { 0, XF86XK_Sleep,		spawn,		{.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
    // { 0, XF86XK_WWW,		spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
    // { 0, XF86XK_DOS,		spawn,		{.v = termcmd } },
    // { 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
    // { 0, XF86XK_TaskPane,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
    // { 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
    // { 0, XF86XK_MyComputer,		spawn,		{.v = (const char*[]){ TERMINAL, "-e",  "lfub",  "/", NULL } } },
    /* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
    // { 0, XF86XK_Launch1,		spawn,		{.v = (const char*[]){ "xset", "dpms", "force", "off", NULL } } },
    // { 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
    // { 0, XF86XK_TouchpadOff,	spawn,		{.v = (const char*[]){ "synclient", "TouchpadOff=1", NULL } } },
    // { 0, XF86XK_TouchpadOn,		spawn,		{.v = (const char*[]){ "synclient", "TouchpadOff=0", NULL } } },
    // { 0, XF86XK_MonBrightnessUp,	spawn,		{.v = (const char*[]){ "xbacklight", "-inc", "15", NULL } } },
    // { 0, XF86XK_MonBrightnessDown,	spawn,		{.v = (const char*[]){ "xbacklight", "-dec", "15", NULL } } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
#ifndef __OpenBSD__
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
    // { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
    { ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
    { ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
    { ClkRootWin,		0,		Button2,	togglebar,	{0} },
};
