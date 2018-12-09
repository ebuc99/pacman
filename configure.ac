#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pacman], [0.9.4])
AC_CONFIG_SRCDIR([config.h.in])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])


# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
dnl Checks for libraries
dnl Check for SDL
SDL_VERSION=2.0
AM_PATH_SDL2($SDL_VERSION, 
	    :, 
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
	   )
AC_CHECK_LIB([SDL2_image], [main],,[AC_MSG_ERROR([SDL_image is required to compile pacman])])
AC_CHECK_LIB([SDL2_ttf], [main],,[AC_MSG_ERROR([SDL_ttf is required to compile pacman])])
AC_CHECK_LIB([SDL2_mixer], [main],,[AC_MSG_ERROR([SDL_mixer is required to compile pacman])])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([atexit])

AC_CONFIG_FILES([Makefile data/Makefile
                 data/fonts/Makefile
                 data/gfx/Makefile
				 data/sounds/Makefile
				 data/txt/Makefile
                 src/Makefile])
AC_OUTPUT
