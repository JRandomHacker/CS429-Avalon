#include "model_data.hpp"

ModelData::ModelData() {

}

ModelData::~ModelData() {
	deallocatePayload();
}

void ModelData::setPayload(int new_payload) {
	allocatePayload(sizeof(int));
	*(int*)payload = new_payload;
}

int* ModelData::getPayload() {
	return (int*)payload;
}

void ModelData::dumpPayload() {
	deallocatePayload();
}

void ModelData::allocatePayload(size_t new_payload_size) {
	payload_size = new_payload_size;
	payload = realloc(payload, payload_size);
}

void ModelData::deallocatePayload() {
	free(payload);
	payload = NULL;
}