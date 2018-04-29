#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "LTexture.h"
#include "LTimer.h"
#include "buton.h"
#include "butonStart.h"
#include "butonModificator.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MARIME_PATRAT = 16;

int NR_COLOANE_MAX=40;
int NR_LINII_MAX=28;

int NR_COLOANE=9; //max 40
int NR_LINII=9; //max 28
int NR_BOMBE=12; //550 e mult cand tabelul e max



float deltaTime;

float counterExplozie=0; //de resetat
bool jocTerminat=false;
bool accesLaFor=true;
int patrateDescoperite=0;
bool jocInceput=false;
bool modCastig=false;



bool regulatorDrClick=true;

bool init();
bool loadMedia();
void close();
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

LTexture patrat_simplu;
LTexture patrat_gol;
LTexture patrat_marcat;
LTexture patrat_indicator_1;
LTexture patrat_indicator_2;
LTexture patrat_indicator_3;
LTexture patrat_indicator_4;
LTexture patrat_indicator_5;
LTexture patrat_indicator_6;
LTexture patrat_indicator_7;
LTexture patrat_indicator_8;
LTexture patrat_distrus;
LTexture patrat_explozie_1;
LTexture patrat_explozie_2;
LTexture patrat_explozie_3;
LTexture patrat_explozie_4;
LTexture buton_start;
LTexture buton_start_apasat;
LTexture buton_plus;
LTexture buton_plus_10;
LTexture buton_plus_apasat;
LTexture buton_minus;
LTexture buton_minus_10;
LTexture buton_minus_apasat;
LTexture numar_0;
LTexture numar_1;
LTexture numar_2;
LTexture numar_3;
LTexture numar_4;
LTexture numar_5;
LTexture numar_6;
LTexture numar_7;
LTexture numar_8;
LTexture numar_9;
LTexture fundal_ceas;
LTexture dimensiune_inaltime;
LTexture dimensiune_latime;
LTexture patrat_gol_castig;

LTexture cifraMii;
LTexture cifraSute;
LTexture cifraZeci;
LTexture cifraUnit;
LTexture cifraAfis;
LTexture credit;

LTexture interfata_test;

butonStart StartBut;



class explozie
{
public:
    void animatie(int ex, int ey);
};

void explozie::animatie(int ex, int ey)
{
    patrat_explozie_1.render(ex,ey);
    //SDL_Delay(66);
    patrat_explozie_2.render(ex,ey);
    //SDL_Delay(66);
    patrat_explozie_3.render(ex,ey);
    //SDL_Delay(66);
    patrat_explozie_4.render(ex,ey);
    //SDL_Delay(66);
}

void actualizeazaSeedRng()
{
    srand (time(NULL));
}

int rng()
{
    int x=rand()%100;
    return x;
}

bool LTexture::loadFromFile( std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

            newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	mTexture = newTexture;
	return mTexture != NULL;
}



void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}






coordBombe bombe[1120];
buton patrateJoc[45][45];



