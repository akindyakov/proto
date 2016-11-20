#pragma once

#include <map/lib/transaction.h>

namespace NAnt {

class IAnt {
public:
    virtual bool Step() = 0;
};

class SnakeAntBody {
public:
    SnakeAntBody(
        , TPoint body
        TPoint head
    );

    /**
    * Move by head side
    */
    std::vector<NField::ShortMovement> DiffHeadMove(NField::ECompass direction) const;

    /**
    * Move by head side
    */
    void HeadMove(NField::ECompass direction);

    /**
    * Move by tail side
    */
    std::vector<NField::ShortMovement> DiffTailMove(NField::ECompass direction) const;

    /**
    * Move by tail side
    */
    void TailMove(NField::ECompass direction);

    /**
    * Add one more point to head side
    * TODO: Check that pt is close to the head
    */
    void AppendPoint(NField::ECompass direction);

    /**
    * Add one more point to head side
    * TODO: Check that pt is close to the head
    */
    void DropPoint(NField::ECompass direction);

    size_t Size() const;

private:
    NField::TPoint base_;
    std::vector<NField::ECompass> head_;
};


class FreeLoader
    : public IAnt
{
public:
    bool Step() override;

private:
    SnakeAntBody body_;
};

}
