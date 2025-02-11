/**
 *  Copyright Notice:
 *  Copyright 2021 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_responder_lib.h"

spdm_get_encapsulated_request_request_t m_libspdm_encapsulated_request_t1 = {
    {SPDM_MESSAGE_VERSION_11, SPDM_GET_ENCAPSULATED_REQUEST, 0, 0}
};
size_t m_libspdm_encapsulated_request_t1_size = sizeof(m_libspdm_encapsulated_request_t1);

spdm_deliver_encapsulated_response_request_t m_libspdm_m_deliver_encapsulated_response_request_t1 =
{
    {SPDM_MESSAGE_VERSION_11, SPDM_DELIVER_ENCAPSULATED_RESPONSE, 0, 0}
};
size_t m_libspdm_m_deliver_encapsulated_response_request_t1_size =
    sizeof(m_libspdm_m_deliver_encapsulated_response_request_t1);

static uint8_t m_libspdm_local_certificate_chain[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];

#if LIBSPDM_ENABLE_CAPABILITY_CERT_CAP

void libspdm_test_get_response_encapsulated_request_case1(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_context_t *spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t data_size;
    void *data;
    size_t response_size;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;

    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;
    spdm_context->encap_context.current_request_op_code = 0;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_GET_DIGESTS;
    spdm_context->encap_context.request_id = 0;

    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_request(spdm_context,
                                                       m_libspdm_encapsulated_request_t1_size,
                                                       &m_libspdm_encapsulated_request_t1,
                                                       &response_size,
                                                       response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_encapsulated_request_response_t) + sizeof(spdm_digest_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ENCAPSULATED_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param1,
                     0x1);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
    free(data);
}

void libspdm_test_get_response_encapsulated_request_case2(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_context_t *spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t data_size;
    void *data;
    size_t response_size;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x2;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;
    spdm_context->encap_context.current_request_op_code = 0;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_GET_CERTIFICATE;
    spdm_context->encap_context.request_id = 0;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    response_size = sizeof(response);

    status = libspdm_get_response_encapsulated_request(spdm_context,
                                                       m_libspdm_encapsulated_request_t1_size,
                                                       &m_libspdm_encapsulated_request_t1,
                                                       &response_size,
                                                       response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_encapsulated_request_response_t) +
                     sizeof(spdm_certificate_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ENCAPSULATED_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param1,
                     0x1);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
    free(data);
}

void libspdm_test_get_response_encapsulated_request_case3(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    uint8_t m_local_certificate_chain[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x3;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NORMAL;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;

    spdm_context->encap_context.current_request_op_code = 0;
    spdm_context->encap_context.request_id = 0;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->local_context.local_cert_chain_provision[0] = m_local_certificate_chain;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;
    libspdm_set_mem(m_local_certificate_chain, LIBSPDM_MAX_MESSAGE_BUFFER_SIZE, (uint8_t)(0xFF));
    spdm_context->local_context.slot_count = 1;

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_request(spdm_context,
                                                       m_libspdm_encapsulated_request_t1_size,
                                                       &m_libspdm_encapsulated_request_t1,
                                                       &response_size,
                                                       response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response_tequester->header.param1,
                     SPDM_ERROR_CODE_UNEXPECTED_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
}

void libspdm_test_get_response_encapsulated_request_case4(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    uint8_t m_local_certificate_chain[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x4;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NEED_RESYNC;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;

    spdm_context->encap_context.current_request_op_code = SPDM_GET_DIGESTS;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CERT_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->local_context.local_cert_chain_provision[0] = m_local_certificate_chain;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;
    libspdm_set_mem(m_local_certificate_chain, LIBSPDM_MAX_MESSAGE_BUFFER_SIZE, (uint8_t)(0xFF));
    spdm_context->local_context.slot_count = 1;

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_request(spdm_context,
                                                       m_libspdm_encapsulated_request_t1_size,
                                                       &m_libspdm_encapsulated_request_t1,
                                                       &response_size,
                                                       response);
    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response_tequester->header.param1,
                     SPDM_ERROR_CODE_REQUEST_RESYNCH);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
}
#endif /* LIBSPDM_ENABLE_CAPABILITY_CERT_CAP*/
#if LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP
void libspdm_test_get_response_encapsulated_request_case5(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_context_t *spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t data_size;
    void *data;
    size_t response_size;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x5;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags = 0;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;
    spdm_context->encap_context.current_request_op_code = 0;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_CHALLENGE;
    spdm_context->encap_context.request_id = 0;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_request(spdm_context,
                                                       m_libspdm_encapsulated_request_t1_size,
                                                       &m_libspdm_encapsulated_request_t1,
                                                       &response_size,
                                                       response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_encapsulated_request_response_t) +
                     sizeof(spdm_challenge_request_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ENCAPSULATED_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param1,
                     0x1);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
    free(data);
}

