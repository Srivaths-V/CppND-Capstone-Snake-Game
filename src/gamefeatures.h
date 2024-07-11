#ifndef GAMEFEATURES_H
#define GAMEFEATURES_H

#include<memory>
#include<vector>
#include<algorithm>
#include "SDL.h"

// Abstract class for game features
class GameFeatures
{

    public:
        virtual void addPosition(const SDL_Point &point2D) = 0;
        virtual bool isPresentInPosition(const SDL_Point &point2D) const = 0;
        virtual void deletePosition(const SDL_Point &point2D) = 0;
};

// Concrete Class for Super food
class SuperFood : public GameFeatures
{
    public:
        
        SuperFood();

        const std::vector<SDL_Point> getSuperFoodCoordinates()
        {
            return _superFoodCoordinates;
        }
     
        void addPosition(const SDL_Point &point2D) override;
        bool isPresentInPosition(const SDL_Point &point2D) const override;
        void deletePosition(const SDL_Point &point2D) override;

    private:

        std::vector<SDL_Point> _superFoodCoordinates;

};

// Concrete Class for Poisons
class Poisons : public GameFeatures
{
    public:
        Poisons(int numberOfPoisons);

        const std::vector<SDL_Point> getPoisonsCoordinates()
        {
            return _PoisonsCoordinates;
        }

        void addPosition(const SDL_Point &point2D) override;
        bool isPresentInPosition(const SDL_Point &point2D) const override;
        void deletePosition(const SDL_Point &point2D) override;

    private:

        std::vector<SDL_Point> _PoisonsCoordinates;
        int _numberOfPoisons;
        
};

#endif