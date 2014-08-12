# we need to exclude these functions from the dispatch becuse they do not exist in apitrace
exclude = [

  'glMatrixIndexPointerOES',
  'glWeightPointerOES',

  'glXBindTexImageATI',
  'glXBlitContextFramebufferAMD',
  'glXChooseFBConfigSGIX',
  'glXCreateAssociatedContextAMD',
  'glXCreateAssociatedContextAttribsAMD',
  'glXDeleteAssociatedContextAMD',
  'glXDrawableAttribATI',
  'glXGetContextGPUIDAMD',
  'glXGetCurrentAssociatedContextAMD',
  'glXGetGPUIDsAMD',
  'glXGetGPUInfoAMD',
  'glXGetTransparentIndexSUN',
  'glXGetVideoResizeSUN',
  'glXMakeAssociatedContextCurrentAMD',
  'glXReleaseTexImageATI',
  'glXVideoResizeSUN',

  'wglChoosePixelFormatARB',
  'wglEnumGpuDevicesNV',
  'wglGetDefaultProcAddress',
  'wglGetPixelFormatAttribfvARB',
  'wglGetPixelFormatAttribfvEXT',
  'wglGetPixelFormatAttribivARB',
  'wglGetPixelFormatAttribivEXT',
  'wglGetProcAddress',

  'EGL_KHR_stream_consumer_gltexture',
  'EGL_KHR_stream_cross_process_fd',
  'EGL_KHR_stream_producer_eglsurface',
  'EGL_KHR_wait_sync',
  'EGL_NV_coverage_sample',

  'GL_AMD_interleaved_elements',
  'GL_AMD_stencil_operation_extended',

  'GL_ANGLE_instanced_arrays',
  'GL_ANGLE_timer_query',
  'GL_ANGLE_translated_shader_source',

  'GL_APPLE_copy_texture_levels',
  'GL_APPLE_sync',

  'GL_ARB_bindless_texture',
  'GL_ARB_cl_event',
  'GL_ARB_compute_variable_group_size',
  'GL_ARB_indirect_parameters',
  'GL_ARB_sparse_texture',

  'GL_EXT_disjoint_timer_query',
  'GL_EXT_fragment_lighting',
  'GL_EXT_map_buffer_range',
  'GL_EXT_multisampled_render_to_texture',
  'GL_EXT_multiview_draw_buffers',
  'GL_EXT_robustness',
  'GL_EXT_scene_marker',
  'GL_EXT_texture_storage',

  'GL_INTEL_map_texture',
  'GL_INTEL_texture_scissor',

  'GL_NV_blend_equation_advanced',
  'GL_NV_blend_equation_advanced_coherent',
  'GL_NV_copy_buffer',
  'GL_NV_non_square_matrices',

  'GL_OES_single_precision',

  'GL_QCOM_alpha_test',
  'GL_QCOM_driver_control',
  'GL_QCOM_extended_get',
  'GL_QCOM_extended_get2',
  'GL_QCOM_tiled_rendering',

  'GL_REGAL_log',

  'GL_SGIX_fog_texture',

  'GL_SUN_read_video_pixels',

  # OpenGL 4.5 additions, August 2014

  'GL_ARB_ES3_1_compatibility',
  'GL_ARB_clip_control',
  'GL_ARB_conditional_render_inverted',
  'GL_ARB_context_flush_control',
  'GL_ARB_cull_distance',
  'GL_ARB_derivative_control',
  'GL_ARB_direct_state_access',
  'GL_ARB_get_texture_sub_image',
  'GL_ARB_pipeline_statistics_query',
  'GL_ARB_shader_texture_image_samples',
  'GL_ARB_sparse_buffer',
  'GL_ARB_texture_barrier',
  'GL_ARB_transform_feedback_overflow_query',
  'GL_EXT_shader_image_load_formatted',
  'GL_EXT_shader_integer_mix',
  'GL_INTEL_fragment_shader_ordering',
  'GL_INTEL_performance_query',
  'GL_KHR_blend_equation_advanced',
  'GL_KHR_blend_equation_advanced_coherent',
  'GL_KHR_robust_buffer_access_behavior',
  'GL_KHR_robustness',
  'GL_KHR_texture_compression_astc_hdr',
  'GL_NV_bindless_multi_draw_indirect_count',
  'GL_NV_shader_atomic_int64',
  'GL_NV_shader_thread_group',
  'GL_NV_shader_thread_shuffle',

  'GLX_MESA_query_renderer',
  'GLX_NV_copy_buffer',
  'GLX_NV_delay_before_swap'
]
