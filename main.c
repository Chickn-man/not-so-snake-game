/* Not so Snake Game - 1.0 */

#include "../config.h"

#include <simple2d.h>

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define VIEWPORT_WIDTH 660
#define VIEWPORT_HEIGHT 360

#define BORDER_THICKNESS 10

S2D_Window *window;

int playerX = VIEWPORT_WIDTH / 2;
int playerY = VIEWPORT_HEIGHT / 2;

int objectiveX;
int objectiveY;

int score = 0;

int highScore = 0;

int state = 0; // menu = 0, playing = 1, loss = 2

int isReset = 0;

S2D_Text *titleTopTxt;
S2D_Text *titleTopTxtBack;
S2D_Text *titleBotTxt;
S2D_Text *titleBotTxtBack;

S2D_Text *startBtnTxt;
S2D_Text *startBtnTxtBack;

S2D_Text *scoreTxt;
S2D_Text *scoreTxtBack;

char *highScoreFileName;

S2D_Text *highScoreTxt;
S2D_Text *highScoreTxtBack;

S2D_Text *gameOverTxt;
S2D_Text *gameOverTxtBack;

/*
** print the string at msg, a colon, then the string associated with errno.
*/
void die(char *what, char *action) {
  if (action) {
    printf("%s: %s: %s\n", what, action, strerror(errno));
  } else {
    printf("%s: %s\n", what);
  }
  exit(errno);
}

void reset() {
  FILE *fp = fopen(highScoreFileName, "r+");

  putc(highScore, fp);

  fclose(fp);

  if (isReset) return;

  isReset = 1;

  playerX = VIEWPORT_WIDTH / 2;
  playerY = VIEWPORT_HEIGHT / 2;

  objectiveX = rand() % VIEWPORT_WIDTH;
  objectiveY = rand() % VIEWPORT_HEIGHT;
  while ((objectiveX - 15 <= BORDER_THICKNESS || 
          objectiveX + 15 >= VIEWPORT_WIDTH - BORDER_THICKNESS || 
          objectiveY - 15 <= BORDER_THICKNESS || 
          objectiveY + 15 >= VIEWPORT_HEIGHT - BORDER_THICKNESS
  )) {
    objectiveX = rand() % VIEWPORT_WIDTH;
    objectiveY = rand() % VIEWPORT_HEIGHT;
  }

  score = 0;

  S2D_SetText(scoreTxt, "Score: %d", score);
  S2D_SetText(scoreTxtBack, "Score: %d", score);

  state = 1;
}

void onKey(S2D_Event e) {
  if (e.type == S2D_KEY_DOWN || e.type == S2D_KEY_HELD) {
    if (state == 1) {
      if (!strcmp(e.key, "W")) {
        playerY -= 2 + score;
      } else if (!strcmp(e.key, "A")) {
        playerX -= 2 + score;
      } else if (!strcmp(e.key, "S")) {
        playerY += 2 + score;
      } else if (!strcmp(e.key, "D")) {
        playerX += 2 + score;
      }
    }

    if (state && !strcmp(e.key, "R")) reset();
    if (state && !strcmp(e.key, "Escape")) {reset(); isReset = 0; state = 0;}
  }
}

void onMouse(S2D_Event e) {
  if (e.type == S2D_MOUSE_DOWN) {
    if (state) return;
    if (window->mouse.x > 192 && window->mouse.x < 472 && window->mouse.y > 215 && window->mouse.y < 315) state = 1;
  }
}

void update() {
  if (playerX - 15 <= BORDER_THICKNESS || 
      playerX + 15 >= VIEWPORT_WIDTH - BORDER_THICKNESS || 
      playerY - 15 <= BORDER_THICKNESS || 
      playerY + 15 >= VIEWPORT_HEIGHT - BORDER_THICKNESS
  ) {state = 2; isReset = 0;}

  if (!((playerY + 30 < objectiveY) || (playerY > objectiveY + 30) || (playerX + 30 < objectiveX) || (playerX > objectiveX + 30))) {
    score++;

    if (score > highScore) highScore = score;

    isReset = 0;

    S2D_SetText(scoreTxt, "Score: %d", score);
    S2D_SetText(scoreTxtBack, "Score: %d", score);

    S2D_SetText(highScoreTxt, "High Score: %d", highScore);
    S2D_SetText(highScoreTxtBack, "High Score: %d", highScore);

    objectiveX = rand() % VIEWPORT_WIDTH;
    objectiveY = rand() % VIEWPORT_HEIGHT;

    while ((objectiveX - 15 <= BORDER_THICKNESS || 
            objectiveX + 15 >= VIEWPORT_WIDTH - BORDER_THICKNESS || 
            objectiveY - 15 <= BORDER_THICKNESS || 
            objectiveY + 15 >= VIEWPORT_HEIGHT - BORDER_THICKNESS
    )) {
      objectiveX = rand() % VIEWPORT_WIDTH;
      objectiveY = rand() % VIEWPORT_HEIGHT;
    }
  }
}

void rect(float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
  S2D_DrawQuad(x1, y1, r, g, b, a,
               x2, y1, r, g, b, a,
               x2, y2, r, g, b, a,
               x1, y2, r, g, b, a
  );
}

void border(float x1, float y1, float x2, float y2, float thickness, float r, float g, float b, float a) {
  rect(x1 - thickness, y1 - thickness, x2 - thickness, y1 + thickness, r, g, b, a);
  rect(x2 - thickness, y1 - thickness, x2 + thickness, y2 + thickness, r, g, b, a);
  rect(x2 - thickness, y2 + thickness, x1 - thickness, y2 - thickness, r, g, b, a);
  rect(x1 - thickness, y1 + thickness, x1 + thickness, y2 - thickness, r, g, b, a);
}

