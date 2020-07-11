#pragma once
class SoundSystem
{
public:
    SoundSystem();
    ~SoundSystem();

    void Init();
    void Uninit();
    void Update();
    static SoundSystem* Create();
private:

};
