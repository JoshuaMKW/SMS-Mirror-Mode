#include <Dolphin/GX.h>
#include <Dolphin/GX_types.h>
#include <Dolphin/MTX.h>

#include <SMS/Camera/PolarSubCamera.hxx>
#include <SMS/Player/Mario.hxx>
#include <SMS/Player/Yoshi.hxx>
#include <SMS/GC2D/SelectShine.hxx>
#include <SMS/raw_fn.hxx>
#include <SMS/macros.h>

#include <BetterSMS/module.hxx>
#include <BetterSMS/memory.hxx>

#include "settings.hxx"

// ------ MIRROR SUNSHINE ------ //
//    Created by JoshuaMK 2022   //
// ----------------------------- //

static bool sIsMirrorModeActive = false;

BETTER_SMS_FOR_EXPORT void SetMirrorModeActive(bool active) {
    sIsMirrorModeActive = active;
    
    // clang-format off
    if (active) {
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
    //clang-format on
}
BETTER_SMS_FOR_EXPORT bool GetMirrorModeActive() { return sIsMirrorModeActive; }

void applyPatches(void *prev, void *cur, Settings::SingleSetting::ValueKind kind) {
    SetMirrorModeActive(*reinterpret_cast<bool *>(cur));
}

#if 1
static float _xyz[3] = {-1.0, 1.0, 1.0};
#define INV_SCALE _xyz[0], _xyz[1], _xyz[2]
#else
#define INV_SCALE -1.0, 1.0, 1.0
#endif

static void mirrorMode(Mtx mtx, u32 perspective) {
    if (GetMirrorModeActive())
        PSMTXScaleApply(mtx, mtx, INV_SCALE);
    GXSetProjection(mtx, perspective);
}
SMS_PATCH_BL(SMS_PORT_REGION(0x800233a4, 0, 0, 0), mirrorMode);

static void mirrorMtxLoadBathwater(Mtx mtx, u32 mtxidx) {
    if (GetMirrorModeActive())
        PSMTXScaleApply(mtx, mtx, INV_SCALE);
    GXLoadPosMtxImm(mtx, mtxidx);
}
SMS_PATCH_BL(SMS_PORT_REGION(0x801AD51C, 0, 0, 0), mirrorMtxLoadBathwater);

Mtx sBathWaterMtx;
static SMS_ASM_FUNC void _selfMtx() {
    SMS_ASM_BLOCK("lis 17, sBathWaterMtx@h      \n\t"
                  "ori 17, 17, sBathWaterMtx@l  \n\t"
                  "blr                          \n\t");
}

static void mirrorCameraMtxBathwater() {
    if (GetMirrorModeActive())
        PSMTXScaleApply(gpCamera->mMatrixTRS, sBathWaterMtx, INV_SCALE);
    else
        PSMTXCopy(gpCamera->mMatrixTRS, sBathWaterMtx);
    _selfMtx();
}
SMS_PATCH_BL(SMS_PORT_REGION(0x801AC96C, 0, 0, 0), mirrorCameraMtxBathwater);

static u32 mirrorCulling(void *factory, u32 mode) {
    u32 cullMode = newCullMode__18J3DMaterialFactoryCFi(factory, mode);

    TMarDirector *director = gpMarDirector;
    if (!director || !GetMirrorModeActive())
        return cullMode;

    if (cullMode == GX_CULL_FRONT) {
        cullMode = GX_CULL_BACK;
    } else if (cullMode == GX_CULL_BACK) {
        cullMode = GX_CULL_FRONT;
    }
    return cullMode;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x802e570c, 0, 0, 0), mirrorCulling);

static u32 mirrorCulling_v21(void *factory, u32 mode) {
    u32 cullMode = newCullMode__22J3DMaterialFactory_v21CFi(factory, mode);

    TMarDirector *director = gpMarDirector;
    if (!director || !GetMirrorModeActive())
        return cullMode;

    if (cullMode == GX_CULL_FRONT) {
        cullMode = GX_CULL_BACK;
    } else if (cullMode == GX_CULL_BACK) {
        cullMode = GX_CULL_FRONT;
    }
    return cullMode;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x8031e7c8, 0, 0, 0), mirrorCulling_v21);

