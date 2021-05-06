#pragma once

#if defined SPM_EU0 || defined SPM_EU1
    #define REL_LOAD_ADDR 0x80c45820
#elif defined SPM_US0
    static_assert(0, "codesections.h not ported for region");
#elif defined SPM_US1
    static_assert(0, "codesections.h not ported for region");
#elif defined SPM_US2
    static_assert(0, "codesections.h not ported for region");
#elif defined SPM_JP0
    static_assert(0, "codesections.h not ported for region");
#elif defined SPM_JP1
    static_assert(0, "codesections.h not ported for region");
#elif defined SPM_KR0
    static_assert(0, "codesections.h not ported for region");
#endif
