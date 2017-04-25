#include "worker.h"

namespace Ant {
EStatus Worker::Step(Field& field, const PostOffice& post) {
    SeeMail();
    Move(field);
    return EStatus::ALIVE;
}

void Worker::Move(Field& field) {
    field.Get(Head() + Direction());
}

}