bool init()
{
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		gWindow = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;
	if( !patrat_simplu.loadFromFile( "imagini/patrat_simplu.png"  ))
	{
		success = false;
	}

	if( !patrat_indicator_1.loadFromFile( "imagini/patrat_indicator_1.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_2.loadFromFile( "imagini/patrat_indicator_2.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_3.loadFromFile( "imagini/patrat_indicator_3.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_4.loadFromFile( "imagini/patrat_indicator_4.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_5.loadFromFile( "imagini/patrat_indicator_5.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_6.loadFromFile( "imagini/patrat_indicator_6.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_7.loadFromFile( "imagini/patrat_indicator_7.png" ))
	{
		success = false;
	}

	if( !patrat_indicator_8.loadFromFile( "imagini/patrat_indicator_8.png" ))
	{
		success = false;
	}

	if( !patrat_distrus.loadFromFile( "imagini/patrat_distrus.png" ))
	{
		success = false;
	}

	if( !patrat_gol.loadFromFile( "imagini/patrat_gol.png" ))
	{
		success = false;
	}

	if( !patrat_explozie_1.loadFromFile( "imagini/patrat_explozie_1.png" ))
	{
		success = false;
	}

	if( !patrat_explozie_2.loadFromFile( "imagini/patrat_explozie_2.png" ))
	{
		success = false;
	}

	if( !patrat_explozie_3.loadFromFile( "imagini/patrat_explozie_3.png" ))
	{
		success = false;
	}

	if( !patrat_explozie_4.loadFromFile( "imagini/patrat_explozie_4.png" ))
	{
		success = false;
	}

	if( !numar_0.loadFromFile( "imagini/numar_0.png" ))
	{
		success = false;
	}

	if( !numar_1.loadFromFile( "imagini/numar_1.png" ))
	{
		success = false;
	}

	if( !numar_2.loadFromFile( "imagini/numar_2.png" ))
	{
		success = false;
	}

	if( !numar_3.loadFromFile( "imagini/numar_3.png" ))
	{
		success = false;
	}

	if( !numar_4.loadFromFile( "imagini/numar_4.png" ))
	{
		success = false;
	}

	if( !numar_5.loadFromFile( "imagini/numar_5.png" ))
	{
		success = false;
	}

	if( !numar_6.loadFromFile( "imagini/numar_6.png" ))
	{
		success = false;
	}

	if( !numar_7.loadFromFile( "imagini/numar_7.png" ))
	{
		success = false;
	}

	if( !numar_8.loadFromFile( "imagini/numar_8.png" ))
	{
		success = false;
	}

	if( !numar_9.loadFromFile( "imagini/numar_9.png" ))
	{
		success = false;
	}

	if( !fundal_ceas.loadFromFile( "imagini/fundal_ceas.png" ))
	{
		success = false;
	}

	if( !buton_start.loadFromFile( "imagini/buton_start.png" ))
	{
		success = false;
	}

	if( !buton_start_apasat.loadFromFile( "imagini/buton_start_apasat.png" ))
	{
		success = false;
	}

	if( !credit.loadFromFile( "imagini/copyright_mihalache_mircea_2018.png" ))
	{
		success = false;
	}

	if( !dimensiune_inaltime.loadFromFile( "imagini/dimensiune_inaltime.png" ))
	{
		success = false;
	}

	if( !dimensiune_latime.loadFromFile( "imagini/dimensiune_latime.png" ))
	{
		success = false;
	}

	if( !buton_plus.loadFromFile( "imagini/buton_plus.png" ))
	{
		success = false;
	}

	if( !buton_minus.loadFromFile( "imagini/buton_minus.png" ))
	{
		success = false;
	}
	if( !buton_plus_10.loadFromFile( "imagini/buton_plus_10.png" ))
	{
		success = false;
	}

	if( !buton_minus_10.loadFromFile( "imagini/buton_minus_10.png" ))
	{
		success = false;
	}

	if( !patrat_gol_castig.loadFromFile( "imagini/patrat_gol_castig.png" ))
	{
		success = false;
	}

	if( !patrat_marcat.loadFromFile( "imagini/patrat_marcat.png" ))
	{
		success = false;
	}

	return success;
}

void close()
{
	//gDotTexture.free();
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

bool existaDejaBomba(int bx, int by, int numar)
{
    for(int i=0;i<numar;i++)
        if(bombe[i].x==bx && bombe[i].y==by)
            return true;

    bombe[numar].x=bx;
    bombe[numar].y=by;
    return false;
}



void actualizeazaTabel()
{
        for(int i=0;i<NR_LINII;i++)
                for(int j=0;j<NR_COLOANE;j++)
                    {if(patrateJoc[i][j].eDescoperit)
                    {
                        if(modCastig)
                        patrat_gol_castig.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);
                        else
                        patrat_gol.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);

                    patrateJoc[i][j].imagineAtribuita.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);

                        if(!patrateJoc[i][j].eIndicator)
                            if(modCastig)
                            patrat_gol_castig.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);

                    patrateJoc[i][j].eMarcat=false;


                    }
                    else
                        patrat_simplu.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);

                        if(patrateJoc[i][j].eMarcat)
                            patrat_marcat.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);

                    /*if(patrateJoc[i][j].eBomba)
                        patrateJoc[i][j].imagineAtribuita.render(j*MARIME_PATRAT,i*MARIME_PATRAT+32);*/
                    }
}

