#ifndef BUTONMODIFICATOR_H_INCLUDED
#define BUTONMODIFICATOR_H_INCLUDED

class butonModificator{
public:
    enum tipControl {latime, inaltime, bombe};

    bool multipluZece=false;
    bool pozitiv=true;

    tipControl tip;

    LTexture imagineAtribuita;

    int x,y;

    void setCoord();
struct pozButoane
{
    int x; int y;

};
    pozButoane pozBut;

void handleEvent( SDL_Event* e );
};





#endif // BUTONMODIFICATOR_H_INCLUDED
