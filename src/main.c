#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#define VERSION "v0.0.1"


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
  bool absolute = false;


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
      absolute = true;
  }

  if(x == 0U && y == 0U)
    help();

  Display *d = XOpenDisplay(display);
  if(d == NULL)
    die("Cannot open display!");

  if(absolute)
    dest = XRootWindow(d, XDefaultScreen(d));

  if(XWarpPointer(d, None, dest, 0, 0, 0, 0, x, y) == BadWindow)
    die("Error from X11: BadWindow");

  XCloseDisplay(d);
  return EXIT_SUCCESS;
}

