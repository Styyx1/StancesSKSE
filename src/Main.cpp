#include "Cache.h"
#include "EventSink.h"
#include "formloader.h"
#include "inputmanager.h"
#include "MenuEventManager.h"
#include "Settings.h"
#include "stance-manager.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    switch (message->type)
    {
    case SKSE::MessagingInterface::kInputLoaded:
        STNG::InputEventListener::RegisterInput();
        STNG::InputEventListener::GetSingleton()->SetKeys();
        break;
    case SKSE::MessagingInterface::kDataLoaded:
        STNG::FormLoader::LoadForms();
        MenuEvent::Register();
#ifdef GENERATE_CONFIG_FILE
        Config::Settings::use_cycling.SetValue(true);
        Config::Settings::UpdateSettings(true);
#endif


        break;
    case SKSE::MessagingInterface::kPostLoadGame:
        STNG::StanceManager::GetSingleton()->ApplyDefaultStance();
        break;
    case SKSE::MessagingInterface::kNewGame:
        STNG::StanceManager::GetSingleton()->ApplyDefaultStance();
        break;
    default:
        break;
    }

}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logs::info("{} {} is loading...", plugin->GetName(), version);
    SKSE::Init(skse);
    Cache::CacheAddLibAddresses();
    Config::Settings::UpdateSettings(false);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logs::info("{} has finished loading.", plugin->GetName());

    return true;
}
