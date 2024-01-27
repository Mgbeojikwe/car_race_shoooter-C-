#include <raylib.h>
#include <vector>
#include <tuple>
#include <random>
#include <iostream>

using std::vector;

const int PIXEL_SIZE{20}, game_width{60*PIXEL_SIZE}, game_height{30*PIXEL_SIZE};

#pragma once 

class Grass {

    private:

    Color  grass_color;
    vector<Vector2> grasses;
        
    
    public:

        Grass ()
            {
                grass_color = Color{0,117,44,255};   // green 

            }





    std::tuple< vector<Vector2>, Color>  create_grasses(){

            float X{}, Y{};
            int start_X{game_width/2}, start_Y{20}, num_of_grasses{};
            
            num_of_grasses= 1+rand()%5;            
            
             for( size_t i{0}; i < num_of_grasses; i++){
            //for( size_t i{0}; i < 5 ; i++){

              Y = start_Y + rand()%game_height;  // making (x,y)-axis of grass a bit far from 
              X =  start_X + rand()% game_width;   // the player's car
             
             X = (X > game_width) ? (game_width-2*PIXEL_SIZE): X;
             Y = ( Y > game_height ) ? ( game_height - (2*PIXEL_SIZE)) : Y;
            // create five grasses

                grasses.push_back( Vector2{X,Y});

            } 

            grasses.shrink_to_fit();

            return {grasses, grass_color};
    }


void clear_grasses(){
        grasses.clear();
}


 ~ Grass (){

 }


};