void generareTabel()
{
        for(int i=0;i<NR_LINII;i++)
                for(int j=0;j<NR_COLOANE;j++)
                    patrateJoc[i][j].imagineAtribuita=patrat_simplu;

        actualizeazaTabel();
}

void aplicaNumere()
{
    int numarB;

    for(int i=0;i<NR_LINII;i++)
                for(int j=0;j<NR_COLOANE;j++)
                {
                    if(!patrateJoc[i][j].eBomba)

                    {numarB=0;
                    if(i-1>=0)
                    {
                        if(patrateJoc[i-1][j].eBomba)
                            numarB++;
                        if(j-1>=0)
                            if(patrateJoc[i-1][j-1].eBomba)
                                numarB++;
                        if(j+1<NR_COLOANE)
                            if(patrateJoc[i-1][j+1].eBomba)
                                numarB++;
                    }
                    if(i+1<NR_LINII)
                    {
                        if(patrateJoc[i+1][j].eBomba)
                            numarB++;
                        if(j-1>=0)
                            if(patrateJoc[i+1][j-1].eBomba)
                                numarB++;
                        if(j+1<NR_COLOANE)
                            if(patrateJoc[i+1][j+1].eBomba)
                                numarB++;
                    }
                    if(j-1>=0)
                        if(patrateJoc[i][j-1].eBomba)
                                numarB++;

                    if(j+1<NR_COLOANE)
                            if(patrateJoc[i][j+1].eBomba)
                                numarB++;

                                switch(numarB){
                                case 1:patrateJoc[i][j].imagineAtribuita=patrat_indicator_1;patrateJoc[i][j].eIndicator=true;break;
                                case 2:patrateJoc[i][j].imagineAtribuita=patrat_indicator_2;patrateJoc[i][j].eIndicator=true;break;
                                case 3:patrateJoc[i][j].imagineAtribuita=patrat_indicator_3;patrateJoc[i][j].eIndicator=true;break;
                                case 4:patrateJoc[i][j].imagineAtribuita=patrat_indicator_4;patrateJoc[i][j].eIndicator=true;break;
                                case 5:patrateJoc[i][j].imagineAtribuita=patrat_indicator_5;patrateJoc[i][j].eIndicator=true;break;
                                case 6:patrateJoc[i][j].imagineAtribuita=patrat_indicator_6;patrateJoc[i][j].eIndicator=true;break;
                                case 7:patrateJoc[i][j].imagineAtribuita=patrat_indicator_7;patrateJoc[i][j].eIndicator=true;break;
                                case 8:patrateJoc[i][j].imagineAtribuita=patrat_indicator_8;patrateJoc[i][j].eIndicator=true;break;
                                }
                }
            }


}


void plaseazaBombe(int evitaX, int evitaY)
{

int bx=0, by=0;
int numar=0;

while(numar<NR_BOMBE)
    {
    bx=rng();by=rng();


    if(!(by==evitaX && bx== evitaY))
        if(by<NR_COLOANE)
            if(bx<NR_LINII)
                if(!existaDejaBomba(bx,by,numar))
                    {
                        patrateJoc[bx][by].eBomba=true;
                        patrateJoc[bx][by].imagineAtribuita=patrat_distrus;
                        numar++;
                        if(bx==evitaX && by== evitaY)
                        std::cout<<"faaaa";
                    }

    }

aplicaNumere();
}

