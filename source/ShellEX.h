#include "C-Linker.h"
#include "Selector.h"
#include "Utilities.h"
#include "GUID.h"

#include "Defenitions.h"

#pragma comment(lib, "shell32.lib")

#define RESULT_CHECK if (RESULT != ERROR_SUCCESS){return E_UNEXPECTED;}