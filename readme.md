

To run this code following the steps:
        step 1 =>  run $bash build.bash
        step 2 => run  $./a.out




This car race is a 2d type of which the player has to shoot-off approaching enemy's cars and also dodge approaching poinous green leaves. The game ends if any of the following occurs:
    i)  Any of the enemy cars successfully by-pass the player's car
    ii) Any of the approaching greeen leaves touches the player's car.
    iii) The shoots utilized all their bullets ( total number of bullets available = 50)

 The game can be re-startred by pressing any key.

1.0        .................GAME STRUCTURE...............

This game has three classes via "Enemy", "Grass"  and "Track". Three classes are related by composition rather than inheritance. Objects of "Enemy"  and "Grass"are created and utilized as instance atttributes in the "Track" class.

1.1    Classes and their methods
	
1.1.1  "Grass" class 
	
	This class has two method has two methods, and they are discussed as follows:
	
1.1.1.1 create_grasses()
	
	This method creates a random number of grasses each time it is called, and each grass is positioned randomly

1.1.1.2 clear_grasses()
	
	This method clears the std::vector container "grasses" since it is an instance attribute. It is called prioir to creating new set of grasses since such container remain alive as long as the Grass object is, and thus not clearing them will lead an excess number of grasses of which most are from previously created grasses. 



1.1.2  "Enemy" class
	This class has 2 methods, and they are discussed below
	
1.1.2.1 create_enemy_cars() 
	
	This method creates the enemy cars used in the game. The steps in acheiving such are:
	step1 => with the help of a loop of 5 iterations and some mathematical logical reasoning, create a container "cars" that has  5 cars
	step2 => use std::sample () STL algorithm to randomly select 1 or more cars from "cars" container, and place the selected cars in the container "selected_cars"
	step3 => return "selected_cars" and the color object 

1.1.2.2 refresh_starting_axis_values()
	This method re-assigns the "start_y" and "start_x" back to their initial values, and it utilized prioir to creating new enemies cars. This process is important because both variables are instance attributes, and thus remain alive as long as the "Enemy" isn't destroyed. without prior utilization of this method, the next set of enemy's cars will likey exit outside the game enviroment.  



1.1.3   "Track" class

	This class has 18 methods, and they are discussed below:


1.1.3.1  create_enemy_cars()
	
	This method creates the enemy cars. The enemy cars are created by calling the "refresh_starting_axis_values()" of the "Enemy" class following by calling the "create_enemy_cars()" method of the "Enemy" class using the "enemy" object of class "Enemy". The values are assigned to the Track instance attributes "enemy_cars" and "enemy_cars_color"

1.1.3.2  move_enemy_cars () 
	
	This method moves the available enemy's car forward or sideways. With the help of std::transform()  STL algorithm, this method acheives the objective via the following steps:
		step1 => iterate over the available enemy cars so we can work on one car at a time
		step 2 => ramdomly create an integer value that is either 0,1 or 2
		step 3 => if 0, car moves forward in steps {-2*movement_cell_size,0}
			 if 1, it moves up in steps {0, -2*movement_cell_size}, and
			if downward it moves in steps {0, 2*movement_cell_size} 
		step 4 => apply the "place_boundary_condition()" to ensure the enemy car doesn't leave the canvas.


1.1.3.3    place_boundary_condition ()
	  
           This method is written in a manner such that it can serve both the enemy cars and the player car. This method ensures that any car in the canvas doesn't leave the canvas sideways. It acheives its method via the steps:
		step1 => with the help of std::minmax_element() STL algorithm, find the minimum and maximum values of the car in question
		step2 => check if the minimum value or maximum value exist outside the canvas, if yes re-assign the position of the car prior to lastest move to the car
		step3 => since the car container is passed by reference, the modified value can exit outside the method block
	
 
1.1.3.4	   move_bullet_forward()
	   
		This method moves all available bullets forward. The bullets are moved in toward higher x-axis region, hence towards the enemy cars.
 
1.1.3.5    create_grass() 
		This method creates the poisonous grasses. The grasses are created by calling the "create_grasses()"  method of the "Grass" class. The created grasses and their color is retrived by structure-binding and assigned to the "all_grasses" and "grass_color" instance attributes of the "Track" class.

