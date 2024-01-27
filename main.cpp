#include <raylib.h>
#include "track.h"

#include <random>


int main(){

srand(time(NULL));
   
    Color  background_color{0,0,0};

InitWindow(game_width+(10*PIXEL_SIZE), game_height," Car Race");

SetTargetFPS(60);
 
Track   obj;

int scoreY = game_height/2;
int scoreX = game_width+(2*PIXEL_SIZE);

while( WindowShouldClose() == false){

    BeginDrawing();

    ClearBackground(background_color);

    obj.play();     
    
    char  buffer[10];
    sprintf(buffer,"score = %d",obj.get_score());
    DrawText(buffer,scoreX,scoreY,30,WHITE );
    DrawLine(game_width,0,game_width,game_height,WHITE);
     
    EndDrawing();
    
}

CloseWindow();

delete(text1,text2, text3);

return 0;

}