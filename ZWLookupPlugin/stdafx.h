// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef STDAFX_H_INCLUDED
#define STDAFX_H_INCLUDED

// Target Windows 10
#ifndef WINVER
#define WINVER 0x0A00
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0A00
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0A00
#endif

// Exclude rarely-used stuff from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Avoid min/max macro conflicts with STL
#ifndef NOMINMAX
#define NOMINMAX
#endif

// MFC includes - must be first
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <afxdialogex.h>    // MFC extended dialogs

// COM support - required before ZWCAD SDK
#include <objbase.h>
#include <comdef.h>

// Standard C++ includes
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <functional>

// ZWCAD ZRX SDK includes
// Note: zarxHeaders.h is the main header that includes most ZRX headers
#include "zarxHeaders.h"

// Additional ZRX headers as needed
#include "zdbmain.h"
#include "zacedads.h"
#include "zacutads.h"
#include "zaced.h"
#include "zacdb.h"
#include "zdbsymtb.h"
#include "zdbents.h"

#endif // STDAFX_H_INCLUDED
