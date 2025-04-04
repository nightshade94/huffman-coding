#include <iostream> // For input/output operations (e.g., std::cout, std::cin, std::ifstream, std::ofstream).
#include <fstream>  // For file input/output operations.
#include "Huffman.hpp"
const int MAX_N = 256; // Defines the maximum number of distinct characters (assuming extended ASCII).

// Default constructor.
HuffmanTreeNode::HuffmanTreeNode()
{
    Left = nullptr;  // Initialize left child to null.
    Right = nullptr; // Initialize right child to null.
}

// Constructor for leaf nodes.
HuffmanTreeNode::HuffmanTreeNode(int Freq, char Character)
{
    this->Freq = Freq;
    this->Character = Character;
    Left = nullptr;
    Right = nullptr;
}

// Constructor for internal nodes.
HuffmanTreeNode::HuffmanTreeNode(int Freq, HuffmanTreeNode *left, HuffmanTreeNode *right)
{
    this->Freq = Freq;
    Left = left;
    Right = right;
}

// Recursive function to delete the Huffman tree and free memory.
void Delete(HuffmanTreeNode *CurrentNode)
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

// Constructor to initialize the priority queue with a given size.
PriorityQueue::PriorityQueue(int n)
{
    // Initiate the priority queue.
    Init(n);
}
// Destructor to properly deallocate memory used by the PriorityQueue.
PriorityQueue::~PriorityQueue()
{
    delete[] NodeArray; // Deallocate the array of pointers itself.
}
// Initializes (or re-initializes) the priority queue.
void PriorityQueue::Init(int n)
{
    NumberOfElement = n;                     // Set the capacity.
    NodeArray = new HuffmanTreeNode *[n] {}; // Allocate memory for the node array.  Initialize all to null.
    CurrentNumberOfElement = 0;              // Reset the number of elements to 0.
}
// Heapify function (sift-down) to maintain the heap property.
void PriorityQueue::sift(int l, int r)
{
    int i = l, j = 2 * i + 1;          // i: index of the current node, j: index of the left child.
    HuffmanTreeNode *x = NodeArray[i]; // Store the current node.

    // While the left child exists within the heap bounds.
    while (j <= r)
    {
        // If the right child exists and has a lower frequency than the left child,
        // move to the right child.
        if (j < r)
            if (NodeArray[j]->Freq > NodeArray[j + 1]->Freq)
                ++j;

        // If the current node's frequency is less than or equal to the child's frequency,
        // the heap property is satisfied, so break.
        if (x->Freq <= NodeArray[j]->Freq)
            break;

        // Move the child up to the current node's position.
        NodeArray[i] = NodeArray[j];
        i = j;            // Move down to the child's position.
        j = (j << 1) + 1; // Calculate the index of the new left child.
    }
    NodeArray[i] = x; // Place the original node in its correct position.
}

// Push a new node into the priority queue.
void PriorityQueue::Push(HuffmanTreeNode *x)
{
    NodeArray[CurrentNumberOfElement++] = x;         // Add the new node at the end.
    int NewNumberIndex = CurrentNumberOfElement - 1; // get new number index
                                                     //  Sift-up to maintain the heap property.  Compare to parent.
    while (NewNumberIndex >= 0 && NodeArray[(NewNumberIndex - 1) / 2]->Freq > NodeArray[NewNumberIndex]->Freq)
    {
        std::swap(NodeArray[NewNumberIndex], NodeArray[(NewNumberIndex - 1) / 2]); // Swap with the parent if the parent is larger.
        NewNumberIndex = (NewNumberIndex - 1) / 2;                                 // Move up to the parent's index.
    }
}

// Pop the node with the minimum frequency from the priority queue.
HuffmanTreeNode *PriorityQueue::Pop()
{
    if (CurrentNumberOfElement <= 0) // Handle empty queue (though this shouldn't happen in this code).
        return nullptr;
    // Store the root node (minimum frequency) to be returned later.
    HuffmanTreeNode *MinNode = NodeArray[0];
    // Move the last element in the heap to the root position.
    std::swap(NodeArray[0], NodeArray[CurrentNumberOfElement - 1]);
    // Decrease the count of elements in the heap.
    --CurrentNumberOfElement;
    // Set the now unused last position to nullptr (optional, but good practice).
    NodeArray[CurrentNumberOfElement] = nullptr;
    // If there are still elements left, restore the heap property by sifting down the new root.
    if (CurrentNumberOfElement > 0)          // Only sift if the heap is not empty after removal
        sift(0, CurrentNumberOfElement - 1); // Sift down from root (index 0) to the new last element index.
    return MinNode;                          // Return the removed node (which was originally at the root).
}