// static SMS_PURE_ASM void mirrorCullingGX(u32 cullmode) {
//   asm volatile("cmpwi 3, 1                \n\t"
//                "beq __n1                  \n\t"
//                "cmpwi 3, 2                \n\t"
//                "bne __n3                  \n\t"
//                "li 3, 1                   \n\t"
//                "b __n3                    \n\t"
//                "__n1:                     \n\t"
//                "li r3, 2                  \n\t"
//                "__n3:                     \n\t"
//                "cmpwi 3, 2                \n\t"
//                "lis 12, 0x8035            \n\t"
//                "ori 12, 12, 0xE214        \n\t"
//                "mtctr 12                  \n\t"
//                "bctr                      \n\t");
// }
// SMS_PATCH_B(SMS_PORT_REGION(0x8035e210, 0, 0, 0), mirrorCullingGX);

static Mtx *getLightPerspectiveForEffectMtx(Mtx *dst, f32 x, f32 y, f32 n, f32 f) {
    C_MTXPerspective(*dst, x, y, n, f);
    if (GetMirrorModeActive())
        PSMTXScaleApply(*dst, *dst, INV_SCALE);
    return dst;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x8022ba9c, 0, 0, 0), getLightPerspectiveForEffectMtx);

static Mtx *invertReflections(Mtx srcA, Mtx srcB, Mtx *dst) {
    if (GetMirrorModeActive())
        PSMTXScaleApply(srcB, srcB, INV_SCALE);
    PSMTXConcat(srcA, srcB, *dst);
    return dst;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x80193470, 0, 0, 0), invertReflections);
SMS_PATCH_BL(SMS_PORT_REGION(0x80193f80, 0, 0, 0), invertReflections);
SMS_PATCH_BL(SMS_PORT_REGION(0x8032f608, 0, 0, 0), invertReflections);
SMS_PATCH_BL(SMS_PORT_REGION(0x8032f7b4, 0, 0, 0), invertReflections);

static Mtx *invertDropletReflections(Mtx *dst, f32 fovy, f32 aspect, f32 scaleS, f32 scaleT,
                                     f32 transS, f32 transT) {
    Mtx invMtx;
    C_MTXLightPerspective(*dst, fovy, aspect, scaleS, scaleT, transS, transT);

    if (GetMirrorModeActive()) {
        PSMTXScale(invMtx, INV_SCALE);
        PSMTXConcat(*dst, invMtx, *dst);
    }

    return dst;
}
SMS_PATCH_BL(SMS_PORT_REGION(0x8027c1f0, 0, 0, 0), invertDropletReflections);

inline s8 floatToS8(float value) {
    return (s8)(((value / 2.0f) + 0.5f) * 255.0f) - 128;
}

static void invertMarioControl(JUTGamePad *controller) {
    controller->update();

    TMarDirector *director = gpMarDirector;
    if (!director || director->mCurState == 0xA || !GetMirrorModeActive())
        return;

    JUTGamePad::CStick &ctrlStick = controller->mControlStick;
    JUTGamePad::CStick &cStick    = controller->mCStick;

    ctrlStick.update(
        255 - floatToS8(ctrlStick.mStickX),
        floatToS8(ctrlStick.mStickY),
        JUTGamePad::Clamped, JUTGamePad::WhichStick_ControlStick);
    cStick.update(
        255 - floatToS8(cStick.mStickX),
        floatToS8(cStick.mStickY),
        JUTGamePad::Clamped, JUTGamePad::WhichStick_CStick);
}
SMS_PATCH_BL(SMS_PORT_REGION(0x802c8e54, 0, 0, 0), invertMarioControl);
