#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 256

static WINDOW *mainwnd;
int lines = 0, cols = 0;
char** array;

struct Input {
  int currVal;
  char str[MAX_LINE_LENGTH];
};

struct Inputs {
  int width;
  int full;
  struct Input** inputs;
};

struct Inputs inputs;

void screen_init(void) {
  mainwnd = initscr();
  noecho();
  cbreak();
  nodelay(mainwnd, TRUE);
  refresh(); // 1)
  wrefresh(mainwnd);
}

void updateSize(int newLines, int newCols) {
  int i, j;

  if (cols != newCols || lines != newLines) {
    inputs.width = newCols;
    inputs.full = 0;

    inputs.inputs = (struct Input**) realloc(inputs.inputs, sizeof(struct Inputs*) * newCols);

    for (j = cols; j < newCols; j++) {
      inputs.inputs[j] = NULL;
    }

    for (j = 0; j < newCols; j++) {
      if (inputs.inputs[j] != NULL) {
        inputs.full++;
      }
    }

    array = (char**) realloc(array, sizeof(char*) * newLines);

    for (i = lines; i < newLines; i++) {
      array[i] = NULL;
    }

    for (i = 0; i < newLines; i++) {
      array[i] = (char*) realloc(array[i], sizeof(char) * newCols + 1);
      if (newCols > cols) {
        for (j = cols; j < newCols; j++) {
          array[i][j] = ' ';
        }
      }

      if (i >= lines ) {
        for (j = 0; j < cols; j++) {
          array[i][j] = ' ';
        }
      }
      array[i][newCols] = 0;
    }

    lines = newLines;
    cols = newCols;
  }
}

void shiftAndPrint() {
  int i, j;
  struct Input* input;
  char character;
  char* bottomRow;

  bottomRow = array[lines - 1];

  for (i = lines - 1; i > 0; i--) {
    array[i] = array[i - 1];
    mvwprintw(mainwnd,i, 0,"%s", array[i]);
  }

  array[0] = bottomRow;

  for (j = 0; j < cols; j++) {
    if (inputs.inputs[j] == NULL) {
      array[0][j] = ' ';
    } else {
      input = inputs.inputs[j];
      if (input->currVal < 0) {
        free(input);
        inputs.inputs[j] = NULL;
        inputs.full--;
        array[0][j] = ' ';
      } else {
        array[0][j] = input->str[input->currVal];
        input->currVal--;
      }
    }
  }
  mvwprintw(mainwnd,0, 0,"%s", array[0]);
}

static void update_display(void) {
  int i, j;

  curs_set(0);

  shiftAndPrint();

  wrefresh(mainwnd);
  refresh();
}

void screen_end(void) {
  endwin();
}

void wait (int ms, clock_t startWait) {
  clock_t endWait;
  endWait = startWait + ms * CLOCKS_PER_SEC / 1000;
  while (clock() < endWait) {}
}

int main(void) {
  struct Input* input;
  clock_t startWait;
  screen_init();
  int j, a, b, c, d, newx, newy;
  char line[MAX_LINE_LENGTH];
  int readResult;

  while (true) {
    startWait = clock();

    updateSize(LINES, COLS);
    if (inputs.full < inputs.width) {
      d = rand() % (int)((inputs.width - inputs.full) / (inputs.width / 4));
      for (c = 0; c < d; c++) {
        readResult = getstr(line);

        if(readResult == OK) {
          input = (struct Input*) malloc(sizeof(struct Input));

          strcpy(input->str, line);
          input->currVal = strlen(input->str) - 1;

          b = rand() % (inputs.width - inputs.full);
          a = 0;

          for (j = 0; j < inputs.width; j++) {
            if (inputs.inputs[j] == NULL) {
              if ( a == b ) {
                inputs.inputs[j] = input;
                inputs.full++;
                break;
              }
              a++;
            }
          }
        }
      }
    }
    update_display();
    wait(40, startWait);
  }
  screen_end();
  return 0;
}
