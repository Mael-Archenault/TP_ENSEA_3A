from Crypto.Hash import MD5, SHA1


md5_hash = MD5.new()
sha1_hash = SHA1.new()

words = ["ENSEA", "eNSEA","eNSeA", "EN5EA"]
long_text = "Torem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."

for word in words:

    md5_hash.update(b'' + word.encode())
    sha1_hash.update(b'' + word.encode())

    print("Initial word :", word)
    print("MD5 : ", md5_hash.hexdigest())
    print("SHA-1 : ",sha1_hash.hexdigest(), "\n")

md5_hash.update(b'' + long_text.encode())
sha1_hash.update(b'' + long_text.encode())
print("Initial word :", long_text[:20] + "...")
print("MD5 : ", md5_hash.hexdigest())
print("SHA-1 : ",sha1_hash.hexdigest(), "\n")
