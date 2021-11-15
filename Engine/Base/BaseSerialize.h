#pragma once

#include "Base.h"

// Base 직렬화 참고.

//namespace BinarySave
//{
//	static void response(BinarySavePayload* payload, std::vector<int>& val)					{ payload->write_array(val->val); }
//	static void response(BinarySavePayload* payload, std::vector<unsigned int>& val)		{ payload->write_array(val->val); }
//	static void response(BinarySavePayload* payload, std::vector<float>& val)				{ payload->write_array(val->val); }
//	static void response(BinarySavePayload* payload, std::vector<double>& val)				{ payload->write_array(val->val); }
//	static void response(BinarySavePayload* payload, bool& val)								{ payload->write(val->val); }
//	static void response(BinarySavePayload* payload, int& val)								{ payload->write(val->val); }
//	static void response(BinarySavePayload* payload, unsigned int& val)						{ payload->write(val->val); }
//	static void response(BinarySavePayload* payload, float& val)							{ payload->write(val->val); }
//	static void response(BinarySavePayload* payload, double& val)							{ payload->write(val->val); }
//}
//
//namespace BinaryLoad
//{
//	static void response(BinaryLoadPayload* payload, std::vector<int>& val)					{ payload->read_array(val->val); }
//	static void response(BinaryLoadPayload* payload, std::vector<unsigned int>& val)		{ payload->read_array(val->val); }
//	static void response(BinaryLoadPayload* payload, std::vector<float>& val)				{ payload->read_array(val->val); }
//	static void response(BinaryLoadPayload* payload, std::vector<double>& val)				{ payload->read_array(val->val); }
//	static void response(BinaryLoadPayload* payload, bool& val)								{ payload->read(val->val); }
//	static void response(BinaryLoadPayload* payload, int& val)								{ payload->read(val->val); }
//	static void response(BinaryLoadPayload* payload, unsigned int& val)						{ payload->read(val->val); }
//	static void response(BinaryLoadPayload* payload, float& val)							{ payload->read(val->val); }
//	static void response(BinaryLoadPayload* payload, double& val)							{ payload->read(val->val); }
//}