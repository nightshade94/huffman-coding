#include <iostream> // For input/output operations.
#include <fstream>  // For file input/output operations.
#include "DecodeHuffman.hpp"

const int MAX_N = 256; // Maximum number of distinct characters (assuming extended ASCII).

// Default constructor.
TrieNode::TrieNode()
{
    Left = nullptr;  // Initialize left child to null.
    Right = nullptr; // Initialize right child to null.
}

// Constructor for leaf nodes.
TrieNode::TrieNode(char Character)
{
    this->Character = Character;
    Left = nullptr;
    Right = nullptr;
}

// Constructor for internal nodes (no character).
TrieNode::TrieNode(TrieNode *left, TrieNode *right)
{
    Left = left;
    Right = right;
}
// Recursive function to delete the Trie and free memory.
void Delete(TrieNode *CurrentNode)
{
    if (!CurrentNode) // Base case: If the node is null, do nothing.
        return;

    if (CurrentNode->Left) // Recursively delete the left subtree.
    {
        Delete(CurrentNode->Left);
        CurrentNode->Left = nullptr; // Set the left pointer to null after deletion.
    }
    if (CurrentNode->Right) // Recursively delete the right subtree.
    {
        Delete(CurrentNode->Right);
        CurrentNode->Right = nullptr; // Set the right pointer to null after deletion.
    }
    delete CurrentNode; // Delete the current node.
}

// Destructor for the Trie class.
Trie::~Trie()
{
    Delete(Head);
}
// Adds a binary code and its corresponding character to the Trie.
void Trie::Add(std::string Binary, char Character)
{
    if (Head == nullptr) // If the Trie is empty, create the root node.
        Head = new TrieNode();

    TrieNode *CurrentNode = Head; // Start from the root node.

    // Traverse the Trie based on the bits in the binary code.
    for (int i = 0; i < Binary.size(); i++)
    {
        if (Binary[i] == '1') // If the current bit is '1', go to the right child.
        {
            if (CurrentNode->Right == nullptr) // If the right child doesn't exist, create it.
                CurrentNode->Right = new TrieNode();
            CurrentNode = CurrentNode->Right; // Move to the right child.
        }
        else // If the current bit is '0', go to the left child.
        {
            if (CurrentNode->Left == nullptr) // If the left child doesn't exist, create it.
                CurrentNode->Left = new TrieNode();
            CurrentNode = CurrentNode->Left; // Move to the left child.
        }
    }
    CurrentNode->Character = Character; // Store the character at the leaf node.
}

// Checks if a given node is a leaf node (has no children).
bool isLeaf(TrieNode *CurrentNode)
{
    return (CurrentNode->Left == nullptr && CurrentNode->Right == nullptr);
}

void DecodeHuffman::ReadHuffmanCode(std::string HuffmanCodeFileName)
{
    std::ifstream HuffmanCode(HuffmanCodeFileName); // Open the file containing the Huffman codes.
    // Check if the file opened successfully.
    if (!HuffmanCode.good())
    {
        std::cerr << "Error: Could not open Huffman code file: " << HuffmanCodeFileName << std::endl;
        IsHuffmanTreeGood = false; // Mark tree as not built
        return;                    // Exit the function if file opening failed.
    }
    // Read the character codes and their corresponding binary representations from the frequency file.
    while (HuffmanCode >> CharacterCode) // Read the character code (as an integer).
    {
        HuffmanCode >> Binary;                       // Read the binary code (as a string).
        TrieObject.Add(Binary, (char)CharacterCode); // Add the character and its code to the Trie.
    }
    // Mark the Huffman tree have successfully built.
    IsHuffmanTreeGood = true;
    HuffmanCode.close(); // Close the file stream.
}

// Reads Huffman codes from an in-memory string array and builds the decoding Trie.
void DecodeHuffman::ReadHuffmanCode(std::string *Label, int LabelSize)
{
    // Iterate through the provided Label array. The index represents the character's ASCII code.
    for (int CharacterCode = 0; CharacterCode < LabelSize; CharacterCode++)
    {
        // Only add if the label string is not empty (meaning the character was present in the original data).
        if (!Label[CharacterCode].empty())
        {
            TrieObject.Add(Label[CharacterCode], (char)CharacterCode); // Add the code and character to the Trie.
        }
    }
    // Mark the Huffman tree have successfully built.
    IsHuffmanTreeGood = true;
}

