
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <slankdev/system.h>

extern "C" void yukaribonk ()
{
  static size_t c = 0;
  printf ("%zd slankdev %lu\n", c++, slankdev::rdtsc());
}

int main (int argc, char** argv)
{
  while (true)
    {
      yukaribonk ();
      sleep (1);
    }
}

