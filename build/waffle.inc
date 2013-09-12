# waffle.inc
#
# http://people.freedesktop.org/~chadversary/waffle/
# http://cgit.freedesktop.org/~chadversary/waffle/
#

# Sources

WAFFLE.C += src/waffle/src/api/waffle_error.c
WAFFLE.C += src/waffle/src/api/waffle_config.c
WAFFLE.C += src/waffle/src/api/waffle_dl.c
WAFFLE.C += src/waffle/src/api/waffle_attrib_list.c
WAFFLE.C += src/waffle/src/api/waffle_enum.c
WAFFLE.C += src/waffle/src/api/api_priv.c
WAFFLE.C += src/waffle/src/api/waffle_gl_misc.c
WAFFLE.C += src/waffle/src/api/waffle_window.c
WAFFLE.C += src/waffle/src/api/waffle_init.c
WAFFLE.C += src/waffle/src/api/waffle_context.c
WAFFLE.C += src/waffle/src/api/waffle_display.c
WAFFLE.C += src/waffle/src/core/wcore_error.c
WAFFLE.C += src/waffle/src/core/wcore_config_attrs.c
WAFFLE.C += src/waffle/src/core/wcore_tinfo.c
WAFFLE.C += src/waffle/src/core/wcore_attrib_list.c
WAFFLE.C += src/waffle/src/core/wcore_display.c
WAFFLE.C += src/waffle/src/core/wcore_util.c

# Mac-specific

WAFFLE.C.MAC += src/waffle/src/cgl/cgl_config.m
WAFFLE.C.MAC += src/waffle/src/cgl/cgl_context.m
WAFFLE.C.MAC += src/waffle/src/cgl/cgl_display.m
WAFFLE.C.MAC += src/waffle/src/cgl/cgl_dl.m
WAFFLE.C.MAC += src/waffle/src/cgl/cgl_error.m
WAFFLE.C.MAC += src/waffle/src/cgl/cgl_platform.m
WAFFLE.C.MAC += src/waffle/src/cgl/cgl_window.m
WAFFLE.C.MAC += src/waffle/src/cgl/WaffleGLView.m

# GLX

WAFFLE.C.GLX += src/waffle/src/glx/glx_window.c
WAFFLE.C.GLX += src/waffle/src/glx/glx_context.c
WAFFLE.C.GLX += src/waffle/src/glx/glx_display.c
WAFFLE.C.GLX += src/waffle/src/glx/glx_platform.c
WAFFLE.C.GLX += src/waffle/src/glx/glx_config.c

# X11

WAFFLE.C.X11 += src/waffle/src/x11/x11_window.c
WAFFLE.C.X11 += src/waffle/src/x11/x11_display.c

# Linux

WAFFLE.C.LINUX += src/waffle/src/linux/linux_platform.c
WAFFLE.C.LINUX += src/waffle/src/linux/linux_dl.c

#

WAFFLE.H :=

WAFFLE.H.OTHER :=
