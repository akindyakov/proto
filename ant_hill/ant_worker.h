#pragma once

#include "2d_field.h"

#include <memory>
#include <map>

enum EAntStatus {
    ALIVE,
    DEAD,
};

class TAntMessage {
};

class TAnt {
public:
    TAnt(
        TPoint head,
        TPoint mesosoma,
        TPoint petiole,
        TPoint gaster
    )
        : Head(head)
        , Mesosoma(mesosoma)
        , Petiole(petiole)
        , Gaster(gaster)
    {
    }

    void Turn();
    void Move();

private:
    TPoint Head;
    TPoint Mesosoma;
    TPoint Petiole;
    TPoint Gaster;
};

class TPostOffice {
public:
    virtual void SendMessage(TOwnerId, TAntMessage);
};

class TAntCatalogue: public TPostOffice {
public:
    void Step();

    void SendMessage(TOwnerId, TAntMessage);
private:
    std::map<TOwnerId, std::unique_ptr<TAnt>> Catalogue;
};

class TWorker: public TAnt {
public:
    TWorker(
       TPoint head,
       TPoint mesosoma,
       TPoint petiole,
       TPoint gaster
    )
        : TAnt(
            head,
            mesosoma,
            petiole,
            gaster
        )
    {
    }

    EAntStatus Step(TField& field, const TPostOffice& post);
    void SendMessage(TAntMessage msg);

private:
    void SeeMail();
    void Move();

private:
    EAntStatus Status;
    std::unique_ptr<TGrain> Load;
    std::vector<TAntMessage> MailBox;
};
