#pragma once

#include "2d_field.h"

#include <memory>
#include <map>

namespace NAnt {

enum class EStatus {
    ALIVE,
    DEAD,
};

class TAntMessage {
    // stacked
    // angry
    // lose way
};

class TAnt {
public:
    TAnt(
        TPoint head,
        TPoint body
    );

    void Turn(const TVector& direction);
    void Move();

private:
    TPoint BodyPos;
    TVector DirectionVec;

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

    EStatus Step(IFieldActor& field, const TPostOffice& post);
    void SendMessage(TAntMessage msg);

private:
    void SeeMail();
    void Move(TField& field);

private:
    EStatus Status;
    std::unique_ptr<TGrain> Load;
    std::vector<TAntMessage> MailBox;
};
}
