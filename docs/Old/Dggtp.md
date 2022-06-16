# DGGTP Networking Protocol

*NOTE: This spec is currently a very early draft for what the protocol should be.*

The **Decent Games Generic Transfer Protocol** (dggtp) is a very basic transfer protocol for content between computers over any stable network stack, like TCP/IP or reliable UDP. It is meant for the sending and getting of messages between computers, and should not feature anything outside of this.

## Format

 * The format of all messages is binary, using only unsigned single byte (8-bit) integers and enums.
 * This is, except for when sending content length, which will use little-endian unsigned four byte (32-bit) integers. As such, it should be easily portable between systems.
 * The protocol is focused on mainly doing to and from transfers without descriptive error messages if they happen.
 * The protocol will be mainly used in games as a means of generic message passing, though it may or may not be used in the "real" netcode.

## Header

```c
typedef struct DGGTPHeader_t {
	uint8_t version;
	uint8_t status;
	uint8_t dictlen;
} DGGTPHeader;
```

### Feild Descriptions

 * `version`: The version of the protocol. See 'Versions' section.
 * `status`: The status code for what happened. See status codes section.
 * `dictlen`: The number of entries in the dicionary, which follows in the immedate bytes.

## Versions

For now, this should be `0x01` since this is the first version. When `0x00` is used, it is assumed something went wrong.

## Status Codes

A few status codes can be used to represent the current situation:

 * `0x00`: Indicates an unknown error.
 * `0x10`: Not implemented.
 * `0x20`: Request to get content.
 * `0x21` - `0x2F`: Reserved for alternate "Request to get content."
 * `0x30`: Request to push content.
 * `0x31` - `0x3F`: Reserved for alternate "Request to push content."
 * `0x40`: Response containing content.
 * `0x41` - `0x4F`: Reserved for alternate "Response containing content."
 * `0x50`: Response requesting content.
 * `0x51` - `0x5F`: Reserved for alternate "Response containing content."
 * `0x60`: The content was sent successfully.
 * `0x61`: The content was found but was not ready.
 * `0x62`: The content was not found.
 * `0x63` - `0x6F`: Reserved for future use

## Dictionary

```c
typedef struct DGGTPDict {
	uint8_t key;
	uint8_t value;
} DGGTPDict;
```
