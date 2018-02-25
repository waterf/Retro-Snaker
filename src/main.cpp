#include "p2.hpp"
//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int SQUARE_HEIGHT = 20;
const int SQUARE_WIDTH = 20;
const int FRAMES_PER_SECOND = 20;
SDL_Rect food;//食物的
bool GAME_OVER = false;
bool EATED  = false;
square *mysquare = NULL;

int main( int argc, char* args[] )
{
	SDL_Surface *image = NULL;
	SDL_Surface *screen = NULL, *message=NULL, *score=NULL;
	TTF_Font *font = NULL;
	SDL_Color textColor = {0, 0, 0};
	bool quit = false;
	int frame = 0;
	SDL_Event event;

	food.x = random(SCREEN_WIDTH-20);
	food.y = random(SCREEN_HEIGHT-20);
	food.w = SQUARE_WIDTH;
	food.h = SQUARE_HEIGHT;
	font = TTF_OpenFont( "images/lazy.ttf", 24 );

	square *mySquare = (square *)malloc(sizeof(square));
	Timer *fps = (Timer *)malloc(sizeof(Timer));
	Timer *tFood = (Timer *)malloc(sizeof(Timer));

    if( init(screen, SCREEN_WIDTH,  SCREEN_HEIGHT, SCREEN_BPP ) == NULL )
    {
        return 1;
    }
    screen = init(screen, SCREEN_WIDTH,  SCREEN_HEIGHT, SCREEN_BPP );
    //Load the image
    image = load_image( "images/square.bmp" );
    message = TTF_RenderText_Solid( font, "Game_Over!", textColor);


    //If there was a problem in loading the image
    if( image == NULL )
    {
        return 1;
    }
    //Apply the surface to the screen

    mySquare->squ();
    tFood->start();
    while( false==quit )
    {
    	fps->start();
    	if( false==GAME_OVER )
    	{
    		if( tFood->get_ticks() >= 4000)
    		{
    			food.x = random(SCREEN_WIDTH-20);
    			food.y = random(SCREEN_HEIGHT-20);
    			food.w = SQUARE_WIDTH;
    			food.h = SQUARE_HEIGHT;
    			tFood->start();
    		}
    	}
    	while( SDL_PollEvent( &event ) )
    	{//假设循环中只处理消息event
    		if( !GAME_OVER )
    		{
    			mySquare->handle_events( event );
    		}
    		if( SDL_QUIT == event.type )quit = true;
    	}
    	frame++;
    	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0xFF, 0xFF, 0xFF));
    	if( !EATED )
    	{
    		SDL_FillRect(screen, &food,SDL_MapRGB(screen->format, 0, 0xFF, 0));
    	}else{
    		SDL_FillRect(screen, &food,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));//立即改变food坐标
    		food.x = -100;
    		food.y = -100;
    		EATED = false;
    	}
    	mySquare->move(SCREEN_HEIGHT, SCREEN_WIDTH);
        mySquare->show(image, screen);//
        if( fps->get_ticks() < 1000/FRAMES_PER_SECOND )//这里帧数的意义不明
        {
        	SDL_Delay( 1000/FRAMES_PER_SECOND - fps->get_ticks() );
        }
    	if( SDL_Flip( screen ) == -1 )//刷新........invalidateRect //Update the screen
    	{
    	    return 1;
    	}
    	//if( GAME_OVER )
    	//{
    		apply_surface( 50, 50, message, screen);
    	//}
    }
    //Free the surface and quit SDL
    clean_up(image);
    clean_up(screen);
    clean_up(message);
    clean_up(score);
    free(mysquare);
    free(fps);
    free(tFood);
    SDL_Quit();
    return 0;
}
