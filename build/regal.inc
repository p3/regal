# regal.inc
#
# Generic gnumake .inc for building Regal
#

# Regal Sources

REGAL.CXX :=
REGAL.CXX += src/regal/RegalIff.cpp
REGAL.CXX += src/regal/Regal.cpp
REGAL.CXX += src/regal/RegalLog.cpp
REGAL.CXX += src/regal/RegalInit.cpp
REGAL.CXX += src/regal/RegalBreak.cpp
REGAL.CXX += src/regal/RegalUtil.cpp
REGAL.CXX += src/regal/RegalEmu.cpp
REGAL.CXX += src/regal/RegalFrame.cpp
REGAL.CXX += src/regal/RegalHelper.cpp
REGAL.CXX += src/regal/RegalMarker.cpp
REGAL.CXX += src/regal/RegalTexC.cpp
REGAL.CXX += src/regal/RegalCacheShader.cpp
REGAL.CXX += src/regal/RegalCacheTexture.cpp
REGAL.CXX += src/regal/RegalConfig.cpp
REGAL.CXX += src/regal/RegalContext.cpp
REGAL.CXX += src/regal/RegalContextInfo.cpp
REGAL.CXX += src/regal/RegalStatistics.cpp
REGAL.CXX += src/regal/RegalLookup.cpp
REGAL.CXX += src/regal/RegalPlugin.cpp
REGAL.CXX += src/regal/RegalToken.cpp
REGAL.CXX += src/regal/RegalDispatchGlobal.cpp
REGAL.CXX += src/regal/RegalDispatcher.cpp
REGAL.CXX += src/regal/RegalDispatcherGL.cpp
REGAL.CXX += src/regal/RegalDispatcherGlobal.cpp
REGAL.CXX += src/regal/RegalDispatchEmu.cpp
REGAL.CXX += src/regal/RegalDispatchGLX.cpp
REGAL.CXX += src/regal/RegalDispatchLog.cpp
REGAL.CXX += src/regal/RegalDispatchCode.cpp
REGAL.CXX += src/regal/RegalDispatchCache.cpp
REGAL.CXX += src/regal/RegalDispatchError.cpp
REGAL.CXX += src/regal/RegalDispatchLoader.cpp
REGAL.CXX += src/regal/RegalDispatchDebug.cpp
REGAL.CXX += src/regal/RegalDispatchPpapi.cpp
REGAL.CXX += src/regal/RegalDispatchStatistics.cpp
REGAL.CXX += src/regal/RegalDispatchStaticES2.cpp
REGAL.CXX += src/regal/RegalDispatchStaticEGL.cpp
REGAL.CXX += src/regal/RegalDispatchTrace.cpp
REGAL.CXX += src/regal/RegalDispatchMissing.cpp
REGAL.CXX += src/regal/RegalPixelConversions.cpp
REGAL.CXX += src/regal/RegalHttp.cpp
REGAL.CXX += src/regal/RegalJson.cpp
REGAL.CXX += src/regal/RegalFavicon.cpp
REGAL.CXX += src/regal/RegalMac.cpp
REGAL.CXX += src/regal/RegalSo.cpp
REGAL.CXX += src/regal/RegalFilt.cpp
REGAL.CXX += src/regal/RegalXfer.cpp
REGAL.CXX += src/regal/RegalX11.cpp
REGAL.CXX += src/regal/RegalDllMain.cpp

# Regal Internal Headers

REGAL.H :=
REGAL.H += src/regal/RegalBin.h
REGAL.H += src/regal/RegalBreak.h
REGAL.H += src/regal/RegalCacheShader.h
REGAL.H += src/regal/RegalCacheTexture.h
REGAL.H += src/regal/RegalConfig.h
REGAL.H += src/regal/RegalContext.h
REGAL.H += src/regal/RegalContextInfo.h
REGAL.H += src/regal/RegalStatistics.h
REGAL.H += src/regal/RegalDebugInfo.h
REGAL.H += src/regal/RegalDispatch.h
REGAL.H += src/regal/RegalDispatchError.h
REGAL.H += src/regal/RegalDispatcher.h
REGAL.H += src/regal/RegalDispatcherGL.h
REGAL.H += src/regal/RegalDispatcherGlobal.h
REGAL.H += src/regal/RegalDsa.h
REGAL.H += src/regal/RegalEmu.h
REGAL.H += src/regal/RegalEnum.h
REGAL.H += src/regal/RegalFavicon.h
REGAL.H += src/regal/RegalFilt.h
REGAL.H += src/regal/RegalXfer.h
REGAL.H += src/regal/RegalFrame.h
REGAL.H += src/regal/RegalHelper.h
REGAL.H += src/regal/RegalHttp.h
REGAL.H += src/regal/RegalJson.h
REGAL.H += src/regal/RegalJson.inl
REGAL.H += src/regal/RegalIff.h
REGAL.H += src/regal/RegalInit.h
REGAL.H += src/regal/RegalLog.h
REGAL.H += src/regal/RegalLookup.h
REGAL.H += src/regal/RegalMarker.h
REGAL.H += src/regal/RegalNamespace.h
REGAL.H += src/regal/RegalTexC.h
REGAL.H += src/regal/RegalTexSto.h
REGAL.H += src/regal/RegalMac.h
REGAL.H += src/regal/RegalObj.h
REGAL.H += src/regal/RegalPpa.h
REGAL.H += src/regal/RegalPpca.h
REGAL.H += src/regal/RegalPrecompile.h
REGAL.H += src/regal/RegalPrivate.h
REGAL.H += src/regal/RegalPush.h
REGAL.H += src/regal/RegalSo.h
REGAL.H += src/regal/RegalSharedList.h
REGAL.H += src/regal/RegalSharedMap.h
REGAL.H += src/regal/RegalSharedPtr.h
REGAL.H += src/regal/RegalScopedPtr.h
REGAL.H += src/regal/RegalPixelConversions.h
REGAL.H += src/regal/RegalPixelConversions.inl
REGAL.H += src/regal/RegalState.h
REGAL.H += src/regal/RegalSystem.h
REGAL.H += src/regal/RegalThread.h
REGAL.H += src/regal/RegalTimer.h
REGAL.H += src/regal/RegalToken.h
REGAL.H += src/regal/RegalUtil.h
REGAL.H += src/regal/RegalVao.h
REGAL.H += src/regal/linear.h
REGAL.H += src/regal/pch.h

# Other Regal Headers

REGAL.H.OTHER :=
REGAL.H.OTHER += include/GL/Regal.h
REGAL.H.OTHER += src/lookup3/lookup3.h

#

REGAL.INCLUDE   :=
REGAL.INCLUDE   += -Isrc/regal
REGAL.INCLUDE   += -Isrc/zlib/include
REGAL.INCLUDE   += -Isrc/libpng/include
REGAL.INCLUDE   += -Isrc/mongoose
REGAL.INCLUDE   += -Isrc/md5/include
REGAL.INCLUDE   += -Isrc/lookup3
REGAL.INCLUDE   += -Isrc/jsonsl
REGAL.INCLUDE   += -Isrc/boost

REGAL.STATIC    ?= libRegallib.a
REGAL.SHARED    ?= libRegal.so
