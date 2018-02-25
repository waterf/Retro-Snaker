/*
 * p2.hpp
 *
 *  Created on: 2018年1月29日
 *      Author: jin
 */

#ifndef P2_HPP_
#define P2_HPP_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>
#define MAX_SIZE 20
#define random(x) rand()%x
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;
extern const int SQAURE_HEIGHT;
extern const int SQUARE_WIDTH;
extern const int FRAMES_PER_SECOND;
extern bool GAME_OVER;
extern bool EATED;
extern SDL_Rect food;//食物的;
bool check_collision(SDL_Rect A, SDL_Rect B);
SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
SDL_Surface *init(SDL_Surface *screen, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCREEN_BPP);
void clean_up(SDL_Surface *image);
class square
{
public:
	void squ();
	~square();
	void handle_events(SDL_Event event);
	void move(int screenHeight, int screenWidth);
	void show(SDL_Surface *square, SDL_Surface *screen);
private:
	SDL_Rect box[MAX_SIZE];
	int xVel;
	int yVel;
	int SIZE;
	int SQUARE_HEIGHT;
	int SQUARE_WIDTH;

};
class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void pause();
	void stop();
	void unpause();
	int get_ticks();

	bool is_started();
	bool is_paused();
private:
	int startTicks;
	int pausedTicks;
	bool paused;
	bool started;
};
#endif /* P2_HPP_ */
