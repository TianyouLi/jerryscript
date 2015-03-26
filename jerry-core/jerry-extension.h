/* Copyright 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JERRY_EXTENSION_H
#define JERRY_EXTENSION_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
# define EXTERN_C "C"
#else /* !__cplusplus */
# define EXTERN_C
#endif /* !__cplusplus */

/** \addtogroup jerry Jerry engine extension interface
 * @{
 */

/**
 * Jerry's extension-related data types
 */
typedef enum
{
  JERRY_EXTENSION_FIELD_TYPE_BOOLEAN, /**< bool */
  JERRY_EXTENSION_FIELD_TYPE_FLOAT32, /**< 32-bit float */
  JERRY_EXTENSION_FIELD_TYPE_FLOAT64, /**< 64-bit float */
  JERRY_EXTENSION_FIELD_TYPE_UINT32, /**< number converted to 32-bit unsigned integer*/
  JERRY_EXTENSION_FIELD_TYPE_STRING, /**< string */
  JERRY_EXTENSION_FIELD_TYPE_OBJECT  /**< object */
} jerry_extension_data_type_t;

/**
 * An interface for Jerry's string value
 */
typedef struct ecma_string_t jerry_string_t;

/**
 * An interface for Jerry's object value
 */
typedef struct ecma_object_t jerry_object_t;

/**
 * Description of an extension object's fields
 */
typedef struct
{
  const char *field_name_p; /**< field name */

  const jerry_extension_data_type_t type; /**< field data type */

  /**
   * Value description
   */
  union
  {
    const char* v_string; /**< string */
    bool v_bool; /**< boolean */
    float v_float32; /**< 32-bit float */
    double v_float64; /**< 64-bit float */
    uint32_t v_uint32; /**< 32-bit unsigned integer */
  };
} jerry_extension_field_t;

/**
 * Description of an extension function's argument
 */
typedef struct
{
  const jerry_extension_data_type_t type; /**< argument data type */

  union
  {
    bool v_bool; /**< boolean */

    float v_float32; /**< 32-bit float */
    double v_float64; /**< 64-bit float */

    uint32_t v_uint32; /**< number converted 32-bit unsigned integer */

    union
    {
      jerry_string_t *v_string; /**< pointer to a JS string */
      jerry_object_t *v_object; /**< pointer to a JS object */
    };
  };
} jerry_extension_function_arg_t;

/**
 * Pointer to extension function implementation
 */
typedef void (*jerry_extension_function_pointer_t) (const struct jerry_extension_function_t *function_block_p);

/**
 * Description of an extension object's function
 */
typedef struct jerry_extension_function_t
{
  const char* function_name_p; /**< name of function */

  jerry_extension_function_pointer_t function_wrapper_p; /**< pointer to function implementation */

  jerry_extension_function_arg_t *args_p; /**< arrays of the function's arguments */
  uint32_t args_number; /**< number of arguments */
} jerry_extension_function_t;

/**
 * Description of an extention object
 */
typedef struct jerry_extension_descriptor_t
{
  const uint32_t fields_count; /**< number of fields */
  const uint32_t functions_count; /**< number of functions */

  const jerry_extension_field_t* const fields_p; /**< array of field descriptor */
  const jerry_extension_function_t* const functions_p; /**< array of function descriptors */

  const char* const name_p; /**< name of the extension */
  struct jerry_extension_descriptor_t *next_p; /**< next descriptor in list of registered extensions */
  uint32_t index; /**< global index of the extension among registered exceptions */
} jerry_extension_descriptor_t;

extern EXTERN_C bool
jerry_extend_with (jerry_extension_descriptor_t *desc_p);

extern EXTERN_C ssize_t
jerry_string_to_char_buffer (const jerry_string_t *string_p,
                             char *buffer_p,
                             ssize_t buffer_size);
extern EXTERN_C jerry_string_t* jerry_acquire_string (jerry_string_t *string_p);
extern EXTERN_C void jerry_release_string (jerry_string_t *string_p);
extern EXTERN_C jerry_object_t* jerry_acquire_object (jerry_object_t *object_p);
extern EXTERN_C void jerry_release_object (jerry_object_t *object_p);

/**
 * @}
 */

#endif /* !JERRY_EXTENSION_H */