1.1.3.6   move_grasses_forward ()
		
		This method moves all available poisonous grasses  forward towards the left i.e lower x-axis region. This movement is acheived by the std::transform() STL algorithm. Each grass is movement forward in steps of Vector2{-movement_cell_size,0}.

1.1.3.7  check_if_bullets_hit_enemy_car()
	
		This method checks if a bullet shot by player, has hit any of the enemy's cars; if yes, then the enemy's car is destroyed from the canvas
		This objective is acheived via the following steps:
		
		step 1 => loop through the entire bullets in "bullets_XY"
		step 2 => delete bullet from canvas if it reacjes right terminal of the game		
		step 3: for each bullet "i", loop through all the available enemy's car  and also over all the parts of enemy_car "i".
		step 4 => check if bullet touched any part of enemy_car "i" based on bullet and the part having same x-axis value and part having y-axis value within an allow range covered by the bullet

		step 5 => If step 3 returns true, then delete the enemy car and the  bullet from the "enemy_cars" and "bullets_XY" container respectivey.
		 

1.1.3.8  move_player_car() 
		
	This method handles the movement of the player's car and also the shooting ability of the player. The player can only move down or up and easily achived with the use of std::transform() STL algorithm. The move_player_car() method achieves its objective in the follwing steps
	
	a) Upward movement:
		step1 => assign Vector{0,-PIXEL_SIZE} to the varible "player_car_dir"
		step 2 => add the value from step 1 to each part of "player_car"(recall that "player_car" is a std::vector<> container whose elements belong to class Vector2)
		step 3 => If the player moves outside the game canvas, return it to its previous location using the "place_boundary_condition ()" method.
	
	b) Downward  movement: 
		step 1 => assign Vector2{0, PIXEL_SIZE} to the object "player_car_dir"
		step 2 => add "player_car_dir" to each elements in the container "player_car"
		step 3 => If player_car leaves the game canvas, return it a position prioir to leaving the canvas using the "place_boundary_condition ()" method
	
	c)  Shoot
		step1 => select the current position of one of the parts of "player_car" as the starting position of the bullet
		step 2 =>  append that position to the std::vector container "bullets_XY".
		step 3 => for each time a bullet is shot, increase the value of "num_of_bullets" by 1.
		step4 => Game ends if player has shot a total of 50 bullets
   
 	

1.1.3.9  print_grasses_on_track(), print_bullets_to_canvas(), print_enemy_car_to_canvas(), and print_player_car_to_canvas()
	This four methods print the grasses, bullets, all enemy's cars, and player's car to the raylib canvas respectively.

1.1.3.10  reset_game ()
		This method return the game to initial state, by creating new enemy's car and grasses, and returning the "score" and "num_of_bullet" fired to zero.

1.1.3.11 keyboard_press() 
	
	This method connects the game with the keys (up, down, and S) on the  keyboard. It also has conditional bloacks that helps in restartin the game, when _game_over" equals "true".  

1.1.3.12:  check_if_any_grass_hits_player()
		This method checks is any of the green grasses hits the player car. The grass are sinlge points, but exapnded by a pixel size of "grass_size". This method acheives its objective through the following steps:

	i) loop through the grasses
	ii) Get the (x,y)-axis value of grass "i"  and loop through all parts of player's car
	iii) check if grass "i" has by-passed any part of the player's car, and also check if the y-axis value of the studied part exit within the pixel_size of the grass
 		

1.1.3.13  check_if_enemy_car_bypass_player_car() 
		
		This method checks is any of the player's car has bypassed  the player car, if true the game ends. 
	Since the enemy's car moves in opposite direction and thus approaches the player's car, the method's objective is acheived by:
		i) get the tip of the player's car i.e part with the largest x-axis value.
		ii)  looping through all available enemy's car
		iii) For each enemy's car, get the part with the smallest x-axis value, i.e the tip
		 iii) check if the "tip of enemy's car" is less than the "tip of player's car" i.e if enemy's car has bypassed player's car

  	 

1.1.3.14  play () 
	 	This method is used to house all the mehtods utilized in the Track class. Thus we only need to call this method in the main.cpp file, thus making the file clean

