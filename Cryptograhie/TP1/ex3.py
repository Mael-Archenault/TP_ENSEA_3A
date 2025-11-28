
ciphered_text = ""
# etter_frequencies = {}
with open("Exo3.txt") as f:
    ciphered_text+=f.read()

# with open("Ressource_Exo3.txt") as f:
    
#     for line in f:
#         parts = line.split(" ")
#         print(line)
#         print(parts[1][:-2])
#         letter_frequencies[parts[0]] = float(parts[1][:-2])/100

# print(letter_frequencies)
def get_coincidence_indice(text):
    occurences = {}
    for character in text:
        if (character in occurences):
            occurences[character] += 1
        else:
            occurences[character] = 1

    n_characters = sum(occurences.values())

    res = 0
    for character in occurences:
        res += occurences[character] * (occurences[character] - 1)
    
    return res/(n_characters*(n_characters-1))

def get_subsequence(text, k, offset):
    res = ""
    for i in range(offset, len(text), k):
        res+= text[i]
    return res

def get_key_size(text):
    values = {}
    for k in range(1, 8):
        subsequence = get_subsequence(text, k, 0)
        values[str(k)] = get_coincidence_indice(subsequence)

    target = 0.0778
    distances = {}
    for k in values:
        distances[k] = abs(values[k] - target)
    
    min_value = min(distances.values())
    for k in distances:
        if distances[k] == min_value:
            return k, values[k]

def get_frequential_analyzis(text, key_length):
    key = ""

    for i in range(key_length):
        subsequence = get_subsequence(text, key_length, i)
        temp = {}
        for character in subsequence:
            if character in temp:
                temp[character] += 1
            else:
                temp[character] = 1

        max_value = max(temp.values())
        max_char = ''
        for character in temp:
            if temp[character] == max_value:
                max_char = character
                break
        
        
        # soustraction circulaire modulo 26
        key += shift_back_4(max_char)

    return key



def shift_back_4(ch):
    # ensure it's uppercase A–Z
    if 'A' <= ch <= 'Z':
        # shift position
        new_pos = (ord(ch) - ord('A') - 4) % 26
        return chr(ord('A') + new_pos)
    else:
        return ch  # return unchanged if not a letter
    
def letter_sub(letter1, letter2 ):
    
    
        
    new_pos = (ord(letter1) - ord(letter2)) % 26
    return chr(ord('A') + new_pos)
    
#print(shift_back_4("E"))   # Output: A

def decipher(text, key):
    res = ""
    for i in range(len(text)):
        res+=letter_sub(text[i], key[i%len(key)])

    return res





print(get_key_size(ciphered_text))
print(get_frequential_analyzis(ciphered_text, 7))
# print(decipher(ciphered_text, "ENSEAIS"))