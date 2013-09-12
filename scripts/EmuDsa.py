#!/usr/bin/python -B

dsaFormulae = {
    'SelectorGetCommands' : {
        'entries' : [ 'glGet(Boolean|Integer|Float|Double)v' ],
        'prefix' : [ '_context->dsa->RestoreGet( _context, ${arg0} );' ],
    },
    'DsaGetCommands1' : {
        'entries' : [ 'glGet(Float|Double)(Indexed|i_)v(EXT|)' ],
        'impl' : [
            'if ( ! _context->dsa->GetIndexedv( _context, ${arg0}, ${arg1}, ${arg2} ) ) {',
            '  _dispatch.call(&_dispatch.glGet${m1}${m2}v${m3})(${arg0}, ${arg1}, ${arg2});',
            '}',
        ],
    },
    'DsaGetCommands2' : {
        'entries' : [ 'glGetPointerIndexedvEXT' ],
        'impl' : [
            '// if ( ! _context->dsa->GetIndexedv( _context, ${arg0}, ${arg1}, ${arg2} ) ) {',
            '//     _dispatch.call(&_dispatch.glGetPointerIndexedvEXT)(${arg0}, ${arg1}, ${arg2});',
            '// }',
        ],
    },
    'DsaGetCommands3' : {
        'entries' : [ 'glGetFramebufferParameterivEXT' ],
        'impl' : [
            '_context->dsa->DsaFramebuffer( _context, GL_FRAMEBUFFER, ${arg0} );',
            '_dispatch.call(&_dispatch.glGetIntegerv)(${arg1plus});',
        ],
    },
    'SelectorIsEnabled' : {
        'entries' : [ 'glIsEnabled' ],
        'prefix' : [ '_context->dsa->RestoreIsEnabled( _context, ${arg0} );']
    },
    'DsaIsEnabled' : {
        'entries' : [ 'glIsEnabled(Indexed|i)EXT' ],
        'impl' : [
            'GLboolean ret;',
            'ret = _context->dsa->IsEnabledIndexed( _context, ${arg0}, ${arg1} );',
            'return ret;',
            ],
    },
    'SelectorMatrixCommands' : {
        'entries' : [
            'gl(Load|Mult)(Transpose|)Matrix(f|d)',
            'glLoadIdentity',
            'gl(Rotate|Scale|Translate)(f|d)',
            'gl(Push|Pop)Matrix',
        ],
        'prefix' : [ '_context->dsa->RestoreMatrixMode( _context );' ],
    },
    'MatrixMode' : {
        'entries' : [ 'glMatrixMode' ],
        'impl' : [
            'if (!_context->dsa->ShadowMatrixMode(${arg0})) {',
            '  _dispatch.call(&_dispatch.glMatrixMode)(${arg0});',
            '}',
        ],
    },
    'MatrixLoadMultStack' : {
        'entries' : [
            'glMatrix(Load|Mult)(Transpose|)(f|d)EXT',
            'glMatrix(Push|Pop)()()EXT',
        ],
        'impl' : [
            '_context->dsa->DsaMatrixMode( _context, ${arg0} );',
            '_dispatch.call(&_dispatch.gl${m1}${m2}Matrix${m3})(${arg1plus});',
        ],
    },
    'MatrixTransform' : {
        'entries' : [
            'glMatrix(LoadIdentity|Ortho|Frustum)()EXT',
            'glMatrix(Rotate|Scale|Translate)(f|d)EXT',
        ],
        'impl' : [
            '_context->dsa->DsaMatrixMode( _context, ${arg0} );',
            '_dispatch.call(&_dispatch.gl${m1}${m2})(${arg1plus});',
        ],
    },

    'ClientActiveTexture' : {
        'entries' : [ 'glClientActiveTexture(ARB|)' ],
        'impl' : [
            'if (!_context->dsa->ShadowClientActiveTexture( ${arg0} )) {',
            '  _dispatch.call(&_dispatch.glClientActiveTexture)( ${arg0} );',
            '}',
        ],
    },
    'ClientActiveCommands' : {
        'entries' : [
            'gl(Enable|Disable)ClientState',
            'glTexCoordPointer',
        ],
        'prefix' : [
          '_context->dsa->RestoreClientActiveTexture( _context );',
          '_context->dsa->RestoreBuffer( _context );',
        ],
    },
    'ClientIndexed' : {
        'entries' : [ 'gl(Enable|Disable)ClientState(Indexed|i)EXT' ],
        'impl' : [
            '_context->dsa->DsaClientActiveTexture( _context, ${arg1} + GL_TEXTURE0 );',
            '_dispatch.call(&_dispatch.gl${m1}ClientState)( ${arg0} );',
        ],
    },
    'ClientTexCoords' : {
        'entries' : [ 'glMultiTexCoordPointerEXT' ],
        'impl' : [
            '_context->dsa->DsaClientActiveTexture( _context, ${arg0} );',
            '_dispatch.call(&_dispatch.glTexCoordPointer)( ${arg1plus} );',
        ],
    },
    'EnableIndexed' : {
        'entries' : [ 'gl(Enable|Disable)IndexedEXT'],
        'impl' : [
            '_context->dsa->DsaActiveTexture( _context, ${arg1} + GL_TEXTURE0 );',
            '_dispatch.call(&_dispatch.gl${m1})( ${arg0} );',
        ],
    },
    'ActiveTexture' : {
        'entries' : [ 'glActiveTexture(ARB|)' ],
        'impl' : [
            'if( false == _context->dsa->ShadowActiveTexture( ${arg0} ) ) {',
            '    _dispatch.call(&_dispatch.glActiveTexture)( ${arg0} );',
            '}',
        ],
    },
    'TexCommands' : {
        'entries' : [
            'gl(Get|)Tex(Env|Parameter)(i|iv|f|fv)',
            'gl(Get|)TexParameter(Iiv|Iuiv)'
            'gl(Copy|Compressed|Get|)Tex(Sub|)Image(1|2|3)D',
            'glTex(Buffer|Renderbuffer)(NV|)',
            'glGenerateMipmap(ARB|EXT|)',
        ],
        'prefix' : [
            '_context->dsa->RestoreTexture( _context );',
            '_context->dsa->RestoreActiveTexture( _context );',
        ],
    },
    'TexCommands2' : {
        'entries' : [ 'glTexGen(i|iv|f|fv)', ],
        'prefix' : [ '_context->dsa->RestoreActiveTexture( _context );', ],
    },
    'TexCommands3' : {
        'entries' : [ 'gl(Enable|Disable)', ], # check that it's a texture target!
        'prefix' : [ '_context->dsa->RestoreActiveTexture( _context );', ],
    },
    'MultiActiveTexCommands' : {
        'entries' : [
            'gl(Get|)MultiTex(Env|Gen|Parameter|LevelParameter)(i|iv|f|fv|d|dv)EXT',
            'gl(Get|)MultiTex(Parameter|LevelParameter)(Iiv|Iuiv)EXT',
            'gl(Copy|Compressed|)MultiTex(Image|SubImage)(1D|2D|3D)EXT',
            'gl(Get|GetCompressed)MultiTex(Image|SubImage)()EXT',
            'gl()MultiTex(Buffer|Renderbuffer)()EXT',
            'gl(Generate)MultiTex(Mipmap)()EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*Renderbuffer.*' : 'glTexRenderbufferNV',
                'glGenerate.*' : 'glGenerateMipmap',
                'default' : 'gl${m1}Tex${m2}${m3}',
            },
        },
        'impl' : [
            '_context->dsa->DsaActiveTexture( _context, ${arg0} );',
            '_dispatch.call(&_dispatch.${nondsa})( ${arg1plus} );',
        ],
    },
    'BindTexture' : {
        'entries' : [ 'glBindTexture(EXT|)' ],
        'impl' : [
            'if (!_context->dsa->ShadowTexture( ${arg0}, ${arg1} )) {',
            '    _dispatch.call(&_dispatch.glBindTexture)( ${arg0}, ${arg1} );',
            '}',
        ],
    },
    'BindMultiTexture' : {
        'entries' : [ 'glBindMultiTextureEXT' ],
        'impl' : [
            '_context->dsa->DsaActiveTexture( _context, ${arg0} );',
            '_context->dsa->ShadowDsaTexture( ${arg1plus} );',
            '_dispatch.call(&_dispatch.glBindTexture)( ${arg1plus} );',
        ],
    },
    'BoundTextureCommands' : {
        'entries' : [
            'gl()Texture(Parameter)(i|iv|f|fv|Iiv|Iuiv)EXT',
            'gl(Get)Texture(Parameter|LevelParameter)(i|iv|f|fv|Iiv|Iuiv)EXT',
            'gl(Get|GetCompressed)Texture(Image)()EXT',
            'gl(Copy|Compressed|)Texture(Image|SubImage)(1D|2D|3D)EXT',
            'gl()Texture(Buffer|Renderbuffer)()EXT',
            'gl(Generate)Texture(Mipmap)()EXT',
            'gl()Texture(Storage)(1D|2D|3D)EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*Renderbuffer.*' : 'glTexRenderbufferNV',
                'glGenerate.*' : 'glGenerateMipmap',
                'default' : 'gl${m1}Tex${m2}${m3}',
            },
        },
        'impl' : [
            '_context->dsa->DsaTexture( _context, ${arg1}, ${arg0} );',
            '_dispatch.call(&_dispatch.${nondsa})( ${arg1plus} );',
        ],
    },

    'UseProgram' : {
        'entries' : [ 'glUseProgram' ],
        'impl' : [
            'if( false == _context->dsa->ShadowGlslProgram( ${arg0} ) ) {',
            '  _dispatch.call(&_dispatch.glUseProgram)(${arg0});',
            '}',
        ],
    },
    'SelectorProgramCommands' : {
        'entries' : [
            'glUniform(1|2|3|4)(i|f|ui|d)(v|)',
            'glUniformMatrix(2|3|4)(x2|x3|x4|)(fv|dv)',
        ],
        'prefix' : [ '_context->dsa->RestoreGlslProgram( _context );' ],
    },
    'DsaProgramCommands' : {
        'entries' : [
            'glProgramUniform()(1|2|3|4)()(i|iv|f|fv|d|dv|ui|uiv)EXT',
            'glProgramUniform(Matrix)(2|3|4)(x2|x3|x4|)(fv|dv)EXT',
        ],
        'impl' : [
            '_context->dsa->DsaGlslProgram( _context, ${arg0});',
            '_dispatch.call(&_dispatch.glUniform${m1}${m2}${m3}${m4})( ${arg1plus} );',
        ],
    },

    'BindAsmProgram' : {
        'entries' : [ 'glBindProgramARB' ],
        'impl' : [
            'if (!_context->dsa->ShadowAsmProgram( ${arg0}, ${arg1} ) ) {',
            '    _dispatch.call(&_dispatch.glBindProgramARB)( ${arg0}, ${arg1} );',
            '}',
        ],
    },
    'AsmProgram' : {
        'entries' : [
            'gl(Get|)Program(Env|Local)Parameter(.*)(ARB|NV)',
            'gl(Get|)Program(String)(EXT)',
        ],
        'prefix' : [ '_context->dsa->RestoreAsmProgram( _context, ${arg0} );' ],
    },
    'DsaAsmProgram' : {
        'entries' : [
            'gl(Get|)NamedProgram(Env|Local)Parameter(.*)(EXT)',
            'gl(Get|)NamedProgram(String|iv)()(EXT)',
        ],
        'subst' : {
            'nondsa' : {
                '.*Parameter4[df]v?EXT' : 'gl${m1}Program${m2}Parameter${m3}ARB',
                '.*Parameter[df]vEXT' : 'gl${m1}Program${m2}Parameter${m3}ARB',
                '.*Parameter(s|)I[4uiv]+EXT' : 'gl${m1}Program${m2}Parameter${m3}NV',
                '.*Program(String|iv).*' : 'gl${m1}Program${m2}ARB',
                'default' : 'gl${m1}Program${m2}Parameter${m3}EXT',
            },
        },
        'impl' : [
            '_context->dsa->DsaAsmProgram( _context, ${arg1}, ${arg0});',
            '_dispatch.call(&_dispatch.${nondsa})( ${arg1plus} );',
        ],
    },

    'BindFramebuffer' : {
        'entries' : [ 'glBindFramebuffer(EXT|)' ],
        'impl' : [
            'if (!_context->dsa->ShadowFramebuffer( ${arg0}, ${arg1} ) ) {',
            '    _dispatch.call(&_dispatch.glBindFramebuffer)( ${arg0}, ${arg1} );',
            '}',
        ],
    },
    'SelectorFramebufferCommands' : {
        'entries' : [
            'glFramebufferTexture(1D|2D|3D|Layer|Face|)(ARB|EXT|)',
            'glFramebufferRenderbuffer(ARB|EXT|)',
            'glDrawBuffer(s|)(ARB|EXT|NV|)'
            'glGet(FramebufferAttachment)Parameteriv(ARB|EXT|)',
        ],
        'prefix' : [ '_context->dsa->RestoreFramebuffer( _context );' ],
    },
    'SelectorFramebufferCommands2' : {
        'entries' : [
            'gl()Named(FramebufferTexture)(1D|2D|3D|Layer|Face|)EXT',
            'gl()Named(FramebufferRenderbuffer)()EXT',
            'gl(Get)Named(FramebufferAttachment)(Parameteriv)EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*CoverageEXT' : 'gl${m1}${m2}${m3}NV',
                '.*MultisampleEXT' : 'gl${m1}${m2}${m3}EXT',
                '.*FaceEXT' : 'gl${m1}${m2}${m3}ARB',
                'default' : 'gl${m1}${m2}${m3}',
            },
        },
        'impl' : [
            '_context->dsa->DsaFramebuffer( _context, GL_FRAMEBUFFER, ${arg0});',
            '_dispatch.call(&_dispatch.${nondsa})( GL_FRAMEBUFFER, ${arg1plus} );',
        ],
    },
    'SelectorFramebufferCommands3' : {
        'entries' : [ 'glFramebuffer(DrawBuffer|ReadBuffer)(s|)EXT' ],
        'impl' : [
            '_context->dsa->DsaFramebuffer( _context, GL_FRAMEBUFFER, ${arg0});',
            '_dispatch.call(&_dispatch.gl${m1}${m2})( ${arg1plus} );',
        ],
    },
    'SelectorFramebufferCommands4' : {
        'entries' : [ 'glCheckNamedFramebufferStatusEXT' ],
        'impl' : [
            '_context->dsa->DsaFramebuffer( _context, GL_FRAMEBUFFER, ${arg0});',
            'return _dispatch.call(&_dispatch.glCheckFramebufferStatus)( ${arg1plus} );',
        ],
    },

    'BindRenderbuffer' : {
        'entries' : [ 'glBindRenderbuffer(EXT|)' ],
        'impl' : [
            'if (!_context->dsa->ShadowRenderbuffer( ${arg0}, ${arg1} ) ) {',
            '    _dispatch.call(&_dispatch.glBindRenderbuffer)( ${arg0}, ${arg1} );',
            '}',
        ],
    },
    'SelectorRenderbufferCommands' : {
        'entries' : [
            'glRenderbufferStorage(Multisample|MultisampleCoverage|)(ARB|EXT|NV|)',
            'glGetRenderbufferParameteriv(ARB|EXT|)',
        ],
        'prefix' : [ '_context->dsa->RestoreRenderbuffer( _context );' ],
    },
    'SelectorRenderbufferCommands2' : {
        'entries' : [
            'gl()Named(RenderbufferStorage)(Multisample|MultisampleCoverage|)EXT',
            'gl(Get)Named(Renderbuffer)(Parameteriv)EXT',
        ],
        'subst' : {
            'nondsa' : {
                '.*CoverageEXT' : 'gl${m1}${m2}${m3}NV',
                '.*MultisampleEXT' : 'gl${m1}${m2}${m3}',
                'default' : 'gl${m1}${m2}${m3}',
            },
        },
        'impl' : [
            '_context->dsa->DsaRenderbuffer( _context, GL_RENDERBUFFER, ${arg0});',
            '_dispatch.call(&_dispatch.${nondsa})( GL_RENDERBUFFER, ${arg1plus} );',
        ],
    },

    'ClientAttribDefault' : {
        'entries' : ['glClientAttribDefaultEXT'],
        'impl' : [ '_context->dsa->ClientAttribDefault( _context, ${arg0} );' ],
    },
    'PushClientAttribDefault' : {
        'entries' : ['glPushClientAttribDefaultEXT'],
        'impl' : [
            'DispatchTableGL &tbl = _context->dispatcher.emulation;',
            'tbl.call(&tbl.glPushClientAttrib)(${arg0});',
            '_context->dsa->ClientAttribDefault(_context, ${arg0});',
        ],
    },
    'BindVertexArray' : {
        'entries' : [ 'glBindVertexArray' ],
        'impl' : [
            'if (!_context->dsa->ShadowVao( ${arg0} )) {',
            '    _dispatch.call(&_dispatch.glBindVertexArray)( ${arg0} );',
            '}',
        ],
    },
    # TODO - GL_ARB_base_instance ?
    'RestoreEverything' : {
        'entries' : [ 'glBegin', 'gl(Multi|)Draw(Arrays|Element|Elements)(Instanced|Indirect|BaseVertex|InstancedBaseVertex|Array|)(ARB|EXT|AMD|ATI|APPLE|)' ],
        'prefix' : [  '_context->dsa->Restore( _context );' ],
    },
    'BindBuffer' : {
        'entries' : [ 'glBindBuffer(ARB|)' ],
        'impl' : [
            'if (!_context->dsa->ShadowBuffer( ${arg0}, ${arg1} ) ) {',
            '    _dispatch.call(&_dispatch.glBindBuffer)(${arg0}, ${arg1});',
            '}',
        ],
    },
    'SelectorBufferCommands' : {
        'entries' : [
            'gl(Get|Copy|)Buffer(Sub|)Data',
            'gl(FlushMapped|Map|Unmap)Buffer(Range|)(ARB|EXT|)',
            'glGetBuffer(Parameteriv|Pointerv)',
            'gl(Vertex|Normal|Color|SecondaryColor)Pointer',
        ],
        'prefix' : [ '_context->dsa->RestoreBuffer( _context );' ],
    },
    'DsaBufferCommands' : {
        'entries' : [
            'gl(Get|)Named(Copy|)Buffer(Data|SubData)EXT',
            'gl(FlushMapped)Named()Buffer(Range)EXT',
            'gl(Get)Named()Buffer(Parameteriv|Pointerv)EXT',
        ],
        'impl' : [
            '_context->dsa->DsaBuffer( _context, ${arg0});',
            '_dispatch.call(&_dispatch.gl${m1}${m2}Buffer${m3})( GL_ARRAY_BUFFER, ${arg1plus} );',
        ],
    },
    'DsaBufferCommands2' : {
        'entries' : [ 'gl(Map)NamedBuffer(Range|)EXT' ],
        'impl' : [
            '_context->dsa->DsaBuffer( _context, ${arg0});',
            'return _dispatch.call(&_dispatch.gl${m1}Buffer${m2})( GL_ARRAY_BUFFER, ${arg1plus} );',
        ],
    },
    'DsaBufferCommands3' : {
        'entries' : [ 'glUnmapNamedBufferEXT' ],
        'impl' : [
            '_context->dsa->DsaBuffer( _context, ${arg0});',
            'return _dispatch.call(&_dispatch.glUnmapBuffer)( GL_ARRAY_BUFFER );',
        ],
    },
    'DsaVertexArrayOffsetCommands3' : {
        'entries' : [ 'glVertexArray(EdgeFlag)OffsetEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );'
            '_context->dsa->DsaBuffer( _context, ${arg1} );',
            '_dispatch.call(&_dispatch.gl${m1}Pointer)( ${arg2}, reinterpret_cast<const GLvoid *>(${arg3}) );',
        ],
    },
    'DsaVertexArrayPointerOffsets4' : {
        'entries' : [ 'glVertexArray(Index|Normal|FogCoord)OffsetEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );'
            '_context->dsa->DsaBuffer( _context, ${arg1} );',
            '_dispatch.call(&_dispatch.gl${m1}Pointer)( ${arg2}, ${arg3}, reinterpret_cast<const GLvoid *>(${arg4}) );',
        ],
    },
    'DsaVertexArrayOffsetCommands5' : {
        'entries' : [ 'glVertexArray(Vertex|Color|TexCoord|SecondaryColor)OffsetEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );'
            '_context->dsa->DsaBuffer( _context, ${arg1} );',
            '_dispatch.call(&_dispatch.gl${m1}Pointer)( ${arg2}, ${arg3}, ${arg4}, reinterpret_cast<const GLvoid *>(${arg5}) );',
        ],
    },
    'DsaVertexArrayOffsetCommands6' : {
        'entries' : [ 'glVertexArray(VertexAttribI)OffsetEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );'
            '_context->dsa->DsaBuffer( _context, ${arg1} );',
            '_dispatch.call(&_dispatch.gl${m1}Pointer)( ${arg2}, ${arg3}, ${arg4}, ${arg5}, reinterpret_cast<const GLvoid *>(${arg6}) );',
        ],
    },
    'DsaVertexArrayOffsetCommands6EXT' : {
        'entries' : [ 'glVertexArray(MultiTexCoord)OffsetEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );'
            '_context->dsa->DsaBuffer( _context, ${arg1} );',
            '_dispatch.call(&_dispatch.gl${m1}PointerEXT)( ${arg2}, ${arg3}, ${arg4}, ${arg5}, reinterpret_cast<const GLvoid *>(${arg6}) );',
        ],
    },
    'DsaVertexArrayOffsetCommands7' : {
        'entries' : [ 'glVertexArray(VertexAttrib)OffsetEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );'
            '_context->dsa->DsaBuffer( _context, ${arg1} );',
            '_dispatch.call(&_dispatch.gl${m1}Pointer)( ${arg2}, ${arg3}, ${arg4}, ${arg5}, ${arg6}, reinterpret_cast<const GLvoid *>(${arg7}) );',
        ],
    },
    'Delete' : {
        'entries' : [
          'glDelete(Buffers)(ARB|)',
          'glDelete(Framebuffers)(EXT|OES|)',
          'glDelete(Renderbuffers)(EXT|OES|)',
          'glDelete(Program)',
          'glDelete(Programs)(ARB|NV)',
          'glDelete(Textures)(EXT|)',
          'glDelete(VertexArrays)(APPLE|OES|)',
        ],
        'subst' : {
            'method' : {
                '.*Program$'                 : 'GlslProgram',
                '.*Programs(ARB|NV)'         : 'AsmPrograms',
                '.*VertexArrays(APPLE|OES|)' : 'Vaos',
                'default'                    : '${m1}',
            },
        },
        'prefix' : [ '_context->dsa->Delete${method}( _context, ${arg0plus} );', ],
    },
    'VertexArrayEnable' : {
        'entries' : [ 'gl(Enable|Disable)VertexArrayEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );',
            'if ( ${arg1} >= GL_TEXTURE0 && ${arg1} <= GL_TEXTURE31) {',
            '  _context->dsa->DsaClientActiveTexture( _context, ${arg1} );',
            '  _dispatch.call(&_dispatch.gl${m1}ClientState)( GL_TEXTURE_COORD_ARRAY );',
            '} else {',
            '  _dispatch.call(&_dispatch.gl${m1}ClientState)( ${arg1} );',
            '}'
        ],
    },
    'VertexArrayAttribEnable' : {
        'entries' : [ 'gl(Enable|Disable)VertexArrayAttribEXT' ],
        'impl' : [
            '_context->dsa->DsaVao( _context, ${arg0} );',
            '_dispatch.call(&_dispatch.gl${m1}VertexAttribArray)( ${arg1} );'
        ],
    },
}

