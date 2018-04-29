#ifndef BUTON_H_INCLUDED
#define BUTON_H_INCLUDED

class buton{
public:
    bool eBomba;
    bool eDescoperit;
    bool eMarcat;
    bool eApasat;
    bool eIndicator;

    LTexture imagineAtribuita;

void handleEvent( SDL_Event* e );
};

struct coordBombe{
int x,y;
};

#endif // BUTON_H_INCLUDED
