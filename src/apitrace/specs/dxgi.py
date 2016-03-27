##########################################################################
#
# Copyright 2014 VMware, Inc
# Copyright 2011 Jose Fonseca
# All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
##########################################################################/


from dxgitype import *


IDXGIObject = Interface("IDXGIObject", IUnknown)
IDXGIDeviceSubObject = Interface("IDXGIDeviceSubObject", IDXGIObject)
IDXGIResource = Interface("IDXGIResource", IDXGIDeviceSubObject)
IDXGIKeyedMutex = Interface("IDXGIKeyedMutex", IDXGIDeviceSubObject)
IDXGISurface = Interface("IDXGISurface", IDXGIDeviceSubObject)
IDXGISurface1 = Interface("IDXGISurface1", IDXGISurface)
IDXGIAdapter = Interface("IDXGIAdapter", IDXGIObject)
IDXGIOutput = Interface("IDXGIOutput", IDXGIObject)
IDXGISwapChain = Interface("IDXGISwapChain", IDXGIDeviceSubObject)
IDXGIFactory = Interface("IDXGIFactory", IDXGIObject)
IDXGIDevice = Interface("IDXGIDevice", IDXGIObject)
IDXGIFactory1 = Interface("IDXGIFactory1", IDXGIFactory)
IDXGIAdapter1 = Interface("IDXGIAdapter1", IDXGIAdapter)
IDXGIDevice1 = Interface("IDXGIDevice1", IDXGIDevice)


DXGI_USAGE = Flags(UINT, [
    "DXGI_CPU_ACCESS_NONE", # 0
    "DXGI_CPU_ACCESS_SCRATCH", # 3
    "DXGI_CPU_ACCESS_DYNAMIC", # 1
    "DXGI_CPU_ACCESS_READ_WRITE", # 2
    "DXGI_USAGE_SHADER_INPUT",
    "DXGI_USAGE_RENDER_TARGET_OUTPUT",
    "DXGI_USAGE_BACK_BUFFER",
    "DXGI_USAGE_SHARED",
    "DXGI_USAGE_READ_ONLY",
    "DXGI_USAGE_DISCARD_ON_PRESENT",
    "DXGI_USAGE_UNORDERED_ACCESS",
])

DXGI_FRAME_STATISTICS = Struct("DXGI_FRAME_STATISTICS", [
    (UINT, "PresentCount"),
    (UINT, "PresentRefreshCount"),
    (UINT, "SyncRefreshCount"),
    (LARGE_INTEGER, "SyncQPCTime"),
    (LARGE_INTEGER, "SyncGPUTime"),
])

DXGI_MAPPED_RECT = Struct("DXGI_MAPPED_RECT", [
    (INT, "Pitch"),
    (LinearPointer(BYTE, "_MappedSize"), "pBits"),
])

DXGI_ADAPTER_DESC = Struct("DXGI_ADAPTER_DESC", [
    (WString, "Description"),
    (UINT, "VendorId"),
    (UINT, "DeviceId"),
    (UINT, "SubSysId"),
    (UINT, "Revision"),
    (SIZE_T, "DedicatedVideoMemory"),
    (SIZE_T, "DedicatedSystemMemory"),
    (SIZE_T, "SharedSystemMemory"),
    (LUID, "AdapterLuid"),
])

DXGI_OUTPUT_DESC = Struct("DXGI_OUTPUT_DESC", [
    (WString, "DeviceName"),
    (RECT, "DesktopCoordinates"),
    (BOOL, "AttachedToDesktop"),
    (DXGI_MODE_ROTATION, "Rotation"),
    (HMONITOR, "Monitor"),
])

DXGI_SHARED_RESOURCE = Struct("DXGI_SHARED_RESOURCE", [
    (HANDLE, "Handle"),
])

DXGI_RESOURCE_PRIORITY = FakeEnum(UINT, [
    "DXGI_RESOURCE_PRIORITY_MINIMUM",
    "DXGI_RESOURCE_PRIORITY_LOW",
    "DXGI_RESOURCE_PRIORITY_NORMAL",
    "DXGI_RESOURCE_PRIORITY_HIGH",
    "DXGI_RESOURCE_PRIORITY_MAXIMUM",
])

DXGI_RESIDENCY = Enum("DXGI_RESIDENCY", [
    "DXGI_RESIDENCY_FULLY_RESIDENT",
    "DXGI_RESIDENCY_RESIDENT_IN_SHARED_MEMORY",
    "DXGI_RESIDENCY_EVICTED_TO_DISK",
])

DXGI_SURFACE_DESC = Struct("DXGI_SURFACE_DESC", [
    (UINT, "Width"),
    (UINT, "Height"),
    (DXGI_FORMAT, "Format"),
    (DXGI_SAMPLE_DESC, "SampleDesc"),
])

DXGI_SWAP_EFFECT = Enum("DXGI_SWAP_EFFECT", [
    "DXGI_SWAP_EFFECT_DISCARD",
    "DXGI_SWAP_EFFECT_SEQUENTIAL",
])

DXGI_SWAP_CHAIN_FLAG = Flags(UINT, [
    "DXGI_SWAP_CHAIN_FLAG_NONPREROTATED",
    "DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH",
    "DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE",
])

DXGI_SWAP_CHAIN_DESC = Struct("DXGI_SWAP_CHAIN_DESC", [
    (DXGI_MODE_DESC, "BufferDesc"),
    (DXGI_SAMPLE_DESC, "SampleDesc"),
    (DXGI_USAGE, "BufferUsage"),
    (UINT, "BufferCount"),
    (HWND, "OutputWindow"),
    (BOOL, "Windowed"),
    (DXGI_SWAP_EFFECT, "SwapEffect"),
    (DXGI_SWAP_CHAIN_FLAG, "Flags"),
])


