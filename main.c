#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv) {

  screen s;
  struct matrix * edges;
  struct matrix * transform;
  struct matrix * test;

  clear_screen(s);

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  printf("\ntransform matrix e.g.\n");
  test = make_translate(3, 4, 5);
  print_matrix(test);
  free_matrix(test);

  printf("\nscale matrix e.g.\n");
  test = make_scale(3, 4, 5);
  print_matrix(test);
  free_matrix(test);

  printf("\nrotation matrix (x-axis) e.g.\n");
  test = make_rotX(90);
  printf("sin(90 degrees) == 1\ncos(90 degrees) == 0\n");
  print_matrix(test);
  free_matrix(test);

  printf("\nrotation matrix (y-axis) e.g.\n");
  test = make_rotY(90);
  printf("sin(90 degrees) == 1\ncos(90 degrees) == 0\n");
  print_matrix(test);
  free_matrix(test);

  printf("\nrotation matrix (z-axis) e.g.\n");
  test = make_rotZ(90);
  printf("sin(90 degrees) == 1\ncos(90 degrees) == 0\n");
  print_matrix(test);
  free_matrix(test);

  /*
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  */

  
  free_matrix( edges );
  free_matrix( transform );
}  
