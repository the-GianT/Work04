#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 move: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);

    if (strncmp(line, "line", 4) == 0) {
      int args[6];
      int numinputs;

      /* Read arguments: */
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n", line);
      numinputs = sscanf(line, "%d %d %d %d %d %d", args, args+1, args+2, args+3, args+4, args+5);
      if (numinputs != 6) {
	printf("Error: Invalid arguments for line\n");
	return;
      }
      
      add_edge(edges, *args, args[1], args[2], args[3], args[4], args[5]);
      
    } else if (strncmp(line, "ident", 5) == 0) {
      ident(transform);
      
    } else if (strncmp(line, "scale", 5) == 0) {
      int args[3];
      int numinputs;
      struct matrix * scale_m;

      /* Read arguments: */
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n", line);
      numinputs = sscanf(line, "%d %d %d", args, args +1, args + 2);
      if (numinputs != 3) {
	printf("Error: Invalid arguments for scale\n");
	return;
      }
      
      scale_m = make_scale(*args, args[1], args[2]);
      matrix_mult(scale_m, transform);
      free_matrix(scale_m);
      
    } else if (strncmp(line, "move", 4) == 0) {
      int args[3];
      int numinputs;
      struct matrix * trans_m;

      /* Read arguments: */
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n", line);
      numinputs = sscanf(line, "%d %d %d", args, args +1, args + 2);
      if (numinputs != 3) {
	printf("Error: Invalid arguments for move\n");
	return;
      }
      
      trans_m = make_translate(*args, args[1], args[2]);
      matrix_mult(trans_m, transform);
      free_matrix(trans_m);
      
    } else if (strncmp(line, "rotate", 6) == 0) {
      char axis;
      double theta;
      int numinputs;
      struct matrix * rot_m;

      /* Read arguments: */
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n", line);
      numinputs = sscanf(line, "%c %lf", &axis, &theta);
      if (numinputs != 2) {
	printf("Error: Invalid arguments for rotate\n");
	return;
      }
      
      if (axis == 'x')
	rot_m = make_rotX(theta);
      else if (axis == 'y')
	rot_m = make_rotY(theta);
      else if (axis == 'z')
	rot_m = make_rotZ(theta);
      else {
	printf("Error: Invalid axis argument for rotate; must be x, y, or z\n");
	return;
      }
      
      matrix_mult(rot_m, transform);
      free_matrix(rot_m);
      
    } else if (strncmp(line, "apply", 5) == 0) {
      matrix_mult(transform, edges);
      
    } else if (strncmp(line, "display", 7) == 0) {
      color c;

      c.red = MAX_COLOR;
      c.green = MAX_COLOR;
      c.blue = MAX_COLOR;
      
      draw_lines(edges, s, c);
      display(s);
      clear_screen(s);
      
    } else if (strncmp(line, "save", 4) == 0) {
      color c;

      c.red = MAX_COLOR;
      c.green = MAX_COLOR;
      c.blue = MAX_COLOR;
      
      draw_lines(edges, s, c);

      /* Read file name argument for save: */
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n", line);
      
      save_extension(s, line);
      clear_screen(s);
      
    } else if (strncmp(line, "quit", 4) == 0) {
      return;

    } else
      printf("%s: command not found\n", line);
  } // end while loop
  
} // end parse_file function
