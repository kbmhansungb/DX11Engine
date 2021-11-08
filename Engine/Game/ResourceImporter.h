#pragma once

#include <string>
#include <map>
class Engine;

// �����ڸ� �����ϰ� ���̸� ���ʿ��� ���� ���� �� �ִٰ� �Ǵܵ�.

namespace RESOURCE_IMPORT_SYSTEM
{
	class ResourceImporter
	{
	public:
		ResourceImporter(std::string exception);
		virtual void import_file(Engine* engine, std::string path) = 0;
	};
	class ResourceImportSystem
	{
	public:
		//std::multimap<std::string, ResourceImporter*> resource_importer;
		std::map<std::string, ResourceImporter*> resource_importer;
		static ResourceImportSystem* get_resource_import_system();
		void import_resource(Engine* engine, std::string path, std::string exception);
	};
}