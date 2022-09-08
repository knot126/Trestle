# INI Document Loader

The INI document API is for loading and unloading INI documents into a strcture where their values can be queried.

An INI document root's allocation method is determined by the user: you could call `DgAlloc(sizeof(DgINIDocument));`, or simpily create the document statically (`DgINIDoucment doc;`).

The XML parser and loader has a similar API.

## DgINIParse

```c
uint32_t DgINIParse(DgINIDocument *doc, const uint32_t length, const char * const content);
```

Parse an INI document from a buffer of length (`length`).

## DgINILoad

```c
uint32_t DgINILoad(DgINIDocument *doc, char * const path);
```

Load an INI document from a file. The path string will be evaluated.

## DgINIGet

```c
char *DgINIGet(const DgINIDocument * const doc, const char * const sect, const char * const key, char *safe);
```

Get an value for an INI entry.

 * **`doc`**: The document node.
 * **`sect`**: The name of the section to get the value from.
 * **`key`**: The key to search for.
 * **`safe`**: The pointer to a string to return if the value is not found.

## DgINIGet

```c
void DgINIFree(DgINIDocument *doc);
```

Free an INI document.
