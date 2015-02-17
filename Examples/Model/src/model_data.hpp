
#ifndef MODEL_DATA_HPP
#define MODEL_DATA_HPP

class ModelData {
public:
	ModelData();
	~ModelData();

	// Interface functions
	int getPayload();

	// Model functions
	void setPayload(int new_payload);

private:
	int payload;

	ModelData(const ModelData& that);
};

#endif // MODEL_DATA_HPP
