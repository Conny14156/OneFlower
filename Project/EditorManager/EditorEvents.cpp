#ifdef _EDITOR_
#ifndef _UNITTESTS_
#include <vector>

#include <Core\IEngineResource\EngineResourceManager.hpp>
#include <EditorManager\WorldEditorExtensions\WorldManagerAddons.hpp>
#include <AssetManager\AssetManagerCore.hpp>

#include <Core\GameVariableMapping.hpp>
#include <Core\Globals.hpp>

#include "EditorEvents.hpp"
#include "InteropHelper.hpp"

Core::String Editor::toString(System::String ^ str)
{
	return toString(str->ToCharArray());
}

Core::String Editor::toString(array<wchar_t>^ arr)
{
	std::wstring w;
	
	for each(wchar_t wc in arr)
		w.push_back(wc);
	
	return Engine::Get<Core::StringConverter>().toUtf8(w);
}

System::String^ Editor::toString(const Core::String & str)
{
	return gcnew System::String(Engine::Get<Core::StringConverter>().toUtf16(str).c_str());
}

EditorResources::Utils::ModDependencyList ^ Editor::Events::loadDependenciesInternal(Core::String mod)
{
	EditorResources::Utils::ModDependencyList^ depList = gcnew EditorResources::Utils::ModDependencyList();
	depList->Mod = gcnew System::String(mod.c_str());
	std::vector<Core::String> dep = Engine::Get<WorldManagerAddon>().getModDependencies(mod);
	
	for each (Core::String str in dep)
		depList->dependencies->Add(loadDependenciesInternal(str));
	
	return depList;
}

void Editor::Events::registerEvents()
{
	EditorEvents::onModCreate += gcnew EventHandler<EditorResources::Functionality::NewModCreateEventArgs^>(this, &Editor::Events::OnEditorCreateMod);
	EditorEvents::onModLoad += gcnew EventHandler<EditorResources::Functionality::ModLoadEventArgs^>(this, &Editor::Events::OnEditorLoadMod);
	EditorEvents::onModSelected += gcnew EventHandler<EditorResources::Functionality::ModFileSelectedEventArgs^>(this, &Editor::Events::OnEditorModFileSelected);
	EditorEvents::onZoneSelectedEvent += gcnew EventHandler<EditorResources::Functionality::EditorZoneSelectedEventArgs^>(this, &Editor::Events::OnEditorZoneSelected);
	EditorEvents::onModSave += gcnew EventHandler<EditorResources::Functionality::ModSaveEventArgs^>(this, &Editor::Events::OnEditorSave);
	EditorEvents::onOVariableCreatedEvent += gcnew EventHandler<EditorResources::Functionality::OnVariableCreatedEventArgs^>(this, &Editor::Events::OnGlobalVaraibleCreated);
	RequestEvents::onRequestGameVariableMapping += gcnew EventHandler<RequestEvents::GameVariableMappingEventArgs^>(this, &Editor::Events::OnVariableMappingRecieved);
}

void Editor::Events::OnEditorCreateMod(Object ^ sender, EditorResources::Functionality::NewModCreateEventArgs ^ args)
{
	Core::String s = toString(args->modName);
	std::vector<Core::String> dependencies;
	
	for each (System::String^ str in args->dependencies)
		dependencies.push_back(toString(str));

	Engine::Get<WorldManagerAddon>().newMod(s, dependencies, (bool)args->createMaster);
}

void Editor::Events::OnEditorLoadMod(Object ^ sender, EditorResources::Functionality::ModLoadEventArgs ^ args)
{
	Core::String s = toString(args->modName);
	WorldManagerAddon& addon = Engine::Get<WorldManagerAddon>();
	addon.loadMod(s);

	auto modfinishedloaded = gcnew EditorResources::Functionality::ModFinishedLoadedEventArgs();

	modfinishedloaded->ModName = args->modName;

	for each (auto v in addon.getAllDbZones())
	{
		auto zinfo = gcnew System::Tuple<System::String^, size_t, System::String^>(toString(v.fromMod), v.ID, toString(v.name));
		modfinishedloaded->zoneFiles->Add(zinfo);
	}

	EditorEvents::onModFinishedLoading(nullptr, modfinishedloaded);
}

void Editor::Events::OnEditorModFileSelected(Object^ sender, EditorResources::Functionality::ModFileSelectedEventArgs^ args)
{
	Core::String mod = toString(args->fileName);
	EditorResources::Utils::ModDependencyList^ depList = loadDependenciesInternal(mod);
	EditorResources::Functionality::EngineOnModSelectedLoadedEventArgs ^evt = gcnew EditorResources::Functionality::EngineOnModSelectedLoadedEventArgs();
	evt->Dependencies = depList;
	EditorEvents::EngineOnModSelectedLoaded(evt);
}

void Editor::Events::OnEditorZoneSelected(Object^ sender, EditorResources::Functionality::EditorZoneSelectedEventArgs^ args)
{

	WorldManagerAddon& addon = Engine::Get<WorldManagerAddon>();

	addon.EditorLoadZone(toString(args->ModOrigin), args->ZoneID);
	EditorResources::Functionality::EditorGetZoneInfoEvent ^evt = gcnew EditorResources::Functionality::EditorGetZoneInfoEvent();
	evt->Origin = args->ModOrigin;
	evt->Id = args->ZoneID;
	evt->ZoneName = args->ZoneName;

	std::vector<Core::String> v = addon.getExtraZoneInfo(toString(args->ModOrigin), args->ZoneID);
	
	evt->BackgroundPath = toString(v[0]);
	evt->LoadingScreenPath = toString(v[1]);
	evt->LoadingScreenMessage = toString(v[2]);
	
	EditorEvents::onGetZoneInfoEvent(nullptr, evt);
	while (addon.myWorldManager.getIsLoading())
	{
		// change this to some kind of indicator in the editor that we are loading, even though the editor game view shows it.
		_sleep(20);
		//;Wait
	}

}