void umplere(int uy, int ux)
{

    patrateJoc[uy][ux].eDescoperit=true;
    patrateDescoperite++;

    if(!patrateJoc[uy][ux].eIndicator)
            {

                patrateJoc[uy][ux].imagineAtribuita=patrat_gol;

                if(uy-1>=0)
                    {
                        if(!patrateJoc[uy-1][ux].eDescoperit&&!patrateJoc[uy-1][ux].eBomba)
                            umplere(uy-1,ux);
                        if(ux-1>=0)
                            if(!patrateJoc[uy-1][ux-1].eDescoperit&&!patrateJoc[uy-1][ux-1].eBomba)
                            umplere(uy-1,ux-1);
                        if(ux+1<NR_COLOANE)
                            if(!patrateJoc[uy-1][ux+1].eDescoperit&&!patrateJoc[uy-1][ux+1].eBomba)
                             umplere(uy-1,ux+1);
                    }
                if(uy+1<NR_LINII)
                    {
                        if(!patrateJoc[uy+1][ux].eDescoperit&&!patrateJoc[uy+1][ux].eBomba)
                            umplere(uy+1,ux);
                        if(ux-1>=0)
                            if(!patrateJoc[uy+1][ux-1].eDescoperit&&!patrateJoc[uy+1][ux-1].eBomba)
                             umplere(uy+1,ux-1);
                        if(ux+1<NR_COLOANE)
                            if(!patrateJoc[uy+1][ux+1].eDescoperit&&!patrateJoc[uy+1][ux+1].eBomba)
                             umplere(uy+1,ux+1);
                    }
                if(ux-1>=0)
                        if(!patrateJoc[uy][ux-1].eDescoperit&&!patrateJoc[uy][ux-1].eBomba)
                                umplere(uy,ux-1);

                if(ux+1<NR_COLOANE)
                            if(!patrateJoc[uy][ux+1].eDescoperit&&!patrateJoc[uy][ux+1].eBomba)
                                umplere(uy,ux+1);
            }

}
LTimer stepTimer;
LTimer gameTimer;
void genereazaTot(int evitaX, int evitaY)
{
            actualizeazaSeedRng();
            generareTabel();

            gameTimer.start();

            plaseazaBombe(evitaX,evitaY);
            StartBut.imagineAtribuita=buton_start;

}


void buton::handleEvent(SDL_Event* e)
{

    if(e->type == SDL_MOUSEBUTTONDOWN)

    {

        int mx, my;

        SDL_GetMouseState( &mx, &my );
        int pozX=mx/16, pozY=(my-32)/16;


        if(!jocTerminat)
        {


            if(my>32&&my<NR_LINII*16+32&&mx<16*NR_COLOANE)
            {if(e->button.button == SDL_BUTTON_LEFT)
            {
                if(!jocInceput)
                {
                genereazaTot(pozX, pozY);
                jocInceput=true;
                std::cout<<pozX<<" "<<pozY<<"\n";
                }

                if(!patrateJoc[pozY][pozX].eMarcat)
                {

                if(!patrateJoc[pozY][pozX].eBomba)
                {

                    if(!patrateJoc[pozY][pozX].eDescoperit)
                    umplere((my-32)/16,mx/16);
                }
                else
                {
                    patrateJoc[pozY][pozX].imagineAtribuita=patrat_distrus;

                    if(accesLaFor)
                    {
                        accesLaFor=false;
                        std::cout<<pozX<<" "<<pozY<<'\n';
                        for(int q=0;q<NR_BOMBE;q++)
                        {
                        //std::cout<<"comparing "<<bombe[q].y<<" to "<<pozY<<"\n";
                        if(bombe[q].y==pozX && bombe[q].x==pozY)
                            {

                            //std::cout<<"found("<<q<<")";
                            //std::swap(bombe[q].x, bombe[0].x);
                            //std::swap(bombe[q].y, bombe[0].y);
                            std::swap(bombe[q], bombe[0]);

                            //std::cout<<"FIRST BOMB IN QUEUE IS NOW: "<<bombe[0].x<<bombe[0].y;
                            }
                        //std::cout<<bombe[i].x<<" "<<bombe[i].y<<'\n';
                        }
                        jocTerminat=true;
                        gameTimer.pause();
                    }


                    //std::cout<<'\n';
                    /*for(int i=0;i<NR_BOMBE;i++)
                    {
                    std::cout<<bombe[i].x<<","<<bombe[i].y<<'\n';
                    }*/

                }
                }

            }

            if(e->button.button == SDL_BUTTON_RIGHT)
                {
                if(regulatorDrClick)
                {
                    regulatorDrClick=false;
                    if(!patrateJoc[pozY][pozX].eDescoperit)
                    {
                        std::cout<<"mmm";
                        patrateJoc[pozY][pozX].eMarcat= !patrateJoc[pozY][pozX].eMarcat;
                    }

                }
                }
            }
            }



    }
}


