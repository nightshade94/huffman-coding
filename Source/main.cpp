// Include necessary header files.
#include "Huffman.hpp"       // Contains HuffmanEncoding class for encoding.
#include "DecodeHuffman.hpp" // Contains DecodeHuffman class for decoding.
#include <iostream>          // For standard input/output operations (std::cout).

int main()
{
    // --- Example 1: Encode from file, save codes and encoded data to files, then decode from files and save decoded data to file. ---
    {
        std::cout << "--- Example 1: Encode/Decode File to File ---" << std::endl;

        // 1. Encoding Part
        HuffmanEncoding HE;         // Create an encoding object.
        HE.EncodeFile("input.txt"); // Read "input.txt", count frequencies, build tree, generate labels, and create the encoded bitstream in memory (HE.Encoded).
        if (HE.IsEncoded)
        {                                           // Check if encoding was successful
            HE.WriteHuffmanCode("HuffmanCode.txt"); // Write the generated Huffman codes (character -> binary string mapping) to "HuffmanCode.txt".
            HE.WriteToFile("output.bin");           // Write the encoded bitstream (including the preceding file size) from memory to "output.bin".
            std::cout << "Encoding complete. Output: HuffmanCode.txt, output.bin" << std::endl;

            // 2. Decoding Part (only if encoding succeeded)
            DecodeHuffman DH;                      // Create a decoding object.
            DH.ReadHuffmanCode("HuffmanCode.txt"); // Read the Huffman codes from the file and build the decoding Trie.
            if (DH.IsHuffmanTreeGood)
            {                                         // Check if codes were read and tree built
                DH.DecodeFromFile("output.bin");      // Read the encoded data from "output.bin", decode it using the Trie.
                DH.WriteDecodedToFile("decoded.txt"); // Write the resulting decoded string to "decoded.txt".
                std::cout << "Decoding complete. Output: decoded.txt" << std::endl;
            }
            else
            {
                std::cerr << "Error: Failed to read Huffman codes for decoding." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Encoding process failed." << std::endl;
        }
        std::cout << "--------------------------------------------" << std::endl;
    }
    // End of Example 1 block

    // --- Example 2: Encode from file, save codes, try to print encoded binary data directly to console (Generally not human-readable). ---
    /* // Uncomment this block to run Example 2
    {
        std::cout << "--- Example 2: Encode File, Print Binary (Not Recommended) ---" << std::endl;
        HuffmanEncoding HE;
        HE.EncodeFile("input.txt");
        if (HE.IsEncoded)
        {
            HE.WriteHuffmanCode("HuffmanCode.txt"); // Still useful to save the codes.
            std::cout << "Encoded binary data (may not display correctly):\n";
            // Access the encoded data buffer. Note: Contains binary data, not printable characters.
            const char *encodedData = HE.EncodedString();
            // Calculate the number of bytes in the encoded data.
            size_t bytesToWrite = (HE.FileSize == 0) ? 0 : (HE.FileSize / 8) + ((HE.FileSize % 8) != 0);
            // Write the raw bytes to cout. This will likely produce garbled output or trigger terminal issues.
            std::cout.write(encodedData, bytesToWrite);
            std::cout << "\n(End of binary data)" << std::endl;
            std::cout << "Warning: Printing raw binary data to console is generally not meaningful." << std::endl;
        }
        else
        {
            std::cerr << "Error: Encoding process failed." << std::endl;
        }
        std::cout << "------------------------------------------------------------" << std::endl;
    }
    */
    // End of Example 2 block

    // --- Example 3: Encode file, save outputs, decode file, print decoded string to console. ---
    /* // Uncomment this block to run Example 3
    {
        std::cout << "--- Example 3: Encode File, Decode File, Print Decoded ---" << std::endl;
        // 1. Encoding
        HuffmanEncoding HE;
        HE.EncodeFile("input.txt");
        if (HE.IsEncoded)
        {
            HE.WriteHuffmanCode("HuffmanCode.txt");
            HE.WriteToFile("output.bin");
            std::cout << "Encoding complete." << std::endl;

            // 2. Decoding
            DecodeHuffman DH;
            DH.ReadHuffmanCode("HuffmanCode.txt");
            if (DH.IsHuffmanTreeGood)
            {
                DH.DecodeFromFile("output.bin");
                std::cout << "Decoded String:" << std::endl;
                std::cout << DH.GetDecodedString() << std::endl; // Print the decoded result to the console.
            }
            else
            {
                std::cerr << "Error: Failed to read Huffman codes for decoding." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Encoding process failed." << std::endl;
        }
        std::cout << "---------------------------------------------------------" << std::endl;
    }
    */
    // End of Example 3 block

    // --- Example 4: Encode string in memory, decode in memory, save decoded result to file. ---
    /* // Uncomment this block to run Example 4
    {
        std::cout << "--- Example 4: Encode String, Decode In-Memory, Save Decoded ---" << std::endl;
        std::string originalString = "Hello world, this is a test string for Huffman encoding!";

        // 1. Encoding
        HuffmanEncoding HE;
        HE.EncodeString(originalString); // Encode the string directly.
        if (HE.IsEncoded)
        {
            std::cout << "String encoding complete." << std::endl;
            // Note: No need to write codes/encoded data to file if decoding happens immediately.

            // 2. Decoding (using in-memory data)
            DecodeHuffman DH;
            // Pass the Label array and size directly from the encoder to the decoder.
            DH.ReadHuffmanCode(HE.Label, HE.MaxCharacterSize);
            if (DH.IsHuffmanTreeGood)
            {
                // Pass the pointer to the encoded data buffer and the total bit count.
                DH.DecodeFromString(HE.EncodedString(), HE.FileSize);
                DH.WriteDecodedToFile("decoded_string.txt"); // Save the decoded result.
                std::cout << "In-memory decoding complete. Output: decoded_string.txt" << std::endl;
                // Optional: Verify content of decoded_string.txt
            }
            else
            {
                std::cerr << "Error: Failed build Huffman tree from labels for decoding." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: String encoding process failed." << std::endl;
        }
        std::cout << "----------------------------------------------------------------" << std::endl;
    }
    */
    // End of Example 4 block

    // --- Example 5: Encode string in memory, decode in memory, print decoded string to console. ---
    /* // Uncomment this block to run Example 5
    {
        std::cout << "--- Example 5: Encode String, Decode In-Memory, Print Decoded ---" << std::endl;
        std::string originalString = "Hello world"; // The string to encode.

        // 1. Encoding
        HuffmanEncoding HE;              // Create encoder object.
        HE.EncodeString(originalString); // Encode the string. Frequencies, tree, labels, and encoded data are generated in memory.
        if (HE.IsEncoded)
        {
            std::cout << "String encoding complete." << std::endl;

            // 2. Decoding
            DecodeHuffman DH; // Create decoder object.
            // Build the decoding Trie directly from the Label array generated by the encoder.
            DH.ReadHuffmanCode(HE.Label, HE.MaxCharacterSize);
            if (DH.IsHuffmanTreeGood)
            {
                // Decode directly from the encoder's in-memory buffer (HE.EncodedString()) using the total bit count (HE.FileSize).
                DH.DecodeFromString(HE.EncodedString(), HE.FileSize);
                std::cout << "Decoded String:" << std::endl;
                std::cout << DH.GetDecodedString() << std::endl; // Print the final decoded string to the console.
            }
            else
            {
                std::cerr << "Error: Failed build Huffman tree from labels for decoding." << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: String encoding process failed." << std::endl;
        }
        std::cout << "----------------------------------------------------------------" << std::endl;
    }
    */
    // End of Example 5 block

    return 0; // Indicate successful program execution.
}