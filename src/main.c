#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#ifndef VERSION
#define VERSION "v1.0.0"
#endif


#define die(msg)  do { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); } while(0)

#define equals(x, y)  (strcmp(x, y) == 0)

__attribute__((noreturn)) void help() {
  puts(
    "xwarp " VERSION "\n"
    "Usage:\n"
    "\t-d\tConnect to specific X-Server\n"
    "\n"
    "\t-x\tX coordinate to move cursor to\n"
    "\n"
    "\t-y\tY coordinate to move cursor to\n"
    "\n"
    "\t-a\tInterprete coordinates as absolute from root window\n"
    "\n"
    "\t-w\tThe window id of the window to use as a coordinate reference\n"
    "\n"
    "\t-v,--version\tShow version\n"
    "\n"
    "\t-h,--help\tShow this"
  );
  exit(EXIT_FAILURE);
}

__attribute__((noreturn)) void version() {
  puts("xwarp " VERSION);
  exit(EXIT_FAILURE);
}

unsigned int strtoui(const char *str, int base) {
  char *endptr;
  unsigned long num = strtoul(str, &endptr, base);
  if(num == 0U && str == endptr)
    die("Not a number!");

  if(num > UINT_MAX)
    die("Number out of range!");

  return (unsigned int) num;
}

int main(int argc, char **argv)
{
  const char *display = NULL;
  Window dest = None;
  struct {
    bool absolute: 1;
    unsigned long winid;
  } flags = {
    .absolute = false,
    .winid = 0UL,
  };


  unsigned int x = 0U,
               y = 0U;

  for(int i = 1; i < argc; i++) {
    if(equals(argv[i], "-h") || equals(argv[i], "--help"))
      help();
    else if(equals(argv[i], "-v") || equals(argv[i], "--version"))
      version();
    else if(equals(argv[i], "-d"))
      display = argv[++i];
    else if(equals(argv[i], "-x"))
      x = strtoui(argv[++i], 0);
    else if(equals(argv[i], "-y"))
      y = strtoui(argv[++i], 0);
    else if(equals(argv[i], "-a"))
      flags.absolute = true;
    else if(equals(argv[i], "-w")) {
      char *c, *arg = argv[++i];
      flags.winid = strtoul(arg, &c, 0);
      if(flags.winid == 0UL && c == arg)
        die("Not a number!");
    }
  }

  if(x == 0U && y == 0U)
    help();

  Display *d = XOpenDisplay(display);
  if(d == NULL)
    die("Cannot open display!");

  if(flags.absolute && flags.winid != 0UL)
    die("-a and -w are mutually exclusive!");

  if(flags.absolute)
    dest = XRootWindow(d, XDefaultScreen(d));

  if(flags.winid != 0UL)
    dest = flags.winid;

  if(XWarpPointer(d, None, dest, 0, 0, 0, 0, x, y) == BadWindow)
    die("Error from X11: BadWindow");

  XCloseDisplay(d);
  return EXIT_SUCCESS;
}