// Returns a pointer to the node with the minimum frequency (the top element)
// *without* removing it from the priority queue.
HuffmanTreeNode *PriorityQueue::Top()
{
    // Check if the priority queue currently contains any elements.
    if (CurrentNumberOfElement > 0)
        // If not empty, return the pointer to the node at index 0.
        // In a min-heap, the element at the root (index 0) always has the minimum value (frequency in this case).
        return NodeArray[0];
    // If the queue is empty (CurrentNumberOfElement is 0 or less),
    // return nullptr to indicate that there is no top element.
    return nullptr;
}

// Default constructor
HuffmanEncoding::HuffmanEncoding()
{
    MaxCharacterSize = MAX_N;                    // Store the maximum character size used for array bounds.
    PQ.Init(MaxCharacterSize);                   // Initialize the priority queue with the maximum character set size.
    Freq = new int[MaxCharacterSize]();          // Allocate and zero-initialize the frequency array.
    Label = new std::string[MaxCharacterSize](); // Allocate the array for Huffman codes.
    // Encoded buffer initially null.
    Encoded = nullptr;
}
// Constructor with parameter
HuffmanEncoding::HuffmanEncoding(int MaxCharacterSetSize)
{
    this->MaxCharacterSize = MaxCharacterSetSize;
    PQ.Init(MaxCharacterSize);
    // Allocate frequency and label arrays based on the specified size.
    Freq = new int[MaxCharacterSize]();
    Label = new std::string[MaxCharacterSize]();
    Encoded = nullptr;
}
// Destructor
HuffmanEncoding::~HuffmanEncoding()
{
    delete[] Freq;    // Deallocate the frequency array.
    delete[] Label;   // Deallocate label array
    delete[] Encoded; // Deallocate the buffer holding the encoded data, if it was allocated.
}
// Recursive function to build the Huffman codes (labels) for each character.
void HuffmanEncoding::Labeling(std::string *LabelArray, HuffmanTreeNode *CurrentNode, std::string Label)
{
    // Check if the current node is valid.
    if (!CurrentNode)
        return;
    // Base case: If it's a leaf node, store the code for the character.
    if (!CurrentNode->Left && !CurrentNode->Right)
    {
        FileSize += Freq[CurrentNode->Character] * Label.size();
        LabelArray[CurrentNode->Character] = Label;
        return;
    }

    // Recursively traverse the left subtree, appending '0' to the code.
    if (CurrentNode->Left)
        Labeling(LabelArray, CurrentNode->Left, Label + '0');
    // Recursively traverse the right subtree, appending '1' to the code.
    if (CurrentNode->Right)
        Labeling(LabelArray, CurrentNode->Right, Label + '1');
}

// Builds the Huffman tree based on character frequencies.
void HuffmanEncoding::BuildTree()
{
    if (!IsFileGood) // If file open failed previously, don't proceed.
        return;

    // Build the priority queue with nodes for each character.
    for (int i = 0; i < MAX_N; i++)
    {
        if (Freq[i]) // If the character appeared in the input.
        {
            HuffmanTreeNode *NewNode = new HuffmanTreeNode(Freq[i], (char)i); // Create a leaf node.
            PQ.Push(NewNode);                                                 // Add the node to the priority queue.
        }
    }

    // Build the Huffman tree.
    while (PQ.CurrentNumberOfElement >= 2) // While there are at least two nodes in the queue.
    {
        HuffmanTreeNode *leftNode = PQ.Pop(), *rightNode = PQ.Pop(); // Get the two nodes with the lowest frequencies.

        // Ensure nodes were actually popped (queue wasn't unexpectedly empty).
        if (!leftNode || !rightNode)
        {
            std::cerr << "Error: Priority queue became empty unexpectedly during tree build." << std::endl;
            // Handle error appropriately, maybe delete popped nodes if necessary and return.
            delete leftNode;  // Clean up if one was popped
            delete rightNode; // Clean up if one was popped
            return;
        }
        HuffmanTreeNode *NewNode = new HuffmanTreeNode(leftNode->Freq + rightNode->Freq, leftNode, rightNode); // Create a new internal node.
        PQ.Push(NewNode);                                                                                      // Add the new node back to the priority queue.
    }

    if (PQ.CurrentNumberOfElement == 1)
    {
        HuffmanTreeNode *root = PQ.Pop(); // Get the root node
        Labeling(Label, root, "");        // Start labeling from the root with an empty code string.
        Delete(root);
    }
    else if (PQ.CurrentNumberOfElement == 0 && FileSize == 0)
    {
        IsLabeled = true;
    }
    else
    {
        std::cerr << "Error: Priority queue did not end with exactly one node." << std::endl;
        // Handle error state.
        return;
    }
    // Mark that the labeling process is complete.
    IsLabeled = true;
}

