#pragma once

// Handler key value
const std::wstring
DLL_NAME = L"QAE";
// Desktop Namespace || GUID value
const std::wstring
DLL_SUBNAME = L"Quick Access Extended";

// Format Value
const std::wstring
DLL_FORMAT = L"SOFTWARE\\Classes\\Folder\\shellex\\ContextMenuHandlers\\";

// Location of "container/folder" for QAE
const std::string
DLL_QAEPATH_S = "C:\\Program Files (x86)\\QAE";
const std::wstring
DLL_QAEPATH = L"C:\\Program Files (x86)\\QAE";
// Icon DLL Path / Icon ID
const std::wstring
DLL_ICONPATH = L"%systemroot%\\system32\\imageres.dll, -1303";

// ...\\Instance\\InitPropertyBag
const DWORD
DLL_SHELL_ATTRIBUTES = 0xF080004D;
// Folder Flags
const DWORD
DLL_FOLDER_FLAGS = 0x28;
// ...\\ShellFolder Attributes
const DWORD
DLL_FOLDER_ATTRIBUTE = 0x11;
// Order Value
const DWORD
DLL_SYSTEM_ORDER = 0x42;
// Pin Key Value
const DWORD
DLL_SYSTEM_PIN = 0x01;
// Panel Key Value
const DWORD
DLL_SYSTEM_GUID = 0x01;

const DWORD
DLL_SYMLINK_FLAG = 0x01;
const std::wstring
DLL_SYSTEM_PARAMETER = L"Quick Access Extended";