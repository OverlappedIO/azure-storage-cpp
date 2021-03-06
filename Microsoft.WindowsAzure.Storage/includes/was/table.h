// -----------------------------------------------------------------------------------------
// <copyright file="table.h" company="Microsoft">
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

#pragma once

#include "service_client.h"

namespace wa { namespace storage {

    class cloud_table;
    class table_operation;
    class table_result_segment;

    /// <summary>
    /// Enumeration containing the types of values that can be stored in a table entity property.
    /// </summary>
    enum class edm_type
    {
        /// <summary>
        /// Represents fixed- or variable-length character data.
        /// </summary>
        string,

        /// <summary>
        /// Represents fixed- or variable-length binary data.
        /// </summary>
        binary,

        /// <summary>
        /// Represents the mathematical concept of binary-valued logic.
        /// </summary>
        boolean,

        /// <summary>
        /// Represents date and time.
        /// </summary>
        datetime,

        /// <summary>
        /// Represents a floating point number with 15 digits precision that can represent values with approximate range of +/- 2.23e -308 through +/- 1.79e +308.
        /// </summary>
        double_floating_point,

        /// <summary>
        /// Represents a 16-byte (128-bit) unique identifier value.
        /// </summary>
        guid,

        /// <summary>
        /// Represents a signed 32-bit integer value.
        /// </summary>
        int32,

        /// <summary>
        /// Represents a signed 64-bit integer value.
        /// </summary>
        int64,
    };

    /// <summary>
    /// Enumeration containing the types of operations that can be performed by a <see cref="wa::storage::table_operation" />.
    /// </summary>
    enum class table_operation_type
    {
        /// <summary>
        /// Represents an insert operation.
        /// </summary>
        insert_operation,

        /// <summary>
        /// Represents a delete operation.
        /// </summary>
        delete_operation,

        /// <summary>
        /// Represents a replace operation.
        /// </summary>
        replace_operation,

        /// <summary>
        /// Represents a merge operation.
        /// </summary>
        merge_operation,

        /// <summary>
        /// Represents an insert or replace operation.
        /// </summary>
        insert_or_replace_operation,

        /// <summary>
        /// Represents an insert or merge operation.
        /// </summary>
        insert_or_merge_operation,

        /// <summary>
        /// Represents a retrieve operation.
        /// </summary>
        retrieve_operation,
    };

    /// <summary>
    /// Describes the JSON payload formats supported for tables.
    /// </summary>
    enum class table_payload_format
    {
        /// <summary>
        /// Use JSON with minimal metadata.
        /// </summary>
        json,

        /// <summary>
        /// Use JSON with full metadata.
        /// </summary>
        json_full_metadata,

        /// <summary>
        /// Use JSON with no metadata.
        /// </summary>
        json_no_metadata
    };

    /// <summary>
    /// Represents a shared access policy, which specifies the start time, expiry time, 
    /// and permissions for a shared access signature on a table.
    /// </summary>
    class table_shared_access_policy : public shared_access_policy
    {
    public:

        /// <summary>
        /// An enumeration describing permissions that may be used for a shared access signature.
        /// </summary>
        enum permissions
        {
            none = 0, read = 1, del = 4, add = 0x10, update = 0x20
        };

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_shared_access_policy" /> class.
        /// </summary>
        table_shared_access_policy()
            : shared_access_policy()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_shared_access_policy" /> class.
        /// </summary>
        /// <param name="expiry">The expiration date and time of the policy.</param>
        /// <param name="permission">A mask containing the permissions of the policy</param>
        table_shared_access_policy(const utility::datetime& expiry, uint8_t permission)
            : shared_access_policy(expiry, permission)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_shared_access_policy" /> class.
        /// </summary>
        /// <param name="start">The start date and time of the policy.</param>
        /// <param name="expiry">The expiration date and time of the policy.</param>
        /// <param name="permission">A mask containing the permissions of the policy</param>
        table_shared_access_policy(const utility::datetime& start, const utility::datetime& expiry, uint8_t permission)
            : shared_access_policy(start, expiry, permission)
        {
        }
    };

    /*
    /// <summary>
    /// Represents the permissions for a Table object.
    /// </summary>
    typedef cloud_permissions<table_shared_access_policy> cloud_table_permissions;
    */

    /// <summary>
    /// Represents the permissions for a table.
    /// </summary>
    class table_permissions : public cloud_permissions<table_shared_access_policy>
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_permissions" /> class.
        /// </summary>
        table_permissions()
            : cloud_permissions()
        {
        }
    };

    /// <summary>
    /// Represents a set of options that may be specified for a request against the Table service.
    /// </summary>
    class table_request_options : public request_options
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_request_options" /> class.
        /// </summary>
        table_request_options()
            : m_payload_format(wa::storage::table_payload_format::json)
        {
        }

        /// <summary>
        /// Applies the default set of request options.
        /// </summary>
        /// <param name="other">A reference to a set of <see cref="table_request_options" />.</param>
        void apply_defaults(const table_request_options& other)
        {
            request_options::apply_defaults(other, true);
            
            m_payload_format.merge(other.m_payload_format);
        }

        /// <summary>
        /// Gets the <see cref="wa::storage::table_payload_format"/> to use for the request.
        /// </summary>
        /// <returns>A <see cref="wa::storage::table_payload_format"/> object.</returns>
        wa::storage::table_payload_format payload_format() const
        {
            return m_payload_format;
        }

        /// <summary>
        /// Sets the <see cref="wa::storage::table_payload_format"/> that will be used for the request.
        /// </summary>
        /// <param name="payload_format">The <see cref="wa::storage::table_payload_format"/> to use.</param>
        void set_payload_format(wa::storage::table_payload_format payload_format)
        {
            m_payload_format = payload_format;
        }

    private:

