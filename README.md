Quick Access Extended
=====================

QAE Shell Extension provides second "Quick Access" panel in
windows explorer's navigation pane.

## General Notes

This extension was created because windows 10 doesn't provide
a way to create second "Quick Access" panel by default.
(Or I just didn't find a way to do so)
It simply creates "QAE" folder inside "Program Files (x86)"
and stores symlinks inside. Extension should work with
Windows 10 and above. (Tested on 18363.1679)

## Features

- C++17
- Quick Access panel similar to default one
- Ability to store not only symlinks, but also folders & files.
- Symlink creation context menu
- Manual symlink creation available
- Single DLL File

## Installation:

Dll can be installing by following method:
Copy dll to Windows\System32, run terminal as administrator and type 'regsrv32 Quick-Access-Extended.dll'
To uninstall extension simply use 'regsrv32 /u Quick-Access-Extended.dll' in terminal.
Installing/Uninstalling may require system restart.
(Note: Windows terminal should be opened in the dll directory.)

## Limitations:

Context symlink creation may fail due to the dll's lack of administrator priviliges. "Drag and Drop" also unavailable.
In the case of any error, it is enough to simply delete the affected register addresses.

Any problems can be resolved by deleting affected registry adresses.

#### Affected Registry Adresses:

```
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{FAA719E9-628E-4286-AAAC-B324EAE4B6F5}
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved\QAE
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Desktop\NameSpace\{FAA719E9-628E-4286-AAAC-B324EAE4B6F5}
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\HideDesktopIcons\NewStartPanel\{FAA719E9-628E-4286-AAAC-B324EAE4B6F5}
Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Folder\shellex\ContextMenuHandlers\QAE
```

## License

QAE Shell Extension is released under MIT license.