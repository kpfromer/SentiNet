/**
 *  @file ActionObjectiveTest
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Tuesday Jul 02, 2019 20:06:05 MDT
 *  @bug No Known Bugs
 */

#include <iostream>
#include "kernelOPS/core/ActionObjective.hpp"
using namespace obps::core;

typedef struct {
  int i;
  int b;
  int c;
} datatype;

int main() {
  ActionObjective* a = new ActionObjective(sizeof(datatype));

  datatype b = a->read<datatype>();
  printf("%d %d %d\n", b.i, b.b, b.c);

  datatype c = {1, 4, 5};

  a->write(&c);

  b = a->read<datatype>();

  printf("%d %d %d\n", b.i, b.b, b.c);

  c.i = 9;
  c.b = 10;
  c.c = 11;

  a->write(&c);
  b = a->read<datatype>();

  printf("%d %d %d\n", b.i, b.b, b.c);

  return 0;
}