LTexture afiseazaCifra(int cifra)
{
    LTexture cifraAfis=numar_0;


    switch(cifra)
    {
        case 0: cifraAfis=numar_0;std::cout<<0;break;
        case 1: cifraAfis=numar_1;std::cout<<1;break;
        case 2: cifraAfis=numar_2;std::cout<<2;break;
        case 3: cifraAfis=numar_3;std::cout<<3;break;
        case 4: cifraAfis=numar_4;std::cout<<4;break;
        case 5: cifraAfis=numar_5;std::cout<<5;break;
        case 6: cifraAfis=numar_6;std::cout<<6;break;
        case 7: cifraAfis=numar_7;std::cout<<7;break;
        case 8: cifraAfis=numar_8;std::cout<<8;break;
        case 9: cifraAfis=numar_9;std::cout<<9;break;
    }

return cifraAfis;

}



void randeazaExplozii(float adaos)
{
    if(jocTerminat)
    {
        patrateJoc[bombe[0].x][bombe[0].y].eDescoperit=true; //fix la un bug care face revert la prima bomba ocazional intr-un patrat neapasat
        counterExplozie+=adaos;
        //std::cout<<counterExplozie<<'\n';
        for(int i=0;i<NR_BOMBE;i++)
        {
            if(counterExplozie<=0.1f+(float)i/10&&counterExplozie>=(float)i/10)
                {patrat_explozie_1.render(bombe[i].y*16-8,bombe[i].x*16+24);
                patrateJoc[bombe[i].x][bombe[i].y].imagineAtribuita=patrat_distrus;
                patrateJoc[bombe[i].x][bombe[i].y].eDescoperit=true;
                }
            if(counterExplozie>=0.1f+(float)i/10 && counterExplozie<0.2f+(float)i/10)
                patrat_explozie_2.render(bombe[i].y*16-8,bombe[i].x*16+24);
            if(counterExplozie>=0.2f+(float)i/10 && counterExplozie<0.3f+(float)i/10)
                patrat_explozie_3.render(bombe[i].y*16-8,bombe[i].x*16+24);
            if(counterExplozie>=0.3f+(float)i/10 && counterExplozie<0.4f+(float)i/10)
                patrat_explozie_4.render(bombe[i].y*16-8,bombe[i].x*16+24);
        }
    }
}



void pregatesteJocNou()
{
    modCastig=false;
    for(int i=0;i<NR_LINII;i++)
        for(int j=0;j<NR_COLOANE;j++)
        {
        patrateJoc[i][j].eApasat=false;
        patrateJoc[i][j].eDescoperit=false;
        patrateJoc[i][j].eBomba=false;
        patrateJoc[i][j].eIndicator=false;
        patrateJoc[i][j].eMarcat=false;
        }

        for(int i=0;i<=NR_BOMBE;i++)
        {
            bombe[i].x=-1;
            bombe[i].y=-1;
        }
            patrateDescoperite=0;
            jocInceput=false; //determina generare


            cifraMii=numar_0;
            cifraSute=numar_0;
            cifraZeci=numar_0;
            cifraUnit=numar_0;

            /*for(int i=0;i<NR_BOMBE;i++)
            {
                std::cout<<bombe[i].x<<","<<bombe[i].y<<'\n';
            }*/

            counterExplozie=0; //de resetat
            jocTerminat=false;
            accesLaFor=true;

            gameTimer.stop();

}



void butonStart::handleEvent(SDL_Event* e)
{

    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx, my;

        SDL_GetMouseState( &mx, &my );

        if(mx>64&&mx<123&&my<32)
        {
            imagineAtribuita=buton_start_apasat;
            pregatesteJocNou();
        }
    }
}
butonModificator butoane[12];



