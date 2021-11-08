#include "ResourceImporter.h"
#include "Engine.h"

using namespace std;

RESOURCE_IMPORT_SYSTEM::ResourceImporter::ResourceImporter(string exception)
{
    RESOURCE_IMPORT_SYSTEM::ResourceImportSystem::get_resource_import_system()
        ->resource_importer.insert(std::pair(exception, this));
}

unique_ptr<RESOURCE_IMPORT_SYSTEM::ResourceImportSystem> resource_import_system;
RESOURCE_IMPORT_SYSTEM::ResourceImportSystem* RESOURCE_IMPORT_SYSTEM::ResourceImportSystem::get_resource_import_system()
{
    if (resource_import_system.get() == nullptr)
    {
        resource_import_system = make_unique<RESOURCE_IMPORT_SYSTEM::ResourceImportSystem>();
    }
    return resource_import_system.get();
}

void RESOURCE_IMPORT_SYSTEM::ResourceImportSystem::import_resource(Engine* engine, string path, string exception)
{
    for (auto importer_iter = resource_importer.lower_bound(exception);
        importer_iter != resource_importer.upper_bound(exception);
        ++importer_iter)
    {
        importer_iter->second->import_file(engine, path);
    }
}
