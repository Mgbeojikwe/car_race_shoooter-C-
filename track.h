#include "enemy.h"

using std::transform;


static const char*  text1= "GAME OVER !!!";
static const char*  text2= "press any key on keyboard to restart game";
static const char*  text3 = "GAME OVER!!! BULLETS USED UP";

#pragma once

class Track {

    private:

    Enemy  enemy ;
    Grass   grass_obj; 

    char direction;
    vector< vector<Vector2> > enemy_cars;
    Color   enemy_cars_color; 

    vector<Vector2>  player_car;
    Color player_color;

    float movement_cell_size;
    bool game_over;

    vector <Vector2>  bullets_XY;

   //grass properties
    vector<Vector2> all_grasses;
    Color grass_color;

    int grass_size;

    int num_of_bullets;

    int score;

    public:



    Track ()
    {

     auto[en_cars, en_color]= enemy.create_enemy_cars();  // create the first set of cars once the the Track object is instantiated
    
      enemy_cars = en_cars;
      enemy_cars_color = en_color;

      //creating the player car
      float start_y =PIXEL_SIZE*7;
      player_car = { Vector2{0, start_y}, Vector2{PIXEL_SIZE,(start_y+PIXEL_SIZE)}, Vector2{0,(start_y+2*PIXEL_SIZE)}, Vector2{ (2*PIXEL_SIZE), start_y+PIXEL_SIZE}, Vector2{(3*PIXEL_SIZE),start_y}, Vector2{(3*PIXEL_SIZE),(start_y+2*PIXEL_SIZE)} };
    
     // assigning value to the stepwise movement of cars in game 

     int cell = PIXEL_SIZE/4;
      movement_cell_size = cell;
      
      player_color = Color{0,117,44,255};


    // creating the grasses
       auto[grs, grs_color ] = grass_obj.create_grasses();
       
       all_grasses = grs;
       grass_color = grs_color;

         grass_size = 1.5*PIXEL_SIZE;

         game_over = false;
         
         num_of_bullets=0;
         score = 0;

    }

      
     void    create_enemy_cars(){

                enemy.refresh_starting_axis_values();
               auto[en_cars, en_color]= enemy.create_enemy_cars();

               enemy_cars = en_cars;    
               enemy_cars_color = en_color;

     }
    
   
    
    void move_enemy_cars(){
            
            /*
                This method moves the enemy cars sideways and forward. each enemy car is movement sideways randomly
                but moved forward by default.

                We now later place boundary condition on the side ways (up and down) movement of enemy cars
                
            */
            
            
            
            Vector2 enermy_car_direction{};


            for ( size_t i{0}; i < enemy_cars.size(); i++){
                
                auto enemy_car = enemy_cars[i];

                auto temp_position = enemy_car;   // getting the initial position of the enemy car "i"
                                                  // prioir to movement

                int movement_dir = rand()%3;              // this controls the side directional movement of enemy car "i"

                switch (movement_dir){

                    case 0:
                        enermy_car_direction = Vector2{- 2*movement_cell_size,0};    // car moves straight
                        break;
                    
                    case 1:
                        enermy_car_direction = Vector2{0, - 2*movement_cell_size};  //car moves up
                        break;
                    
                    case 2: 
                            enermy_car_direction = Vector2{0, 2*movement_cell_size};  // car moves downward
                            break;
                }   

                //apply the new direction to the enemy car "i"
                
                std::transform(enemy_car.begin(), enemy_car.end(), enemy_car.begin(),[enermy_car_direction]( Vector2 part ){
                            return  Vector2{part.x+enermy_car_direction.x , part.y + enermy_car_direction.y};
                });

                // Applying the boundary condition to enemy car "i"
                
             //   auto[min, max] = place_boundary_condition("enemy",temp_position, enemy_car);
                place_boundary_condition(temp_position, enemy_car);

                // if ( ( min <=0 ) || (max >= (game_height-PIXEL_SIZE)) )
                //        enemy_car = temp_position;

                
                 enemy_cars[i] = enemy_car;   //updating the actual container

                // deleting enemy car "i"  from container and enventually from canvas if it reaches far left of canvas
                auto min_part = std::min_element(enemy_car.begin(), enemy_car.end(), []( Vector2 part1, Vector2 part2){
                                return part1.x < part2.x ;
                 });
            
                               auto tip_of_enemy_car_i = *min_part;

                                // delteing cars that left board
                                if (tip_of_enemy_car_i.x  <= 10 ){

                                        enemy_cars.erase(i+enemy_cars.begin());
                                }
                                       
            }


    }



    
void place_boundary_condition(  vector<Vector2> temp_XY, vector<Vector2> &car){

        /*
                This method checks if the car (enemy's or player's car) still lies within the game canvas. If false 
                it returns the car to it previous position
        */

               auto [ min, max] = std::minmax_element(car.begin(), car.end(), []( Vector2  part1, Vector2 part2){

                                return part1.y < part2.y;
                });

              Vector2 car_part_min =  *min;
             Vector2 car_part_max = *max;

             if ( car_part_min.y <0 || car_part_max.y > (game_height-2*PIXEL_SIZE))
                                car      =  temp_XY;       // modified value(enemy's or player's car) leaves the block 

}


