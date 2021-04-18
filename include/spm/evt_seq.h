#pragma once

#include <spm/evtmgr.h>

namespace spm::evt_seq {

extern "C" {

// evt_seq_set_seq(int seq, void * p0, void * p1)
EVT_DECLARE_USER_FUNC(evt_seq_set_seq, 3)

// evt_seq_wait(int seq)
EVT_DECLARE_USER_FUNC(evt_seq_wait, 1)

}

}
