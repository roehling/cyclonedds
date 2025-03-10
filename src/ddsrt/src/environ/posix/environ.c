// Copyright(c) 2006 to 2020 ZettaScale Technology and others
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License v. 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
// v. 1.0 which is available at
// http://www.eclipse.org/org/documents/edl-v10.php.
//
// SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dds/ddsrt/environ.h"
#include "dds/ddsrt/retcode.h"

static int
isenvvar(const char *name)
{
  return (*name == '\0' || strchr(name, '=') != NULL) == 0;
}

dds_return_t
ddsrt_getenv(const char *name, const char **value)
{
  char *env;

  assert(name != NULL);
  assert(value != NULL);

  if (!isenvvar(name))
    return DDS_RETCODE_BAD_PARAMETER;
  if ((env = getenv(name)) != NULL) {
    *value = env;
    return DDS_RETCODE_OK;
  }
  return DDS_RETCODE_NOT_FOUND;
}

dds_return_t
ddsrt_setenv(const char *name, const char *value)
{
  assert(name != NULL);
  assert(value != NULL);

  if (strlen(value) == 0)
    return ddsrt_unsetenv(name);
  if (!isenvvar(name))
    return DDS_RETCODE_BAD_PARAMETER;
  if (setenv(name, value, 1) == 0)
    return DDS_RETCODE_OK;

  switch (errno) {
    case EINVAL:
      return DDS_RETCODE_BAD_PARAMETER;
    case ENOMEM:
      return DDS_RETCODE_OUT_OF_RESOURCES;
    default:
      break;
  }

  return DDS_RETCODE_ERROR;
}

dds_return_t
ddsrt_unsetenv(const char *name)
{
  assert(name != NULL);

  if (!isenvvar(name))
    return DDS_RETCODE_BAD_PARAMETER;
  if (unsetenv(name) == 0)
    return DDS_RETCODE_OK;

  switch (errno) {
    case EINVAL:
      return DDS_RETCODE_BAD_PARAMETER;
    case ENOMEM:
      return DDS_RETCODE_OUT_OF_RESOURCES;
    default:
      break;
  }

  return DDS_RETCODE_ERROR;
}
