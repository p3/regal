# glslopt.inc
#
# Generic gnumake .inc for building GLSL Optimizer 
#

# Sources

GLSLOPT.CXX :=
GLSLOPT.CXX += src/glsl/src/glsl/ast_array_index.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ast_expr.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ast_function.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ast_to_hir.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ast_type.cpp
GLSLOPT.CXX += src/glsl/src/glsl/builtin_function.cpp
GLSLOPT.CXX += src/glsl/src/glsl/builtin_variables.cpp
GLSLOPT.CXX += src/glsl/src/glsl/glsl_lexer.cpp
GLSLOPT.CXX += src/glsl/src/glsl/glsl_optimizer.cpp
GLSLOPT.CXX += src/glsl/src/glsl/glsl_parser.cpp
GLSLOPT.CXX += src/glsl/src/glsl/glsl_parser_extras.cpp
GLSLOPT.CXX += src/glsl/src/glsl/glsl_symbol_table.cpp
GLSLOPT.CXX += src/glsl/src/glsl/glsl_types.cpp
GLSLOPT.CXX += src/glsl/src/glsl/hir_field_selection.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_basic_block.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_builder.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_clone.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_constant_expression.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_expression_flattening.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_function.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_function_can_inline.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_function_detect_recursion.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_hierarchical_visitor.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_hv_accept.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_import_prototypes.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_print_glsl_visitor.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_print_visitor.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_reader.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_rvalue_visitor.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_unused_structs.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_validate.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ir_variable_refcount.cpp
GLSLOPT.CXX += src/glsl/src/glsl/link_functions.cpp
GLSLOPT.CXX += src/glsl/src/glsl/link_uniform_block_active_visitor.cpp
GLSLOPT.CXX += src/glsl/src/glsl/link_uniform_blocks.cpp
GLSLOPT.CXX += src/glsl/src/glsl/link_uniform_initializers.cpp
GLSLOPT.CXX += src/glsl/src/glsl/link_uniforms.cpp
GLSLOPT.CXX += src/glsl/src/glsl/link_varyings.cpp
GLSLOPT.CXX += src/glsl/src/glsl/linker.cpp
GLSLOPT.CXX += src/glsl/src/glsl/loop_analysis.cpp
GLSLOPT.CXX += src/glsl/src/glsl/loop_controls.cpp
GLSLOPT.CXX += src/glsl/src/glsl/loop_unroll.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_clip_distance.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_discard.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_discard_flow.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_if_to_cond_assign.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_instructions.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_jumps.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_mat_op_to_vec.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_noise.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_packed_varyings.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_variable_index_to_cond_assign.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_vec_index_to_cond_assign.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_vec_index_to_swizzle.cpp
GLSLOPT.CXX += src/glsl/src/glsl/lower_vector.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_algebraic.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_array_splitting.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_constant_folding.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_constant_propagation.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_constant_variable.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_copy_propagation.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_copy_propagation_elements.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_dead_code.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_dead_code_local.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_dead_functions.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_flatten_nested_if_blocks.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_function_inlining.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_if_simplification.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_noop_swizzle.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_redundant_jumps.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_structure_splitting.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_swizzle_swizzle.cpp
GLSLOPT.CXX += src/glsl/src/glsl/opt_tree_grafting.cpp
GLSLOPT.CXX += src/glsl/src/glsl/ralloc.c
GLSLOPT.CXX += src/glsl/src/glsl/s_expression.cpp
GLSLOPT.CXX += src/glsl/src/glsl/standalone_scaffolding.cpp
GLSLOPT.CXX += src/glsl/src/glsl/strtod.c
GLSLOPT.CXX += src/glsl/src/glsl/glcpp/glcpp-lex.c
GLSLOPT.CXX += src/glsl/src/glsl/glcpp/glcpp-parse.c
GLSLOPT.CXX += src/glsl/src/glsl/glcpp/pp.c
GLSLOPT.CXX += src/glsl/src/glsl/../mesa/main/imports.c
GLSLOPT.CXX += src/glsl/src/glsl/../mesa/main/hash_table.c
GLSLOPT.CXX += src/glsl/src/glsl/../mesa/program/symbol_table.c
GLSLOPT.CXX += src/glsl/src/glsl/../mesa/program/prog_hash_table.c

# Internal Headers
GLSLOPT.H :=

# Other Headers
GLSLOPT.H.OTHER :=

#
# Includes 
#

GLSLOPT.INCLUDE :=
GLSLOPT.INCLUDE += -Isrc/glsl/include
GLSLOPT.INCLUDE += -Isrc/glsl/src/glsl
GLSLOPT.INCLUDE += -Isrc/glsl/src/mesa

ifeq ($(PROFILE_OS),Windows)
GLSLOPT.INCLUDES += -Isrc/glsl/include/c99
endif

GLSLOPT.STATIC ?= libglslopt.a
