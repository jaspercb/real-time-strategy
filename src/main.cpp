#include "Game.hpp"
#include "Unit.hpp"

#include <iostream>

int main(){
	std::cout<<"	Testing..."<<std::endl;
	Game g = Game();
	Team t = Team(g);
	
	std::cout<<"	Done testing."<<std::endl;
}