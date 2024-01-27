#include <raylib.h>
#include <algorithm>
#include "grasses.h"

using std::vector;




#pragma once
class Enemy{

    private:
        float start_x;
        float start_y;

    public:

    Enemy()
        {
             start_y = PIXEL_SIZE;                  // part of first car placed at y-axis = PIXEL_SIZE
             start_x = game_width - PIXEL_SIZE;     // part of fisrt car placed at x-axis few distance from board termional
           

        }

std::tuple< vector<vector<Vector2>>, Color>    create_enemy_cars(){

           
           

            vector<vector<Vector2> >  selected_cars{}, cars(5);

             float cell_size = PIXEL_SIZE;
             
             

            for( int i{0}; i < cars.size(); i++){
                
                cars[i]= { Vector2{ start_x, start_y}, Vector2{start_x, (start_y+PIXEL_SIZE)}, Vector2{start_x,start_y+(2*PIXEL_SIZE)}, Vector2{(start_x-PIXEL_SIZE),(start_y+PIXEL_SIZE)}};
                
                start_y += 6*cell_size;    //placing separating distance between the cars
            
            }

            

        // randomly  select the N-enemy cars from "cars" and place tham in "selected cars"

       int num_of_cars = 1 + rand()%cars.size(); // must select atleast a car
        std::cout<<num_of_cars<<"\n";
        
        std::sample( cars.begin(), cars.end(), std::back_inserter(selected_cars),num_of_cars, std::mt19937{std::random_device{}()});

        return { selected_cars, Color{230,41,55, 255}};
}

      void  refresh_starting_axis_values(){
                start_y = PIXEL_SIZE;
               start_x = game_width - PIXEL_SIZE;
        }
~Enemy(){

}


};