

sel = "JR53k2vFWO11bPrXZLcCYEE01fxSQhTy/8 oWaco0bIs="
mdp = "123PetitsChats"
# decode from base 64
import base64
decoded = base64.b64decode(sel)

## apply BPKDF2 with SHA256
from hashlib import pbkdf2_hmac
key = pbkdf2_hmac('sha256', mdp.encode(), decoded, 100000, dklen=16)
for i in range(599000):
    print(i)
    key = pbkdf2_hmac('sha256', key, decoded, 100000, dklen=16)

print("Key :", key.hex())