void butonModificator::handleEvent(SDL_Event* e)
{

    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx, my;
            //std::cout<<"utjgjtgn";
        SDL_GetMouseState( &mx, &my );

        std::cout<<"\n" <<pozBut.y<<" " <<pozBut.x<<"\n";
        std::cout<<"\n" <<mx<<" " <<my<<"\n";

        std::cout<<mx<<">"<<pozBut.x<<"\n";
        std::cout<<mx<<"<"<<pozBut.x+32<<"\n";
        std::cout<<my<<">"<<0<<"\n";
        std::cout<<my<<"<"<<16<<"\n";

        if(mx>pozBut.x && mx<pozBut.x+32 && my<pozBut.y+16 && my>pozBut.y)
        {
            if(tip==latime)
            {
                if(pozitiv)
                {

                if(multipluZece)
                    NR_COLOANE+=10;
                    else
                    NR_COLOANE++;
                }
                else
                {
                if(multipluZece)
                    NR_COLOANE-=10;
                    else
                    NR_COLOANE--;
                }
                if(NR_COLOANE>NR_COLOANE_MAX)
                    NR_COLOANE=NR_COLOANE_MAX;
                if(NR_COLOANE<4)
                    NR_COLOANE=4;
            }
            if(tip==inaltime)
            {
            if(pozitiv)
                {

                if(multipluZece)
                    NR_LINII+=10;
                    else
                    NR_LINII++;
                }
                else
                {
                if(multipluZece)
                    NR_LINII-=10;
                    else
                    NR_LINII--;
                }
                if(NR_LINII>NR_LINII_MAX)
                    NR_LINII=NR_LINII_MAX;
                if(NR_LINII<4)
                    NR_LINII=4;
            }

            if(tip==bombe)
            {
            if(pozitiv)
                {

                if(multipluZece)
                    NR_BOMBE+=10;
                    else
                    NR_BOMBE++;
                }
                else
                {
                if(multipluZece)
                    NR_BOMBE-=10;
                    else
                    NR_BOMBE--;
                }

            }

            if(NR_BOMBE>NR_LINII*NR_COLOANE-2)
                NR_BOMBE=NR_LINII*NR_COLOANE-2;
            if(NR_BOMBE<2)
                NR_BOMBE=2;

            pregatesteJocNou();


            //std::cout<<"asdasd";
        }
    }
}

