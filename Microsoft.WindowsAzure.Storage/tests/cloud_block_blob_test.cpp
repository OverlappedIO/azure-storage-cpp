// -----------------------------------------------------------------------------------------
// <copyright file="cloud_block_blob_test.cpp" company="Microsoft">
//    Copyright 2013 Microsoft Corporation
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// </copyright>
// -----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "blob_test_base.h"
#include "check_macros.h"

#pragma region Fixture

utility::string_t block_blob_test_base::get_block_id(uint16_t block_index)
{
    return utility::conversions::to_base64(block_index);
}

void block_blob_test_base::check_block_list_equal(const std::vector<wa::storage::block_list_item>& committed_put_block_list, const std::vector<wa::storage::block_list_item>& uncommitted_put_block_list)
{
    {
        auto get_block_list = m_blob.download_block_list(wa::storage::block_listing_filter::committed, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(committed_put_block_list.size(), get_block_list.size());
        for (size_t i = 0; i < committed_put_block_list.size(); ++i)
        {
            CHECK_UTF8_EQUAL(committed_put_block_list[i].id(), get_block_list[i].id());
            CHECK(wa::storage::block_list_item::block_mode::committed == get_block_list[i].mode());
        }
    }

    {
        auto get_block_list = m_blob.download_block_list(wa::storage::block_listing_filter::uncommitted, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(uncommitted_put_block_list.size(), get_block_list.size());
        for (size_t i = 0; i < uncommitted_put_block_list.size(); ++i)
        {
            CHECK_UTF8_EQUAL(uncommitted_put_block_list[i].id(), get_block_list[i].id());
            CHECK(wa::storage::block_list_item::block_mode::uncommitted == get_block_list[i].mode());
        }
    }

    {
        auto get_block_list = m_blob.download_block_list(wa::storage::block_listing_filter::all, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(committed_put_block_list.size() + uncommitted_put_block_list.size(), get_block_list.size());
        for (size_t i = 0; i < committed_put_block_list.size(); ++i)
        {
            CHECK_UTF8_EQUAL(committed_put_block_list[i].id(), get_block_list[i].id());
            CHECK(wa::storage::block_list_item::block_mode::committed == get_block_list[i].mode());
        }

        for (size_t i = 0; i < uncommitted_put_block_list.size(); ++i)
        {
            CHECK_UTF8_EQUAL(uncommitted_put_block_list[i].id(), get_block_list[committed_put_block_list.size() + i].id());
            CHECK(wa::storage::block_list_item::block_mode::uncommitted == get_block_list[committed_put_block_list.size() + i].mode());
        }
    }
}

#pragma endregion

SUITE(Blob)
{
    TEST_FIXTURE(block_blob_test_base, block_upload)
    {
        std::vector<uint8_t> buffer;
        buffer.resize(16 * 1024);
        wa::storage::blob_request_options options;
        std::vector<wa::storage::block_list_item> uncommitted_blocks;
        std::vector<wa::storage::block_list_item> committed_blocks;

        utility::string_t md5_header;
        m_context.set_sending_request([&md5_header] (web::http::http_request& request, wa::storage::operation_context)
        {
            if (!request.headers().match(web::http::header_names::content_md5, md5_header))
            {
                md5_header.clear();
            }
        });

        options.set_use_transactional_md5(false);
        for (int i = 0; i < 3; ++i)
        {
            fill_buffer_and_get_md5(buffer);
            auto stream = concurrency::streams::bytestream::open_istream(buffer);
            auto block_id = get_block_id(i);
            uncommitted_blocks.push_back(wa::storage::block_list_item(block_id));
            m_blob.upload_block(block_id, stream, utility::string_t(), wa::storage::access_condition(), options, m_context);
            CHECK_UTF8_EQUAL(utility::string_t(), md5_header);
        }

        check_block_list_equal(committed_blocks, uncommitted_blocks);
        std::copy(uncommitted_blocks.begin(), uncommitted_blocks.end(), std::back_inserter(committed_blocks));
        m_blob.upload_block_list(committed_blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        uncommitted_blocks.clear();

        options.set_use_transactional_md5(false);
        for (int i = 3; i < 6; ++i)
        {
            auto md5 = fill_buffer_and_get_md5(buffer);
            auto stream = concurrency::streams::bytestream::open_istream(buffer);
            auto block_id = get_block_id(i);
            uncommitted_blocks.push_back(wa::storage::block_list_item(block_id));
            m_blob.upload_block(block_id, stream, md5, wa::storage::access_condition(), options, m_context);
            CHECK_UTF8_EQUAL(md5, md5_header);
        }

        check_block_list_equal(committed_blocks, uncommitted_blocks);
        std::copy(uncommitted_blocks.begin(), uncommitted_blocks.end(), std::back_inserter(committed_blocks));
        m_blob.upload_block_list(committed_blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        uncommitted_blocks.clear();

        options.set_use_transactional_md5(true);
        for (int i = 6; i < 9; ++i)
        {
            auto md5 = fill_buffer_and_get_md5(buffer);
            auto stream = concurrency::streams::bytestream::open_istream(buffer);
            auto block_id = get_block_id(i);
            uncommitted_blocks.push_back(wa::storage::block_list_item(block_id));
            m_blob.upload_block(block_id, stream, utility::string_t(), wa::storage::access_condition(), options, m_context);
            CHECK_UTF8_EQUAL(md5, md5_header);
        }

        check_block_list_equal(committed_blocks, uncommitted_blocks);
        std::copy(uncommitted_blocks.begin(), uncommitted_blocks.end(), std::back_inserter(committed_blocks));
        m_blob.upload_block_list(committed_blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        uncommitted_blocks.clear();

        options.set_use_transactional_md5(true);
        fill_buffer_and_get_md5(buffer);
        auto stream = concurrency::streams::bytestream::open_istream(buffer);
        CHECK_THROW(m_blob.upload_block(get_block_id(0), stream, dummy_md5, wa::storage::access_condition(), options, m_context), wa::storage::storage_exception);
        CHECK_UTF8_EQUAL(dummy_md5, md5_header);

        check_block_list_equal(committed_blocks, uncommitted_blocks);

        m_context.set_sending_request(std::function<void(web::http::http_request &, wa::storage::operation_context)>());
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload)
    {
        const size_t size = 6 * 1024 * 1024;
        wa::storage::blob_request_options options;

        options.set_store_blob_content_md5(false);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 1, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_use_transactional_md5(true);
        options.set_store_blob_content_md5(true);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 1, true), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_single_blob_upload_threshold_in_bytes(4 * 1024 * 1024);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 3, true), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_store_blob_content_md5(false);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 3, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_stream_write_size_in_bytes(1 * 1024 * 1024);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 7, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_use_transactional_md5(false);
        options.set_single_blob_upload_threshold_in_bytes(6 * 1024 * 1024);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 1, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_parallelism_factor(4);
        options.set_use_transactional_md5(true);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 7, false), 4);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_parallelism_factor(8);
        options.set_store_blob_content_md5(true);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, true, options, 7, true), 6);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload_with_nonseekable)
    {
        const size_t size = 6 * 1024 * 1024;
        wa::storage::blob_request_options options;
        options.set_use_transactional_md5(true);

        options.set_store_blob_content_md5(false);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 3, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_store_blob_content_md5(true);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 3, true), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_single_blob_upload_threshold_in_bytes(4 * 1024 * 1024);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 3, true), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_store_blob_content_md5(false);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 3, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_stream_write_size_in_bytes(1 * 1024 * 1024);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 7, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_single_blob_upload_threshold_in_bytes(6 * 1024 * 1024);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 7, false), 1);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_parallelism_factor(4);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 7, false), 4);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());

        options.set_parallelism_factor(8);
        options.set_store_blob_content_md5(true);
        check_parallelism(upload_and_download(m_blob, size, 0, 0, false, options, 7, true), 6);
        m_blob.delete_blob();
        m_blob.properties().set_content_md5(utility::string_t());
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload_with_size)
    {
        const size_t buffer_size = 6 * 1024 * 1024;
        const size_t blob_size = 4 * 1024 * 1024;
        wa::storage::blob_request_options options;

        const size_t buffer_offsets[2] = { 0, 1024 };
        for (auto buffer_offset : buffer_offsets)
        {
            options.set_stream_write_size_in_bytes(blob_size);
            options.set_use_transactional_md5(false);
            options.set_store_blob_content_md5(false);
            options.set_parallelism_factor(1);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, true, options, 1, false), 1);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());

            options.set_use_transactional_md5(true);
            options.set_store_blob_content_md5(true);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, true, options, 1, true), 1);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());

            options.set_stream_write_size_in_bytes(1 * 1024 * 1024);
            options.set_store_blob_content_md5(false);
            options.set_parallelism_factor(4);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, true, options, 5, false), 4);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());

            options.set_parallelism_factor(8);
            options.set_store_blob_content_md5(true);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, true, options, 5, true), 4);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());
        }
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload_with_size_with_nonseekable)
    {
        const size_t buffer_size = 6 * 1024 * 1024;
        const size_t blob_size = 4 * 1024 * 1024;
        wa::storage::blob_request_options options;

        const size_t buffer_offsets[2] = { 0, 1024 };
        for (auto buffer_offset : buffer_offsets)
        {
            options.set_stream_write_size_in_bytes(blob_size);
            options.set_use_transactional_md5(false);
            options.set_store_blob_content_md5(false);
            options.set_parallelism_factor(1);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, false, options, 1, false), 1);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());

            options.set_use_transactional_md5(true);
            options.set_store_blob_content_md5(true);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, false, options, 1, true), 1);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());

            options.set_stream_write_size_in_bytes(1 * 1024 * 1024);
            options.set_store_blob_content_md5(false);
            options.set_parallelism_factor(4);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, false, options, 5, false), 4);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());

            options.set_parallelism_factor(8);
            options.set_store_blob_content_md5(true);
            check_parallelism(upload_and_download(m_blob, buffer_size, buffer_offset, blob_size, false, options, 5, true), 4);
            m_blob.delete_blob();
            m_blob.properties().set_content_md5(utility::string_t());
        }
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload_with_invalid_size)
    {
        const size_t buffer_size = 2 * 1024 * 1024;
        wa::storage::blob_request_options options;
        options.set_store_blob_content_md5(false);
        CHECK_THROW(upload_and_download(m_blob, buffer_size, 0, buffer_size + 1, true, options, 0, false), wa::storage::storage_exception);
        CHECK_THROW(upload_and_download(m_blob, buffer_size, 0, buffer_size + 1, false, options, 0, false), std::invalid_argument);
        CHECK_THROW(upload_and_download(m_blob, buffer_size, 1024, buffer_size - 1023, true, options, 0, false), wa::storage::storage_exception);
        CHECK_THROW(upload_and_download(m_blob, buffer_size, 1024, buffer_size - 1023, false, options, 0, false), std::invalid_argument);
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_text_upload)
    {
        wa::storage::blob_request_options options;
        options.set_store_blob_content_md5(true);

        utility::string_t text;
        m_blob.upload_text(text, wa::storage::access_condition(), options, m_context);
        CHECK(m_blob.download_text(wa::storage::access_condition(), options, m_context).empty());

        text = U("test");
        m_blob.upload_text(text, wa::storage::access_condition(), options, m_context);
        CHECK_UTF8_EQUAL(text, m_blob.download_text(wa::storage::access_condition(), options, m_context));

        m_blob.properties().set_content_md5(dummy_md5);
        m_blob.upload_properties();
        CHECK_THROW(m_blob.download_text(wa::storage::access_condition(), options, m_context), wa::storage::storage_exception);
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_constructor)
    {
        m_blob.upload_block_list(std::vector<wa::storage::block_list_item>(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK(!m_blob.properties().etag().empty());

        wa::storage::cloud_block_blob blob1(m_blob.uri());
        CHECK_UTF8_EQUAL(m_blob.name(), blob1.name());
        CHECK_UTF8_EQUAL(m_blob.uri().primary_uri().to_string(), blob1.uri().primary_uri().to_string());
        CHECK_UTF8_EQUAL(m_blob.uri().secondary_uri().to_string(), blob1.uri().secondary_uri().to_string());
        CHECK(blob1.properties().etag().empty());

        wa::storage::cloud_blob blob2(m_blob);
        CHECK_UTF8_EQUAL(m_blob.name(), blob2.name());
        CHECK_UTF8_EQUAL(m_blob.uri().primary_uri().to_string(), blob2.uri().primary_uri().to_string());
        CHECK_UTF8_EQUAL(m_blob.uri().secondary_uri().to_string(), blob2.uri().secondary_uri().to_string());
        CHECK_UTF8_EQUAL(m_blob.properties().etag(), blob2.properties().etag());

        wa::storage::cloud_block_blob blob3(blob2);
        CHECK_UTF8_EQUAL(m_blob.name(), blob3.name());
        CHECK_UTF8_EQUAL(m_blob.uri().primary_uri().to_string(), blob3.uri().primary_uri().to_string());
        CHECK_UTF8_EQUAL(m_blob.uri().secondary_uri().to_string(), blob3.uri().secondary_uri().to_string());
        CHECK_UTF8_EQUAL(m_blob.properties().etag(), blob2.properties().etag());
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_block_list_with_metadata)
    {
        m_blob.metadata()[U("key1")] = U("value1");
        m_blob.metadata()[U("key2")] = U("value2");
        m_blob.upload_block_list(std::vector<wa::storage::block_list_item>());

        auto same_blob = m_container.get_block_blob_reference(m_blob.name());
        CHECK(same_blob.metadata().empty());
        same_blob.download_attributes(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(2, same_blob.metadata().size());
        CHECK_UTF8_EQUAL(U("value1"), same_blob.metadata()[U("key1")]);
        CHECK_UTF8_EQUAL(U("value2"), same_blob.metadata()[U("key2")]);
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_put_blob_with_metadata)
    {
        m_blob.metadata()[U("key1")] = U("value1");
        m_blob.metadata()[U("key2")] = U("value2");
        m_blob.upload_text(utility::string_t(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);

        auto same_blob = m_container.get_block_blob_reference(m_blob.name());
        CHECK(same_blob.metadata().empty());
        same_blob.download_attributes(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(2, same_blob.metadata().size());
        CHECK_UTF8_EQUAL(U("value1"), same_blob.metadata()[U("key1")]);
        CHECK_UTF8_EQUAL(U("value2"), same_blob.metadata()[U("key2")]);
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload_invalid_options)
    {
        wa::storage::blob_request_options options;
        options.set_store_blob_content_md5(false);
        options.set_use_transactional_md5(true);

        CHECK_THROW(m_blob.upload_text(utility::string_t(), wa::storage::access_condition(), options, m_context), std::invalid_argument);
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_snapshot_metadata)
    {
        m_blob.metadata()[U("key1")] = U("value1");
        m_blob.metadata()[U("key2")] = U("value2");
        m_blob.upload_text(U("1"));

        auto snapshot1 = m_blob.create_snapshot(wa::storage::cloud_metadata(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK(snapshot1.metadata().empty());
        snapshot1.download_attributes(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(2, snapshot1.metadata().size());
        CHECK_UTF8_EQUAL(U("value1"), snapshot1.metadata()[U("key1")]);
        CHECK_UTF8_EQUAL(U("value2"), snapshot1.metadata()[U("key2")]);

        wa::storage::cloud_metadata snapshot_metadata;
        snapshot_metadata[U("key3")] = U("value1");
        snapshot_metadata[U("key4")] = U("value2");
        auto snapshot2 = m_blob.create_snapshot(snapshot_metadata, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK(snapshot2.metadata().empty());
        snapshot2.download_attributes(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_EQUAL(2, snapshot1.metadata().size());
        CHECK_UTF8_EQUAL(U("value1"), snapshot2.metadata()[U("key3")]);
        CHECK_UTF8_EQUAL(U("value2"), snapshot2.metadata()[U("key4")]);
    }

    TEST_FIXTURE(block_blob_test_base, block_reordering)
    {
        m_blob.properties().set_content_type(U("text/plain; charset=utf-8"));

        std::vector<wa::storage::block_list_item> blocks;
        for (int i = 0; i < 10; i++)
        {
            auto id = get_block_id(i);
            auto utf8_body = utility::conversions::to_utf8string(utility::conversions::print_string(i));
            auto length = utf8_body.size();
            auto stream = concurrency::streams::bytestream::open_istream(std::move(utf8_body));
            m_blob.upload_block(id, stream, utility::string_t(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
            blocks.push_back(wa::storage::block_list_item(id));
        }

        m_blob.upload_block_list(blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_UTF8_EQUAL(U("0123456789"), m_blob.download_text(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context));

        blocks.erase(blocks.begin());
        m_blob.upload_block_list(blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_UTF8_EQUAL(U("123456789"), m_blob.download_text(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context));

        auto iter = blocks.begin();
        std::advance(iter, 3);
        blocks.erase(iter);
        m_blob.upload_block_list(blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_UTF8_EQUAL(U("12356789"), m_blob.download_text(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context));

        auto id = get_block_id(4);
        auto utf8_body = utility::conversions::to_utf8string(utility::conversions::print_string(4));
        auto length = utf8_body.size();
        auto stream = concurrency::streams::bytestream::open_istream(std::move(utf8_body));
        m_blob.upload_block(id, stream, utility::string_t(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        blocks.insert(blocks.begin(), wa::storage::block_list_item(id));
        m_blob.upload_block_list(blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_UTF8_EQUAL(U("412356789"), m_blob.download_text(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context));

        blocks.push_back(wa::storage::block_list_item(id));
        m_blob.upload_block_list(blocks, wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);
        CHECK_UTF8_EQUAL(U("4123567894"), m_blob.download_text(wa::storage::access_condition(), wa::storage::blob_request_options(), m_context));
    }
    
    TEST_FIXTURE(block_blob_test_base, list_uncommitted_blobs)
    {
        std::vector<uint8_t> buffer;
        buffer.resize(16 * 1024);
        fill_buffer_and_get_md5(buffer);
        auto stream = concurrency::streams::bytestream::open_istream(buffer);
        auto ucblob = m_container.get_block_blob_reference(U("ucblob"));
        ucblob.upload_block(get_block_id(0), stream, utility::string_t(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);

        m_blob.upload_text(utility::string_t(), wa::storage::access_condition(), wa::storage::blob_request_options(), m_context);

        wa::storage::blob_listing_includes includes;
        auto blobs = list_all_blobs(utility::string_t(), includes, 0, wa::storage::blob_request_options());
        CHECK_EQUAL(1, blobs.size());

        includes.set_uncommitted_blobs(true);
        blobs = list_all_blobs(utility::string_t(), includes, 0, wa::storage::blob_request_options());
        CHECK_EQUAL(2, blobs.size());
    }

    TEST_FIXTURE(block_blob_test_base, block_blob_upload_maximum_execution_time)
    {
        std::chrono::seconds duration(10);

        std::vector<uint8_t> buffer;
        buffer.resize(2 * 1024 * 1024);

        wa::storage::blob_request_options options;
        options.set_maximum_execution_time(duration);
        options.set_stream_write_size_in_bytes(buffer.size() / 2);
        options.set_single_blob_upload_threshold_in_bytes(buffer.size() / 2);

        m_context.set_response_received([duration] (web::http::http_request&, const web::http::http_response&, wa::storage::operation_context)
        {
            std::this_thread::sleep_for(duration);
        });

        CHECK_THROW(m_blob.upload_from_stream(concurrency::streams::bytestream::open_istream(std::move(buffer)), wa::storage::access_condition(), options, m_context), wa::storage::storage_exception);
        CHECK_EQUAL(2, m_context.request_results().size());

        m_context.set_response_received(std::function<void(web::http::http_request &, const web::http::http_response&, wa::storage::operation_context)>());
    }
}