void Editor::Events::OnEditorSave(Object^ sender, EditorResources::Functionality::ModSaveEventArgs^ args)
{
	Engine::Get<WorldManagerAddon>().EditorSave();
}

void Editor::Events::OnGlobalVaraibleCreated(Object ^ sender, EditorResources::Functionality::OnVariableCreatedEventArgs ^ args)
{
	AssetManager& am = Engine::Get<AssetManager>();

	if (args->VariableType == EditorResources::Resources::NameValidator::ValidationType::Int)
	{
		PrimitiveSaveable<int> var(System::Convert::ToInt32(args->Value), toString(args->Name));
		var.fromMod = toString(args->Origin);
		var.ID = args->ID;
		am.getIntRequestor().add(var);
		Engine::Get<Globals>().longGlobals.insert({ toString(args->Name), System::Convert::ToInt32(args->Value) });
	}
	else if (args->VariableType == EditorResources::Resources::NameValidator::ValidationType::Double)
	{
		PrimitiveSaveable<double> var(System::Convert::ToDouble(args->Value), toString(args->Name));
		var.fromMod = toString(args->Origin);
		var.ID = args->ID;
		am.getDoubleRequestor().add(var);
		Engine::Get<Globals>().doubleGlobals.insert({ toString(args->Name), System::Convert::ToDouble(args->Value) });
	}
	else if (args->VariableType == EditorResources::Resources::NameValidator::ValidationType::String)
	{
		PrimitiveSaveable<Core::String> var(toString(args->Value->ToCharArray()), toString(args->Name));
		var.fromMod = toString(args->Origin);
		var.ID = args->ID;
		am.getStringRequestor().add(var);
		Engine::Get<Globals>().stringGlobals.insert({ toString(args->Name), toString(args->Value) });
	}
}
void Editor::Events::OnVariableMappingRecieved(Object ^ sender, RequestEvents::GameVariableMappingEventArgs ^ args)
{
	if (args->VariableToRequest == RequestEvents::GameVariableMappingEventArgs::Variable::Animation)
	{
		EngineEvents::AnimationVariableMappingEventArgs^ arg = gcnew EngineEvents::AnimationVariableMappingEventArgs();
		arg->AnimationVariable = toString(Engine::Get<GameVariableMapping>().getAnimationVariableName());
		EngineEvents::EngineOnSendVariableMapping(arg);
	}
}

void Editor::onObjectLoaded(const Core::String Origin, const size_t ID, const Core::String Name, const ObjectSaveMode mode, const DatabaseIndex::ObjectTypeEnum type, const Core::String value)
{
	EditorResources::Functionality::OnObjectLoadEventArgs^args = gcnew EditorResources::Functionality::OnObjectLoadEventArgs();

	args->Origin = toString(Origin);
	args->ID = ID;
	args->Name = toString(Name);
	switch (mode)
	{
	case ObjectSaveMode::ADD:
		args->Flag = EditorResources::Enums::EnumCollection::ObjectFlag::Added;
		break;
	case ObjectSaveMode::EDIT:
		args->Flag = EditorResources::Enums::EnumCollection::ObjectFlag::Edited;
		break;
	case ObjectSaveMode::REMOVE:
		args->Flag = EditorResources::Enums::EnumCollection::ObjectFlag::Deleted;
		break;
	default:
		args->Flag = EditorResources::Enums::EnumCollection::ObjectFlag::Default;
	}

	switch (type)
	{
	case DatabaseIndex::ObjectTypeEnum::Zone:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Zone;
		break;
	case DatabaseIndex::ObjectTypeEnum::DBZone:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Zone;
		break;
	case DatabaseIndex::ObjectTypeEnum::Prefab:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::GameObject;
		break;
	case DatabaseIndex::ObjectTypeEnum::GameObject:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::GameObject;
		break;
	case DatabaseIndex::ObjectTypeEnum::Quest:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Quest;
		break;
	case DatabaseIndex::ObjectTypeEnum::Item:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Item;
		break;
	case DatabaseIndex::ObjectTypeEnum::Model:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Model;
		break;
	case DatabaseIndex::ObjectTypeEnum::Undefined:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Unknown;
		break;
	case DatabaseIndex::ObjectTypeEnum::TextureMap:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::TextureMap;
		break;
	case DatabaseIndex::ObjectTypeEnum::PrimitiveInt:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::IntVariable;
		break;
	case DatabaseIndex::ObjectTypeEnum::PrimitiveDouble:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::DoubleVariable;
		break;
	case DatabaseIndex::ObjectTypeEnum::PrimitiveString:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::StringVariable;
		break;
	case DatabaseIndex::ObjectTypeEnum::StringList:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::ListVariable;
		break;
	default:
		args->Type = EditorResources::Enums::EnumCollection::ObjectType::Unknown;
		break;
	}

	args->Value = toString(value);
	EditorEvents::OnObjectLoadEvent(args);
}
#endif
#endif