    void move_bullet_forward(){

        transform( bullets_XY.begin(), bullets_XY.end(), bullets_XY.begin(), [this]( Vector2 bullet){
                
                return Vector2{bullet.x + movement_cell_size, bullet.y+0}; 
        });
    }



     void  create_grass(){

        
        if (game_over == false) {   // create new grasses only when game hasn't ends, thus supporting the 
                                    // keyboard_press()  method
        
                grass_obj.clear_grasses();     // first empty the "grasses" container in grass_obj

               auto[grss,color] = grass_obj.create_grasses();

               all_grasses = grss;
              grass_color = color;
        }

       }

   void move_grasses_forward(){
                /*
                        Here grasses are moved forward, and game ends if any of the grasses touch the 
                        player's car

                */

               // create new grasses if all have reached game loop
                Vector2 grass_i{};
                vector<Vector2>  all_grasses_copy{};
         

               //move all grass forward
               Vector2 grass_speed{-movement_cell_size,0};

         transform(all_grasses.begin(), all_grasses.end(), all_grasses.begin(),[grass_speed]( Vector2 grass){
                        
                        return Vector2{grass.x+grass_speed.x, grass.y+ grass_speed.y}; 
         });
         

         // deleting grass that have reached terminal left -side of game  
          
         for (size_t index{0}; index < all_grasses.size(); index++){
                grass_i =all_grasses[index];

                if ( grass_i.x <= 40)
                        all_grasses.erase(all_grasses.begin()+index);
         }


        // check if grasses still exit, if false create new ones
     
        if (all_grasses.size() == 0){
                create_grass();
        }

   }



    void  check_if_bullet_hits_enemy_car(){

                /*
                   This method moves all bullets in "bullet_XY" forward i.e increasing the x-value but maintaining
                   the y-value of each bullet. It removes bullets
                   that might have gotten to the far right of the game. It also checks if 
                   a given bullet hits an enemy car; if yes, then the enemy car is deleted from the 
                   canvas. and player score is increased. 
                    Index-style for-loop, instead of std::transform STL algorithm, is used to enable us
                    carry-out the two aforesaid operations simultaneously
                    N:B: An enemy car is shot if its (x,y)-position matches with that of bullet "i"
                    */
                   

                  vector<int> x_axis_values{}, y_axis_values{};
                   int index{};

                   for (size_t index{0}; index < bullets_XY.size(); index++){

                        auto bullet = bullets_XY[index];
                        
                        if ( bullet.x >= (game_width - 5*PIXEL_SIZE))   {
                                bullets_XY.erase( bullets_XY.begin()+index);  //remove bullet "i"
                                                      // if it reaches far right
                                continue;
                        }


                                for (size_t j{0}; j < enemy_cars.size(); j++){
                                        auto enemy_car = enemy_cars[j];
                        

                                        for (auto part: enemy_car){

                                                bool lesserthan  = (bullet.y -PIXEL_SIZE/2) <= part.y;
                                                bool greaterthan = part.y<= (bullet.y + PIXEL_SIZE/2);
                                               
                                                if ( bullet.x  == part.x   && lesserthan == true && greaterthan == true) {
                                                         enemy_cars.erase(enemy_cars.begin()+j);
                                                         bullets_XY.erase( bullets_XY.begin()+index);

                                                        score += 5;  // increase score by 5

                                                        // since a bullet is used to destroy a single enemy car,
                                                        // we need to move to next bullet, thus optimizing the code 

                                                        break;
                                                }

                                                else 
                                                        continue;

                                        }
                                
                       }



                 }

               
    }



