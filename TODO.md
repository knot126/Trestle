# Quick Run Engine To-Do

This doesn't list the obvious things like physics, graphics, etc. More about background stuff that I will actually not notice until it's a bit late..

 * Better memory alloctor, one that can actually free stuff :-)
   * Test the idea of a frame allocator: small initial alloc forgotten each frame (`DgFrameAlloc`)
     * And make sure its not patented.
 * Const variant of `DgBinaryFile` (name: `DgBinaryFileView`?)
 * Text file reading and writing
 * Serialisation to and from the following formats:
   * XML
   * JSON
   * INI
 * Compression algoritm for storing and loading compressed files
   * Zlib or Zstd
   * ZIP folder mounting and unmounting
 * Make sure that everything is well optimised 
   * Ex.: using bool arrays instead of bools in structs? This is better for CPU cache access.
 * Update the build system to something better
 * Make sure that all headers have `#pragma once` at the top
 * Windows port
 * Android port (?)