        option_with_default<table_payload_format> m_payload_format;
    };

    /// <summary>
    /// Class for storing information about a single property in an entity in a table.
    /// </summary>
    class entity_property
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class.
        /// </summary>
        entity_property()
            : m_property_type(edm_type::string), m_is_null(true)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// byte array value of the property.
        /// </summary>
        /// <param name="input">A byte array.</param>
        entity_property(const std::vector<uint8_t>& value)
            : m_property_type(edm_type::binary), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// boolean value of the property.
        /// </summary>
        /// <param name="input">A boolean value.</param>
        entity_property(bool value)
            : m_property_type(edm_type::boolean), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// date/time value of the property.
        /// </summary>
        /// <param name="input">A datetime value.</param>
        entity_property(const utility::datetime& value)
            : m_property_type(edm_type::datetime), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// double precision floating point number value of the property.
        /// </summary>
        /// <param name="input">A double value.</param>
        entity_property(double value)
            : m_property_type(edm_type::double_floating_point), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// GUID value of the property.
        /// </summary>
        /// <param name="input">A GUID value.</param>
        entity_property(const utility::uuid& value)
            : m_property_type(edm_type::guid), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// 32-bit integer value of the property.
        /// </summary>
        /// <param name="input">A 32-bit integer value.</param>
        entity_property(int32_t value)
            : m_property_type(edm_type::int32), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// 64-bit integer value of the property.
        /// </summary>
        /// <param name="input">A 64-bit integer value.</param>
        entity_property(int64_t value)
            : m_property_type(edm_type::int64), m_is_null(false)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="entity_property"/> class by using the
        /// string value of the property.
        /// </summary>
        /// <param name="input">A string value.</param>
        entity_property(const utility::string_t& value)
            : m_property_type(edm_type::string), m_value(value), m_is_null(false)
        {
        }

        /// <summary>
        /// Gets the property type of the <see cref="entity_property" /> object.
        /// </summary>
        /// <returns>An <see cref="edm_type" /> object.</returns>
        wa::storage::edm_type property_type() const
        {
            return m_property_type;
        }

        /// <summary>
        /// Sets the property type of the <see cref="entity_property" /> object.
        /// </summary>
        /// <param name="property_type">An <see cref="edm_type" /> object indicating the property type.</param>
        void set_property_type(wa::storage::edm_type property_type)
        {
            m_property_type = property_type;
        }

        /// <summary>
        /// Indicates whether the value is null.
        /// </summary>
        /// <returns><c>true</c> if the value is null.</returns>
        bool is_null() const
        {
            return m_is_null;
        }

        /// <summary>
        /// Sets the value to null.
        /// </summary>
        /// <param name="value"><c>true</c> to set the value to null.</param>
        void set_is_null(bool value)
        {
            m_is_null = value;
        }

        // TODO: Consider renaming the getters to as_binary, as_boolean, etc.

        /// <summary>
        /// Gets the byte array value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The byte array value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a byte array.
        /// </remarks>
        const std::vector<uint8_t> binary_value() const
        {
            if (m_property_type != edm_type::binary)
            {
                throw std::runtime_error("The type of the entity property is not binary.");
            }

            return std::vector<uint8_t>(utility::conversions::from_base64(m_value));
        }

        /// <summary>
        /// Gets the boolean value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The boolean value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a boolean value.
        /// </remarks>
        bool boolean_value() const
        {
            if (m_property_type != edm_type::boolean)
            {
                throw std::runtime_error("The type of the entity property is not boolean.");
            }

            if (m_value.compare(U("false")) == 0)
            {
                return false;
            }
            else if (m_value.compare(U("true")) == 0)
            {
                return true;
            }
            else
            {
                throw std::runtime_error("An error occurred parsing the boolean.");
            }
        }

        /*
        /// <summary>
        /// Gets the date/time value of the <see cref="entity_property"/> object.
        /// An exception will be thrown if you attempt to set this property to anything other than a date/time object.
        /// </summary>
        /// <returns>The date/time value of the <see cref="entity_property"/> object.</returns>
        const utility::datetime datetime_value() const
        {
            if (m_property_type != edm_type::datetime)
            {
                throw std::runtime_error("The type of the entity property is not date/time.");
            }

            utility::datetime result = utility::datetime::from_string(m_value, utility::datetime::ISO_8601);
            if (!result.is_initialized())
            {
                throw std::runtime_error("An error occurred parsing the date/time.");
            }

            return result;
        }
        */

        /// <summary>
        /// Gets the datetime value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The datetime value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a datetime value.
        /// </remarks>
        WASTORAGE_API const utility::datetime datetime_value() const;

        /// <summary>
        /// Gets the double-precision floating point value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The double-precision floating point value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a double-precision floating point value.
        /// </remarks>
        double double_value() const
        {
            if (m_property_type != edm_type::double_floating_point)
            {
                throw std::runtime_error("The type of the entity property is not double.");
            }

            if (m_value.compare(protocol::double_not_a_number) == 0)
            {
                return std::numeric_limits<double>::quiet_NaN();
            }
            else if (m_value.compare(protocol::double_infinity) == 0)
            {
                return std::numeric_limits<double>::infinity();
            }
            else if (m_value.compare(protocol::double_negative_infinity) == 0)
            {
                return -std::numeric_limits<double>::infinity();
            }

            // TODO: Test and verify this throws an appropriate exception on failure
            double result;
            utility::istringstream_t buffer(m_value);
            buffer >> result;
            return result;
        }

        /// <summary>
        /// Gets the GUID value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The GUID value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a GUID value.
        /// </remarks>
        const utility::uuid guid_value() const
        {
            if (m_property_type != edm_type::guid)
            {
                throw std::runtime_error("The type of the entity property is not GUID.");
            }

            utility::uuid result = utility::string_to_uuid(m_value);
            return result;
        }

        /// <summary>
        /// Gets the 32-bit integer value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The 32-bit integer value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a 32-bit integer value.
        /// </remarks>
        int32_t int32_value() const
        {
            if (m_property_type != edm_type::int32)
            {
                throw std::runtime_error("The type of the entity property is not 32-bit integer.");
            }

            int32_t result;
            utility::istringstream_t buffer(m_value);
            buffer >> result;
            return result;
        }

        /// <summary>
        /// Gets the 64-bit integer value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The 64-bit integer value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a 64-bit integer value.
        /// </remarks>
        int64_t int64_value() const
        {
            if (m_property_type != edm_type::int64)
            {
                throw std::runtime_error("The type of the entity property is not 64-bit integer.");
            }

            int64_t result;
            utility::istringstream_t buffer(m_value);
            buffer >> result;
            return result;
        }

        /// <summary>
        /// Gets the string value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <returns>The string value of the <see cref="entity_property"/> object.</returns>
        /// <remarks>
        /// An exception is thrown if this property is set to a value other than a string value.
        /// </remarks>
        const utility::string_t string_value() const
        {
            if (m_property_type != edm_type::string)
            {
                throw std::runtime_error("The type of the entity property is not string.");
            }

            return m_value;
        }

        // TODO: Use std::vector<uint8_t> for binary data throughout the library

        /// <summary>
        /// Sets the byte array value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The byte array value.</param>
        void set_value(const std::vector<uint8_t>& value)
        {
            set_value_impl(value);
        }

        /// <summary>
        /// Sets the boolean value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The boolean value.</param>
        void set_value(bool value)
        {
            m_property_type = edm_type::boolean;
            m_is_null = false;
            set_value_impl(value);
        }

        // TODO: Test timezone parsing

        /// <summary>
        /// Sets the datetime value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The datetime value.</param>
        void set_value(const utility::datetime& value)
        {
            m_property_type = edm_type::datetime;
            m_is_null = false;
            set_value_impl(value);
        }

        /// <summary>
        /// Sets the double-precision floating point value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The double-precision floating point value.</param>
        void set_value(double value)
        {
            m_property_type = edm_type::double_floating_point;
            m_is_null = false;
            set_value_impl(value);
        }

        /// <summary>
        /// Sets the GUID value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The GUID point value.</param>
        void set_value(utility::uuid value)
        {
            m_property_type = edm_type::guid;
            m_is_null = false;
            set_value_impl(value);
        }

        /// <summary>
        /// Sets the 32-bit integer value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The 32-bit integer value.</param>
        void set_value(int32_t value)
        {
            m_property_type = edm_type::int32;
            m_is_null = false;
            set_value_impl(value);
        }

        /// <summary>
        /// Sets the 64-bit integer value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The 64-bit integer value.</param>
        void set_value(int64_t value)
        {
            m_property_type = edm_type::int64;
            m_is_null = false;
            set_value_impl(value);
        }

        /// <summary>
        /// Sets the string value of the <see cref="entity_property"/> object.
        /// </summary>
        /// <param name="value">The string value.</param>
        void set_value(utility::string_t value)
        {
            m_property_type = edm_type::string;
            m_is_null = false;
            m_value = value;
        }

        /// <summary>
        /// Returns the value of the <see cref="entity_property"/> object as a string.
        /// </summary>
        /// <returns>A string containing the property value.</returns>
        const utility::string_t& str() const
        {
            return m_value;
        }

    private:

        void set_value_impl(const std::vector<uint8_t>& value)
        {
            m_value = utility::conversions::to_base64(value);
        }

        void set_value_impl(bool value)
        {
            m_value = value ? U("true") : U("false");
        }

        /*
        void set_value_impl(const utility::datetime& value)
        {
            m_value = value.to_string(utility::datetime::ISO_8601);
        }
        */

        WASTORAGE_API void set_value_impl(const utility::datetime& value);

        WASTORAGE_API void set_value_impl(double value);

        void set_value_impl(utility::uuid value)
        {
            m_value = utility::uuid_to_string(value);
        }

        void set_value_impl(int32_t value)
        {
            utility::ostringstream_t buffer;
            buffer << value;
            m_value = buffer.str();
        }

        void set_value_impl(int64_t value)
        {
            utility::ostringstream_t buffer;
            buffer << value;
            m_value = buffer.str();
        }

        edm_type m_property_type;
        bool m_is_null;
        utility::string_t m_value;
    };

    /// <summary>
    /// Represents an entity in a table.
    /// </summary>
    class table_entity
    {
    public:

        typedef std::unordered_map<utility::string_t, wa::storage::entity_property> properties_type;
        typedef std::unordered_map<utility::string_t, wa::storage::entity_property>::value_type property_type;

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_entity"/> class.
        /// </summary>
        table_entity()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_entity"/> class with the specified partition key and row key.
        /// </summary>
        /// <param name="partition_key">The partition key value for the entity.</param>
        /// <param name="row_key">The row key value for the entity.</param>
        table_entity(const utility::string_t& partition_key, const utility::string_t& row_key)
            : m_partition_key(partition_key), m_row_key(row_key)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_entity"/> class with the entity's partition key, row key, ETag (if available/required), and properties.
        /// </summary>
        /// <param name="partition_key">The entity's partition key.</param>
        /// <param name="row_key">The entity's row key.</param>
        /// <param name="etag">The entity's current ETag.</param>
        /// <param name="properties">The entity's properties, indexed by property name.</param>
        table_entity(const utility::string_t& partition_key, const utility::string_t& row_key, const utility::string_t& etag, const properties_type& properties)
            : m_partition_key(partition_key), m_row_key(row_key), m_etag(etag), m_properties(properties)
        {
        }

        /// <summary>
        /// Gets the properties in the table entity, indexed by property name.
        /// </summary>
        /// <returns>The entity properties.</returns>
        properties_type& properties()
        {
            return m_properties;
        }

        /// <summary>
        /// Gets the properties in the table entity, indexed by property name.
        /// </summary>
        /// <returns>The entity properties.</returns>
        const properties_type& properties() const
        {
            return m_properties;
        }

        /*
        /// <summary>
        /// Sets the properties in the table entity, indexed by property name.
        /// </summary>
        /// <param name="properties">The entity properties.</param>
        void set_properties(const std::unordered_map<utility::string_t, wa::storage::entity_property>& properties)
        {
            m_properties = std::move(properties);
        }
        */

        /// <summary>
        /// Gets the entity's partition key.
        /// </summary>
        /// <returns>The entity partition key.</returns>
        const utility::string_t& partition_key() const
        {
            return m_partition_key;
        }

        /// <summary>
        /// Sets the entity's partition key.
        /// </summary>
        /// <param name="partition_key">The entity partition key.</param>
        void set_partition_key(const utility::string_t& partition_key)
        {
            m_partition_key = std::move(partition_key);
        }

        /// <summary>
        /// Gets the entity's row key.
        /// </summary>
        /// <returns>The entity row key.</returns>
        const utility::string_t& row_key() const
        {
            return m_row_key;
        }

        /// <summary>
        /// Sets the entity's row key.
        /// </summary>
        /// <param name="row_key">The entity row key.</param>
        void set_row_key(const utility::string_t& row_key)
        {
            m_row_key = std::move(row_key);
        }

        /// <summary>
        /// Gets the entity's timestamp.
        /// </summary>
        /// <returns>The entity timestamp.</returns>
        const utility::datetime& timestamp() const
        {
            return m_timestamp;
        }

        /// <summary>
        /// Sets the entity's timestamp.
        /// </summary>
        /// <param name="timestamp">The entity timestamp.</param>
        void set_timestamp(const utility::datetime& timestamp)
        {
            m_timestamp = std::move(timestamp);
        }

        /// <summary>
        /// Gets the entity's current ETag.
        /// </summary>
        /// <returns>The entity's ETag value, as a string.</returns>
        /// <remarks>
        /// Set this value to "*" in order to overwrite an entity as part of an update operation.
        /// </remarks>
        const utility::string_t& etag() const
        {
            return m_etag;
        }

        /// <summary>
        /// Sets the entity's current ETag.
        /// </summary>
        /// <param name="etag">The entity's ETag value, as a string.</param>
        /// <remarks>
        /// Set this value to "*" in order to overwrite an entity as part of an update operation.
        /// </remarks>
        void set_etag(const utility::string_t& etag)
        {
            m_etag = std::move(etag);
        }

    private:

        properties_type m_properties;
        utility::string_t m_partition_key;
        utility::string_t m_row_key;
        utility::datetime m_timestamp;
        utility::string_t m_etag;
    };

    /// <summary>
    /// Represents a single table operation.
    /// </summary>
    class table_operation
    {
    public:

        /// <summary>
        /// Gets the entity being operated upon.
        /// </summary>
        /// <returns>A <see cref="table_entity"/> object.</returns>
        const table_entity& entity() const
        {
            return m_entity;
        }

        /// <summary>
        /// Gets the type of operation.
        /// </summary>
        /// <returns>A <see cref="table_operation_type"/> object.</returns>
        table_operation_type operation_type() const
        {
            return m_operation_type;
        }

        /// <summary>
        /// Creates a new table operation to delete the specified entity.
        /// </summary>
        /// <param name="entity">The entity to be deleted from the table.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation delete_entity(const table_entity& entity)
        {
            return table_operation(table_operation_type::delete_operation, entity);
        }

        /// <summary>
        /// Creates a new table operation to insert the specified entity.
        /// </summary>
        /// <param name="entity">The entity to be inserted into the table.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation insert_entity(const table_entity& entity)
        {
            return table_operation(table_operation_type::insert_operation, entity);
        }

        /// <summary>
        /// Creates a new table operation to insert the specified entity if it does not exist; 
        /// if the entity does exist, then the contents of the specified entity are merged with the existing entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being inserted or merged.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation insert_or_merge_entity(const table_entity& entity)
        {
            return table_operation(table_operation_type::insert_or_merge_operation, entity);
        }

        /// <summary>
        /// Creates a new table operation to insert the specified entity if the entity does not exist; 
        /// if the entity does exist, then its contents are replaced with the specified entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being inserted or replaced.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation insert_or_replace_entity(const table_entity& entity)
        {
            return table_operation(table_operation_type::insert_or_replace_operation, entity);
        }

        /// <summary>
        /// Creates a new table operation to merge the contents of the specified entity with the existing entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being merged.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation merge_entity(const table_entity& entity)
        {
            return table_operation(table_operation_type::merge_operation, entity);
        }

        /// <summary>
        /// Creates a new table operation to replace the contents of the specified entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being replaced.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation replace_entity(const table_entity& entity)
        {
            return table_operation(table_operation_type::replace_operation, entity);
        }

        /// <summary>
        /// Creates a new table operation to retrieve the contents of the specified entity.
        /// </summary>
        /// <param name="partition_key">The partition key of the entity to be replaced.</param>
        /// <param name="row_key">The row key of the entity to be replaced.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        static const table_operation retrieve_entity(const utility::string_t& partition_key, const utility::string_t& row_key)
        {
            table_entity entity;
            entity.set_partition_key(partition_key);
            entity.set_row_key(row_key);

            return table_operation(table_operation_type::retrieve_operation, entity);
        }

    private:

        table_operation(table_operation_type operation_type, const table_entity& entity)
            : m_operation_type(operation_type), m_entity(entity)
        {
        }

        wa::storage::table_operation_type m_operation_type;
        wa::storage::table_entity m_entity;
    };

    /// <summary>
    /// Represents a batch operation on a table.
    /// </summary>
    class table_batch_operation
    {
    public:

        typedef std::vector<wa::storage::table_operation> operations_type;

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_batch_operation"/> class.
        /// </summary>
        table_batch_operation()
        {
        }

        /// <summary>
        /// Creates a new table operation to delete the specified entity.
        /// </summary>
        /// <param name="entity">The entity to be deleted from the table.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void delete_entity(const table_entity& entity)
        {
            m_operations.push_back(wa::storage::table_operation::delete_entity(entity));
        }

        /// <summary>
        /// Creates a new table operation to insert the specified entity.
        /// </summary>
        /// <param name="entity">The entity to be inserted into the table.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void insert_entity(const table_entity& entity)
        {
            m_operations.push_back(wa::storage::table_operation::insert_entity(entity));
        }

        /// <summary>
        /// Creates a new table operation to insert the specified entity if it does not exist; 
        /// if the entity does exist, then the contents of the specified entity are merged with the existing entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being inserted or merged.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void insert_or_merge_entity(const table_entity& entity)
        {
            m_operations.push_back(wa::storage::table_operation::insert_or_merge_entity(entity));
        }

        /// <summary>
        /// Creates a new table operation to insert the specified entity if the entity does not exist; 
        /// if the entity does exist, then its contents are replaced with the specified entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being inserted or replaced.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void insert_or_replace_entity(const table_entity& entity)
        {
            m_operations.push_back(wa::storage::table_operation::insert_or_replace_entity(entity));
        }

        /// <summary>
        /// Creates a new table operation to merge the contents of the specified entity with the existing entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being merged.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void merge_entity(const table_entity& entity)
        {
            m_operations.push_back(wa::storage::table_operation::merge_entity(entity));
        }

        /// <summary>
        /// Creates a new table operation to replace the contents of the specified entity.
        /// </summary>
        /// <param name="entity">The entity whose contents are being replaced.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void replace_entity(const table_entity& entity)
        {
            m_operations.push_back(wa::storage::table_operation::replace_entity(entity));
        }

        /// <summary>
        /// Creates a new table operation to replace the contents of the specified entity.
        /// </summary>
        /// <param name="partition_key">The partition key of the entity to be replaced.</param>
        /// <param name="row_key">The row key of the entity to be replaced.</param>
        /// <returns>A <see cref="wa::storage::table_operation"/> object.</returns>
        void retrieve_entity(const utility::string_t& partition_key, const utility::string_t& row_key)
        {
            m_operations.push_back(wa::storage::table_operation::retrieve_entity(partition_key, row_key));
        }

        /// <summary>
        /// Gets a reference to an <see cref="operations_type"/> object containing an enumerable collection
        /// of operations comprising a batch operation.
        /// </summary>
        /// <returns>An <see cref="operations_type"/> object.</returns>
        operations_type& operations()
        {
            return m_operations;
        }

        /// <summary>
        /// Gets a reference to an <see cref="operations_type"/> object containing an enumerable collection
        /// of operations comprising a batch operation.
        /// </summary>
        /// <returns>An <see cref="operations_type"/> object.</returns>
        const operations_type& operations() const
        {
            return m_operations;
        }

    private:

        std::vector<table_operation> m_operations;
    };

    /// <summary>
    /// Defines the set of comparison operators that may be used for constructing queries.
    /// </summary>
    class query_comparison_operator
    {
    public:

        /// <summary>
        /// Represents the Equal operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t equal;

        /// <summary>
        /// Represents the Not Equal operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t not_equal;

        /// <summary>
        /// Represents the Greater Than operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t greater_than;

        /// <summary>
        /// Represents the Greater Than or Equal operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t greater_than_or_equal;

        /// <summary>
        /// Represents the Less Than operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t less_than;

        /// <summary>
        /// Represents the Less Than or Equal operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t less_than_or_equal;
    };

    /// <summary>
    /// Defines the set of Boolean operators for constructing queries.
    /// </summary>
    class query_logical_operator
    {
    public:

        /// <summary>
        /// Represents the And operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t and;

        /// <summary>
        /// Represents the Not operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t not;

        /// <summary>
        /// Represents the Or operator.
        /// </summary>
        WASTORAGE_API const static utility::string_t or;
    };

    /// <summary>
    /// Represents a query against a table.
    /// </summary>
    class table_query
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_query"/> class.
        /// </summary>
        table_query()
            : m_take_count(-1)
        {
        }

        /// <summary>
        /// Gets the maximum number of entities the query will return. 
        /// </summary>
        /// <returns>The maximum number of entities the query will return.</returns>
        int take_count() const
        {
            return m_take_count;
        }

        /// <summary>
        /// Sets the maximum number of entities the query may return.
        /// </summary>
        /// <param name="value">The maximum number of entities the query may return.</param>
        void set_take_count(int value)
        {
            m_take_count = value;
        }

        /// <summary>
        /// Gets the filter expression to use for the query.
        /// </summary>
        /// <returns>A string containing the filter expression.</returns>
        utility::string_t filter_string() const
        {
            return m_filter_string;
        }

        /// <summary>
        /// Sets the filter expression to use for the query.
        /// </summary>
        /// <param name="value">A string containing the filter expression.</param>
        void set_filter_string(const utility::string_t& value)
        {
            m_filter_string = std::move(value);
        }

        // TODO: Consider if this should really be returning a copy of the vector (and same for other properties)

        /// <summary>
        /// Gets the names of the entity properties to return when the query is executed.
        /// </summary>
        /// <returns>An enumerable collection of strings containing the names of the properties to return when the query is executed.</returns>
        std::vector<utility::string_t> select_columns() const
        {
            return m_select_columns;
        }

        /// <summary>
        /// Sets the names of the entity properties to return when the table query is executed.
        /// </summary>
        /// <param name="value">An enumerable collection of strings containing the names of the properties to return when the query is executed.</param>
        void set_select_columns(const std::vector<utility::string_t>& value)
        {
            m_select_columns = std::move(value);
        }

        /// <summary>
        /// Generates a filter condition string for the specified string value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A string containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        WASTORAGE_API static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, const utility::string_t& value);

        /// <summary>
        /// Generates a filter condition string for the specified character value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A character containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, const utility::char_t* value)
        {
            // TODO: Test with special characters, especially: /?:@&=+,$
            return generate_filter_condition(property_name, comparison_operator, utility::string_t(value));
        }

        /// <summary>
        /// Generates a filter condition string for the specified binary value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A byte array containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        WASTORAGE_API static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, const std::vector<uint8_t>& value);

        /// <summary>
        /// Generates a filter condition string for the specified boolean value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A boolean containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, bool value)
        {
            utility::string_t string_value = value ? U("true") : U("false");
            return generate_filter_condition_impl(property_name, comparison_operator, string_value);
        }

        /*
        /// <summary>
        /// Generates a filter condition string for the specified date/time value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A date/time containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, const utility::datetime& value)
        {
            // TODO: Wait for Casablanca team to support fractional seconds in the string format
            utility::string_t string_data_value = value.to_string(utility::datetime::ISO_8601);

            utility::string_t string_value;
            string_value.reserve(string_data_value.size() + 10U);

            string_value.append(U("datetime'"));
            string_value.append(string_data_value);
            string_value.push_back(U('\''));

            return generate_filter_condition_impl(property_name, comparison_operator, string_value);
        }
        */

        /// <summary>
        /// Generates a filter condition string for the specified datetime value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A datetime containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        WASTORAGE_API static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, const utility::datetime& value);

        /// <summary>
        /// Generates a filter condition string for the specified double-precision floating point number value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A double-precision floating number point containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, double value)
        {
            utility::ostringstream_t buffer;
            buffer.precision(std::numeric_limits<double>::digits10 + 2);
            buffer << value;

            return generate_filter_condition_impl(property_name, comparison_operator, buffer.str());
        }

        /// <summary>
        /// Generates a filter condition string for the specified GUID value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A GUID containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, const utility::uuid& value)
        {
            utility::string_t string_data_value = utility::uuid_to_string(value);

            utility::string_t string_value;
            string_value.reserve(string_data_value.size() + 6U);

            string_value.append(U("guid'"));
            string_value.append(string_data_value);
            string_value.push_back(U('\''));

            return generate_filter_condition_impl(property_name, comparison_operator, string_value);
        }

        /// <summary>
        /// Generates a filter condition string for the specified 32-bit integer value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A 32-bit integer containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, int32_t value)
        {
            utility::ostringstream_t buffer;
            buffer << value;

            return generate_filter_condition_impl(property_name, comparison_operator, buffer.str());
        }

        /// <summary>
        /// Generates a filter condition string for the specified 64-bit integer value.
        /// </summary>
        /// <param name="property_name">A string containing the name of the property to compare.</param>
        /// <param name="comparison_operator">A string containing the comparison operator to use.</param>
        /// <param name="value">A 64-bit integer containing the value to compare with the property value.</param>
        /// <returns>A string containing the formatted filter condition.</returns>
        static const utility::string_t generate_filter_condition(const utility::string_t& property_name, const utility::string_t& comparison_operator, int64_t value)
        {
            utility::ostringstream_t buffer;
            buffer << value << U('L');

            return generate_filter_condition_impl(property_name, comparison_operator, buffer.str());
        }

        /// <summary>
        /// Creates a filter condition using the specified logical operator on two filter conditions.
        /// </summary>
        /// <param name="left_filter">A string containing the first formatted filter condition.</param>
        /// <param name="logical_operator">A string containing the AND operator or the OR operator.</param>
        /// <param name="right_filter">A string containing the second formatted filter condition.</param>
        /// <returns>A string containing the combined filter expression.</returns>
        static const utility::string_t combine_filter_conditions(const utility::string_t& left_filter, const utility::string_t& logical_operator, const utility::string_t& right_filter)
        {
            utility::string_t result;
            result.reserve(left_filter.size() + logical_operator.size() + right_filter.size() + 6U);

            result.push_back(U('('));
            result.append(left_filter);
            result.push_back(U(')'));
            result.push_back(U(' '));
            result.append(logical_operator);
            result.push_back(U(' '));
            result.push_back(U('('));
            result.append(right_filter);
            result.push_back(U(')'));

            return result;
        }

    private:

        WASTORAGE_API static const utility::string_t generate_filter_condition_impl(const utility::string_t& property_name, const utility::string_t& comparison_operator, const utility::string_t& value);

        int m_take_count;
        utility::string_t m_filter_string;
        std::vector<utility::string_t> m_select_columns;
    };

    /*
    /// <summary>
    /// Represents a continuation token for listing operations.
    /// </summary>
    /// <remarks>A method that may return a partial set of results via a <see cref="wa::storage::table_result_segment" /> object also returns a continuation token,
    /// which can be used in a subsequent call to return the next set of available results. </remarks>
    class continuation_token
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::continuation_token"/> class.
        /// </summary>
        continuation_token()
        {
        }

        /// <summary>
        /// Gets or sets the next partition key for <see cref="wa::storage::table_entity" /> enumeration operations.
        /// </summary>
        /// <returns>The next partition key.</returns>
        utility::string_t next_partition_key() const
        {
            return 0;
        }

        /// <summary>
        /// Gets or sets the next partition key for <see cref="wa::storage::table_entity" /> enumeration operations.
        /// </summary>
        /// <param name="value">The next partition key.</param>
        void set_next_partition_key(const utility::string_t& value)
        {
        }

        /// <summary>
        /// Gets or sets the next row key for <see cref="wa::storage::table_entity" /> enumeration operations.
        /// </summary>
        /// <returns>The next row key.</returns>
        utility::string_t next_row_key() const
        {
            return 0;
        }

        /// <summary>
        /// Gets or sets the next row key for <see cref="wa::storage::table_entity" /> enumeration operations.
        /// </summary>
        /// <param name="value">The next row key.</param>
        void set_next_row_key(const utility::string_t& value)
        {
        }

        /// <summary>
        /// Gets or sets the next table name for <see cref="wa::storage::table_entity" /> enumeration operations.
        /// </summary>
        /// <returns>The name of the next table.</returns>
        utility::string_t next_table_name() const
        {
            return 0;
        }

        /// <summary>
        /// Gets or sets the next table name for <see cref="wa::storage::table_entity" /> enumeration operations.
        /// </summary>
        /// <param name="value">The name of the next table.</param>
        void set_next_table_name(const utility::string_t& value)
        {
        }

        bool empty() const
        {
        }
    };
    */

    /// <summary>
    /// Represents the result of a table operation.
    /// </summary>
    /// <remarks>The <see cref="wa::storage::table_result"/> class encapsulates the HTTP response and any query 
    /// results returned for a particular <see cref="wa::storage::table_operation"/>.</remarks>
    class table_result
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_result"/> class.
        /// </summary>
        table_result()
            : m_http_status_code(0)
        {
        }

        /// <summary>
        /// Gets a <see cref="table_entity" /> object returned as part of a <see cref="wa::storage::table_result" /> object.
        /// </summary>
        /// <returns>A <see cref="table_entity" /> object.</returns>
        const wa::storage::table_entity& entity() const
        {
            return m_entity;
        }

        /// <summary>
        /// Sets a <see cref="table_entity" /> object returned as part of a <see cref="wa::storage::table_result" /> object.
        /// </summary>
        /// <param name="value">A <see cref="table_entity" /> object.</param>
        void set_entity(const wa::storage::table_entity& value)
        {
            m_entity = std::move(value);
        }

        /// <summary>
        /// Gets the HTTP status code for a <see cref="wa::storage::table_result" /> object.
        /// </summary>
        /// <returns>The HTTP status code.</returns>
        int http_status_code() const
        {
            return m_http_status_code;
        }

        /// <summary>
        /// Sets the HTTP status code for a <see cref="wa::storage::table_result" /> object.
        /// </summary>
        /// <param name="value">The HTTP status code.</param>
        void set_http_status_code(int value)
        {
            m_http_status_code = value;
        }

        /// <summary>
        /// Gets the ETag for a <see cref="wa::storage::table_result" /> object.
        /// </summary>
        /// <returns>The ETag, as a string.</returns>
        const utility::string_t& etag() const
        {
            return m_etag;
        }

        /// <summary>
        /// Sets the ETag for a <see cref="wa::storage::table_result" /> object.
        /// </summary>
        /// <param name="value">The ETag, as a string.</param>
        void set_etag(const utility::string_t& value)
        {
            m_etag = std::move(value);
        }

    private:

        wa::storage::table_entity m_entity;
        int m_http_status_code;
        utility::string_t m_etag;
    };

    /// <summary>
    /// Represents a segment of <see cref="wa::storage::cloud_table" /> results. May include a continuation token for 
    /// retrieving the next page of results.
    /// </summary>
    class table_result_segment
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_result_segment"/> class.
        /// </summary>
        table_result_segment()
        {
        }

        /// <summary>
        /// Gets an enumerable collection of <see cref="wa::storage::cloud_table" /> results.
        /// </summary>
        /// <returns>An enumerable collection of <see cref="wa::storage::cloud_table" /> results.</returns>
        std::vector<wa::storage::cloud_table> results() const
        {
            return m_results;
        }

        /// <summary>
        /// Gets the continuation token to use to retrieve the next segment of <see cref="wa::storage::cloud_table" /> results. 
        /// </summary>
        /// <returns>An object of type <see cref="continuation_token" />.</returns>
        wa::storage::continuation_token continuation_token() const
        {
            return m_continuation_token;
        }

    private:

        void set_results(const std::vector<wa::storage::cloud_table>& results)
        {
            m_results = std::move(results);
        }

        void set_continuation_token(wa::storage::continuation_token continuation_token)
        {
            m_continuation_token = std::move(continuation_token);
        }

        std::vector<wa::storage::cloud_table> m_results;
        wa::storage::continuation_token m_continuation_token;

        friend class cloud_table_client;
    };

    /// <summary>
    /// Represents a segment of <see cref="wa::storage::table_entity" /> results. May include a continuation token for 
    /// retrieving the next page of results.
    /// </summary>
    class table_query_segment
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::table_query_segment"/> class.
        /// </summary>
        table_query_segment()
        {
        }

        /// <summary>
        /// Gets an enumerable collection of <see cref="wa::storage::table_entity" /> results.
        /// </summary>
        /// <returns>An enumerable collection of <see cref="wa::storage::table_entity" /> results.</returns>
        std::vector<wa::storage::table_entity> results() const
        {
            return m_results;
        }

        /// <summary>
        /// Gets the continuation token to use to retrieve the next segment of <see cref="wa::storage::table_entity" /> results. 
        /// </summary>
        /// <returns>An object of type <see cref="continuation_token" />.</returns>
        wa::storage::continuation_token continuation_token() const
        {
            return m_continuation_token;
        }

    private:

        void set_results(const std::vector<wa::storage::table_entity>& results)
        {
            m_results = std::move(results);
        }

        void set_continuation_token(wa::storage::continuation_token continuation_token)
        {
            m_continuation_token = std::move(continuation_token);
        }

        std::vector<wa::storage::table_entity> m_results;
        wa::storage::continuation_token m_continuation_token;

        friend class cloud_table;
    };

    /// <summary>
    /// Provides a client-side logical representation of the Windows Azure Table service. 
    /// This client is used to configure and execute requests against the Table service.
    /// </summary>
    /// <remarks>The service client encapsulates the base URI for the Table service. 
    /// If the service client will be used for authenticated access, it also encapsulates the 
    /// credentials for accessing the storage account.</remarks>
    class cloud_table_client : public cloud_client
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table_client" /> class.
        /// </summary>
        cloud_table_client()
            : cloud_client()
        {
            initialize();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table_client" /> class using the specified Table service endpoint
        /// and anonymous credentials.
        /// </summary>
        /// <param name="base_uri">A <see cref="storage_uri" /> object containing the Table service endpoint for all locations.</param>
        cloud_table_client(const storage_uri& base_uri)
            : cloud_client(base_uri)
        {
            initialize();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table_client" /> class using the specified Table service endpoint
        /// and account credentials.
        /// </summary>
        /// <param name="base_uri">A <see cref="storage_uri" /> object containing the Table service endpoint for all locations.</param>
        /// <param name="credentials">The <see cref="storage_credentials" /> to use.</param>
        cloud_table_client(const storage_uri& base_uri, const storage_credentials& credentials)
            : cloud_client(base_uri, credentials)
        {
            initialize();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table_client" /> class using the specified Table service endpoint
        /// and account credentials.
        /// </summary>
        /// <param name="base_uri">A <see cref="storage_uri" /> object containing the Table service endpoint for all locations.</param>
        /// <param name="credentials">The <see cref="storage_credentials" /> to use.</param>
        cloud_table_client(const storage_uri& base_uri, const storage_credentials& credentials, const table_request_options& default_request_options)
            : cloud_client(base_uri, credentials), m_default_request_options(default_request_options)
        {
            initialize();
        }

        /// <summary>
        /// Sets the authentication scheme to use to sign HTTP requests.
        /// </summary>
        /// <param name="value">The authentication scheme.</param>
        WASTORAGE_API void set_authentication_scheme(wa::storage::authentication_scheme value) override;

        /// <summary>
        /// Returns an enumerable collection of tables.
        /// </summary>
        /// <returns>An enumerable collection of <see cref="cloud_table" /> objects.</returns>
        std::vector<cloud_table> list_tables() const
        {
            return list_tables_async(utility::string_t(), table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Returns an enumerable collection of tables, retrieved lazily, that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <returns>An enumerable collection of <see cref="cloud_table" /> objects that are retrieved lazily.</returns>
        std::vector<cloud_table> list_tables(const utility::string_t& prefix) const
        {
            return list_tables_async(prefix, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Returns an enumerable collection of tables that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>An enumerable collection of <see cref="cloud_table" /> objects.</returns>
        std::vector<cloud_table> list_tables(const utility::string_t& prefix, const table_request_options& options, operation_context context) const
        {
            return list_tables_async(prefix, options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that returns an enumerable collection of tables.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="cloud_table" />, that represents the current operation.</returns>
        pplx::task<std::vector<cloud_table>> list_tables_async() const
        {
            return list_tables_async(utility::string_t(), table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that returns an enumerable collection of tables, retrieved lazily, that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="cloud_table" />, that represents the current operation.</returns>
        pplx::task<std::vector<cloud_table>> list_tables_async(const utility::string_t& prefix) const
        {
            return list_tables_async(prefix, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that returns an enumerable collection of tables that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="cloud_table" />, that represents the current operation.</returns>
        WASTORAGE_API pplx::task<std::vector<cloud_table>> list_tables_async(const utility::string_t& prefix, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Returns a <see cref="table_result_segment" /> containing an enumerable collection of tables.
        /// </summary>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token" /> returned by a previous listing operation.</param>
        /// <returns>A <see cref="table_result_segment" /> containing an enumerable collection of tables.</returns>
        table_result_segment list_tables_segmented(const continuation_token& continuation_token) const
        {
            return list_tables_segmented_async(utility::string_t(), 0, continuation_token, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Returns a <see cref="table_result_segment" /> containing an enumerable collection of tables, retrieved lazily, that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token" /> returned by a previous listing operation.</param>
        /// <returns>A <see cref="table_result_segment" /> containing an enumerable collection of tables.</returns>
        table_result_segment list_tables_segmented(const utility::string_t& prefix, const continuation_token& continuation_token) const
        {
            return list_tables_segmented_async(prefix, 0, continuation_token, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Returns a <see cref="table_result_segment" /> containing an enumerable collection of tables that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <param name="max_results">A non-negative integer value that indicates the maximum number of results to be returned at a time, up to the 
        /// per-operation limit of 5000. If this value is zero the maximum possible number of results will be returned, up to 5000.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token"/> returned by a previous listing operation.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="table_result_segment" /> containing an enumerable collection of tables.</returns>
        table_result_segment list_tables_segmented(const utility::string_t& prefix, int max_results, const continuation_token& continuation_token, const table_request_options& options, operation_context context) const
        {
            return list_tables_segmented_async(prefix, max_results, continuation_token, options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that returns a <see cref="table_result_segment" /> containing an enumerable collection of tables.
        /// </summary>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token" /> returned by a previous listing operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result_segment" /> that represents the current operation.</returns>
        pplx::task<table_result_segment> list_tables_segmented_async(const continuation_token& continuation_token) const
        {
            return list_tables_segmented_async(utility::string_t(), 0, continuation_token, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that returns a <see cref="table_result_segment" /> containing an enumerable collection of tables, retrieved lazily, that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token" /> returned by a previous listing operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result_segment" /> that represents the current operation.</returns>
        pplx::task<table_result_segment> list_tables_segmented_async(const utility::string_t& prefix, const continuation_token& continuation_token) const
        {
            return list_tables_segmented_async(prefix, 0, continuation_token, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that returns a <see cref="table_result_segment" /> containing an enumerable collection of tables that begin with the specified prefix.
        /// </summary>
        /// <param name="prefix">The table name prefix.</param>
        /// <param name="max_results">A non-negative integer value that indicates the maximum number of results to be returned at a time, up to the
        /// per-operation limit of 5000. If this value is zero the maximum possible number of results will be returned, up to 5000.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token" /> returned by a previous listing operation.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result_segment" /> that represents the current operation.</returns>
        WASTORAGE_API pplx::task<table_result_segment> list_tables_segmented_async(const utility::string_t& prefix, int max_results, const continuation_token& continuation_token, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Gets the service properties for the service client.
        /// </summary>
        /// <returns>The <see cref="service_properties" /> for the service client.</returns>
        service_properties download_service_properties() const
        {
            return download_service_properties_async().get();
        }

        /// <summary>
        /// Gets the service properties for the service client.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>The <see cref="service_properties" /> for the service client.</returns>
        service_properties download_service_properties(const table_request_options& options, operation_context context) const
        {
            return download_service_properties_async(options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation to get the properties of the service.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="service_properties" /> that represents the current operation.</returns>
        pplx::task<service_properties> download_service_properties_async() const
        {
            return download_service_properties_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation to get the properties of the service.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="service_properties" /> that represents the current operation.</returns>
        WASTORAGE_API pplx::task<service_properties> download_service_properties_async(const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Sets the service properties for the service client.
        /// </summary>
        /// <param name="properties">The <see cref="service_properties" /> for the service client.</param>
        /// <param name="includes">A <see cref="wa::storage::service_properties_includes"/> enumeration describing which items to include when setting service properties.</param>
        void upload_service_properties(const service_properties& properties, const service_properties_includes& includes) const
        {
            upload_service_properties_async(properties, includes).wait();
        }

        /// <summary>
        /// Sets the service properties for the service client.
        /// </summary>
        /// <param name="properties">The <see cref="service_properties" /> for the service client.</param>
        /// <param name="includes">A <see cref="wa::storage::service_properties_includes"/> enumeration describing which items to include when setting service properties.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        void upload_service_properties(const service_properties& properties, const service_properties_includes& includes, const table_request_options& options, operation_context context) const
        {
            upload_service_properties_async(properties, includes, options, context).wait();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation to set the service properties for the service client.
        /// </summary>
        /// <param name="properties">The <see cref="service_properties" /> for the service client.</param>
        /// <param name="includes">A <see cref="wa::storage::service_properties_includes"/> enumeration describing which items to include when setting service properties.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<void> upload_service_properties_async(const service_properties& properties, const service_properties_includes& includes) const
        {
            return upload_service_properties_async(properties, includes, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation to set the service properties for the service client.
        /// </summary>
        /// <param name="properties">The <see cref="service_properties" /> for the service client.</param>
        /// <param name="includes">A <see cref="wa::storage::service_properties_includes"/> enumeration describing which items to include when setting service properties.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<void> upload_service_properties_async(const service_properties& properties, const service_properties_includes& includes, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Gets a reference to the specified table.
        /// </summary>
        /// <param name="table_name">The name of the table.</param>
        /// <returns>A reference to a <see cref="cloud_table" /> object.</returns>
        WASTORAGE_API cloud_table get_table_reference(const utility::string_t& table_name) const;

        /// <summary>
        /// Returns the default set of request options.
        /// </summary>
        /// <returns>A <see cref="table_request_options" /> object.</returns>
        const table_request_options& default_request_options() const
        {
            return m_default_request_options;
        }

    private:

        void initialize()
        {
            set_authentication_scheme(authentication_scheme::shared_key);
        }

        table_request_options get_modified_options(const table_request_options& options) const;

        table_request_options m_default_request_options;
    };

    /// <summary>
    /// Represents a table object in the Table service.
    /// </summary>
    class cloud_table
    {
    public:

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table" /> class.
        /// </summary>
        cloud_table()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table" /> class using an absolute URI to the table.
        /// </summary>
        /// <param name="uri">A <see cref="storage_uri" /> object containing the absolute URI to the table for all locations.</param>
        WASTORAGE_API cloud_table(const storage_uri& uri);

        /// <summary>
        /// Initializes a new instance of the <see cref="wa::storage::cloud_table" /> class using an absolute URI to the table.
        /// </summary>
        /// <param name="uri">A <see cref="storage_uri" /> object containing the absolute URI to the table for all locations.</param>
        /// <param name="credentials">The <see cref="storage_credentials" /> to use.</param>
        WASTORAGE_API cloud_table(const storage_uri& uri, const storage_credentials& credentials);

        /// <summary>
        /// Executes an operation on a table.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_operation"/> object that represents the operation to perform.</param>
        /// <returns>A <see cref="wa::storage::table_result" /> containing the result of the operation.</returns>
        table_result execute(const table_operation& operation) const
        {
            return execute_async(operation, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Executes an operation on a table.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_operation"/> object that represents the operation to perform.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns>A <see cref="wa::storage::table_result" /> containing the result of the operation.</returns>
        table_result execute(const table_operation& operation, const table_request_options& options, operation_context context) const
        {
            return execute_async(operation, options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes an operation on a table.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_operation" /> object that represents the operation to perform.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result" /> that represents the current operation.</returns>
        pplx::task<table_result> execute_async(const table_operation& operation) const
        {
            return execute_async(operation, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes an operation on a table.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_operation" /> object that represents the operation to perform.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result" /> that represents the current operation.</returns>
        WASTORAGE_API pplx::task<table_result> execute_async(const table_operation& operation, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Executes a batch operation on a table as an atomic operation.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_batch_operation"/> object that represents the operation to perform.</param>
        /// <returns>An enumerable collection of <see cref="wa::storage::table_result"/> objects that contains the results, in order, 
        /// of each operation in the <see cref="wa::storage::table_batch_operation"/>.</returns>
        std::vector<table_result> execute_batch(const table_batch_operation& operation) const
        {
            return execute_batch_async(operation, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Executes a batch operation on a table as an atomic operation.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_batch_operation"/> object that represents the operation to perform.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns>An enumerable collection of <see cref="wa::storage::table_result"/> objects that contains the results, in order, 
        /// of each operation in the <see cref="wa::storage::table_batch_operation"/>.</returns>
        std::vector<table_result> execute_batch(const table_batch_operation& operation, const table_request_options& options, operation_context context) const
        {
            return execute_batch_async(operation, options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes a batch operation on a table as an atomic operation.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_batch_operation" /> object that represents the operation to perform.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="table_result" />, that represents the current operation.</returns>
        pplx::task<std::vector<table_result>> execute_batch_async(const table_batch_operation& operation) const
        {
            return execute_batch_async(operation, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes a batch operation on a table as an atomic operation.
        /// </summary>
        /// <param name="operation">A <see cref="wa::storage::table_batch_operation" /> object that represents the operation to perform.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="table_result" />, that represents the current operation.</returns>
        WASTORAGE_API pplx::task<std::vector<table_result>> execute_batch_async(const table_batch_operation& operation, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Executes a query on a table.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query"/> object.</param>
        /// <returns>An enumerable collection of <see cref="wa::storage::table_entity"/> objects.</returns>
        std::vector<table_entity> execute_query(const table_query& query) const
        {
            return execute_query_async(query, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Executes a query on a table.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query"/> object.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns>An enumerable collection of <see cref="wa::storage::table_entity"/> objects.</returns>
        std::vector<table_entity> execute_query(const table_query& query, const table_request_options& options, operation_context context) const
        {
            return execute_query_async(query, options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes a query on a table.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query" /> representing the query to execute.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="table_entity" />, that represents the current operation.</returns>
        pplx::task<std::vector<table_entity>> execute_query_async(const table_query& query) const
        {
            return execute_query_async(query, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes a query on a table.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query" /> representing the query to execute.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="std::vector" />, of type <see cref="table_entity" />, that represents the current operation.</returns>
        WASTORAGE_API pplx::task<std::vector<table_entity>> execute_query_async(const table_query& query, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Executes a query with the specified <see cref="wa::storage::continuation_token"/> to retrieve the next page of results.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query"/> object.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token"/> object.</param>
        /// <returns>A <see cref="wa::storage::table_query_segment"/> object containing the results of the query.</returns>
        table_query_segment execute_query_segmented(const table_query& query, continuation_token continuation_token) const
        {
            return execute_query_segmented_async(query, continuation_token, table_request_options(), operation_context()).get();
        }

        /// <summary>
        /// Executes a query with the specified <see cref="wa::storage::continuation_token"/> to retrieve the next page of results.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query"/> object.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token"/> object.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns>A <see cref="wa::storage::table_query_segment"/> object containing the results of the query.</returns>
        table_query_segment execute_query_segmented(const table_query& query, continuation_token continuation_token, const table_request_options& options, operation_context context) const
        {
            return execute_query_segmented_async(query, continuation_token, options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes a query with the specified <see cref="wa::storage::continuation_token"/> to retrieve the next page of results.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query"/> object.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token"/> object.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result_segment" /> that represents the current operation.</returns>
        pplx::task<table_query_segment> execute_query_segmented_async(const table_query& query, continuation_token continuation_token) const
        {
            return execute_query_segmented_async(query, continuation_token, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that executes a query with the specified <see cref="wa::storage::continuation_token"/> to retrieve the next page of results.
        /// </summary>
        /// <param name="query">A <see cref="wa::storage::table_query"/> object.</param>
        /// <param name="continuation_token">A <see cref="wa::storage::continuation_token"/> object.</param>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_result_segment" /> that represents the current operation.</returns>
        WASTORAGE_API pplx::task<table_query_segment> execute_query_segmented_async(const table_query& query, continuation_token continuation_token, const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Creates a table.
        /// </summary>
        void create()
        {
            create_async().wait();
        }

        /// <summary>
        /// Creates a table.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        void create(const table_request_options& options, operation_context context)
        {
            create_async(options, context).wait();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that creates a table.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<void> create_async()
        {
            return create_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that creates a table.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<void> create_async(const table_request_options& options, operation_context context);

        /// <summary>
        /// Creates the table if it does not already exist.
        /// </summary>
        /// <returns><c>true</c> if table was created; otherwise, <c>false</c>.</returns>
        bool create_if_not_exists()
        {
            return create_if_not_exists_async().get();
        }

        /// <summary>
        /// Creates the table if it does not already exist.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns><c>true</c> if table was created; otherwise, <c>false</c>.</returns>
        bool create_if_not_exists(const table_request_options& options, operation_context context)
        {
            return create_if_not_exists_async(options, context).get();
        }

        /// <summary>
        /// Returns a task to create the table if it does not already exist.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<bool> create_if_not_exists_async()
        {
            return create_if_not_exists_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task to create the table if it does not already exist.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<bool> create_if_not_exists_async(const table_request_options& options, operation_context context);

        /// <summary>
        /// Deletes a table.
        /// </summary>
        void delete_table()
        {
            delete_table_async().wait();
        }

        /// <summary>
        /// Deletes a table.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        void delete_table(const table_request_options& options, operation_context context)
        {
            delete_table_async(options, context).wait();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that deletes a table.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<void> delete_table_async()
        {
            return delete_table_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that deletes a table.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<void> delete_table_async(const table_request_options& options, operation_context context);

        /// <summary>
        /// Deletes the table if it exists.
        /// </summary>
        /// <returns><c>true</c> if the table was deleted; otherwise, <c>false</c>.</returns>
        bool delete_table_if_exists()
        {
            return delete_table_if_exists_async().get();
        }

        /// <summary>
        /// Deletes the table if it exists.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns><c>true</c> if the table was deleted; otherwise, <c>false</c>.</returns>
        bool delete_table_if_exists(const table_request_options& options, operation_context context)
        {
            return delete_table_if_exists_async(options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that deletes the table if it exists.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<bool> delete_table_if_exists_async()
        {
            return delete_table_if_exists_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that deletes the table if it exists.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<bool> delete_table_if_exists_async(const table_request_options& options, operation_context context);

        /// <summary>
        /// Checks whether the table exists.
        /// </summary>
        /// <returns><c>true</c> if table exists; otherwise, <c>false</c>.</returns>
        bool exists() const
        {
            return exists_async().get();
        }

        /// <summary>
        /// Checks whether the table exists.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation. </param>
        /// <returns><c>true</c> if table exists; otherwise, <c>false</c>.</returns>
        bool exists(const table_request_options& options, operation_context context) const
        {
            return exists_async(options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that checks whether the table exists.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<bool> exists_async() const
        {
            return exists_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that checks whether the table exists.
        /// </summary>
        /// <param name="options">A <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation. This object is used to track requests to the storage service, and to provide additional runtime information about the operation.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<bool> exists_async(const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Gets the permissions settings for the table.
        /// </summary>
        /// <returns>A <see cref="table_permissions" /> object.</returns>
        table_permissions download_permissions() const
        {
            return download_permissions_async().get();
        }

        /// <summary>
        /// Gets the permissions settings for the table.
        /// </summary>
        /// <param name="options">An <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="table_permissions" /> object.</returns>
        table_permissions download_permissions(const table_request_options& options, operation_context context) const
        {
            return download_permissions_async(options, context).get();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that gets the permissions settings for the table.
        /// </summary>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_permissions" /> that represents the current operation.</returns>
        pplx::task<table_permissions> download_permissions_async() const
        {
            return download_permissions_async(table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that gets the permissions settings for the table.
        /// </summary>
        /// <param name="options">An <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="pplx::task" /> object of type <see cref="table_permissions" /> that represents the current operation.</returns>
        WASTORAGE_API pplx::task<table_permissions> download_permissions_async(const table_request_options& options, operation_context context) const;

        /// <summary>
        /// Sets permissions for the table.
        /// </summary>
        /// <param name="permissions">A <see cref="table_permissions" /> object.</param>
        void upload_permissions(const table_permissions& permissions)
        {
            upload_permissions_async(permissions).wait();
        }

        /// <summary>
        /// Sets permissions for the table.
        /// </summary>
        /// <param name="permissions">A <see cref="table_permissions" /> object.</param>
        /// <param name="options">An <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        void upload_permissions(const table_permissions& permissions, const table_request_options& options, operation_context context)
        {
            upload_permissions_async(permissions, options, context).wait();
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that sets permissions for the table.
        /// </summary>
        /// <param name="permissions">A <see cref="table_permissions" /> object.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        pplx::task<void> upload_permissions_async(const table_permissions& permissions)
        {
            return upload_permissions_async(permissions, table_request_options(), operation_context());
        }

        /// <summary>
        /// Returns a task that performs an asynchronous operation that sets permissions for the table.
        /// </summary>
        /// <param name="permissions">A <see cref="table_permissions" /> object.</param>
        /// <param name="options">An <see cref="wa::storage::table_request_options" /> object that specifies additional options for the request.</param>
        /// <param name="context">An <see cref="wa::storage::operation_context" /> object that represents the context for the current operation.</param>
        /// <returns>A <see cref="pplx::task" /> object that represents the current operation.</returns>
        WASTORAGE_API pplx::task<void> upload_permissions_async(const table_permissions& permissions, const table_request_options& options, operation_context context);

        /// <summary>
        /// Returns a shared access signature for the table.
        /// </summary>
        /// <param name="policy">The <see cref="table_shared_access_policy" /> for the shared access signature.</param>
        /// <returns>A string containing a shared access signature.</returns>
        utility::string_t get_shared_access_signature(const table_shared_access_policy& policy) const
        {
            return get_shared_access_signature(policy, utility::string_t());
        }

        /// <summary>
        /// Returns a shared access signature for the table.
        /// </summary>
        /// <param name="policy">The <see cref="table_shared_access_policy" /> for the shared access signature.</param>
        /// <param name="stored_policy_identifier">A string identifying a table-level access policy.</param>
        /// <returns>A string containing a shared access signature.</returns>
        utility::string_t get_shared_access_signature(const table_shared_access_policy& policy, const utility::string_t& stored_policy_identifier) const
        {
            return get_shared_access_signature(policy, stored_policy_identifier, utility::string_t(), utility::string_t(), utility::string_t(), utility::string_t());
        }

        /// <summary>
        /// Returns a shared access signature for the table.
        /// </summary>
        /// <param name="policy">The <see cref="table_shared_access_policy" /> for the shared access signature.</param>
        /// <param name="stored_policy_identifier">A string identifying a table-level access policy.</param>
        /// <returns>A string containing a shared access signature.</returns>
        WASTORAGE_API utility::string_t get_shared_access_signature(const table_shared_access_policy& policy, const utility::string_t& stored_policy_identifier, const utility::string_t& start_partition_key, const utility::string_t& start_row_key, const utility::string_t& end_partition_key, const utility::string_t& end_row_key) const;

        /// <summary>
        /// Gets the <see cref="wa::storage::cloud_table_client" /> object that represents the Table service.
        /// </summary>
        /// <returns>A client object that specifies the Table service endpoint.</returns>
        const cloud_table_client& service_client() const
        {
            return m_client;
        }

        /// <summary>
        /// Gets the table name.
        /// </summary>
        /// <returns>The table name.</returns>
        const utility::string_t& name() const
        {
            return m_name;
        }

        /// <summary>
        /// Gets the table URI for all locations.
        /// </summary>
        /// <returns>A <see cref="storage_uri" /> object containing the absolute URI to the table for all locations.</returns>
        const storage_uri& uri() const
        {
            return m_uri;
        }

    private:

        cloud_table(const cloud_table_client& client, const utility::string_t& name, const storage_uri& uri)
            : m_client(client), m_name(name), m_uri(uri)
        {
        }

        static cloud_table_client create_client(const storage_uri& uri, const storage_credentials& credentials);
        static utility::string_t read_table_name(const storage_uri& uri);
        table_request_options get_modified_options(const table_request_options& options) const;
        pplx::task<bool> create_async_impl(const table_request_options& options, operation_context context, bool allow_conflict);
        pplx::task<bool> delete_async_impl(const table_request_options& options, operation_context context, bool allow_not_found);
        pplx::task<bool> exists_async_impl(const table_request_options& options, operation_context context, bool allow_secondary) const;

        /*
        void set_service_client(const cloud_table_client& client)
        {
            m_client = std::move(client);
        }
        */

        cloud_table_client m_client;
        utility::string_t m_name;
        storage_uri m_uri;

        friend class cloud_table_client;
    };

}} // namespace wa::storage
