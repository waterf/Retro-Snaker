/*
* @ https://www.youtube.com/watch?v=44tO977slsU
*/
#include "p2.hpp"
void square::squ()
{
	SIZE = 2;
	SQUARE_HEIGHT=20;
	SQUARE_WIDTH=20;
	box[1].x = 0;
	box[1].y = 0;
	for( int i=1;i<=MAX_SIZE;i++ )
	{
		box[i].w = SQUARE_WIDTH;
		box[i].h = SQUARE_HEIGHT;
	}
	xVel = 0;
	yVel = 0;
}

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	started = false;
	paused =  false;
}
void Timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}
void Timer::pause()
{
	started = false;
	paused = false;
}
void Timer::stop()
{
	if( true==started && false==paused )
		{
			paused = true;
			pausedTicks = SDL_GetTicks() - startTicks;
		}
}
void Timer::unpause()
{
	if( true == paused )
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}
int Timer::get_ticks()
{
	if( true==started )
	{
		if( true==paused )
		{
			return pausedTicks;
		}else{
			return SDL_GetTicks() - startTicks;
		}
	}
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}


void square::handle_events( SDL_Event event )
{
	if( SDL_KEYDOWN == event.type )
	{
		switch( event.key.keysym.sym )//系统键
		{
			case SDLK_UP:
				if( xVel)xVel = 0;
				yVel = -1*SQUARE_HEIGHT/2;
				break;
			case SDLK_DOWN:
				if( xVel )xVel = 0;
				yVel = SQUARE_HEIGHT/2;
				break;
			case SDLK_LEFT:
				if( yVel )yVel = 0;
				xVel = -1*SQUARE_WIDTH/2;
				break;
			case SDLK_RIGHT:
				if( yVel )yVel = 0;
				xVel = SQUARE_WIDTH/2;
				break;
			default:
				break;
		}
	}
}

void square::move(int screenHeight, int screenWidth)
{
	if(SIZE>=2)
		{
				if(xVel>0)
				{
					box[SIZE].x = box[SIZE-1].x - SQUARE_WIDTH;
					box[SIZE].y = box[SIZE-1].y;
				}
				if(xVel<0)
				{
					box[SIZE].x = box[SIZE-1].x + SQUARE_WIDTH;
					box[SIZE].y = box[SIZE-1].y;
				}
				if(yVel>0)
				{
					box[SIZE].y = box[SIZE-1].y - SQUARE_WIDTH;
					box[SIZE].x = box[SIZE-1].x;
				}
				if(yVel<0)
				{
					box[SIZE].y = box[SIZE-1].y + SQUARE_WIDTH;
					box[SIZE].x = box[SIZE-1].x;
				}
		}
		box[1].x += xVel;
		if( MAX_SIZE==SIZE )GAME_OVER = true;
		if( box[1].x<0 || (box[1].x+SQUARE_WIDTH > screenWidth) )
		{
			box[1].x -= xVel;
			GAME_OVER = true;
		}
		box[1].y += yVel;
		if( box[1].y<0 || (box[1].y+SQUARE_HEIGHT > screenHeight) )
		{
			box[1].y -= yVel;
			GAME_OVER = true;
		}
		for(int i=SIZE;i>1;i--)
		{
			box[i].x = box[i-1].x;
			box[i].y = box[i-1].y;
		}
		if( check_collision(box[1], food) )
		{
			SIZE++;
			EATED = true;
		}
}
void square::show(SDL_Surface *square, SDL_Surface *screen)
{
	SDL_Rect t;
	t.x = 0;
	t.y = 0;
	t.w = SQUARE_WIDTH;
	t.h = SQUARE_HEIGHT;
	for(int i=1;i<=SIZE;i++)
	{
		apply_surface(box[i].x, box[i].y, square, screen);
	}
	/* 解决（0, 0）总是出现bug的问题 */
	if( EATED )SDL_FillRect(screen, &t, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

bool check_collision(SDL_Rect A, SDL_Rect B)
{
	int leftA, rightA;
	int topA, bottomA;
	int leftB, rightB;
	int topB, bottomB;

	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;
	//此处理解应加入计算机默认坐标系的特性
	if(bottomA <= topB)return false;
	if(topA >= bottomB)return false;
	if(leftA >= rightB)return false;
	if(rightA <= leftB)return false;
	return true;
}
SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
        if(optimizedImage!=NULL)
        {
        	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF,0xFF));
        }
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

SDL_Surface *init(SDL_Surface *screen, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SCREEN_BPP  )
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return NULL;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return NULL;
    }
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format,0xFF, 0xFF, 0xFF));
    //Set the window caption
    SDL_WM_SetCaption( "snaker-game-1.0", NULL );

    //If everything initialized fine
    return screen;
}

void clean_up(SDL_Surface *image)
{
    //Free the surface
    SDL_FreeSurface( image );
}






