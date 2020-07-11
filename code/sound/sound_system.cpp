#include "sound_system.h"


SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::Init()
{

}
void SoundSystem::Uninit()
{

}
void SoundSystem::Update()
{

}

SoundSystem* SoundSystem::Create()
{
    SoundSystem* soundsystem = new SoundSystem;
    return soundsystem;
}