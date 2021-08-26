import dolphin_memory_engine as dme
from time import sleep
from enum import Enum

class ConsoleParams:
    # Must be synced with C++
    FIELDS = {
        "bufferPtr" : (0x0, 0x4),
        "bufferSize" : (0x4, 0x4)
    }

    paramPtr: int
    bufferPtr: int
    
    def _getField(self, name) -> int:
        offset, size = ConsoleParams.FIELDS[name]
        dat = dme.read_bytes(self.paramPtr + offset, size)
        return int.from_bytes(dat, 'big')

    def __init__(self, paramPtr):
        self.paramPtr = paramPtr
        self.bufferPtr = self._getField("bufferPtr")
        self.bufferSize = self._getField("bufferSize")

class ConsoleError(Enum):
    NONE = 0
    TOO_LONG = 1

class Console:
    params: ConsoleParams

    def __init__(self, paramPtr):
        self.params = ConsoleParams(paramPtr)
    
    def _freeCheck(self) -> bool:
        return dme.read_byte(self.params.bufferPtr) == 0

    def push(self, text) -> None:
        dat = text.encode('ascii') + b'\x00'
        if len(dat) > self.params.bufferSize:
            return ConsoleError.TOO_LONG

        if not self._freeCheck():
            print("Last push not processed yet, sleeping...")
            while not self._freeCheck():
                sleep(0.01) # 10ms
            
        dme.write_bytes(self.params.bufferPtr, dat)

def validate_mem1_addr(addr: int) -> bool:
    return 0x80000000 <= addr <= 0x817fffff

# Must be synced with C++
PARAM_PTR_ADDR = 0x800001a0

dme.hook()
if not dme.is_hooked():
    print("DME not ready yet, sleeping")
    while not dme.is_hooked():
        sleep(0.01) # 10ms
        dme.hook()

paramPtr = dme.read_word(PARAM_PTR_ADDR)
if not validate_mem1_addr(paramPtr):
    print("Client not ready yet, sleeping")
    while not validate_mem1_addr(paramPtr):
        sleep(0.01) # 10ms
        paramPtr = dme.read_word(PARAM_PTR_ADDR)

console = Console(paramPtr)
while True:
    msg = input("Console > ").strip()
    if msg == "exit":
        break
    else:
        console.push(msg)

