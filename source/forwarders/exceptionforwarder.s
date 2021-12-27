.global OSPanicForwarder
.type OSPanicForwarder, @function
OSPanicForwarder:
    li 0, 1
    lis 4, inOSPanic@ha
    stw 0, inOSPanic@l (4)
    addi 3, 1, 0x78
    b exceptionMessageHandler

.global exceptionOSReportForwarder
.type OSPanicForwarder, @function
exceptionOSReportForwarder:
    mflr 0
    stw 0, 4 (1)
    stwu 1, -32 (1)
    mr 10, 9
    mr 9, 8
    mr 8, 7
    mr 7, 6
    mr 6, 5
    mr 5, 4
    mr 4, 3
    lis 3, exceptionWorkingText@h
    ori 3, 3, exceptionWorkingText@l
    bl sprintf

    lis 3, exceptionWorkingText@h
    ori 3, 3, exceptionWorkingText@l    
    addi 1, 1, 32
    lwz 0, 4 (1)
    mtlr 0
    b exceptionOSReport
