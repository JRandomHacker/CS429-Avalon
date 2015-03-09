#include "model.hpp"

Model::Model() {

}

Model::~Model() {

}

bool Model::subscribe(std::string data_id, Subscriber* new_subscriber) {
    auto data_iter = flat_data.find(data_id);
    if (data_iter == flat_data.end()) {
        return false;
    }
    return data_iter->second.addSubscriber(new_subscriber);
}

bool Model::unsubscribe(std::string data_id, Subscriber* old_subscriber) {
    auto data_iter = flat_data.find(data_id);
    if (data_iter == flat_data.end()) {
        return false;
    }
    return data_iter->second.detachSubscriber(old_subscriber);
}

bool Model::addData(std::string data_id) {
    auto data_iter = flat_data.find(data_id);
    if (data_iter != flat_data.end()) {
        return false;
    }
    DataBlock new_block;
    flat_data.insert({data_id, new_block});
    return true;
}

bool Model::removeData(std::string data_id) {
    auto data_iter = flat_data.find(data_id);
    if (data_iter == flat_data.end()) {
        return false;
    }
    flat_data.erase(data_iter);
    return true;
}

bool Model::doesDataExist(std::string data_id) {
    return flat_data.find(data_id) != flat_data.end();
}
