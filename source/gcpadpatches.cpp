#include "gcpadpatches.h"

#include <spm/mario.h>
#include <spm/wpadmgr.h>
#include <wii/wpad.h>
#include <types.h>

void marioUpdateKeyDataPatch() {
  u16 WVar1;
  spm::mario::MarioWork *marioWork;
  u32 held;
  u32 pressed;
  u32 released;
  u32 rpt;
  u16 WVar4;
  s32 iVar3;
  u32 uVar5;
  u32 uVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;

  marioWork = spm::mario::marioGetPtr();
  held = spm::wpadmgr::wpadGetButtonsHeld(0);
  pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
  released = spm::wpadmgr::wpadGetButtonsReleased(0);
  rpt = spm::wpadmgr::wpadGetButtonsHeldRepeat(0);
  WVar4 = spm::wpadmgr::FUN_802376e4(0);
  iVar3 = spm::mario::FUN_8012b4f8();
  if ((iVar3 == 0) && ((marioWork->statusFlags & STATUS_FLIPPED_CONTROLS) == 0)) {
    if ((held & (WPAD_BTN_DOWN|WPAD_BTN_UP)) == WPAD_BTN_UP) {
      marioWork->pseudoJoystickX = 0x81;
    }
    else if ((held & (WPAD_BTN_DOWN|WPAD_BTN_UP)) == WPAD_BTN_DOWN) {
      marioWork->pseudoJoystickX = 0x7f;
    }
    else {
      marioWork->pseudoJoystickX = 0;
    }
    if ((held & (WPAD_BTN_LEFT|WPAD_BTN_RIGHT)) == WPAD_BTN_RIGHT) {
      marioWork->pseudoJoystickY = 0x81;
    }
    else if ((held & (WPAD_BTN_LEFT|WPAD_BTN_RIGHT)) == WPAD_BTN_LEFT) {
      marioWork->pseudoJoystickY = 0x7f;
    }
    else {
      marioWork->pseudoJoystickY = 0;
    }
    marioWork->certainButtonsPressedMask =
         pressed & (WPAD_BTN_LEFT|WPAD_BTN_RIGHT|WPAD_BTN_DOWN|WPAD_BTN_UP);
    marioWork->certainButtonsHeldMask =
         held & (WPAD_BTN_LEFT|WPAD_BTN_RIGHT|WPAD_BTN_DOWN|WPAD_BTN_UP);
  }
  else {
    if ((held & (WPAD_BTN_DOWN|WPAD_BTN_UP)) == WPAD_BTN_UP) {
      marioWork->pseudoJoystickX = 0x7f;
    }
    else if ((held & (WPAD_BTN_DOWN|WPAD_BTN_UP)) == WPAD_BTN_DOWN) {
      marioWork->pseudoJoystickX = 0x81;
    }
    else {
      marioWork->pseudoJoystickX = 0;
    }
    if ((held & (WPAD_BTN_LEFT|WPAD_BTN_RIGHT)) == WPAD_BTN_RIGHT) {
      marioWork->pseudoJoystickY = 0x7f;
    }
    else if ((held & (WPAD_BTN_LEFT|WPAD_BTN_RIGHT)) == WPAD_BTN_LEFT) {
      marioWork->pseudoJoystickY = 0x81;
    }
    else {
      marioWork->pseudoJoystickY = 0;
    }
    marioWork->certainButtonsPressedMask = 0;
    if ((pressed & WPAD_BTN_UP) != 0) {
      marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_DOWN;
    }
    if ((pressed & WPAD_BTN_DOWN) != 0) {
      marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_UP;
    }
    if ((pressed & WPAD_BTN_LEFT) != 0) {
      marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_RIGHT;
    }
    if ((pressed & WPAD_BTN_RIGHT) != 0) {
      marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_LEFT;
    }
    marioWork->certainButtonsHeldMask = 0;
    if ((held & WPAD_BTN_UP) != 0) {
      marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_DOWN;
    }
    if ((held & WPAD_BTN_DOWN) != 0) {
      marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_UP;
    }
    if ((held & WPAD_BTN_LEFT) != 0) {
      marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_RIGHT;
    }
    if ((held & WPAD_BTN_RIGHT) != 0) {
      marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_LEFT;
    }
  }
  marioWork->unknown_0x318 = 0;
  marioWork->unknown_0x319 = 0;
  marioWork->unknown_0x31a = 0;
  marioWork->unknown_0x31b = 0;
  marioWork->unknown_0x31c = 0;
  marioWork->unknown_0x31d = 0;
  iVar3 = -1;
  if (marioWork->buttonMaskFilter != 0) {
    iVar3 = marioWork->buttonMaskFilter;
  }
  WVar1 = iVar3;
  marioWork->buttonsHeld = held & WVar1;
  marioWork->buttonsPressed = pressed & WVar1;
  marioWork->buttonsHeldRepeat = rpt & WVar1;
  marioWork->buttonsReleased = released & WVar1;
  marioWork->unknown_0x314 = WVar4 & WVar1;
  if ((marioWork->flags & 10) == 0) {
    if ((marioWork->trigFlags & 0x8000000) != 0) {
      marioWork->pseudoJoystickX = 0;
      marioWork->pseudoJoystickY = 0;
      marioWork->certainButtonsPressedMask =
           marioWork->certainButtonsPressedMask &
           ~(WPAD_BTN_LEFT|WPAD_BTN_RIGHT|WPAD_BTN_DOWN|WPAD_BTN_UP);
      marioWork->certainButtonsHeldMask =
           marioWork->certainButtonsHeldMask &
           ~(WPAD_BTN_LEFT|WPAD_BTN_RIGHT|WPAD_BTN_DOWN|WPAD_BTN_UP);
    }
    if ((marioWork->trigFlags & 0x4000000) != 0) {
      marioWork->unknown_0x318 = 0;
      marioWork->unknown_0x319 = 0;
      marioWork->certainButtonsPressedMask =
           marioWork->certainButtonsPressedMask & ~(WPAD_BTN_2|WPAD_BTN_1|WPAD_BTN_B|WPAD_BTN_A);
      marioWork->certainButtonsHeldMask =
           marioWork->certainButtonsHeldMask & ~(WPAD_BTN_2|WPAD_BTN_1|WPAD_BTN_B|WPAD_BTN_A);
    }
    if ((marioWork->trigFlags & 0x2000000) != 0) {
      marioWork->unknown_0x31a = 0;
      marioWork->unknown_0x31b = 0;
    }
    if ((marioWork->trigFlags & 0x1000000) != 0) {
      marioWork->unknown_0x31c = 0;
      marioWork->unknown_0x31d = 0;
    }
  }
  if (((marioWork->flags & 10) != 0) || spm::mario::marioChkKey() == 0) {
    marioWork->pseudoJoystickX = 0;
    marioWork->pseudoJoystickY = 0;
    marioWork->unknown_0x318 = 0;
    marioWork->unknown_0x319 = 0;
    marioWork->unknown_0x31a = 0;
    marioWork->unknown_0x31b = 0;
    marioWork->unknown_0x31c = 0;
    marioWork->unknown_0x31d = 0;
    marioWork->certainButtonsPressedMask = 0;
    marioWork->certainButtonsHeldMask = 0;
    if (marioWork->buttonMaskFilter == 0) {
      marioWork->buttonsHeld = 0;
      marioWork->buttonsPressed = 0;
      marioWork->buttonsHeldRepeat = 0;
      marioWork->buttonsReleased = 0;
      marioWork->unknown_0x314 = 0;
      spm::wpadmgr::FUN_80237710(0);
    }
  }
  if ((marioWork->pseudoJoystickX == 0) && (marioWork->pseudoJoystickY == 0)) {
    marioWork->joystickMagnitude = 0.0;
  }
  else {
    uVar5 = marioWork->pseudoJoystickX;
    uVar6 = marioWork->pseudoJoystickY;
    dVar8 = ((int)uVar5 >> 0x1f ^ uVar5) - ((int)uVar5 >> 0x1f);
    dVar9 = ((int)uVar6 >> 0x1f ^ uVar6) - ((int)uVar6 >> 0x1f);
    dVar7 = (double)sqrt(uVar5 * uVar5 + uVar6 * uVar6);
    dVar10 = (double)(float)dVar7;
    dVar7 = (double)sqrt((double)(float)(dVar8 * dVar8 + (double)(float)(dVar9 * dVar9)));
    marioWork->joystickMagnitude = (float)(dVar10 / (double)(float)dVar7);
    marioWork->joystickMagnitude = ((float)(dVar10 / (double)(float)dVar7) * 100.0 + 0.5) / 100.0;
  }
  return;
}
