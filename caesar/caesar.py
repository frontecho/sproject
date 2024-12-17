# Define the offset value of the encryption algorithm
OFFSET = 3  
# Define the number of types of letters in the text	
NUMBER_OF_LETTERS = 26  
# Standard letter frequency distribution table
LETTER_FREQUENCY_TABLE = ['e','t','a','o','i','n','s','h','r','d','l','c','u','m','w',
'f','g','y','p','b','v','k','j','x','q','z']

def read_file(path):
    """Read file function
    Args:
        path (str): The path of the file to read.
    
    Returns:
        text (str): The text content of the file.
    """
    with open(path,"r", encoding='UTF-8') as f:
        text = f.read()
        return text


def encrypt(text, offset=OFFSET):
    """Replacement encryption function

    Replace letters with the Caesar method: ASCII + offset

    Args:
        text (str): The text content of the original file.
    
    Returns:
        encrypted_text (str): The encrypted text.
    """
    encrypted_text = ''
    for c in text:
        if c >= 'A' and c <= 'Z':
            encrypted_text += chr(ord('A') + (ord(c)-ord('A') + offset) % NUMBER_OF_LETTERS)
        elif c >= 'a' and c <= 'z':
            encrypted_text += chr(ord('a') + (ord(c)-ord('a') + offset) % NUMBER_OF_LETTERS)
        else:
            encrypted_text += c
    return encrypted_text

def save_to_file(text, path):
    """Save the result to a file

    Args:
        text (str): The text content of the file to save.
        path (str): The path of the file to save.
    
    Returns:
        (None)
    """
    with open(path, "w") as f:
        f.write(text)

def get_letter_count(text):
    """Count letters in the text

    Args:
        text (str): The text to count letters.
    
    Returns:
        letter_count (int): The number of letters in the text.

    """
    letter_count = 0
    for c in text:
        if(c >= 'A' and c <= 'Z' ) or (c >= 'a' and c <= 'z'):
            letter_count += 1
    return letter_count


def get_letter_frequency(text):
    """Letter frequency statistics on text
    
    Args:
        text (str): The text to calculate letter frequency.

    Returns:
        letter_frequency (dict): An unordered dictionary of the letter frequency, 
        i.e., {Key (char): letter, Value (float): frequency of the letter}
    """
    letter_count = get_letter_count(text) # Get the total number of text letters
    letter_frequency = {} # Get the alphabet frequency dictionary

    for c in text:
        if(c >= 'A' and c <= 'Z' ) or (c >= 'a' and c <= 'z'):
            if c.lower() in letter_frequency.keys():
                letter_frequency[c.lower()] += 1
            else:
                letter_frequency[c.lower()] = 1

    for key, value in letter_frequency.items():
        letter_frequency[key] = value / letter_count
        
    return letter_frequency

def get_accuracy(origin_text, encrypted_text, letter_frequency, tol=1):
    """Calculate the decryption accuracy of the letter frequency method

    Args:
        origin_text (str): The text content of the original file, i.e., the ground-truth.
        encrypted_text (str): The encrypted text obtained by the Caesar method.
        letter_frequency (dict): A dictionary of the letter frequency obtained by the `get_letter_frequency()` function.
        tol (int, default=1): A hyperparameter to adjust the error tolerance. For example, if we use the letter frequency 
                            decryption method and get the decrypted letter 'l', but the ground-truth is 'c', we can think 
                            this is correct under the error tolerance 1, because the difference between the position of 'l' 
                            and the position of 'c' in the LETTER_FREQUENCY_TABLE is only 1. 

    Returns:
        accuracy (float): The decryption accuracy of the letter frequency method, i.e., the number of correctly decrypted 
                        letters / the total number of letters.
    """
    letter_frequency = sorted(letter_frequency.items(), key=lambda x:x[1], reverse=True)
    frequency_table = [i[0] for i in letter_frequency]
    letter_count = get_letter_count(origin_text)

    count = 0
    for i, c in enumerate(encrypted_text):
        if(c >= 'A' and c <= 'Z' ) or (c >= 'a' and c <= 'z'):
            idx = frequency_table.index(c.lower())
            candidates = LETTER_FREQUENCY_TABLE[max(idx-tol, 0) : min(idx+tol+1, NUMBER_OF_LETTERS)]
            if(origin_text[i].lower() in candidates):
                count += 1
    
    accuracy = count / letter_count
    return accuracy

def print_letter_frequency(letter_frequency):
    """Print the letter frequency

    Print the letter frequency with the format: "letter : frequency of the letter"

    Args:
        letter_frequency (dict): A dictionary of the letter frequency obtained by the `get_letter_frequency()` function.

    Returns:
        (None)
    """
    letter_frequency = sorted(letter_frequency.items(), key=lambda x:x[1], reverse=True)
    for c, f in letter_frequency:
        print("%c : %f"%(c, f))

if __name__ == '__main__':
    # read, encrypt and save file
    text = read_file('test.txt')
    en_text = encrypt(text)
    save_to_file(en_text, 'en_test.txt')

    # use letter frency method to decrypt text and calculate accuracy
    en_letter_frequency = get_letter_frequency(en_text)
    decrypt_tolerance = 1
    accuracy = get_accuracy(text, en_text, en_letter_frequency, tol = decrypt_tolerance)
    letter_count = get_letter_count(text)

    # print results
    print("The letter frequency of the encrypted text:")
    print_letter_frequency(en_letter_frequency)
    print("The total number of letters in the origin text is %d."%(letter_count))
    print("The decryption accuracy is %f under tolerance %d."%(accuracy, decrypt_tolerance))