// Writes the generated Huffman codes to a specified file.
void HuffmanEncoding::WriteHuffmanCode(std::string HuffmanCodeFileName)
{
    // Open the output file for writing the codes.
    std::ofstream HuffmanCode(HuffmanCodeFileName);
    // Check if the file opened successfully and if the codes have been generated.
    if (!HuffmanCode.good())
    {
        std::cerr << "Error: Could not open Huffman code output file: " << HuffmanCodeFileName << std::endl;
        return;
    }
    if (!IsLabeled)
    {
        std::cerr << "Error: Cannot write Huffman codes, labeling not complete." << std::endl;
        HuffmanCode.close();
        return;
    }
    // Save the character labels (codes) to a file for decoding.
    for (int i = 0; i < MAX_N; i++)
    {
        if (Freq[i] > 0)
        {
            // Write the character's ASCII code, a space, and its Huffman code string to the file.
            HuffmanCode << i << ' ' << Label[i] << '\n'; // Store the character index and its code.
        }
    }

    HuffmanCode.close(); // Close the file stream.
}

// Encodes an input file using the Huffman algorithm.
void HuffmanEncoding::EncodeFile(std::string FileToEncodeName)
{
    // Open the input file to be encoded.
    std::ifstream FileToEncode(FileToEncodeName);

    // Check if the file opened successfully.
    IsFileGood = FileToEncode.good();
    if (!IsFileGood)
    {
        std::cerr << "Error: Could not open input file for encoding: " << FileToEncodeName << std::endl;
        return; // Exit if file opening failed.
    }

    FileSize = 0; // Reset FileSize before counting frequencies and building tree

    // Read the input file and count character frequencies.
    while (FileToEncode.get(Character))
        Freq[Character]++; // Increment the frequency count for the character read.

    BuildTree(); // Build the tree based on the counted frequencies.

    // Check if labeling completed successfully (implicitly checked by IsLabeled).
    if (!IsLabeled)
    {
        std::cerr << "Error: Failed to build Huffman tree or generate labels." << std::endl;
        FileToEncode.close(); // Close the file if open
        return;
    }

    // Reset the input file stream to the beginning.
    FileToEncode.clear();                 // Clear any error flags (like EOF).
    FileToEncode.seekg(0, std::ios::beg); // Seek back to the beginning of the file.

    // Allocate memory for the encoded data buffer.
    // Size calculation: (Total bits / 8) + 1 byte for potential partial byte.
    // Ensure FileSize is calculated correctly during Labeling.
    if (FileSize == 0 && PQ.CurrentNumberOfElement > 0)
    { // Check if FileSize calculation failed but tree exists
        std::cerr << "Error: FileSize is zero after labeling, cannot allocate buffer." << std::endl;
        FileToEncode.close();
        return;
    }

    int EncodedPointer = 0; // Index for writing into the Encoded buffer.
    Buffer = 0;             // Reset the bit buffer.
    NumberOfBit = 0;        // Reset the bit counter.
    // Calculate required buffer size carefully. Add 1 ensures space for the partial byte.
    size_t bufferSize = (FileSize == 0) ? 1 : (FileSize / 8) + ((FileSize % 8) != 0);
    if (Encoded != nullptr)
        delete[] Encoded; // Delete previous buffer if any
    Encoded = new char[bufferSize];

    // Encode the input file using the Huffman codes.
    while (FileToEncode.get(Character))
    {
        for (char Bit : Label[Character]) // Iterate through the bits in the character's code.
        {
            Buffer <<= 1;    // Shift the buffer left by 1 bit.
            if (Bit == '1')  // If the current bit is '1'.
                Buffer |= 1; // Set the least significant bit of the buffer to 1.
            NumberOfBit++;   // Increment the bit counter.

            // If the buffer is full (8 bits), write it to the output file.
            if (NumberOfBit == 8)
            {
                Encoded[EncodedPointer++] = Buffer; // Write the buffer to the file.
                Buffer = 0;                         // Reset the buffer.
                NumberOfBit = 0;                    // Reset the bit counter.
            }
        }
    }

    // Handle any remaining bits in the buffer.
    if (NumberOfBit != 0)
    {
        Buffer <<= (8 - NumberOfBit); // Shift the buffer left to fill the remaining bits with 0s.
                                      // Ensure we don't write past the allocated buffer.
        if (EncodedPointer < bufferSize)
        {
            Encoded[EncodedPointer++] = Buffer; // Store the last byte.
        }
        else
        {
            std::cerr << "Error: Buffer overflow writing last byte." << std::endl;
            // Handle error, maybe clean up.
            return;
        }
    }
    // Mark encoding as complete.
    IsEncoded = true;
}

