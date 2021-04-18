#pragma once

#include <types.h>
#include <spm/seqdrv.h>

namespace spm::seq_logo {

extern "C" {

void seq_logoMain(spm::seqdrv::SeqWork * wp);

}

}