#endif /* LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP*/

void libspdm_test_get_response_encapsulated_request_case6(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_context_t *spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t data_size;
    void *data;
    size_t response_size;
    uint32_t session_id;
    libspdm_session_info_t *session_info;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_KEY_UPD_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_KEY_UPD_CAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;
    spdm_context->encap_context.current_request_op_code = 0;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_KEY_UPDATE;
    spdm_context->encap_context.request_id = 0;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    session_id = 0xFFFFFFFF;
    spdm_context->latest_session_id = session_id;
    spdm_context->last_spdm_request_session_id_valid = true;
    spdm_context->last_spdm_request_session_id = session_id;
    session_info = &spdm_context->session_info[0];
    libspdm_session_info_init(spdm_context, session_info, session_id, true);
    libspdm_secured_message_set_session_state(session_info->secured_message_context,
                                              LIBSPDM_SESSION_STATE_ESTABLISHED);

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_request(spdm_context,
                                                       m_libspdm_encapsulated_request_t1_size,
                                                       &m_libspdm_encapsulated_request_t1,
                                                       &response_size,
                                                       response);

    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_encapsulated_request_response_t) +
                     sizeof(spdm_key_update_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ENCAPSULATED_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param1,
                     0x1);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
    free(data);
}

void libspdm_test_get_response_encapsulated_response_ack_case1(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    spdm_deliver_encapsulated_response_request_t *spdm_response_tequester;
    spdm_digest_response_t *spdm_response_tequester_digest;
    uint8_t temp_buf[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_tequester_size;
    size_t spdm_response_tequester_digest_size;
    size_t data_size;
    size_t response_size;
    uint8_t *digest;
    void *data;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;

    spdm_context->encap_context.current_request_op_code = SPDM_GET_DIGESTS;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_GET_DIGESTS;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    spdm_response_tequester_size = sizeof(spdm_digest_response_t) +
                                   libspdm_get_hash_size(m_libspdm_use_hash_algo) *
                                   SPDM_MAX_SLOT_COUNT +
                                   sizeof(spdm_deliver_encapsulated_response_request_t);

    spdm_response_tequester = (void *)temp_buf;
    libspdm_copy_mem(spdm_response_tequester,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                     &m_libspdm_m_deliver_encapsulated_response_request_t1,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size);

    spdm_response_tequester_digest_size = sizeof(spdm_digest_response_t) +
                                          libspdm_get_hash_size(m_libspdm_use_hash_algo) *
                                          SPDM_MAX_SLOT_COUNT;
    spdm_response_tequester_digest =
        (void *)(temp_buf + sizeof(spdm_deliver_encapsulated_response_request_t));
    spdm_response_tequester_digest->header.spdm_version = SPDM_MESSAGE_VERSION_11;
    spdm_response_tequester_digest->header.param1 = 0;
    spdm_response_tequester_digest->header.request_response_code = SPDM_DIGESTS;
    spdm_response_tequester_digest->header.param2 = 0;
    digest = (void *)(spdm_response_tequester_digest + 1);

    /*send all eight certchains digest
     * but only No.7 is right*/
    digest += libspdm_get_hash_size(m_libspdm_use_hash_algo) * (SPDM_MAX_SLOT_COUNT - 2);
    libspdm_hash_all(m_libspdm_use_hash_algo, m_libspdm_local_certificate_chain,
                     LIBSPDM_MAX_MESSAGE_BUFFER_SIZE, &digest[0]);
    spdm_response_tequester_digest->header.param2 |= (0xFF << 0);

    libspdm_set_mem(m_libspdm_local_certificate_chain, LIBSPDM_MAX_MESSAGE_BUFFER_SIZE,
                    (uint8_t)(0xFF));

    libspdm_copy_mem(spdm_response_tequester + 1,
                     spdm_response_tequester_digest_size,
                     spdm_response_tequester_digest,
                     spdm_response_tequester_digest_size);

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            spdm_response_tequester_size,
                                                            spdm_response_tequester, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    free(data);
}

void libspdm_test_get_response_encapsulated_response_ack_case2(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    spdm_deliver_encapsulated_response_request_t *spdm_response_tequester;
    spdm_certificate_response_t *spdm_response_tequester_certificate;
    uint8_t temp_buf[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_tequester_size;
    size_t data_size;
    size_t response_size;
    void *data;
    uint16_t portion_length;
    uint16_t remainder_length;
    static size_t calling_index = 0;

    static void *libspdm_local_certificate_chain;
    static size_t libspdm_local_certificate_chain_size;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;

    spdm_context->encap_context.current_request_op_code = SPDM_GET_CERTIFICATE;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_GET_CERTIFICATE;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    spdm_response_tequester = (void *)temp_buf;
    libspdm_copy_mem(spdm_response_tequester,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                     &m_libspdm_m_deliver_encapsulated_response_request_t1,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size);

    spdm_response_tequester_certificate =
        (void *)(temp_buf + sizeof(spdm_deliver_encapsulated_response_request_t));

    libspdm_read_responder_public_certificate_chain(
        m_libspdm_use_hash_algo, m_libspdm_use_asym_algo,
        &libspdm_local_certificate_chain,
        &libspdm_local_certificate_chain_size, NULL, NULL);

    portion_length = LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN;
    remainder_length =
        (uint16_t)(libspdm_local_certificate_chain_size -
                   LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN *
                   (calling_index + 1));

    spdm_response_tequester_certificate->header.spdm_version = SPDM_MESSAGE_VERSION_11;
    spdm_response_tequester_certificate->header.request_response_code = SPDM_CERTIFICATE;
    spdm_response_tequester_certificate->header.param1 = 0;
    spdm_response_tequester_certificate->header.param2 = 0;
    spdm_response_tequester_certificate->portion_length = portion_length;
    spdm_response_tequester_certificate->remainder_length = remainder_length;

    libspdm_copy_mem(spdm_response_tequester_certificate + 1,
                     sizeof(temp_buf) - sizeof(*spdm_response_tequester_certificate),
                     (uint8_t *)libspdm_local_certificate_chain +
                     LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN * calling_index,
                     portion_length);

    free(libspdm_local_certificate_chain);

    response_size = sizeof(response);
    spdm_response_tequester_size = sizeof(spdm_certificate_response_t) + portion_length +
                                   sizeof(spdm_deliver_encapsulated_response_request_t);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            spdm_response_tequester_size,
                                                            spdm_response_tequester, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    free(data);
}

void libspdm_test_get_response_encapsulated_response_ack_case3(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    spdm_deliver_encapsulated_response_request_t *spdm_response_tequester;
    spdm_key_update_response_t *spdm_response_tequester_key_update;
    uint8_t temp_buf[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_tequester_size;
    size_t data_size;
    size_t response_size;
    void *data;
    uint32_t session_id;
    libspdm_session_info_t *session_info;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    spdm_context->last_spdm_request_session_id_valid = true;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;

    spdm_context->encap_context.current_request_op_code = SPDM_KEY_UPDATE;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_KEY_UPDATE;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;

    libspdm_reset_message_b(spdm_context);

    session_id = 0xFFFFFFFF;
    spdm_context->latest_session_id = session_id;
    spdm_context->last_spdm_request_session_id_valid = true;
    spdm_context->last_spdm_request_session_id = session_id;
    session_info = &spdm_context->session_info[0];
    libspdm_session_info_init(spdm_context, session_info, session_id, true);
    libspdm_secured_message_set_session_state(session_info->secured_message_context,
                                              LIBSPDM_SESSION_STATE_ESTABLISHED);

    spdm_context->encap_context.last_encap_request_header.spdm_version = SPDM_MESSAGE_VERSION_11;
    spdm_context->encap_context.last_encap_request_header.request_response_code =
        SPDM_KEY_UPDATE_ACK;
    spdm_context->encap_context.last_encap_request_header.param1 =
        SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY;
    spdm_context->encap_context.last_encap_request_header.param2 = 0;

    spdm_response_tequester = (void *)temp_buf;
    libspdm_copy_mem(spdm_response_tequester,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                     &m_libspdm_m_deliver_encapsulated_response_request_t1,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size);

    spdm_response_tequester_key_update =
        (void *)(temp_buf + sizeof(spdm_deliver_encapsulated_response_request_t));

    spdm_response_tequester_key_update->header.spdm_version = SPDM_MESSAGE_VERSION_11;
    spdm_response_tequester_key_update->header.request_response_code =
        SPDM_KEY_UPDATE_ACK;
    spdm_response_tequester_key_update->header.param1 =
        SPDM_KEY_UPDATE_OPERATIONS_TABLE_UPDATE_KEY;
    spdm_response_tequester_key_update->header.param2 = 0;

    response_size = sizeof(response);
    spdm_response_tequester_size = sizeof(spdm_key_update_response_t) +
                                   sizeof(spdm_deliver_encapsulated_response_request_t);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            spdm_response_tequester_size,
                                                            spdm_response_tequester, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    free(data);
}

void libspdm_test_get_response_encapsulated_response_ack_case4(void **State)
{
    return_status status;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;
    spdm_context->encap_context.current_request_op_code = 0;
    libspdm_reset_message_b(spdm_context);

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                                                            &m_libspdm_m_deliver_encapsulated_response_request_t1, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response_tequester->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
}

void libspdm_test_get_response_encapsulated_response_ack_case5(void **State)
{
    return_status status;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t response_size;
    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NORMAL;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;
    spdm_context->encap_context.current_request_op_code = 0;

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                                                            &m_libspdm_m_deliver_encapsulated_response_request_t1, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response_tequester->header.param1,
                     SPDM_ERROR_CODE_UNEXPECTED_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
}

void libspdm_test_get_response_encapsulated_response_ack_case6(void **State)
{
    return_status status;
    spdm_encapsulated_request_response_t *spdm_response_tequester;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t response_size;
    size_t data_size;
    void *data;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NEED_RESYNC;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;

    status = libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                             m_libspdm_use_asym_algo, &data,
                                                             &data_size,
                                                             NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;

    response_size = sizeof(response);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                                                            &m_libspdm_m_deliver_encapsulated_response_request_t1, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response_tequester->header.param1,
                     SPDM_ERROR_CODE_REQUEST_RESYNCH);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
}

void libspdm_test_get_response_encapsulated_response_ack_case7(void **State)
{
    return_status status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    spdm_deliver_encapsulated_response_request_t *spdm_response_tequester;
    spdm_certificate_response_t *spdm_response_tequester_certificate;
    uint8_t temp_buf[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    size_t spdm_response_tequester_size;
    size_t data_size;
    size_t response_size;
    void *data;
    uint16_t portion_length;
    uint16_t remainder_length;
    static size_t calling_index = 0;

    static void *libspdm_local_certificate_chain;
    static size_t libspdm_local_certificate_chain_size;

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    spdm_context->encap_context.request_id = 0xFF;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_PROCESSING_ENCAP;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_ENCAP_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_ENCAP_CAP;
    spdm_context->encap_context.request_op_code_count =
        LIBSPDM_MAX_ENCAP_REQUEST_OP_CODE_SEQUENCE_COUNT;

    spdm_context->encap_context.current_request_op_code = SPDM_GET_CERTIFICATE;
    spdm_context->encap_context.request_op_code_sequence[0] = SPDM_GET_CERTIFICATE;

    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |= SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data,
                                                    &data_size,
                                                    NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size;
    spdm_context->local_context.local_cert_chain_provision[0] = data;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    libspdm_reset_message_b(spdm_context);

    spdm_response_tequester = (void *)temp_buf;
    libspdm_copy_mem(spdm_response_tequester,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size,
                     &m_libspdm_m_deliver_encapsulated_response_request_t1,
                     m_libspdm_m_deliver_encapsulated_response_request_t1_size);

    spdm_response_tequester_certificate =
        (void *)(temp_buf + sizeof(spdm_deliver_encapsulated_response_request_t));

    libspdm_read_responder_public_certificate_chain(
        m_libspdm_use_hash_algo, m_libspdm_use_asym_algo,
        &libspdm_local_certificate_chain,
        &libspdm_local_certificate_chain_size, NULL, NULL);

    portion_length = LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN;
    remainder_length =
        (uint16_t)(libspdm_local_certificate_chain_size -
                   LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN *
                   (calling_index + 1));

    spdm_response_tequester_certificate->header.spdm_version = SPDM_MESSAGE_VERSION_11;
    spdm_response_tequester_certificate->header.request_response_code = SPDM_CERTIFICATE;
    spdm_response_tequester_certificate->header.param1 = 0;
    spdm_response_tequester_certificate->header.param2 = 0;
    spdm_response_tequester_certificate->portion_length = portion_length;
    spdm_response_tequester_certificate->remainder_length = remainder_length;

    libspdm_copy_mem(spdm_response_tequester_certificate + 1,
                     sizeof(temp_buf) - sizeof(*spdm_response_tequester_certificate),
                     (uint8_t *)libspdm_local_certificate_chain +
                     LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN * calling_index,
                     portion_length);

    free(libspdm_local_certificate_chain);

    response_size = sizeof(response);
    spdm_response_tequester_size = sizeof(spdm_certificate_response_t) + portion_length +
                                   sizeof(spdm_deliver_encapsulated_response_request_t);
    status = libspdm_get_response_encapsulated_response_ack(spdm_context,
                                                            spdm_response_tequester_size,
                                                            spdm_response_tequester, &response_size,
                                                            response);
    assert_int_equal(status, RETURN_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response_tequester = (void *)response;
    assert_int_equal(spdm_response_tequester->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response_tequester->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response_tequester->header.param2, 0);
    free(data);
}

libspdm_test_context_t m_libspdm_response_encapsulated_request_test_context = {
    LIBSPDM_TEST_CONTEXT_SIGNATURE,
    false,
};

int libspdm_responder_encapsulated_response_test_main(void)
{

    const struct CMUnitTest spdm_responder_encapsulated_response_tests[] = {
#if LIBSPDM_ENABLE_CAPABILITY_CERT_CAP
        /*Success Case request_op_code_sequence: SPDM_GET_DIGESTS*/
        cmocka_unit_test(libspdm_test_get_response_encapsulated_request_case1),
        /*Success Case current_request_op_code: SPDM_GET_CERTIFICATE */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_request_case2),
        /*response_state : LIBSPDM_RESPONSE_STATE_NORMAL */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_request_case3),
        /*response_state : LIBSPDM_RESPONSE_STATE_NEED_RESYNC */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_request_case4),
#endif
#if LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP
        /*Success Case current_request_op_code: SPDM_CHALLENGE */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_request_case5),
#endif
        /*Success Case current_request_op_code: SPDM_KEY_UPDATE */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_request_case6),
#if LIBSPDM_ENABLE_CAPABILITY_CERT_CAP
        /*Success Case current_request_op_code: SPDM_GET_DIGESTS*/
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case1),
        /*Success Case current_request_op_code: SPDM_GET_CERTIFICATE*/
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case2),
#endif
        /*Success Case current_request_op_code: SPDM_KEY_UPDATE */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case3),
        /* current_request_op_code: NULL */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case4),
        /*response_state : LIBSPDM_RESPONSE_STATE_NORMAL */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case5),
        /*response_state : LIBSPDM_RESPONSE_STATE_NEED_RESYNC */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case6),
        /*spdm_request->header.param1 != spdm_context->encap_context.request_id */
        cmocka_unit_test(libspdm_test_get_response_encapsulated_response_ack_case7),
    };

    libspdm_setup_test_context(&m_libspdm_response_encapsulated_request_test_context);

    return cmocka_run_group_tests(spdm_responder_encapsulated_response_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}
