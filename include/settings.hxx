#pragma once

#include <BetterSMS/settings.hxx>

using namespace BetterSMS;

class MirrorSetting final : public Settings::BoolSetting {
public:
    MirrorSetting() : Settings::BoolSetting("Is Enabled", &mMirrorValue) {
        extern void applyPatches(void *, void *, ValueKind);
        mValueChangedCB = applyPatches;
    }

private:
    bool mMirrorValue;
};