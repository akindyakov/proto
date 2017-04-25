#pragma once

#include <memory>
#include <map>

namespace Ant {

enum class EStatus {
    ALIVE,
    DEAD,
};

class AntMessage {
    // stacked
    // angry
    // lose way
};

class Ant {
public:
    Ant(
        Point head,
        Point body
    );

    void Turn(const Vector& direction);
    void Move();

private:
    Point BodyPos;
    Vector DirectionVec;

};

class PostOffice {
public:
    virtual void SendMessage(OwnerId, AntMessage);
};

class AntCatalogue: public PostOffice {
public:
    void Step();

    void SendMessage(OwnerId, AntMessage);
private:
    std::map<OwnerId, std::unique_ptr<Ant>> Catalogue;
};

class Worker: public Ant {
public:
    Worker(
        Point head,
        Point body
    )
        : Ant(head, body)
    {
    }

    EStatus Step(IFieldActor& field, const PostOffice& post);
    void SendMessage(AntMessage msg);

private:
    void SeeMail();
    void Move(Field& field);

private:
    EStatus Status;
    std::unique_ptr<Grain> Load;
    std::vector<AntMessage> MailBox;
};
}
