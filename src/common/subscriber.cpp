#include "subscriber.hpp"

Subscriber::Subscriber() {

}

Subscriber::~Subscriber() {
    if (data_block != NULL) {
        data_block->detachSubscriberQuietly(this);
    }
}

void Subscriber::setDataBlock(DataBlock* new_data_block) {
    data_block = new_data_block;
}

ClosureSubscriber::ClosureSubscriber(std::function<void (Subscriber*)> duc,
        std::function<void (Subscriber*)> ddc) : dataUpdatedClosure(duc), dataDestroyedClosure(ddc) {
    freeOnDestruction = false;
}

ClosureSubscriber::ClosureSubscriber(std::function<void (Subscriber*)> duc,
        std::function<void (Subscriber*)> ddc, bool freeSelfOnDestruction ) : dataUpdatedClosure(duc), dataDestroyedClosure(ddc) {
    freeOnDestruction = freeSelfOnDestruction;
}

void ClosureSubscriber::dataUpdated() {
    if (dataUpdatedClosure) {
        dataUpdatedClosure((Subscriber*)this);
    }
}

bool ClosureSubscriber::dataDestroyed() {
    if (dataDestroyedClosure) {
        dataDestroyedClosure((Subscriber*)this);
    }
    return freeOnDestruction;
}

void MockSubscriber::dataUpdated() {
    updatedCount++;
}

bool MockSubscriber::dataDestroyed() {
    wasDestroyed = true;
    return false;
}