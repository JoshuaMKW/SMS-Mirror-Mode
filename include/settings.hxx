#pragma once

#include <BetterSMS/settings.hxx>
#include <BetterSMS/memory.hxx>

using namespace BetterSMS;

class MirrorSetting final : public Settings::BoolSetting {
public:
    MirrorSetting() : Settings::BoolSetting("Is Enabled", &mMirrorValue) {
        extern void syncSettings(void *, void *, ValueKind);
        mValueChangedCB = syncSettings;
    }

private:
    bool mMirrorValue;
};

class VirtualMirrorSetting final : public Settings::SwitchSetting {
public:
    VirtualMirrorSetting() : Settings::SwitchSetting("mirror_on", &mMirrorValue) {
        mValueChangedCB = applyMirrorPatches;
    }

    bool isUnlocked() const override { return false; }
    void load(JSUMemoryInputStream &in) override {}
    void save(JSUMemoryOutputStream &out) override {}

private:
    static void applyMirrorPatches(void *old, void *cur, ValueKind kind) {
        if (*reinterpret_cast<bool *>(cur)) {
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x800DF008, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x800DF04C, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8010FFF8, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x80110064, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x80199324, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801A01F8, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801AD534, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801AE6B0, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801D9340, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801DA778, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801AE6B0, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801F3814, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801F4334, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022D294, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022D700, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F294, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F314, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F368, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F710, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F740, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F7C0, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F8F0, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x80244288, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027CA80, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027CAA8, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D5A4, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D5CC, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D660, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D688, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D72C, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D824, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D8F8, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027DC08, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027DE80, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027DFBC, 0, 0, 0)), 0x38600002);
        } else {
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x800DF008, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x800DF04C, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8010FFF8, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x80110064, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x80199324, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801A01F8, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801AD534, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801AE6B0, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801D9340, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801DA778, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801AE6B0, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801F3814, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x801F4334, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022D294, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022D700, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F294, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F314, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F368, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F710, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F740, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F7C0, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8022F8F0, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x80244288, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027CA80, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027CAA8, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D5A4, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D5CC, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D660, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D688, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D72C, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D824, 0, 0, 0)), 0x38600001);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027D8F8, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027DC08, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027DE80, 0, 0, 0)), 0x38600002);
            PowerPC::writeU32(reinterpret_cast<u32 *>(SMS_PORT_REGION(0x8027DFBC, 0, 0, 0)), 0x38600001);
        }
    }

    bool mMirrorValue;
};

bool isMirrorModeActive();