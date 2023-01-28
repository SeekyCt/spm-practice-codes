#include <common.h>
#include <wii/exi.h>

#include "ug.h"

namespace mod {

bool ugProbe(s32 chan)
{
	using namespace wii::exi;

	if (!EXIProbe(chan))
		return false;

	// USB Gecko doesn't use normal IDs; we check that the usual ID command
	// just returns zeros to try and avoid sending random commands to e.g.
	// memory cards.
	u32 id;
	if (!EXIGetID(chan, 0, &id))
		return false;
	if (id != 0)
		return false;

	if (!EXILock(chan, 0, nullptr))
		return false;
	if (!EXISelect(chan, 0, 5))
	{
		EXIUnlock(chan);
		return false;
	}

	u16 cmd = 0x9000;
	if (!EXIImm(chan, &cmd, sizeof(u16), 2, nullptr)
		|| !EXISync(chan))
	{
		EXIDeselect(chan);
		EXIUnlock(chan);
		return false;
	}

	EXIDeselect(chan);
	EXIUnlock(chan);
	return (cmd & 0x0fff) == 0x0470;
}

static s32 ugTransfer(s32 chan, void * data, s32 len, bool write)
{
	using namespace wii::exi;

	// Lock device
	if (!EXILock(chan, 0, nullptr))
	{
		return -1;
	}

	// Set speed
	if (!EXISelect(chan, 0, 5))
	{
		EXIUnlock(chan);
		return -1;
	}

	bool fail = false;

	u8 *p = (u8 *)data;

	int xfer_len = 0;
	for (; xfer_len < len; ++xfer_len)
	{
		u16 cmd;
		if (write)
			cmd = 0xb000 | p[xfer_len] << 4;
		else
			cmd = 0xa000;

		if (!EXIImm(chan, &cmd, sizeof(u16), 2, nullptr) || 
		    !EXISync(chan))
		{
			fail = true;
			break;
		}

		// Exit early if buffers are full
		u16 success_mask = write ? 0x0400 : 0x0800;
		if (!(cmd & success_mask))
		{
			break;
		}

		// Read out response byte if reading
		if (!write)
			p[xfer_len] = cmd & 0xff;
	}

	EXIDeselect(chan);
	EXIUnlock(chan);

	return fail ? -1 : xfer_len;
}

s32 ugSend(s32 chan, const void *data, s32 len)
{
	return ugTransfer(chan, (void *)data, len, true);
}

s32 ugRecv(s32 chan, void * data, s32 len)
{
	return ugTransfer(chan, data, len, false);
}

}
