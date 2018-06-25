/*
 * Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not use
 * this file except in compliance with the License. A copy of the License is
 * located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Invalid CMMs for testing virtual function error handling.
 */

#include "bad_cmm.h"
#include <stdlib.h>

struct bad_cmm {const struct aws_cryptosdk_cmm_vt * vt;};

int aws_cryptosdk_cmm_destroy_with_failed_return_value(struct aws_cryptosdk_cmm * cmm) {
    aws_cryptosdk_cmm_destroy(cmm);
    return AWS_OP_ERR;
}

/**
 * VFs which should never get called because of the failed check on the vt_size field.
 */
void destroy_abort(struct aws_cryptosdk_cmm * cmm) {
    abort();
}

int generate_abort(struct aws_cryptosdk_cmm * cmm,
                   struct aws_cryptosdk_encryption_materials ** output,
                   struct aws_cryptosdk_encryption_request * request) {
    abort();
    return AWS_OP_SUCCESS;
}

int decrypt_abort(struct aws_cryptosdk_cmm * cmm,
                  struct aws_cryptosdk_decryption_materials ** output,
                  struct aws_cryptosdk_decryption_request * request) {
    abort();
    return AWS_OP_SUCCESS;
}

/**
 * A totally correct VT except for the zero size.
 */
static const struct aws_cryptosdk_cmm_vt zero_size_cmm_vt = {
    .vt_size = 0,
    .name = "zero size cmm",
    .destroy = destroy_abort,
    .generate_encryption_materials = generate_abort,
    .decrypt_materials = decrypt_abort
};

static struct bad_cmm zero_size_cmm_singleton = {.vt = &zero_size_cmm_vt};
static struct aws_cryptosdk_cmm * zero_size_cmm = (struct aws_cryptosdk_cmm *) &zero_size_cmm_singleton;

struct aws_cryptosdk_cmm * aws_cryptosdk_zero_size_cmm_new() {return zero_size_cmm;}

static const struct aws_cryptosdk_cmm_vt null_cmm_vt = {
    .vt_size = sizeof(struct aws_cryptosdk_cmm_vt),
    .name = "null cmm",
    .destroy = NULL,
    .generate_encryption_materials = NULL,
    .decrypt_materials = NULL
};

static struct bad_cmm null_cmm_singleton = {.vt = &null_cmm_vt};
static struct aws_cryptosdk_cmm * null_cmm = (struct aws_cryptosdk_cmm *) &null_cmm_singleton;

struct aws_cryptosdk_cmm * aws_cryptosdk_null_cmm_new() {return null_cmm;}
