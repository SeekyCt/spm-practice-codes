.global OSPanicForwarder
.type OSPanicForwarder, @function
OSPanicForwarder:
    li r0, 1
    lis r4, inOSPanic@ha
    stw 0, inOSPanic@l (r4)
    addi r3, r1, 0x78
    b exceptionMessageHandler

.global exceptionOSReportForwarder
.type OSPanicForwarder, @function
exceptionOSReportForwarder:
    mflr r0
    stw r0, 4 (r1)
    stwu r1, -32 (r1)
    mr r10, 9
    mr r9, 8
    mr r8, 7
    mr r7, 6
    mr r6, 5
    mr r5, 4
    mr r4, 3
    lis r3, exceptionWorkingText@h
    ori r3, r3, exceptionWorkingText@l
    bl sprintf

    lis r3, exceptionWorkingText@h
    ori r3, r3, exceptionWorkingText@l    
    addi r1, r1, 32
    lwz r0, 4 (r1)
    mtlr r0
    b exceptionOSReport