void DecodeHuffman::DecodeFromFile(std::string EncryptedFileName)
{
    std::ifstream EncryptedFile(EncryptedFileName);

    if (!EncryptedFile.good() || !IsHuffmanTreeGood)
        return;

    // Read the size of the encoded file
    EncryptedFile.read((char *)&FileSize, 4);

    // Decode the encoded file.
    CurrentNode = TrieObject.Head; // Start from the root of the Trie.

    while (EncryptedFile.read(&Character, 1))
    {
        // Process each bit in the byte.
        for (int i = 7; i >= 0; i--) // Iterate from the most significant bit to the least significant bit.
        {
            if (FileSize <= 0) // Check File Size
                break;
            // Traverse the Trie based on the current bit.
            if (Character & (1 << i)) // If the i-th bit is 1, go to the right child.
                CurrentNode = CurrentNode->Right;
            else // If the i-th bit is 0, go to the left child.
                CurrentNode = CurrentNode->Left;

            // It's possible to reach a null pointer if the encoded data is corrupt or the tree is incomplete.
            if (!CurrentNode)
            {
                std::cerr << "Error: Decoding error - encountered invalid path in Huffman Trie." << std::endl;
                EncryptedFile.close();
                IsFileGood = false; // Indicate an error occurred during decoding.
                return;             // Stop decoding.
            }

            // If a leaf node is reached, output the character and reset to the root.
            if (isLeaf(CurrentNode))
            {
                DecodedString.push_back(CurrentNode->Character); // Write the decoded character to the output file.
                CurrentNode = TrieObject.Head;                   // Reset to the root of the Trie for the next character.
            }
            --FileSize; // Decrease FileSize.
        }
    }

    EncryptedFile.close(); // Close the encoded file stream.
}

// Decodes data from an in-memory character array (encoded string) using the pre-built Huffman Trie.
void DecodeHuffman::DecodeFromString(const char *EncodedString, int ProvidedFileSize)
{
    // Check if the Huffman Trie was built correctly.
    if (!IsHuffmanTreeGood)
    {
        std::cerr << "Error: Huffman tree not built. Cannot decode string." << std::endl;
        return; // Exit if the Trie is not ready.
    }

    FileSize = ProvidedFileSize; // Use the provided total bit count.

    // Decode the encoded file.
    CurrentNode = TrieObject.Head; // Start from the root of the Trie.

    // Check if the Trie is empty
    if (CurrentNode == nullptr)
    { // Check if the Trie is empty
        std::cerr << "Error: Huffman tree is empty. Cannot decode." << std::endl;
        return;
    }
    // Calculate the number of full bytes to process based on the FileSize (total bits).
    // Add 7 before dividing by 8 to handle potential partial bytes correctly (ceiling division).
    int numberOfBytes = (FileSize + 7) / 8;

    // Iterate through the necessary bytes in the input character array.
    for (int Index = 0; Index < numberOfBytes; Index++)
    {
        // Get the current byte from the encoded string buffer.
        Character = EncodedString[Index];
        // Process each bit in the byte.
        for (int Bit = 7; Bit >= 0; Bit--) // Iterate from the most significant bit to the least significant bit.
        {
            if (FileSize <= 0) // Check File Size
                break;
            // Traverse the Trie based on the current bit.
            if (Character & (1 << Bit)) // If the i-th bit is 1, go to the right child.
                CurrentNode = CurrentNode->Right;
            else // If the i-th bit is 0, go to the left child.
                CurrentNode = CurrentNode->Left;

            // Check for potential decoding errors (invalid path).
            if (!CurrentNode)
            {
                std::cerr << "Error: Decoding error - encountered invalid path in Huffman Trie." << std::endl;
                return; // Stop decoding.
            }

            // If a leaf node is reached, output the character and reset to the root.
            if (isLeaf(CurrentNode))
            {
                DecodedString.push_back(CurrentNode->Character); // Write the decoded character to the output file.
                CurrentNode = TrieObject.Head;                   // Reset to the root of the Trie for the next character.
            }
        }
    }
}

// Returns the accumulated decoded string.
std::string DecodeHuffman::GetDecodedString()
{
    // Simply return the member variable where the decoded characters were stored.
    return DecodedString;
}

// Writes the decoded string content to a specified file.
void DecodeHuffman::WriteDecodedToFile(std::string DecodedFileName)
{
    // Open the output file for writing the decoded text.
    std::ofstream DecodedFile(DecodedFileName);
    // Check if the file opened successfully.
    if (!DecodedFile.good())
    {
        std::cerr << "Error: Could not open decoded output file: " << DecodedFileName << std::endl;
        return; // Exit if file opening failed.
    }
    // Write the entire content of the DecodedString member variable to the file.
    DecodedFile << DecodedString;
    DecodedFile.close(); // Close the output file stream.
}