  void   move_player_car(){


        /*
        

        when the player hits keyboard 'S', a bullet is released. the (x,y)-axis of the bullet is appended
        to the container "bullet_XY"
        */

        Vector2 player_car_dir{};

       auto temporal_position = player_car;


        switch (direction) {

                    case 'U':
                           
                          player_car_dir= Vector2{0,-PIXEL_SIZE};
                          
                          transform(player_car.begin(), player_car.end(), player_car.begin(),[player_car_dir]( Vector2 part){

                                    return Vector2{part.x+player_car_dir.x, part.y+player_car_dir.y};
                          });

                        //    place_boundary_condition("player",temporal_position,player_car);
                                place_boundary_condition(temporal_position, player_car);
                            break;

                    case 'D':

                           player_car_dir = Vector2{0, PIXEL_SIZE};
                           
                           transform( player_car.begin(), player_car.end(), player_car.begin(),[player_car_dir]( Vector2 part){

                                    return Vector2{part.x + player_car_dir.x, part.y + player_car_dir.y};
                           });
                            // place_boundary_condition("player",temporal_position,player_car);
                            place_boundary_condition(temporal_position, player_car);
                            break;

                   case 'S':
                        bullets_XY. push_back(player_car[0]);
                        num_of_bullets +=1;

                        if (num_of_bullets >= 50)  {        // game ends when all 50 bullets are used up
                                       game_over =true;
                                       
                        }
                        break;                        

        }

                direction= 'B';   // direction always given a default value, though not defined , as 'B'. 
                                  // This defalut value prevents the car from continously moving "up" or "down"
    }



   void     print_player_car_to_canvas () {
                int partX{}, partY{};

        for (auto part: player_car) {
                     partX =  part.x;
                     partY = part.y;
                     
                     DrawRectangle(partX + PIXEL_SIZE, partY + PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, player_color);
                     
        }

        }

     void   print_enemy_car_to_canvas () {

                
                for (auto enemy_car: enemy_cars) {

                        for (auto part: enemy_car){

                                DrawRectangle(part.x +PIXEL_SIZE, part.y + PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, enemy_cars_color);
                        
                        }
                } 

                 if (enemy_cars.size() == 0)
                        
                         create_enemy_cars();
                
            
                
        }

  void      print_bullets_to_canvas(){

                for (auto bullet: bullets_XY){

                        DrawRectangle( bullet.x + PIXEL_SIZE, bullet.y +PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, player_color);
                }
        }

 void print_grasses_on_track(){

      
      for (auto grass: all_grasses)

               DrawPoly(grass,1,grass_size,45,grass_color);
               
              
      
      
 }

     void    reset_game(){

                        create_enemy_cars();
                        create_grass();

                        // reset number of bullet
                        num_of_bullets = 0;
                        // reset score 

                       score = 0;

        }