void render() {
  if (state == 0) {
    S2D_DrawText(titleTopTxtBack);
    S2D_DrawText(titleTopTxt);
    S2D_DrawText(titleBotTxtBack);
    S2D_DrawText(titleBotTxt);

    rect(197, 220, 467, 310, 0, 0.9, 0, 1);
    rect(472, 215, 482, 315, 0.3, 0.3, 0.3, 1);
    border(197, 220, 467, 310, 5, 0, 0.4, 0.2, 1);

    S2D_DrawText(startBtnTxtBack);
    S2D_DrawText(startBtnTxt);
    
  }

  if (state) {
    rect(playerX - 15, playerY - 15, playerX + 15, playerY + 15, 1, 1, 1, 1); // player

    rect(objectiveX - 15, objectiveY - 15, objectiveX + 15, objectiveY + 15, 0.1, 0.7, 0, 1); // objective

    S2D_DrawText(scoreTxtBack); // score
    S2D_DrawText(scoreTxt); // score

    S2D_DrawText(highScoreTxtBack); // score
    S2D_DrawText(highScoreTxt); // score
  }

  if (state == 2) {S2D_DrawText(gameOverTxtBack); S2D_DrawText(gameOverTxt);} // Game Over

  // game border
  border(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, BORDER_THICKNESS, 1, 0, 0, 1);
}

#define FONT_FILE "/usr/share/fonts/droid/DroidSansMono.ttf"

S2D_Text *makeText(char *text, int size, float x, float y, float r, float g, float b, float a) {
  S2D_Text *tmpTxt = S2D_CreateText(FONT_FILE, text, size);

  tmpTxt->x = x;
  tmpTxt->y = y;

  tmpTxt->color.r = r;
  tmpTxt->color.g = g;
  tmpTxt->color.b = b;
  tmpTxt->color.a = a;

  return tmpTxt;
}

int main(int argc, char **argv) {
#ifdef NIX
  char *HOME_DIR = malloc(strlen(getenv("HOME")));
  strcpy(HOME_DIR, getenv("HOME"));

  highScoreFileName = malloc(strlen(HOME_DIR) + strlen("/nssg/highscore") + 1);
  strcpy(highScoreFileName, HOME_DIR);
  strcat(highScoreFileName, "/nssg/highscore");

  char *dataFolderName = malloc(strlen(HOME_DIR) + strlen("/nssg/highscore") + 1);
  strcpy(dataFolderName, HOME_DIR);
  strcat(dataFolderName, "/nssg/");

  FILE *highScoreFile = fopen(highScoreFileName, "r");

  if (!highScoreFile) {
      if (errno == 2) {
        if (mkdir(dataFolderName, S_IRWXU) == -1) {
          die("mkdir", dataFolderName);
        } else highScoreFile = fopen(highScoreFileName, "w");
    } else {
      die("fopen", highScoreFileName);
    }
  }

  fclose(highScoreFile);

  fopen(highScoreFileName, "r");

  highScore = getc(highScoreFile);
#endif
#ifdef WIN
#endif
  window = S2D_CreateWindow("Not So Snake Game", WINDOW_WIDTH, WINDOW_HEIGHT, update, render, S2D_RESIZABLE);

  // extra window parameters
  window->viewport.width = VIEWPORT_WIDTH;
  window->viewport.height = VIEWPORT_HEIGHT;

  // window events
  window->on_key = onKey;
  window->on_mouse = onMouse;

  // text
  titleTopTxt = makeText("Not So", 75, 195, 35, 1, 1, 0, 1);
  titleTopTxtBack = makeText("Not So", 75, 200, 35, 0.8, 0.6, 0, 1);
  titleBotTxt = makeText("Snake Game", 75, 105, 115, 1, 1, 0, 1);
  titleBotTxtBack = makeText("Snake Game", 75, 110, 115, 0.8, 0.6, 0, 1);

  startBtnTxt = makeText("Start", 75, 215, 220, 0.3, 0.3, 0.3, 1);
  startBtnTxtBack = makeText("Start", 75, 220, 220, 0, 0, 0, 1);

  scoreTxt = makeText("Score: ", 30, 20, 15, 0, 0, 1, 1);
  scoreTxtBack = makeText("Score: ", 30, 22, 15, 0, 0, 0.5, 1);
  S2D_SetText(scoreTxt, "Score: %d", score);
  S2D_SetText(scoreTxtBack, "Score: %d", score);

  highScoreTxt = makeText("High Score: ", 30, 20, 315, 0, 0, 1, 1);
  highScoreTxtBack = makeText("High Score: ", 30, 22, 315, 0, 0, 0.5, 1);
  S2D_SetText(highScoreTxt, "High Score: %d", highScore);
  S2D_SetText(highScoreTxtBack, "High Score: %d", highScore);


  gameOverTxt = makeText("Game Over", 75, 125, 135, 1, 0, 0, 1);
  gameOverTxtBack = makeText("Game Over", 75, 130, 135, 0.5, 0, 0, 1);
  // end text

  objectiveX = rand() % VIEWPORT_WIDTH;
  objectiveY = rand() % VIEWPORT_HEIGHT;
  while ((objectiveX - 15 <= BORDER_THICKNESS || 
          objectiveX + 15 >= VIEWPORT_WIDTH - BORDER_THICKNESS || 
          objectiveY - 15 <= BORDER_THICKNESS || 
          objectiveY + 15 >= VIEWPORT_HEIGHT - BORDER_THICKNESS
  )) {
    objectiveX = rand() % VIEWPORT_WIDTH;
    objectiveY = rand() % VIEWPORT_HEIGHT;
  }

  S2D_Show(window); // start game

  reset();

  return 0;
}