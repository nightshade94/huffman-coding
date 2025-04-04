#pragma once // Ensures this header file is included only once during compilation, preventing redefinition errors.
// Structure to represent a node in the Huffman tree (used here as a Trie).
struct TrieNode
{
    char Character = 0;     // Character stored at this node (only for leaf nodes).  Initialized to 0.
    TrieNode *Left, *Right; // Pointers to the left and right child nodes.

    // Default constructor.
    TrieNode();

    // Constructor for leaf nodes.
    TrieNode(char Character);

    // Constructor for internal nodes (no character).
    TrieNode(TrieNode *left, TrieNode *right);
};

// Recursive function to delete the Trie and free memory.
void Delete(TrieNode *CurrentNode);

// Structure to represent the Huffman tree as a Trie.
struct Trie
{
    TrieNode *Head = nullptr; // Pointer to the root node of the Trie. Initialized to null.
    // Destructor
    ~Trie();
    // Adds a binary code and its corresponding character to the Trie.
    void Add(std::string Binary, char Character);
};

// Checks if a given node is a leaf node (has no children).
bool isLeaf(TrieNode *CurrentNode);

// Class encapsulating the Huffman decoding process.
struct DecodeHuffman
{
    // Initialize variables.
    Trie TrieObject;                 // An instance of the Trie structure to store the Huffman codes for efficient lookup.
    int CharacterCode;               // Temporary variable, likely used when reading character codes from the Huffman code file.
    int FileSize;                    // Stores the total number of *bits* in the encoded data stream (read from the encoded file/data). Crucial for handling padding.
    std::string Binary;              // Temporary string variable used to store the binary code read from the Huffman code file.
    std::string DecodedString;       // String to accumulate the characters as they are decoded from the bitstream.
    char Character;                  // Temporary variable to hold a byte read from the encoded input stream/data.
    TrieNode *CurrentNode = nullptr; // Pointer used to keep track of the current position while traversing the Trie during decoding.
    bool IsFileGood = false;         // Flag: True if necessary input/output files were opened successfully.
    bool IsHuffmanTreeGood = false;  // Flag: True if the Huffman codes were successfully read and the Trie was built.

    // Reads Huffman codes from a specified file ('HuffmanCodeFileName') and builds the internal TrieObject.
    void ReadHuffmanCode(std::string HuffmanCodeFileName);
    // Reads Huffman codes from an in-memory array of strings ('Label') of a given size ('LabelSize') and builds the internal TrieObject.
    void ReadHuffmanCode(std::string *Label, int LabelSize);

    // Reads the encoded bitstream from a specified file ('EncryptedFileName'), decodes it using the TrieObject, and stores the result in 'DecodedString'.
    void DecodeFromFile(std::string EncryptedFileName);
    // Reads the encoded bitstream from an in-memory character array ('EncodedString') given the total number of bits ('FileSize'),
    // decodes it using the TrieObject, and stores the result in 'DecodedString'.
    void DecodeFromString(const char *EncodedString, int ProvidedFileSize);

    // Writes the contents of the 'DecodedString' to a specified output file ('DecodedFileName').
    void WriteDecodedToFile(std::string DecodedFileName);

    // Returns the accumulated decoded string.
    std::string GetDecodedString();
};