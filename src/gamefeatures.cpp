#include"gamefeatures.h"
#include<memory>
#include <iostream>

// Functions for speed increaser

SuperFood::SuperFood()
{

}

void SuperFood::addPosition(const SDL_Point &point2D) 
{
    _superFoodCoordinates.emplace_back(point2D);
}

bool SuperFood::isPresentInPosition(const SDL_Point &point2D) const
{
    
    for(const SDL_Point &superFoodCoordinate : _superFoodCoordinates)
    {

        if(superFoodCoordinate.x == point2D.x && 
            superFoodCoordinate.y == point2D.y)
        {
            return true;
        }
    }       
    
    return false;
}

void SuperFood::deletePosition(const SDL_Point &point2D)
{

    _superFoodCoordinates.erase(std::remove_if(_superFoodCoordinates.begin(), _superFoodCoordinates.end(),
                                     [&point2D](auto coordinate){ return (coordinate.x == point2D.x && coordinate.y == point2D.y);}),
                                     _superFoodCoordinates.end());

}

// Functions for Poison
Poisons::Poisons(int numberOfPoisons):_numberOfPoisons(numberOfPoisons)
{

}

void Poisons::addPosition(const SDL_Point &point2D) 
{
    _PoisonsCoordinates.emplace_back(point2D);
}

bool Poisons::isPresentInPosition(const SDL_Point &point2D) const
{  
    
    for(const SDL_Point &PoisonsCoordinate : _PoisonsCoordinates)
    {

        if(PoisonsCoordinate.x == point2D.x && 
            PoisonsCoordinate.y == point2D.y)
        {
            return true;
        }
    }
        return false;

}

void Poisons::deletePosition(const SDL_Point &point2D)
{

    _PoisonsCoordinates.erase(std::remove_if(_PoisonsCoordinates.begin(), _PoisonsCoordinates.end(),
                              [&point2D](auto coordinate){ return (coordinate.x == point2D.x && coordinate.y == point2D.y);}),
                              _PoisonsCoordinates.end());

}