 void keyboard_press () {
        /*
                This method controls the Keyboard press and also prints certain texts to screen if "game_over" equals true

        */

                

                 int keypress = GetKeyPressed();

                
                if (game_over == true  && num_of_bullets >= 50){

                        DrawText(text3, 5*PIXEL_SIZE,game_height/2, 30, RED);
                        DrawText(text2, 3*PIXEL_SIZE,game_height-(3*PIXEL_SIZE),30, WHITE);
                              
                              
                }

                else if (game_over == true  && keypress !=0 ){ // i.e if any key is pressed when game ends
                                                                // restart game 
                       game_over = false;

                       reset_game();                        
                }

                else if (game_over == true){
                        DrawText(text1, 5*PIXEL_SIZE,game_height/2,50, RED);                              
                        DrawText(text2, 3*PIXEL_SIZE,game_height-(3*PIXEL_SIZE),30, WHITE);
                              
                }

                else{

                  switch (keypress){

                        case KEY_UP:
                              direction = 'U';
                              break;
                        case KEY_DOWN:
                                direction= 'D';
                                break;
                
                        case KEY_S:
                                direction = 'S';
                                break;
                        
                  }
                }

 }
 

 bool check_if_any_grass_hits_player_car(){
        
        /*
                
                This method checks if a one of the moving grasses touches the player's car. 
                If true, then game ends.
                This method functions
        */

       
        for (auto grass: all_grasses){
                
                //compare grass "i" with all parts of player's car

                for ( auto part: player_car){

                        bool greaterthan = (grass.y- grass_size) <= part.y;
                        bool lessthan = part.y <= (grass.y + grass_size); 
                        
                        if ( (grass.x <= part.x)==true && greaterthan == true && lessthan == true  )
                               return true;
                        
                }
        }

        return false;   // if after comparing all grasses to player car and none suits, return false
  }



    bool    check_if_any_enemy_car_bypass_player_car(){

        /*
                This method checks if any of the enemy's cars got close, in terms of x-axis value, to the player's car
                If true, then game ends. This objective is acheived by looping through the "enemy_cars" container, get the
                tip of the enemy's car in question, and see if x-axis value of this tip is lower than that(x-axis value) of the player's car,
                i.e if the incoming enemy's car has by-passed the player's car.

        */
                auto max = std::max_element( player_car.begin(), player_car.end(), []( Vector2 part1, Vector2 part2){
                                return part1.x < part2.x ;
                });
                
                Vector2 player_car_tip = *max;

                //loop through the enemy's cars

                for (auto enemy_car: enemy_cars){

                        //getting the tip of enemy's car "i"
                       auto min= std::min_element(enemy_car.begin(), enemy_car.end(), []( Vector2 part1, Vector2 part2) {

                                        return part1.x < part2.x; 
                        });

                      Vector2 enemy_car_tip =  *min;   
                      
                      if ( enemy_car_tip.x < player_car_tip.x)
                                return true;
                      

                       else     
                                continue;     // i.e check the next enemy's car
                }

                return false;     // i,e if on chcecking all enemy's cars, none actually by-passed the palyer's car
        }



        void     Game_over(){

                        /*
                                This method checks if game ends by running the methods that checks if
                                any grass or enemy's cars hits players car.
                        */

                        if ( check_if_any_enemy_car_bypass_player_car() == true || check_if_any_grass_hits_player_car() == true) 
                                               game_over = true;                      

                }

     int   get_score(){

                return score;
        }


 void play(){
        
     
      if (game_over == false ) {
                move_enemy_cars();
                move_player_car();
                print_enemy_car_to_canvas();
                print_player_car_to_canvas();
                move_bullet_forward();
                print_bullets_to_canvas();
                keyboard_press();
                move_grasses_forward();
                print_grasses_on_track();
                Game_over();
                check_if_bullet_hits_enemy_car();

                
      }

      else {
        print_bullets_to_canvas();
        print_enemy_car_to_canvas();
        print_grasses_on_track();
        print_player_car_to_canvas();
        keyboard_press();    // Track object should also be able to access keyboard_press so as to enable restarting o game
      }
  }



~ Track (){

}



};