
#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <unordered_map>

#include "model_data.hpp"
#include "data_block.hpp"
#include "subscriber.hpp"

class Model {
public:
    Model();
    ~Model();

    bool subscribe(std::string data_id, Subscriber* new_subscriber);
    bool unsubscribe(std::string data_id, Subscriber* old_subscriber);

    template <typename T>
    bool updateData(std::string data_id, const T& new_data);
    template <typename T>
    bool addData(std::string data_id, const T& initial_data);
    bool addData(std::string data_id);
    bool removeData(std::string data_id);

private:
    bool doesDataExist(std::string data_id);

    std::unordered_map<std::string, DataBlock> flat_data;

    Model(const Model& that);
};

template <typename T>
bool Model::updateData(std::string data_id, const T& new_data) {
    auto data_iter = flat_data.find(data_id);
    if (data_iter == flat_data.end()) {
        return false;
    }
    data_iter->second.updateData<T>(new_data);
    return true;
}

template <typename T>
bool Model::addData(std::string data_id, const T& initial_value) {
    bool data_created = addData(data_id);
    if (!data_created) {
        return false;
    }
    return updateData<T>(data_id, initial_value);
}

#endif // MODEL_HPP