// Encodes a given string and writes the encoded data and Huffman codes to files.
void HuffmanEncoding::EncodeString(std::string StringToEncode)
{
    // Assume the operation is valid initially.
    IsFileGood = true; // Reusing this flag, maybe rename for clarity (e.g., IsInputValid).
    FileSize = 0;      // Reset FileSize.

    // Read the input string and count character frequencies.
    for (int Index = 0; Index < StringToEncode.size(); Index++)
        Freq[StringToEncode[Index]]++;

    // Build Huffman Tree and Generate Codes
    BuildTree();

    // Check if labeling completed.
    if (!IsLabeled)
    {
        std::cerr << "Error: Failed to build Huffman tree or generate labels for string." << std::endl;
        return;
    }

    int EncodedPointer = 0;
    Buffer = 0;
    NumberOfBit = 0;
    // Calculate required buffer size carefully.
    int bufferSize = (FileSize == 0) ? 1 : (FileSize / 8) + ((FileSize % 8) != 0);

    if (Encoded != nullptr)
        delete[] Encoded; // Delete previous buffer if any

    Encoded = new char[bufferSize]; // Allocate and zero-initialize.

    // Encode the input file using the Huffman codes.
    for (int Index = 0; Index < StringToEncode.size(); Index++)
    {
        Character = StringToEncode[Index];
        for (char Bit : Label[Character]) // Iterate through the bits in the character's code.
        {
            Buffer <<= 1;    // Shift the buffer left by 1 bit.
            if (Bit == '1')  // If the current bit is '1'.
                Buffer |= 1; // Set the least significant bit of the buffer to 1.
            NumberOfBit++;   // Increment the bit counter.

            // If the buffer is full (8 bits), write it to the output file.
            if (NumberOfBit == 8)
            {
                if (EncodedPointer < bufferSize)
                {
                    Encoded[EncodedPointer++] = Buffer; // Store the byte.
                }
                else
                {
                    std::cerr << "Error: Buffer overflow during string encoding." << std::endl;
                    return; // Stop encoding on error.
                }
                Buffer = 0;      // Reset the buffer.
                NumberOfBit = 0; // Reset the bit counter.
            }
        }
    }

    // Handle any remaining bits in the buffer (padding).
    if (NumberOfBit > 0)
    {
        Buffer <<= (8 - NumberOfBit); // Shift left to pad.
                                      // Bounds check before writing.
        if (EncodedPointer < bufferSize)
        {
            Encoded[EncodedPointer++] = Buffer; // Store the last byte.
        }
        else
        {
            std::cerr << "Error: Buffer overflow writing last byte for string." << std::endl;
            return; // Stop on error.
        }
    }
    // Mark encoding as complete.
    IsEncoded = true;
}

// Writes the encoded data (FileSize and the Encoded buffer) to a specified file.
void HuffmanEncoding::WriteToFile(std::string EncodedFileName)
{
    // Open the output file in binary mode.
    std::ofstream EncodedFile(EncodedFileName, std::ios::binary);
    // Check if encoding is complete and the file opened successfully.
    if (!IsEncoded)
    {
        std::cerr << "Error: Encoding not complete. Cannot write to file." << std::endl;
        return;
    }
    if (!EncodedFile.good())
    {
        std::cerr << "Error: Could not open output file for writing encoded data: " << EncodedFileName << std::endl;
        return;
    }
    // Write the total number of *bits* (FileSize) to the beginning of the file.
    EncodedFile.write((char *)&FileSize, sizeof FileSize);

    // Calculate the number of bytes to write from the Encoded buffer.
    size_t bytesToWrite = (FileSize == 0) ? 0 : (FileSize / 8) + ((FileSize % 8) != 0);

    // Write the actual encoded data from the buffer.
    if (Encoded && bytesToWrite > 0)
    { // Check if Encoded is not null and there's data to write
        EncodedFile.write(Encoded, bytesToWrite);
    }
    else if (FileSize > 0 && !Encoded)
    {
        std::cerr << "Error: FileSize > 0 but Encoded buffer is null." << std::endl;
    }

    EncodedFile.close(); // Close the output file.
}

// Returns a pointer to the internally stored encoded data buffer.
const char *HuffmanEncoding::EncodedString()
{
    // Return the pointer to the dynamically allocated encoded data buffer.
    return Encoded;
}