IDXGIObject.methods += [
    StdMethod(HRESULT, "SetPrivateData", [(REFGUID, "Name"), (UINT, "DataSize"), (OpaqueBlob(Const(Void), "DataSize"), "pData")], sideeffects=False),
    StdMethod(HRESULT, "SetPrivateDataInterface", [(REFGUID, "Name"), (OpaquePointer(Const(IUnknown)), "pUnknown")], sideeffects=False),
    StdMethod(HRESULT, "GetPrivateData", [(REFGUID, "Name"), InOut(Pointer(UINT), "pDataSize"), Out(OpaquePointer(Void), "pData")], sideeffects=False),
    StdMethod(HRESULT, "GetParent", [(REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppParent")]),
]

IDXGIDeviceSubObject.methods += [
    StdMethod(HRESULT, "GetDevice", [(REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppDevice")]),
]

IDXGIResource.methods += [
    StdMethod(HRESULT, "GetSharedHandle", [Out(Pointer(HANDLE), "pSharedHandle")]),
    StdMethod(HRESULT, "GetUsage", [Out(Pointer(DXGI_USAGE), "pUsage")], sideeffects=False),
    StdMethod(HRESULT, "SetEvictionPriority", [(DXGI_RESOURCE_PRIORITY, "EvictionPriority")]),
    StdMethod(HRESULT, "GetEvictionPriority", [Out(Pointer(DXGI_RESOURCE_PRIORITY), "pEvictionPriority")], sideeffects=False),
]

IDXGIKeyedMutex.methods += [
    StdMethod(HRESULT, "AcquireSync", [(UINT64, "Key"), (DWORD, "dwMilliseconds")]),
    StdMethod(HRESULT, "ReleaseSync", [(UINT64, "Key")]),
]

DXGI_MAP = Flags(UINT, [
    "DXGI_MAP_READ",
    "DXGI_MAP_WRITE",
    "DXGI_MAP_DISCARD",
])

IDXGISurface.methods += [
    StdMethod(HRESULT, "GetDesc", [Out(Pointer(DXGI_SURFACE_DESC), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "Map", [Out(Pointer(DXGI_MAPPED_RECT), "pLockedRect"), (DXGI_MAP, "MapFlags")]),
    StdMethod(HRESULT, "Unmap", []),
]

IDXGISurface1.methods += [
    StdMethod(HRESULT, "GetDC", [(BOOL, "Discard"), Out(Pointer(HDC), "phdc")]),
    StdMethod(HRESULT, "ReleaseDC", [(Pointer(RECT), "pDirtyRect")]),
]

IDXGIAdapter.methods += [
    StdMethod(HRESULT, "EnumOutputs", [(UINT, "Output"), Out(Pointer(ObjPointer(IDXGIOutput)), "ppOutput")]),
    StdMethod(HRESULT, "GetDesc", [Out(Pointer(DXGI_ADAPTER_DESC), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "CheckInterfaceSupport", [(REFGUID, "InterfaceName"), Out(Pointer(LARGE_INTEGER), "pUMDVersion")], sideeffects=False),
]

DXGI_ENUM_MODES = Flags(UINT, [
    "DXGI_ENUM_MODES_INTERLACED",
    "DXGI_ENUM_MODES_SCALING",
    "DXGI_ENUM_MODES_STEREO",
    "DXGI_ENUM_MODES_DISABLED_STEREO",
])

IDXGIOutput.methods += [
    StdMethod(HRESULT, "GetDesc", [Out(Pointer(DXGI_OUTPUT_DESC), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "GetDisplayModeList", [(DXGI_FORMAT, "EnumFormat"), (DXGI_ENUM_MODES, "Flags"), InOut(Pointer(UINT), "pNumModes"), Out(Array(DXGI_MODE_DESC, "*pNumModes"), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "FindClosestMatchingMode", [(Pointer(Const(DXGI_MODE_DESC)), "pModeToMatch"), Out(Pointer(DXGI_MODE_DESC), "pClosestMatch"), (ObjPointer(IUnknown), "pConcernedDevice")], sideeffects=False),
    StdMethod(HRESULT, "WaitForVBlank", []),
    StdMethod(HRESULT, "TakeOwnership", [(ObjPointer(IUnknown), "pDevice"), (BOOL, "Exclusive")]),
    StdMethod(Void, "ReleaseOwnership", []),
    StdMethod(HRESULT, "GetGammaControlCapabilities", [Out(Pointer(DXGI_GAMMA_CONTROL_CAPABILITIES), "pGammaCaps")], sideeffects=False),
    StdMethod(HRESULT, "SetGammaControl", [(Pointer(Const(DXGI_GAMMA_CONTROL)), "pArray")]),
    StdMethod(HRESULT, "GetGammaControl", [Out(Pointer(DXGI_GAMMA_CONTROL), "pArray")], sideeffects=False),
    StdMethod(HRESULT, "SetDisplaySurface", [(ObjPointer(IDXGISurface), "pScanoutSurface")]),
    StdMethod(HRESULT, "GetDisplaySurfaceData", [(ObjPointer(IDXGISurface), "pDestination")]),
    StdMethod(HRESULT, "GetFrameStatistics", [Out(Pointer(DXGI_FRAME_STATISTICS), "pStats")], sideeffects=False),
]

DXGI_PRESENT = Flags(UINT, [
    "DXGI_PRESENT_TEST",
    "DXGI_PRESENT_DO_NOT_SEQUENCE",
    "DXGI_PRESENT_RESTART",
    "DXGI_PRESENT_DO_NOT_WAIT",
    "DXGI_PRESENT_STEREO_PREFER_RIGHT",
    "DXGI_PRESENT_STEREO_TEMPORARY_MONO",
    "DXGI_PRESENT_RESTRICT_TO_OUTPUT",
    "DXGI_PRESENT_USE_DURATION",
])

IDXGISwapChain.methods += [
    StdMethod(HRESULT, "Present", [(UINT, "SyncInterval"), (DXGI_PRESENT, "Flags")]),
    StdMethod(HRESULT, "GetBuffer", [(UINT, "Buffer"), (REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppSurface")]),
    StdMethod(HRESULT, "SetFullscreenState", [(BOOL, "Fullscreen"), (ObjPointer(IDXGIOutput), "pTarget")]),
    StdMethod(HRESULT, "GetFullscreenState", [Out(Pointer(BOOL), "pFullscreen"), Out(Pointer(ObjPointer(IDXGIOutput)), "ppTarget")]),
    StdMethod(HRESULT, "GetDesc", [Out(Pointer(DXGI_SWAP_CHAIN_DESC), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "ResizeBuffers", [(UINT, "BufferCount"), (UINT, "Width"), (UINT, "Height"), (DXGI_FORMAT, "NewFormat"), (DXGI_SWAP_CHAIN_FLAG, "SwapChainFlags")]),
    StdMethod(HRESULT, "ResizeTarget", [(Pointer(Const(DXGI_MODE_DESC)), "pNewTargetParameters")]),
    StdMethod(HRESULT, "GetContainingOutput", [Out(Pointer(ObjPointer(IDXGIOutput)), "ppOutput")]),
    StdMethod(HRESULT, "GetFrameStatistics", [Out(Pointer(DXGI_FRAME_STATISTICS), "pStats")], sideeffects=False),
    StdMethod(HRESULT, "GetLastPresentCount", [Out(Pointer(UINT), "pLastPresentCount")], sideeffects=False),
]

DXGI_MWA = Flags(UINT, [
    "DXGI_MWA_NO_WINDOW_CHANGES",
    "DXGI_MWA_NO_ALT_ENTER",
    "DXGI_MWA_NO_PRINT_SCREEN",
    "DXGI_MWA_VALID",
])

IDXGIFactory.methods += [
    StdMethod(HRESULT, "EnumAdapters", [(UINT, "Adapter"), Out(Pointer(ObjPointer(IDXGIAdapter)), "ppAdapter")]),
    StdMethod(HRESULT, "MakeWindowAssociation", [(HWND, "WindowHandle"), (DXGI_MWA, "Flags")]),
    StdMethod(HRESULT, "GetWindowAssociation", [Out(Pointer(HWND), "pWindowHandle")], sideeffects=False),
    StdMethod(HRESULT, "CreateSwapChain", [(ObjPointer(IUnknown), "pDevice"), (Pointer(DXGI_SWAP_CHAIN_DESC), "pDesc"), Out(Pointer(ObjPointer(IDXGISwapChain)), "ppSwapChain")]),
    StdMethod(HRESULT, "CreateSoftwareAdapter", [(HMODULE, "Module"), Out(Pointer(ObjPointer(IDXGIAdapter)), "ppAdapter")]),
]

IDXGIDevice.methods += [
    StdMethod(HRESULT, "GetAdapter", [Out(Pointer(ObjPointer(IDXGIAdapter)), "pAdapter")]),
    StdMethod(HRESULT, "CreateSurface", [(Pointer(Const(DXGI_SURFACE_DESC)), "pDesc"), (UINT, "NumSurfaces"), (DXGI_USAGE, "Usage"), (Pointer(Const(DXGI_SHARED_RESOURCE)), "pSharedResource"), Out(Pointer(ObjPointer(IDXGISurface)), "ppSurface")]),
    StdMethod(HRESULT, "QueryResourceResidency", [(Array(Const(ObjPointer(IUnknown)), "NumResources"), "ppResources"), Out(Array(DXGI_RESIDENCY, "NumResources"), "pResidencyStatus"), (UINT, "NumResources")], sideeffects=False),
    StdMethod(HRESULT, "SetGPUThreadPriority", [(INT, "Priority")]),
    StdMethod(HRESULT, "GetGPUThreadPriority", [Out(Pointer(INT), "pPriority")], sideeffects=False),
]

DXGI_ADAPTER_FLAG = FakeEnum(UINT, [
    "DXGI_ADAPTER_FLAG_NONE",
    "DXGI_ADAPTER_FLAG_REMOTE",
    "DXGI_ADAPTER_FLAG_SOFTWARE",
])

DXGI_ADAPTER_DESC1 = Struct("DXGI_ADAPTER_DESC1", [
    (WString, "Description"),
    (UINT, "VendorId"),
    (UINT, "DeviceId"),
    (UINT, "SubSysId"),
    (UINT, "Revision"),
    (SIZE_T, "DedicatedVideoMemory"),
    (SIZE_T, "DedicatedSystemMemory"),
    (SIZE_T, "SharedSystemMemory"),
    (LUID, "AdapterLuid"),
    (DXGI_SWAP_CHAIN_FLAG, "Flags"),
])

DXGI_DISPLAY_COLOR_SPACE = Struct("DXGI_DISPLAY_COLOR_SPACE", [
    (Array(Array(FLOAT, 8), 2), "PrimaryCoordinates"),
    (Array(Array(FLOAT, 16), 2), "WhitePoints"),
])

IDXGIFactory1.methods += [
    StdMethod(HRESULT, "EnumAdapters1", [(UINT, "Adapter"), Out(Pointer(ObjPointer(IDXGIAdapter1)), "ppAdapter")]),
    StdMethod(BOOL, "IsCurrent", []),
]

IDXGIAdapter1.methods += [
    StdMethod(HRESULT, "GetDesc1", [Out(Pointer(DXGI_ADAPTER_DESC1), "pDesc")], sideeffects=False),
]

IDXGIDevice1.methods += [
    StdMethod(HRESULT, "SetMaximumFrameLatency", [(UINT, "MaxLatency")]),
    StdMethod(HRESULT, "GetMaximumFrameLatency", [Out(Pointer(UINT), "pMaxLatency")], sideeffects=False),
]


IDXGIFactoryDWM = Interface("IDXGIFactoryDWM", IUnknown)
IDXGISwapChainDWM = Interface("IDXGISwapChainDWM", IDXGIDeviceSubObject)

IDXGIFactoryDWM.methods += [
    StdMethod(HRESULT, "CreateSwapChain", [(ObjPointer(IUnknown), "pDevice"), (Pointer(DXGI_SWAP_CHAIN_DESC), "pDesc"), (ObjPointer(IDXGIOutput), "pOutput"), Out(Pointer(ObjPointer(IDXGISwapChainDWM)), "ppSwapChain")]),
]

# http://shchetinin.blogspot.co.uk/2012/04/dwm-graphics-directx-win8win7.html
IDXGISwapChainDWM.methods += [
    StdMethod(HRESULT, "Present", [(UINT, "SyncInterval"), (DXGI_PRESENT, "Flags")]),
    StdMethod(HRESULT, "GetBuffer", [(UINT, "Buffer"), (REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppSurface")]),
    StdMethod(HRESULT, "GetDesc", [Out(Pointer(DXGI_SWAP_CHAIN_DESC), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "ResizeBuffers", [(UINT, "BufferCount"), (UINT, "Width"), (UINT, "Height"), (DXGI_FORMAT, "NewFormat"), (DXGI_SWAP_CHAIN_FLAG, "SwapChainFlags")]),
    StdMethod(HRESULT, "ResizeTarget", [(Pointer(Const(DXGI_MODE_DESC)), "pNewTargetParameters")]),
    StdMethod(HRESULT, "GetContainingOutput", [Out(Pointer(ObjPointer(IDXGIOutput)), "ppOutput")]),
    StdMethod(HRESULT, "GetFrameStatistics", [Out(Pointer(DXGI_FRAME_STATISTICS), "pStats")], sideeffects=False),
    StdMethod(HRESULT, "GetLastPresentCount", [Out(Pointer(UINT), "pLastPresentCount")], sideeffects=False),
]


dxgi = Module('dxgi')
dxgi.addInterfaces([
    IDXGIKeyedMutex,
    IDXGIFactory1,
    IDXGIFactoryDWM,
    IDXGIDevice1,
    IDXGIAdapter1,
    IDXGIResource,
])
dxgi.addFunctions([
    StdFunction(HRESULT, "CreateDXGIFactory", [(REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppFactory")]),
    StdFunction(HRESULT, "CreateDXGIFactory1", [(REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppFactory")]),
])



#
# DXGI 1.2
#

IDXGIDisplayControl = Interface("IDXGIDisplayControl", IUnknown)
IDXGIDisplayControl.methods += [
    StdMethod(BOOL, "IsStereoEnabled", [], sideeffects=False),
    StdMethod(Void, "SetStereoEnabled", [(BOOL, "enabled")]),
]

DXGI_OUTDUPL_MOVE_RECT = Struct("DXGI_OUTDUPL_MOVE_RECT", [
    (POINT, "SourcePoint"),
    (RECT, "DestinationRect"),
])

DXGI_OUTDUPL_DESC = Struct("DXGI_OUTDUPL_DESC", [
    (DXGI_MODE_DESC, "ModeDesc"),
    (DXGI_MODE_ROTATION, "Rotation"),
    (BOOL, "DesktopImageInSystemMemory"),
])

DXGI_OUTDUPL_POINTER_POSITION = Struct("DXGI_OUTDUPL_POINTER_POSITION", [
    (POINT, "Position"),
    (BOOL, "Visible"),
])

DXGI_OUTDUPL_POINTER_SHAPE_TYPE = Enum("DXGI_OUTDUPL_POINTER_SHAPE_TYPE", [
    "DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME",
    "DXGI_OUTDUPL_POINTER_SHAPE_TYPE_COLOR",
    "DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MASKED_COLOR",
])

DXGI_OUTDUPL_POINTER_SHAPE_INFO = Struct("DXGI_OUTDUPL_POINTER_SHAPE_INFO", [
    (UINT, "Type"),
    (UINT, "Width"),
    (UINT, "Height"),
    (UINT, "Pitch"),
    (POINT, "HotSpot"),
])

DXGI_OUTDUPL_FRAME_INFO = Struct("DXGI_OUTDUPL_FRAME_INFO", [
    (LARGE_INTEGER, "LastPresentTime"),
    (LARGE_INTEGER, "LastMouseUpdateTime"),
    (UINT, "AccumulatedFrames"),
    (BOOL, "RectsCoalesced"),
    (BOOL, "ProtectedContentMaskedOut"),
    (DXGI_OUTDUPL_POINTER_POSITION, "PointerPosition"),
    (UINT, "TotalMetadataBufferSize"),
    (UINT, "PointerShapeBufferSize"),
])

IDXGIOutputDuplication = Interface("IDXGIOutputDuplication", IDXGIObject)
IDXGIOutputDuplication.methods += [
    StdMethod(Void, "GetDesc", [Out(Pointer(DXGI_OUTDUPL_DESC), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "AcquireNextFrame", [(UINT, "TimeoutInMilliseconds"), Out(Pointer(DXGI_OUTDUPL_FRAME_INFO), "pFrameInfo"), Out(Pointer(ObjPointer(IDXGIResource)), "ppDesktopResource")]),
    StdMethod(HRESULT, "GetFrameDirtyRects", [(UINT, "DirtyRectsBufferSize"), Out(Array(RECT, "DirtyRectsBufferSize"), "pDirtyRectsBuffer"), Out(Pointer(UINT), "pDirtyRectsBufferSizeRequired")], sideeffects=False),
    StdMethod(HRESULT, "GetFrameMoveRects", [(UINT, "MoveRectsBufferSize"), Out(Array(DXGI_OUTDUPL_MOVE_RECT, "MoveRectsBufferSize"), "pMoveRectBuffer"), Out(Pointer(UINT), "pMoveRectsBufferSizeRequired")], sideeffects=False),
    StdMethod(HRESULT, "GetFramePointerShape", [(UINT, "PointerShapeBufferSize"), Out(OpaqueBlob(Void, "PointerShapeBufferSize"), "pPointerShapeBuffer"), Out(Pointer(UINT), "pPointerShapeBufferSizeRequired"), Out(Pointer(DXGI_OUTDUPL_POINTER_SHAPE_INFO), "pPointerShapeInfo")], sideeffects=False),
    StdMethod(HRESULT, "MapDesktopSurface", [Out(Pointer(DXGI_MAPPED_RECT), "pLockedRect")], sideeffects=False),
    StdMethod(HRESULT, "UnMapDesktopSurface", [], sideeffects=False),
    StdMethod(HRESULT, "ReleaseFrame", []),
]

DXGI_ALPHA_MODE = Enum("DXGI_ALPHA_MODE", [
    "DXGI_ALPHA_MODE_UNSPECIFIED",
    "DXGI_ALPHA_MODE_PREMULTIPLIED",
    "DXGI_ALPHA_MODE_STRAIGHT",
    "DXGI_ALPHA_MODE_IGNORE",
])

IDXGISurface2 = Interface("IDXGISurface2", IDXGISurface1)
IDXGISurface2.methods += [
    StdMethod(HRESULT, "GetResource", [(REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppParentResource"), Out(Pointer(UINT), "pSubresourceIndex")]),
]

DXGI_SHARED_RESOURCE_FLAG = Flags(DWORD, [
    "DXGI_SHARED_RESOURCE_READ",
    "DXGI_SHARED_RESOURCE_WRITE",
])

IDXGIResource1 = Interface("IDXGIResource1", IDXGIResource)
IDXGIResource1.methods += [
    StdMethod(HRESULT, "CreateSubresourceSurface", [(UINT, "index"), Out(Pointer(ObjPointer(IDXGISurface2)), "ppSurface")]),
    StdMethod(HRESULT, "CreateSharedHandle", [(Pointer(Const(SECURITY_ATTRIBUTES)), "pAttributes"), (DXGI_SHARED_RESOURCE_FLAG, "dwAccess"), (LPCWSTR, "lpName"), Out(Pointer(HANDLE), "pHandle")]),
]

DXGI_OFFER_RESOURCE_PRIORITY = Enum("DXGI_OFFER_RESOURCE_PRIORITY", [
    "DXGI_OFFER_RESOURCE_PRIORITY_LOW",
    "DXGI_OFFER_RESOURCE_PRIORITY_NORMAL",
    "DXGI_OFFER_RESOURCE_PRIORITY_HIGH",
])

IDXGIDevice2 = Interface("IDXGIDevice2", IDXGIDevice1)
IDXGIDevice2.methods += [
    StdMethod(HRESULT, "OfferResources", [(UINT, "NumResources"), (Array(Const(ObjPointer(IDXGIResource)), "NumResources"), "ppResources"), (DXGI_OFFER_RESOURCE_PRIORITY, "Priority")]),
    StdMethod(HRESULT, "ReclaimResources", [(UINT, "NumResources"), (Array(Const(ObjPointer(IDXGIResource)), "NumResources"), "ppResources"), Out(Pointer(BOOL), "pDiscarded")]),
    StdMethod(HRESULT, "EnqueueSetEvent", [(HANDLE, "hEvent")]),
]

DXGI_MODE_DESC1 = Struct("DXGI_MODE_DESC1", [
    (UINT, "Width"),
    (UINT, "Height"),
    (DXGI_RATIONAL, "RefreshRate"),
    (DXGI_FORMAT, "Format"),
    (DXGI_MODE_SCANLINE_ORDER, "ScanlineOrdering"),
    (DXGI_MODE_SCALING, "Scaling"),
    (BOOL, "Stereo"),
])

DXGI_SCALING = Enum("DXGI_SCALING", [
    "DXGI_SCALING_STRETCH",
    "DXGI_SCALING_NONE",
    "DXGI_SCALING_ASPECT_RATIO_STRETCH",
])

DXGI_SWAP_CHAIN_DESC1 = Struct("DXGI_SWAP_CHAIN_DESC1", [
    (UINT, "Width"),
    (UINT, "Height"),
    (DXGI_FORMAT, "Format"),
    (BOOL, "Stereo"),
    (DXGI_SAMPLE_DESC, "SampleDesc"),
    (DXGI_USAGE, "BufferUsage"),
    (UINT, "BufferCount"),
    (DXGI_SCALING, "Scaling"),
    (DXGI_SWAP_EFFECT, "SwapEffect"),
    (DXGI_ALPHA_MODE, "AlphaMode"),
    (DXGI_SWAP_CHAIN_FLAG, "Flags"),
])

DXGI_SWAP_CHAIN_FULLSCREEN_DESC = Struct("DXGI_SWAP_CHAIN_FULLSCREEN_DESC", [
    (DXGI_RATIONAL, "RefreshRate"),
    (DXGI_MODE_SCANLINE_ORDER, "ScanlineOrdering"),
    (DXGI_MODE_SCALING, "Scaling"),
    (BOOL, "Windowed"),
])

DXGI_PRESENT_PARAMETERS = Struct("DXGI_PRESENT_PARAMETERS", [
    (UINT, "DirtyRectsCount"),
    (Array(RECT, "{self}.DirtyRectsCount"), "pDirtyRects"),
    (Pointer(RECT), "pScrollRect"),
    (Pointer(POINT), "pScrollOffset"),
])

IDXGISwapChain1 = Interface("IDXGISwapChain1", IDXGISwapChain)
IDXGISwapChain1.methods += [
    StdMethod(HRESULT, "GetDesc1", [(Out(Pointer(DXGI_SWAP_CHAIN_DESC1), "pDesc"))], sideeffects=False),
    StdMethod(HRESULT, "GetFullscreenDesc", [(Out(Pointer(DXGI_SWAP_CHAIN_FULLSCREEN_DESC), "pDesc"))], sideeffects=False),
    StdMethod(HRESULT, "GetHwnd", [(Out(Pointer(HWND), "pHwnd"))], sideeffects=False),
    StdMethod(HRESULT, "GetCoreWindow", [(REFIID, "riid"), (Out(Pointer(ObjPointer(Void)), "ppUnk"))]),
    StdMethod(HRESULT, "Present1", [(UINT, "SyncInterval"), (DXGI_PRESENT, "Flags"), (Pointer(Const(DXGI_PRESENT_PARAMETERS)), "pPresentParameters")]),
    StdMethod(BOOL, "IsTemporaryMonoSupported", [], sideeffects=False),
    StdMethod(HRESULT, "GetRestrictToOutput", [(Out(Pointer(ObjPointer(IDXGIOutput)), "ppRestrictToOutput"))]),
    StdMethod(HRESULT, "SetBackgroundColor", [(Pointer(Const(DXGI_RGBA)), "pColor")]),
    StdMethod(HRESULT, "GetBackgroundColor", [(Out(Pointer(DXGI_RGBA), "pColor"))], sideeffects=False),
    StdMethod(HRESULT, "SetRotation", [(DXGI_MODE_ROTATION, "Rotation")]),
    StdMethod(HRESULT, "GetRotation", [(Out(Pointer(DXGI_MODE_ROTATION), "pRotation"))], sideeffects=False),
]

IDXGIFactory2 = Interface("IDXGIFactory2", IDXGIFactory1)
IDXGIFactory2.methods += [
    StdMethod(BOOL, "IsWindowedStereoEnabled", [], sideeffects=False),
    StdMethod(HRESULT, "CreateSwapChainForHwnd", [(ObjPointer(IUnknown), "pDevice"), (HWND, "WindowHandle"), (Pointer(Const(DXGI_SWAP_CHAIN_DESC1)), "pDesc"), (Pointer(Const(DXGI_SWAP_CHAIN_FULLSCREEN_DESC)), "pFullscreenDesc"), (ObjPointer(IDXGIOutput), "pRestrictToOutput"), Out(Pointer(ObjPointer(IDXGISwapChain1)), "ppSwapChain")]),
    StdMethod(HRESULT, "CreateSwapChainForCoreWindow", [(ObjPointer(IUnknown), "pDevice"), (ObjPointer(IUnknown), "pWindow"), (Pointer(Const(DXGI_SWAP_CHAIN_DESC1)), "pDesc"), (ObjPointer(IDXGIOutput), "pRestrictToOutput"), Out(Pointer(ObjPointer(IDXGISwapChain1)), "ppSwapChain")]),
    StdMethod(HRESULT, "GetSharedResourceAdapterLuid", [(HANDLE, "hResource"), Out(Pointer(LUID), "pLuid")], sideeffects=False),
    StdMethod(HRESULT, "RegisterStereoStatusWindow", [(HWND, "WindowHandle"), (UINT, "wMsg"), Out(Pointer(DWORD), "pdwCookie")], sideeffects=False),
    StdMethod(HRESULT, "RegisterStereoStatusEvent", [(HANDLE, "hEvent"), Out(Pointer(DWORD), "pdwCookie")], sideeffects=False),
    StdMethod(Void, "UnregisterStereoStatus", [(DWORD, "dwCookie")], sideeffects=False),
    StdMethod(HRESULT, "RegisterOcclusionStatusWindow", [(HWND, "WindowHandle"), (UINT, "wMsg"), Out(Pointer(DWORD), "pdwCookie")], sideeffects=False),
    StdMethod(HRESULT, "RegisterOcclusionStatusEvent", [(HANDLE, "hEvent"), Out(Pointer(DWORD), "pdwCookie")], sideeffects=False),
    StdMethod(Void, "UnregisterOcclusionStatus", [(DWORD, "dwCookie")], sideeffects=False),
    StdMethod(HRESULT, "CreateSwapChainForComposition", [(ObjPointer(IUnknown), "pDevice"), (Pointer(Const(DXGI_SWAP_CHAIN_DESC1)), "pDesc"), (ObjPointer(IDXGIOutput), "pRestrictToOutput"), Out(Pointer(ObjPointer(IDXGISwapChain1)), "ppSwapChain")]),
]

DXGI_GRAPHICS_PREEMPTION_GRANULARITY = Enum("DXGI_GRAPHICS_PREEMPTION_GRANULARITY", [
    "DXGI_GRAPHICS_PREEMPTION_DMA_BUFFER_BOUNDARY",
    "DXGI_GRAPHICS_PREEMPTION_PRIMITIVE_BOUNDARY",
    "DXGI_GRAPHICS_PREEMPTION_TRIANGLE_BOUNDARY",
    "DXGI_GRAPHICS_PREEMPTION_PIXEL_BOUNDARY",
    "DXGI_GRAPHICS_PREEMPTION_INSTRUCTION_BOUNDARY",
])

DXGI_COMPUTE_PREEMPTION_GRANULARITY = Enum("DXGI_COMPUTE_PREEMPTION_GRANULARITY", [
    "DXGI_COMPUTE_PREEMPTION_DMA_BUFFER_BOUNDARY",
    "DXGI_COMPUTE_PREEMPTION_DISPATCH_BOUNDARY",
    "DXGI_COMPUTE_PREEMPTION_THREAD_GROUP_BOUNDARY",
    "DXGI_COMPUTE_PREEMPTION_THREAD_BOUNDARY",
    "DXGI_COMPUTE_PREEMPTION_INSTRUCTION_BOUNDARY",
])

DXGI_ADAPTER_DESC2 = Struct("DXGI_ADAPTER_DESC2", [
    (WString, "Description"),
    (UINT, "VendorId"),
    (UINT, "DeviceId"),
    (UINT, "SubSysId"),
    (UINT, "Revision"),
    (SIZE_T, "DedicatedVideoMemory"),
    (SIZE_T, "DedicatedSystemMemory"),
    (SIZE_T, "SharedSystemMemory"),
    (LUID, "AdapterLuid"),
    (DXGI_ADAPTER_FLAG, "Flags"),
    (DXGI_GRAPHICS_PREEMPTION_GRANULARITY, "GraphicsPreemptionGranularity"),
    (DXGI_COMPUTE_PREEMPTION_GRANULARITY, "ComputePreemptionGranularity"),
])

IDXGIAdapter2 = Interface("IDXGIAdapter2", IDXGIAdapter1)
IDXGIAdapter2.methods += [
    StdMethod(HRESULT, "GetDesc2", [Out(Pointer(DXGI_ADAPTER_DESC2), "pDesc")], sideeffects=False),
]

IDXGIOutput1 = Interface("IDXGIOutput1", IDXGIOutput)
IDXGIOutput1.methods += [
    StdMethod(HRESULT, "GetDisplayModeList1", [(DXGI_FORMAT, "EnumFormat"), (DXGI_ENUM_MODES, "Flags"), InOut(Pointer(UINT), "pNumModes"), Out(Array(DXGI_MODE_DESC1, "*pNumModes"), "pDesc")], sideeffects=False),
    StdMethod(HRESULT, "FindClosestMatchingMode1", [(Pointer(Const(DXGI_MODE_DESC1)), "pModeToMatch"), Out(Pointer(DXGI_MODE_DESC1), "pClosestMatch"), (ObjPointer(IUnknown), "pConcernedDevice")], sideeffects=False),
    StdMethod(HRESULT, "GetDisplaySurfaceData1", [(ObjPointer(IDXGIResource), "pDestination")]),
    StdMethod(HRESULT, "DuplicateOutput", [(ObjPointer(IUnknown), "pDevice"), Out(Pointer(ObjPointer(IDXGIOutputDuplication)), "ppOutputDuplication")]),
]

dxgi.addInterfaces([
    IDXGIDisplayControl,
    IDXGIDevice2,
    IDXGISwapChain1,
    IDXGIFactory2,
    IDXGIResource1,
    IDXGIAdapter2,
    IDXGIOutput1,
])

#
# DXGI 1.3
#

DXGI_CREATE_FACTORY_FLAGS = Flags(UINT, [
    "DXGI_CREATE_FACTORY_DEBUG",
])

dxgi.addFunctions([
    StdFunction(HRESULT, "CreateDXGIFactory2", [(DXGI_CREATE_FACTORY_FLAGS, "Flags"), (REFIID, "riid"), Out(Pointer(ObjPointer(Void)), "ppFactory")]),
])

IDXGIDevice3 = Interface("IDXGIDevice3", IDXGIDevice2)
IDXGIDevice3.methods += [
    StdMethod(Void, "Trim", []),
]

DXGI_MATRIX_3X2_F = Struct("DXGI_MATRIX_3X2_F", [
    (FLOAT, "_11"),
    (FLOAT, "_12"),
    (FLOAT, "_21"),
    (FLOAT, "_22"),
    (FLOAT, "_31"),
    (FLOAT, "_32"),
])

IDXGISwapChain2 = Interface("IDXGISwapChain2", IDXGISwapChain1)
IDXGISwapChain2.methods += [
    StdMethod(HRESULT, "SetSourceSize", [(UINT, "Width"), (UINT, "Height")]),
    StdMethod(HRESULT, "GetSourceSize", [Out(Pointer(UINT), "pWidth"), Out(Pointer(UINT), "pHeight")], sideeffects=False),
    StdMethod(HRESULT, "SetMaximumFrameLatency", [(UINT, "MaxLatency")]),
    StdMethod(HRESULT, "GetMaximumFrameLatency", [Out(Pointer(UINT), "pMaxLatency")], sideeffects=False),
    StdMethod(HANDLE, "GetFrameLatencyWaitableObject", [], sideeffects=False),
    StdMethod(HRESULT, "SetMatrixTransform", [(Pointer(Const(DXGI_MATRIX_3X2_F)), "pMatrix")]),
    StdMethod(HRESULT, "GetMatrixTransform", [Out(Pointer(DXGI_MATRIX_3X2_F), "pMatrix")], sideeffects=False),
]

IDXGIOutput2 = Interface("IDXGIOutput2", IDXGIOutput1)
IDXGIOutput2.methods += [
    StdMethod(BOOL, "SupportsOverlays", [], sideeffects=False),
]

IDXGIFactory3 = Interface("IDXGIFactory3", IDXGIFactory2)
IDXGIFactory3.methods += [
    StdMethod(DXGI_CREATE_FACTORY_FLAGS, "GetCreationFlags", [], sideeffects=False),
]

DXGI_DECODE_SWAP_CHAIN_DESC = Struct("DXGI_DECODE_SWAP_CHAIN_DESC", [
    (UINT, "Flags"),
])

# XXX: Flags
DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS = Enum("DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS", [
    "DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_NOMINAL_RANGE",
    "DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_BT709",
    "DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_xvYCC",
])

IDXGIDecodeSwapChain = Interface("IDXGIDecodeSwapChain", IUnknown)
IDXGIDecodeSwapChain.methods += [
    StdMethod(HRESULT, "PresentBuffer", [(UINT, "BufferToPresent"), (UINT, "SyncInterval"), (DXGI_PRESENT, "Flags")]),
    StdMethod(HRESULT, "SetSourceRect", [(Pointer(Const(RECT)), "pRect")]),
    StdMethod(HRESULT, "SetTargetRect", [(Pointer(Const(RECT)), "pRect")]),
    StdMethod(HRESULT, "SetDestSize", [(UINT, "Width"), (UINT, "Height")]),
    StdMethod(HRESULT, "GetSourceRect", [Out(Pointer(RECT), "pRect")], sideeffects=False),
    StdMethod(HRESULT, "GetTargetRect", [Out(Pointer(RECT), "pRect")], sideeffects=False),
    StdMethod(HRESULT, "GetDestSize", [Out(Pointer(UINT), "pWidth"), Out(Pointer(UINT), "pHeight")], sideeffects=False),
    StdMethod(HRESULT, "SetColorSpace", [(DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS, "ColorSpace")]),
    StdMethod(DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS, "GetColorSpace", [], sideeffects=False),
]

IDXGIFactoryMedia = Interface("IDXGIFactoryMedia", IUnknown)
IDXGIFactoryMedia.methods += [
    StdMethod(HRESULT, "CreateSwapChainForCompositionSurfaceHandle", [(ObjPointer(IUnknown), "pDevice"), (HANDLE, "hSurface"), (Pointer(Const(DXGI_SWAP_CHAIN_DESC1)), "pDesc"), (ObjPointer(IDXGIOutput), "pRestrictToOutput"), Out(Pointer(ObjPointer(IDXGISwapChain1)), "ppSwapChain")]),
    StdMethod(HRESULT, "CreateDecodeSwapChainForCompositionSurfaceHandle", [(ObjPointer(IUnknown), "pDevice"), (HANDLE, "hSurface"), (Pointer(DXGI_DECODE_SWAP_CHAIN_DESC), "pDesc"), (ObjPointer(IDXGIResource), "pYuvDecodeBuffers"), (ObjPointer(IDXGIOutput), "pRestrictToOutput"), Out(Pointer(ObjPointer(IDXGIDecodeSwapChain)), "ppSwapChain")]),
]

DXGI_FRAME_PRESENTATION_MODE = Enum("DXGI_FRAME_PRESENTATION_MODE", [
    "DXGI_FRAME_PRESENTATION_MODE_COMPOSED",
    "DXGI_FRAME_PRESENTATION_MODE_OVERLAY",
    "DXGI_FRAME_PRESENTATION_MODE_NONE",
])

DXGI_FRAME_STATISTICS_MEDIA = Struct("DXGI_FRAME_STATISTICS_MEDIA", [
    (UINT, "PresentCount"),
    (UINT, "PresentRefreshCount"),
    (UINT, "SyncRefreshCount"),
    (LARGE_INTEGER, "SyncQPCTime"),
    (LARGE_INTEGER, "SyncGPUTime"),
    (DXGI_FRAME_PRESENTATION_MODE, "CompositionMode"),
    (UINT, "ApprovedPresentDuration"),
])

IDXGISwapChainMedia = Interface("IDXGISwapChainMedia", IUnknown)
IDXGISwapChainMedia.methods += [
    StdMethod(HRESULT, "GetFrameStatisticsMedia", [Out(Pointer(DXGI_FRAME_STATISTICS_MEDIA), "pStats")], sideeffects=False),
    StdMethod(HRESULT, "SetPresentDuration", [(UINT, "Duration")]),
    StdMethod(HRESULT, "CheckPresentDurationSupport", [(UINT, "DesiredPresentDuration"), Out(Pointer(UINT), "pClosestSmallerPresentDuration"), Out(Pointer(UINT), "pClosestLargerPresentDuration")], sideeffects=False),
]

DXGI_OVERLAY_SUPPORT_FLAG = FakeEnum(UINT, [
    "DXGI_OVERLAY_SUPPORT_FLAG_DIRECT",
    "DXGI_OVERLAY_SUPPORT_FLAG_SCALING",
])

IDXGIOutput3 = Interface("IDXGIOutput3", IDXGIOutput2)
IDXGIOutput3.methods += [
    StdMethod(HRESULT, "CheckOverlaySupport", [(DXGI_FORMAT, "EnumFormat"), (ObjPointer(IUnknown), "pConcernedDevice"), Out(Pointer(DXGI_OVERLAY_SUPPORT_FLAG), "pFlags")], sideeffects=False),
]

dxgi.addInterfaces([
    IDXGIDevice3,
    IDXGISwapChain2,
    IDXGISwapChainMedia,
    IDXGIOutput3,
    IDXGIFactory3,
    IDXGIFactoryMedia,
])
