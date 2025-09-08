# LIBPFF
libpff is a library to access the Personal Folder File (PFF) and the Offline Folder File (OFF) format.
This library provides a foundational way to programmatically read and analyze the contents of PST files, which are used by Microsoft Outlook to store emails, calendar events, contacts, and other data.

## PFF/OFF is used in several file types:
- PST (Personal Storage Table)
- PAB (Personal Address Book)
- OST (Offline Storage Table)

### Supported formats:
- 32-bit ANSI (string)
- 64-bit Unicode (string)
- 64-bit with 4k pages with DEFLATE compression (Compressed OST files) (as of version 20130722)


## Project information:
- Status: alpha
- Licence: LGPLv3+



### Additional features:
- item recovery
- handles corrupted encrypted PFF with encryption type none

## Also see:
- [PFF forensics - analyzing the horrible reference file format](https://github.com/libyal/documentation/blob/master/PFF%20Forensics%20-%20analyzing%20the%20horrible%20reference%20file%20format.pdf)
- [PFF forensics - e-mail and appointment falsification analysis](https://github.com/libyal/documentation/blob/master/PFF%20forensics%20-%20e-mail%20and%20appoinment%20falsification%20analysis.pdf)
- [MAPI definitions](https://github.com/libyal/libfmapi/blob/master/documentation/MAPI%20definitions.pdf)

## Planned:
- Multi-threading support
- The Java (JNI) bindings are not operational and are there as long-term work in progress.

## For more information see:

* Project documentation: https://github.com/libyal/libpff/wiki/Home
* How to build from source: https://github.com/libyal/libpff/wiki/Building

