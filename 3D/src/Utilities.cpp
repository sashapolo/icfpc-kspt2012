#include "stdinclude.h"
#include "Utilities.h"

Field* createField(std::string mapFileName) {
	ifstream file;
	char* file_buf;
	int file_size;

	file.open(mapFileName.c_str(), ifstream::in);

	if(!file.is_open()) {
		LOGERROR("Can't load map from \"%s\": can't open file", mapFileName.c_str());
		return NULL;
	};

	file.seekg (0, ios::end);
	file_size = file.tellg();
	if(file_size==0) {
		LOGERROR("Can't load map from \"%s\": file is empty", mapFileName.c_str());
		file.close();
		return NULL;
	}

	file_buf = new char[file_size];
	file.seekg(0, ios::beg);
	file.read(file_buf, file_size);
	file.close();
	Field *result;
	try {
		result = new Field(file_buf);
	} catch (FieldParseException&) {
		LOGERROR("Can't load map from \"%s\": map is incorrect", mapFileName.c_str());
		file.close();
		delete [] file_buf;
		return NULL;
	}
	delete [] file_buf;
	LOGINFO("Map loaded from \"%s\"", mapFileName.c_str());
	return result;
}
