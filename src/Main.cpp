#include "Cache.h"
#include "EventManager.h"
#include "EventSink.h"
#include "Logging.h"
#include "SKSE/Interfaces.h"
#include "Settings.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type <=> SKSE::MessagingInterface::kDataLoaded == 0) {
        auto settings = Settings::GetSingleton();
        settings->LoadSettings();
        settings->LoadForms();
        // MenuEvent::Register();
        InputEventSink::Register();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitializeLogging();

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} is loading...", plugin->GetName(), version);
    SKSE::Init(skse);
    Cache::CacheAddLibAddresses();
    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", plugin->GetName());

    return true;
}
