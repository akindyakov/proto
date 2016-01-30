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
        TPoint body
    )
        : Head(head)
        , Body(body)
    {
    }

    enum ETurnDirection {
        CLOCKWISE,
        CONTERCLOCKWISE,
    };
    void Turn();
    void Move();

private:
    TPoint Head;
    TPoint Body;
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
        TPoint body
    )
        : TAnt(head, body)
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
