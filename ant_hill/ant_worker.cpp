#include "ant_worker.h"

namespace NAnt {
EStatus TWorker::Step(TField& field, const TPostOffice& post) {
    SeeMail();
    Move(field);
    return EStatus::ALIVE;
}

void TWorker::Move(TField& field) {
    field.Get(Head() + Direction());
}

}
