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
        NField::TPoint body
        , NField::TPoint head
    );

    /**
    * Get diff for moving head to specified direction
    */
    std::vector<NField::ShortMovement>
    DiffHeadMove(NField::ECompass direction) const;

    /**
    * Move head to specified direction
    */
    void HeadMove(NField::ECompass direction);

    /**
    * Get diff for moving tail to specified direction
    */
    std::vector<NField::ShortMovement>
    DiffTailMove(NField::ECompass direction) const;

    /**
    * Move tail to specified direction
    */
    void TailMove(NField::ECompass direction);

    /**
    * Add one more point to head side
    */
    void AppendPoint(NField::ECompass direction);

    /**
    * Drop first point from head side
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
