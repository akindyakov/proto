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
    )
        : BodyPos(body)
        , DirectionVec(body - head)
    {
        if (DirectionVec.Lenght() >= 4) {
            throw std::exception();
        }
    }

    void Turn(const TVector& direction);
    void Move();

    TPoint Head() const;
    const TPoint& Body() const;
    const TVector& Direction() const;

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

    EStatus Step(TField& field, const TPostOffice& post);
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
