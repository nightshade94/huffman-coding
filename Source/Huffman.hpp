#pragma once       // Ensures this header file is included only once during compilation, preventing redefinition errors.
#include <fstream> // Provides file stream classes (ifstream, ofstream) for file operations.

// Structure to represent a node in the Huffman tree.
struct HuffmanTreeNode
{
    int Freq = 0;                                      // Frequency of the character (for leaf nodes) or the sum of frequencies of child nodes (for internal nodes).
    char Character = 0;                                // The character represented by this node (only for leaf nodes).
    HuffmanTreeNode *Left = nullptr, *Right = nullptr; // Pointers to the left and right child nodes.

    // Default constructor.
    HuffmanTreeNode();

    // Constructor for leaf nodes.
    HuffmanTreeNode(int Freq, char Character);

    // Constructor for internal nodes.
    HuffmanTreeNode(int Freq, HuffmanTreeNode *left, HuffmanTreeNode *right);
};

// Recursively deletes a Huffman tree starting from the given node to prevent memory leaks.
void Delete(HuffmanTreeNode *CurrentNode);

// Structure to represent a priority queue (min-heap) for Huffman tree construction.
struct PriorityQueue
{
    HuffmanTreeNode **NodeArray;                     // Array of Node pointers to store the heap.
    int NumberOfElement, CurrentNumberOfElement = 0; // Number of possible elements, Current Number of Element.

    // Use the compiler-generated default constructor.
    PriorityQueue() = default;
    // Constructor to initialize the priority queue with a given size.
    PriorityQueue(int n);
    // Destructor to properly deallocate memory used by the PriorityQueue.
    ~PriorityQueue();
    // Initializes (or re-initializes) the priority queue.
    void Init(int n);
    // Heapify function (sift-down) to maintain the heap property.
    void sift(int l, int r);
    // Push a new node into the priority queue.
    void Push(HuffmanTreeNode *x);
    // Pop the node with the minimum frequency from the priority queue.
    HuffmanTreeNode *Pop();

    HuffmanTreeNode *Top();
};

// Class encapsulating the entire Huffman encoding process.
// Manages frequency counting, tree building, code generation, and file/string encoding.
struct HuffmanEncoding
{
    PriorityQueue PQ;             // Priority queue for building the Huffman tree.
    int *Freq = nullptr;          // Dynamically allocated array to store character frequencies. Index corresponds to character ASCII value.
    int FileSize = 0;             // Total number of bits in the final encoded output (used for decoding).
    int NumberOfBit = 0;          // Counter for the current number of bits accumulated in the 'Buffer'.
    int MaxCharacterSize;         // Maximum possible character value (e.g., 256 for extended ASCII). Used for array sizing.
    bool IsFileGood = false;      // Flag: True if necessary input/output files were opened successfully.
    bool IsLabeled = false;       // Flag: True if Huffman codes have been generated (Labeling function called).
    bool IsEncoded = false;       // Flag: True if the encoding process (creating the bitstream) has been completed.
    std::string *Label = nullptr; // Dynamically allocated array to store the generated Huffman code (as a string of '0's and '1's) for each character.
    char Character;               // Temporary variable to hold the character being processed.
    char Buffer = 0;              // 8-bit buffer used to accumulate bits before writing a full byte to the output.
    char *Encoded = nullptr;      // Pointer to a dynamically allocated buffer holding the final encoded bitstream (optional, depending on usage).

    // Default constructor
    HuffmanEncoding();
    // Constructor with parameter
    HuffmanEncoding(int MaxCharacterSetSize);
    // Destructor
    ~HuffmanEncoding();

    // Recursive function to build the Huffman codes (labels) for each character.
    void Labeling(std::string *LabelArray, HuffmanTreeNode *CurrentNode, std::string Label);
    // Builds the Huffman tree based on character frequencies.
    void BuildTree();

    // Encodes the content of a file specified by 'FileToEncodeName'. Manages file opening, frequency counting, tree building, code writing, and final encoding.
    void EncodeFile(std::string FileToEncodeName);
    // Encodes a given string and writes the encoded data and Huffman codes to files.
    void EncodeString(std::string StringToEncode);

    // Writes the generated Huffman codes to a file (for later decoding).
    void WriteHuffmanCode(std::string HuffmanCodeFileName);
    // Writes the final encoded bitstream (stored potentially in the 'Encoded' buffer or generated on the fly) to the specified output file.
    void WriteToFile(std::string EncodedFileName);

    // Returns a pointer to the dynamically allocated buffer containing the encoded bitstream. (Use with caution regarding memory management).
    const char *EncodedString();
};