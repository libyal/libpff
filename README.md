# LIBPFF
libpff is a library to access the Personal Folder File (PFF) and the Offline Folder File (OFF) format.
This library provides a foundational way to programmatically read and analyze the contents of:
- **PST** (Personal Storage Table)
- **PAB** (Personal Address Book)
- **OST** (Offline Storage Table)
files, which are used by Microsoft Outlook to store emails, calendar events, contacts, and other data.

Supported formats are:
- 32-bit ANSI (string)
- 64-bit Unicode (string)
- 64-bit with 4k pages with DEFLATE compression (Compressed OST files) (as of version 20130722).

## INSTALLING
While installing LIBPFF on GNU/Linux is straihgfoward, installing it on Windows requires a little bit effort. The suggested method here is to bypass windows using the [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) functionality, that is the best and quite confortable solution for windows user.


## USAGE
### Terminal
Three tool are available from terminal: pffinfo, pffexport, pffrecovery
```
>> pffexport -f -all -t my_pstfile.pst my_folder
```


### Python
```
import pypff as pf

mailbox = pypff.file()
mailbox.open("mailbox.pst", mode = "r")
root = mailbox.root_folder

for folder in root.sub_folders:
    print(folder.name)
```

## Project information:
- Status: alpha
- Licence: LGPLv3+

## Additional features:
- item recovery
- handles corrupted encrypted PFF with encryption type none

## For more information see:

- [Project documentation](https://github.com/libyal/libpff/wiki/Home)
- [How to build from source](https://github.com/libyal/libpff/wiki/Building)
- [PFF forensics - analyzing the horrible reference file format](https://github.com/libyal/documentation/blob/master/PFF%20Forensics%20-%20analyzing%20the%20horrible%20reference%20file%20format.pdf)
- [PFF forensics - e-mail and appointment falsification analysis](https://github.com/libyal/documentation/blob/master/PFF%20forensics%20-%20e-mail%20and%20appoinment%20falsification%20analysis.pdf)
- [MAPI definitions](https://github.com/libyal/libfmapi/blob/master/documentation/MAPI%20definitions.pdf)

## Planned:
- Multi-threading support
- The Java (JNI) bindings are not operational and are there as long-term work in progress.

