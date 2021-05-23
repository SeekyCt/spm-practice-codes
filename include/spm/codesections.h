#pragma once

#if defined SPM_EU0 || defined SPM_EU1
    #define REL_LOAD_ADDR 0x80c45820
#elif defined SPM_US0
    #define REL_LOAD_ADDR 0x80c16840
#elif defined SPM_US1
    #define REL_LOAD_ADDR 0x80c03dc0
#elif defined SPM_US2
    #define REL_LOAD_ADDR 0x80c03f40
#elif defined SPM_JP0
    #define REL_LOAD_ADDR 0x80c31760
#elif defined SPM_JP1
    #define REL_LOAD_ADDR 0x80c1ea60
#elif defined SPM_KR0
    #define REL_LOAD_ADDR 0x80c5f080
#endif
