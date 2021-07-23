# Creates pcrel.bin files to be used in extracted save files on dolphin

regions = ["eu0", "eu1", "jp0", "jp1", "kr0", "us0", "us1", "us2"]

for region in regions:
    with open(f"spm-practice-codes.{region}.rel", 'rb') as f:
        dat = f.read()
    
    with open(f"pcrel.{region}.bin", 'wb') as f:
        f.write(int.to_bytes(len(dat), 4, 'big') + bytearray(0x1c) + dat)
