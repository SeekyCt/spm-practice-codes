#include <common.h>
#include "gcpadpatches.h"
#include "gcpad.h"

#include <spm/mario.h>
#include <spm/system.h>
#include <spm/wpadmgr.h>
#include <wii/wpad.h>
#include <msl/math.h>

namespace mod::gcpadpatches
{
	void marioUpdateKeyDataPatch()
	{
		u16 WVar1;
		spm::mario::MarioWork *marioWork;
		u32 held;
		u32 pressed;
		u32 released;
		u32 rpt;
		u16 WVar4;
		s32 iVar3;
		f32 signedStickX;
		f32 signedStickY;
		double joystickMagnitude;
		
		marioWork = spm::mario::marioGetPtr();
		held = spm::wpadmgr::wpadGetButtonsHeld(0);
		pressed = spm::wpadmgr::wpadGetButtonsPressed(0);
		released = spm::wpadmgr::wpadGetButtonsReleased(0);
		rpt = spm::wpadmgr::wpadGetButtonsHeldRepeat(0);
		WVar4 = spm::wpadmgr::func_802376e4(0);
		iVar3 = spm::mario::func_8012b4f8();
		if ((iVar3 == 0) && ((marioWork->statusFlags & 0x10) == 0))
		{
			marioWork->pseudoJoystickX = mod::pads[0].lStickX;
			marioWork->pseudoJoystickY = -mod::pads[0].lStickY;
			marioWork->certainButtonsPressedMask = 0;
			if ((pressed & WPAD_BTN_UP) != 0)
			{
				marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_DOWN;
			}
			if ((pressed & WPAD_BTN_DOWN) != 0)
			{
				marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_UP;
			}
			if ((pressed & WPAD_BTN_LEFT) != 0)
			{
				marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_RIGHT;
			}
			if ((pressed & WPAD_BTN_RIGHT) != 0)
			{
				marioWork->certainButtonsPressedMask = marioWork->certainButtonsPressedMask | WPAD_BTN_LEFT;
			}
			marioWork->certainButtonsHeldMask = 0;
			if ((held & WPAD_BTN_UP) != 0)
			{
				marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_DOWN;
			}
			if ((held & WPAD_BTN_DOWN) != 0)
			{
				marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_UP;
			}
			if ((held & WPAD_BTN_LEFT) != 0)
			{
				marioWork->certainButtonsHeldMask = marioWork->certainButtonsHeldMask | WPAD_BTN_RIGHT;
			}
			if ((held & WPAD_BTN_RIGHT) != 0)
			{
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
		if (marioWork->buttonMaskFilter != 0)
		{
			iVar3 = marioWork->buttonMaskFilter;
		}
		WVar1 = iVar3;
		marioWork->buttonsHeld = held & WVar1;
		marioWork->buttonsPressed = pressed & WVar1;
		marioWork->buttonsHeldRepeat = rpt & WVar1;
		marioWork->buttonsReleased = released & WVar1;
		marioWork->unknown_0x314 = WVar4 & WVar1;
		if ((marioWork->flags & 10) == 0)
		{
			if ((marioWork->trigFlags & 0x8000000) != 0)
			{
				marioWork->pseudoJoystickX = 0;
				marioWork->pseudoJoystickY = 0;
				marioWork->certainButtonsPressedMask =
					marioWork->certainButtonsPressedMask &
					~(WPAD_BTN_LEFT | WPAD_BTN_RIGHT | WPAD_BTN_DOWN | WPAD_BTN_UP);
				marioWork->certainButtonsHeldMask =
					marioWork->certainButtonsHeldMask &
					~(WPAD_BTN_LEFT | WPAD_BTN_RIGHT | WPAD_BTN_DOWN | WPAD_BTN_UP);
			}
			if ((marioWork->trigFlags & 0x4000000) != 0)
			{
				marioWork->unknown_0x318 = 0;
				marioWork->unknown_0x319 = 0;
				marioWork->certainButtonsPressedMask =
					marioWork->certainButtonsPressedMask & ~(WPAD_BTN_2 | WPAD_BTN_1 | WPAD_BTN_B | WPAD_BTN_A);
				marioWork->certainButtonsHeldMask =
					marioWork->certainButtonsHeldMask & ~(WPAD_BTN_2 | WPAD_BTN_1 | WPAD_BTN_B | WPAD_BTN_A);
			}
			if ((marioWork->trigFlags & 0x2000000) != 0)
			{
				marioWork->unknown_0x31a = 0;
				marioWork->unknown_0x31b = 0;
			}
			if ((marioWork->trigFlags & 0x1000000) != 0)
			{
				marioWork->unknown_0x31c = 0;
				marioWork->unknown_0x31d = 0;
			}
		}
		if (((marioWork->flags & 10) != 0) || spm::mario::marioChkKey() == 0)
		{
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
			if (marioWork->buttonMaskFilter == 0)
			{
				marioWork->buttonsHeld = 0;
				marioWork->buttonsPressed = 0;
				marioWork->buttonsHeldRepeat = 0;
				marioWork->buttonsReleased = 0;
				marioWork->unknown_0x314 = 0;
				spm::wpadmgr::func_80237710(0);
			}
		}
		if ((marioWork->pseudoJoystickX == 0) && (marioWork->pseudoJoystickY == 0))
		{
			marioWork->joystickMagnitude = 0.0;
		}
		else
		{
			signedStickX = (float)(s8)marioWork->pseudoJoystickX;
			signedStickY = (float)(s8)marioWork->pseudoJoystickY;
			joystickMagnitude = msl::math::sqrt(signedStickX * signedStickX + signedStickY * signedStickY);
			marioWork->joystickMagnitude = joystickMagnitude / MAX_MAGNITUDE;
		}
		return;
	}

} // mod::gcpadpatches
