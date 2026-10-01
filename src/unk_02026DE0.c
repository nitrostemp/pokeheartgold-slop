#include "unk_02026DE0.h"

#include "global.h"

BillboardTexPlttIndex sub_02026DE0(const BillboardGfxSequence *gfxSequence, const u16 index) {
    BillboardTexPlttIndex ret;
    u32 i;

    for (i = 0; i < gfxSequence->seqCount - 1; i++) {
        if (gfxSequence->startFrame[i + 1] > index) {
            break;
        }
    }
    ret.textureIdx = gfxSequence->textureIdx[i];
    ret.plttIdx = gfxSequence->plttIdx[i];
    return ret;
}

void sub_02026E18(const u32 *data, BillboardGfxSequence *gfxSequence) {
    gfxSequence->seqCount = *data;
    gfxSequence->startFrame = (const u16 *)(data + 1);
    gfxSequence->textureIdx = (const u8 *)(gfxSequence->startFrame + gfxSequence->seqCount);
    gfxSequence->plttIdx = gfxSequence->textureIdx + gfxSequence->seqCount;
}