void atribuireButoane()
{
butoane[0].pozitiv=true;
butoane[0].pozBut.x=164;
butoane[0].pozBut.y=0;
butoane[0].tip=butonModificator::latime;
butoane[0].multipluZece=false;

butoane[1].pozitiv=true;
butoane[1].pozBut.x=164;
butoane[1].pozBut.y=16;
butoane[1].tip=butonModificator::latime;
butoane[1].multipluZece=true;

butoane[2].pozitiv=false;
butoane[2].pozBut.x=132;
butoane[2].pozBut.y=0;
butoane[2].tip=butonModificator::latime;
butoane[2].multipluZece=false;

butoane[3].pozitiv=false;
butoane[3].pozBut.x=132;
butoane[3].pozBut.y=16;
butoane[3].tip=butonModificator::latime;
butoane[3].multipluZece=true;

butoane[4].pozitiv=true;
butoane[4].pozBut.x=264;
butoane[4].pozBut.y=0;
butoane[4].tip=butonModificator::inaltime;
butoane[4].multipluZece=false;

butoane[5].pozitiv=true;
butoane[5].pozBut.x=264;
butoane[5].pozBut.y=16;
butoane[5].tip=butonModificator::inaltime;
butoane[5].multipluZece=true;

butoane[6].pozitiv=false;
butoane[6].pozBut.x=232;
butoane[6].pozBut.y=0;
butoane[6].tip=butonModificator::inaltime;
butoane[6].multipluZece=false;

butoane[7].pozitiv=false;
butoane[7].pozBut.x=232;
butoane[7].pozBut.y=16;
butoane[7].tip=butonModificator::inaltime;
butoane[7].multipluZece=true;

butoane[8].pozitiv=true;
butoane[8].pozBut.x=428;
butoane[8].pozBut.y=0;
butoane[8].tip=butonModificator::bombe;
butoane[8].multipluZece=false;

butoane[9].pozitiv=true;
butoane[9].pozBut.x=428;
butoane[9].pozBut.y=16;
butoane[9].tip=butonModificator::bombe;
butoane[9].multipluZece=true;

butoane[10].pozitiv=false;
butoane[10].pozBut.x=396;
butoane[10].pozBut.y=0;
butoane[10].tip=butonModificator::bombe;
butoane[10].multipluZece=false;

butoane[11].pozitiv=false;
butoane[11].pozBut.x=396;
butoane[11].pozBut.y=16;
butoane[11].tip=butonModificator::bombe;
butoane[11].multipluZece=true;

}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			////////////////////////////////////////////////////////////////////////////////////start
			StartBut.imagineAtribuita=buton_start;

			atribuireButoane();

            for(int i=0;i<12;i++)
            {
                if(butoane[i].pozitiv)
                    if(butoane[i].multipluZece)
                    butoane[i].imagineAtribuita=buton_plus_10;
                    else
                    butoane[i].imagineAtribuita=buton_plus;
                else
                    if(butoane[i].multipluZece)
                    butoane[i].imagineAtribuita=buton_minus_10;
                    else
                    butoane[i].imagineAtribuita=buton_minus;
            }

			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					for(int i=0;i<NR_LINII;i++)
                        for(int j=0;j<NR_COLOANE;j++)
                            patrateJoc[i][j].handleEvent(&e);
                    StartBut.handleEvent(&e);

                    for(int i=0;i<12;i++)
                    {
                        butoane[i].handleEvent(&e);
                    }
                    //butoane[0].handleEvent(&e);
				}

				deltaTime = stepTimer.getTicks() / 1000.f;
				stepTimer.start();

				int timp=gameTimer.getTicks()/1000;
				if(timp>9999)
                    timp=9999;

                //afiseazaTimp();
                //std::cout<<gameTimer.getTicks()<<'\n';


                //std::cout<<deltaTime<<'\n';

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( gRenderer );




                actualizeazaTabel();
                randeazaExplozii(deltaTime);

                fundal_ceas.render(0,0);

                switch(timp%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(MARIME_PATRAT*3,0);
                switch(timp/10%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(MARIME_PATRAT*2,0);
                switch(timp/10/10%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(MARIME_PATRAT,0);
                switch(timp/10/10/10%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(0,0);

                fundal_ceas.render(332,0);

                switch(NR_BOMBE%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(MARIME_PATRAT*3+332,0);
                switch(NR_BOMBE/10%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(MARIME_PATRAT*2+332,0);
                switch(NR_BOMBE/10/10%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(MARIME_PATRAT+332,0);
                switch(NR_BOMBE/10/10/10%10)
                {
                    case 0: cifraAfis=numar_0;break;
                    case 1: cifraAfis=numar_1;break;
                    case 2: cifraAfis=numar_2;break;
                    case 3: cifraAfis=numar_3;break;
                    case 4: cifraAfis=numar_4;break;
                    case 5: cifraAfis=numar_5;break;
                    case 6: cifraAfis=numar_6;break;
                    case 7: cifraAfis=numar_7;break;
                    case 8: cifraAfis=numar_8;break;
                    case 9: cifraAfis=numar_9;break;
                }
                cifraAfis.render(332,0);


                for(int r=0;r<12;r++)
                {

                    butoane[r].imagineAtribuita.render(butoane[r].pozBut.x,butoane[r].pozBut.y);
                    //std::cout<<"rendered at "<<butoane[r].pozBut.x<<" "<<butoane[r].pozBut.y<<"\n";
                }


                StartBut.imagineAtribuita.render(64,0);
                dimensiune_latime.render(196,0);
                dimensiune_inaltime.render(296,0);
                credit.render(467,0);

                /*cifraMii.render(0,0);
                cifraSute.render(16,0);
                cifraZeci.render(32,0);
                cifraUnit.render(48,0);*/

                if(patrateDescoperite==NR_LINII*NR_COLOANE-NR_BOMBE)
                    {
                        modCastig=true;
                        gameTimer.pause();
                    }
                //std::cout<<patrateDescoperite<<"\n";

				SDL_RenderPresent( gRenderer );

				regulatorDrClick=true;
			}
		}
	}
	close();



	return 0;
}
