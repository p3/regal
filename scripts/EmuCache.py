#!/usr/bin/python -B

# Regal.cpp - Application-side shader caching
#
# Note - for driver-side caching, RegalDispatchCache.cpp is used.

formulaeGlobal = {

    # GLSL shader caching

    'glShaderSource' : {
        'entries' : [ 'glShaderSource' ],
        'pre' : [
          'if (REGAL_CACHE && REGAL_CACHE_SHADER)',
          '{',
          '  Cache::shaderSource(_next->call(&_next->glShaderSource), shader, count, string, length);',
          '  return;',
          '}'
        ]
    },

    #
    # Texture caching
    #
    # At glBindTexture time we can optionally write out the texture data
    # as a .png file, for debugging.

    'glBindTexture' : {
        'entries' : [ 'glBindTexture' ],
        'pre' : [
          'if (REGAL_CACHE && REGAL_CACHE_TEXTURE)',
          '{',
          '  Cache::bindTexture(_next->call(&_next->glBindTexture),_next->call(&_next->glGetTexLevelParameteriv),_next->call(&_next->glGetTexImage), target, texture);',
          '  return;',
          '}'
        ]
    }

}
