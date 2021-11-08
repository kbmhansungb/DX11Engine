#pragma once

#include <string>
#include <vector>

#include <fstream>

#define Binary_API
namespace Binary_API BinarySave
{
	class Binary_API BinarySavePayload
	{
		std::fstream f;
	public:
		BinarySavePayload(std::string path)
		{
			f.open(path, std::ios::out | std::ios::binary);
		}
		//~BinarySavePayload()
		//{
		//	if (f.is_open()) f.close();
		//}
	public:
		bool is_open()
		{
			return f.is_open();
		}
		void close()
		{
			f.close();
		}
		template <typename Type>
		void write(Type* val)
		{
			f.write((char*)val, sizeof(Type));
		}

		// 대체 bool array가 왜 안되는 건지 이해가 안가넴... 다른 방식으로 저장하기 때문인가?
		template <typename Type>
		void write_array(std::vector<Type>* arr)
		{
			SIZE_T array_size = arr->size();
			f.write((char*)&array_size, sizeof(SIZE_T));

			f.write((char*)arr->data(), sizeof(Type) * (SIZE_T)array_size); // 어떻게 숨기는지 모르겠네,
		}
		void write_string(std::string& str)
		{
			SIZE_T string_size = str.size();
			f.write((char*)&string_size, sizeof(SIZE_T));

			str.resize(string_size);
			f.write((char*)str.data(), sizeof(char) * string_size);
		}
		template <typename _RHS>
		void with(_RHS* rhs);
	};

	static void response(BinarySavePayload* payload, std::vector<int>* val)					{ payload->write_array(val); }
	static void response(BinarySavePayload* payload, std::vector<unsigned int>* val)		{ payload->write_array(val); }
	static void response(BinarySavePayload* payload, std::vector<float>* val)				{ payload->write_array(val); }
	static void response(BinarySavePayload* payload, std::vector<double>* val)				{ payload->write_array(val); }
	static void response(BinarySavePayload* payload, bool* val)								{ payload->write(val); }
	static void response(BinarySavePayload* payload, int* val)								{ payload->write(val); }
	static void response(BinarySavePayload* payload, unsigned int* val)						{ payload->write(val); }
	static void response(BinarySavePayload* payload, float* val)							{ payload->write(val); }
	static void response(BinarySavePayload* payload, double* val)							{ payload->write(val); }

	template <typename _RHS>
	void Binary_API BinarySavePayload::with(_RHS* rhs)
	{
		BinarySave::response(this, rhs);
	}
};
namespace Binary_API BinaryLoad
{
	class Binary_API BinaryLoadPayload
	{
		std::fstream f;
	public:
		BinaryLoadPayload(std::string path)
		{
			f.open(path, std::ios::in | std::ios::binary);
		}
		//~BinarySavePayload()
		//{
		//	if (f.is_open()) f.close();
		//}
	public:
		bool is_open()
		{
			return f.is_open();
		}
		void close()
		{
			f.close();
		}
		template <typename Type>
		void read(Type* insert)
		{
			f.read((char*)insert, sizeof(Type));
		}
		template <typename Type>
		void read_array(std::vector<Type>* arr)
		{
			SIZE_T array_size;
			f.read((char*)&array_size, sizeof(SIZE_T));

			arr->resize(array_size);
			f.read((char*)arr->data(), sizeof(Type) * array_size); // 어떻게 숨기는지 모르겠네,
		}
		void read_string(std::string& str)
		{
			SIZE_T string_size;
			f.read((char*)&string_size, sizeof(SIZE_T));

			str.resize(string_size);
			f.read((char*)str.data(), sizeof(char) * string_size);
		}
		template <typename _RHS>
		void with(_RHS* rhs);
	};

	//static void sequence(BinarySavePayload* payload, vector<bool>& val)					{ payload->write(val.size(), &val[0]); }
	static void response(BinaryLoadPayload* payload, std::vector<int>* val)					{ payload->read_array(val); }
	static void response(BinaryLoadPayload* payload, std::vector<unsigned int>* val)		{ payload->read_array(val); }
	static void response(BinaryLoadPayload* payload, std::vector<float>* val)				{ payload->read_array(val); }
	static void response(BinaryLoadPayload* payload, std::vector<double>* val)				{ payload->read_array(val); }
	static void response(BinaryLoadPayload* payload, bool* val)								{ payload->read(val); }
	static void response(BinaryLoadPayload* payload, int* val)								{ payload->read(val); }
	static void response(BinaryLoadPayload* payload, unsigned int* val)						{ payload->read(val); }
	static void response(BinaryLoadPayload* payload, float* val)							{ payload->read(val); }
	static void response(BinaryLoadPayload* payload, double* val)							{ payload->read(val); }

	template <typename _RHS>
	void Binary_API BinaryLoadPayload::with(_RHS* rhs)
	{
		BinaryLoad::response(this, rhs);
	}
};