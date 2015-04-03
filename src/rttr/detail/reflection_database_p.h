/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 Axel Menzel <info@axelmenzel.de>                       *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_REFLECTION_DATABASE_P_H_
#define RTTR_REFLECTION_DATABASE_P_H_

#include "rttr/type.h"
#include "rttr/detail/metadata_container.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <functional>

#define RTTR_MAX_TYPE_COUNT 32767
#define RTTR_MAX_INHERIT_TYPES_COUNT 50

#define RTTR_DEFAULT_TYPE_COUNT 4096

namespace rttr
{
namespace detail
{

/*!
 * This class holds all reflection and type data.
 * It is not part of the rttr API
 */
class RTTR_LOCAL reflection_database
{
    public:
        reflection_database() 
        {
            m_orig_names.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_custom_names.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_orig_name_to_id.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_custom_name_to_id.reserve(RTTR_DEFAULT_TYPE_COUNT);

            m_base_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
            m_derived_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
            m_conversion_list.reserve(RTTR_DEFAULT_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT);
            m_get_derived_info_func_list.reserve(RTTR_DEFAULT_TYPE_COUNT);

            m_raw_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_array_raw_type_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_variant_create_func_list.reserve(RTTR_DEFAULT_TYPE_COUNT);


            m_is_class_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_enum_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_array_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_primitive_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_function_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_member_object_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_is_member_function_pointer_list.reserve(RTTR_DEFAULT_TYPE_COUNT);
            m_pointer_dim_list.reserve(RTTR_DEFAULT_TYPE_COUNT);


            // for the invalid type we have to fill some dummy text
            m_orig_names.push_back("!invalid_type!");
            m_custom_names.push_back(m_orig_names[0]);

            m_base_class_list.push_back(0);
            m_derived_class_list.push_back(0);
            m_conversion_list.push_back(0);
            m_get_derived_info_func_list.push_back(nullptr);

            m_raw_type_list.push_back(0);
            m_array_raw_type_list.push_back(0);
            m_variant_create_func_list.push_back(nullptr);

            m_is_class_list.push_back(false);
            m_is_enum_list.push_back(false);
            m_is_array_list.push_back(false);
            m_is_pointer_list.push_back(false);
            m_is_primitive_list.push_back(false);
            m_is_function_pointer_list.push_back(false);
            m_is_member_object_pointer_list.push_back(false);
            m_is_member_function_pointer_list.push_back(false);
            m_pointer_dim_list.push_back(0);
            m_meta_data_list.push_back(metadata_container());

        }
        static reflection_database& instance() { static reflection_database obj; return obj; }

        typedef std::size_t hash_type;
        static hash_type generate_hash(const std::string& text) { return std::hash<std::string>()(text); }
        static hash_type generate_hash(const char* text)
        {
            const std::size_t  magic_prime = static_cast<std::size_t>(0x01000193);
            std::size_t               hash = static_cast<std::size_t>(0xcbf29ce4);

            for (; *text; ++text)
              hash = (hash ^ *text) * magic_prime;

            return hash;
        }

        typedef void*(*rttr_cast_func)(void*);
        typedef derived_info(*get_derived_info_func)(void*);

        struct comparison_char
        {
            RTTR_INLINE bool operator() (const char* str1, const char* str2) const
            {
                for (; *str1 && *str1 == *str2; ++str1, ++str2) {}
                    return (*str1 == *str2);
            }
        };
        /*

        typedef std::vector< std::unique_ptr< property_container_base>>             property_container;
        typedef std::vector< std::unique_ptr< constructor_container_base>>          constructor_container;
        typedef std::vector< std::unique_ptr< constructor_container_base>>          destructor_container;
        typedef std::vector< std::unique_ptr< method_container_base>>               method_container;

        typedef std::vector< std::unique_ptr< constructor_container_base>>          ctor_list;
        typedef std::unordered_map< std::string, property_container::size_type>     property_map;
        typedef std::unordered_multimap< std::string, method_container::size_type>  method_map;
        */
#if 0
        /*!
         * \brief This function returns true, when the given types in \p param_list are the same type like in \p args,
         *         otherwise false.
         */
        static bool does_signature_match_arguments(const std::vector<type>& param_list, const std::vector<type>& args);

        /*!
         * \brief This function return from the given list of arguments \p args a list of corresponding type objects.
         */
        static std::vector<type> extract_types(const std::vector<detail::argument>& args);

        /*!
         * \brief This function returns true, when in \p method_container exists already a method with the same name
         *         and signature like the given \p method.
         */
        static bool is_method_already_registered(const std::unique_ptr<detail::method_container_base>& method, const method_map& method_container);


        static void register_property(std::unique_ptr<detail::property_container_base> prop, property_map& property_container);
        static void register_method(std::unique_ptr<detail::method_container_base> method, method_map& method_container);

        static detail::property_container_base*     find_property(const std::string& name, property_map& property_container);
        static detail::method_container_base*       find_method(const std::string& name, const method_map& method_container);
        static detail::method_container_base*       find_method(const std::string& name, const std::vector<type>& params, const method_map& method_container);


        struct class_data
        {
            void add_constructor(std::unique_ptr<detail::constructor_container_base> ctor);

            reflection_database::ctor_list       m_ctor_list;
            reflection_database::property_map    m_property_map;
            reflection_database::method_map      m_method_map;
        };

    private:
#endif

        reflection_database(const reflection_database& other);
        reflection_database& operator=(const reflection_database& other);

    public:
        struct name_to_id
        {
            type::type_id   m_id;
            hash_type       m_hash_value;

            struct order_by_name
            {
                RTTR_INLINE bool operator () ( const name_to_id& _left, const name_to_id& _right )  const
                {
                    return _left.m_hash_value < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const hash_type& _left, const name_to_id& _right ) const
                {
                    return _left < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const name_to_id& _left, const hash_type& _right ) const
                {
                    return _left.m_hash_value < _right;
                }
            };
        };


        struct global_item_data
        {
            std::size_t m_index;
            hash_type   m_hash_value;

            struct order_by_name
            {
                RTTR_INLINE bool operator () ( const global_item_data& _left, const global_item_data& _right )  const
                {
                    return _left.m_hash_value < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const hash_type& _left, const global_item_data& _right ) const
                {
                    return _left < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const global_item_data& _left, const hash_type& _right ) const
                {
                    return _left.m_hash_value < _right;
                }
            };
        };

        struct item_data
        {
            type::type_id   m_id;
            std::size_t     m_index;
            hash_type       m_hash_value;

            struct order_by_name
            {
                RTTR_INLINE bool operator () ( const item_data& _left, const item_data& _right )  const
                {
                    return _left.m_hash_value < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const hash_type& _left, const item_data& _right ) const
                {
                    return _left < _right.m_hash_value;
                }
                RTTR_INLINE bool operator () ( const item_data& _left, const hash_type& _right ) const
                {
                    return _left.m_hash_value < _right;
                }
            };
        };


        type::type_id                                               m_type_id_counter;      //!< The global incremented id counter, this is unique for every type.
        std::vector<const char*>                                    m_orig_names;           //!< Contains all the raw names provied by 'type::register_type'; The type id is the index in this container
        std::vector<std::string>                                    m_custom_names;         //!< Contains all the names of m_orig_names, but the names are cleaned up (garbage strings are removed)
                                                                                            //!< and also custom names, provided during manual register (e.g. class_)
        std::vector<name_to_id>                                     m_orig_name_to_id;      //!< This is a sorted vector which contains hash values of the names in \p m_orig_names
        std::vector<name_to_id>                                     m_custom_name_to_id;    //!< This is a sorted vector which contains hash values of the names in \p m_custom_names

        std::vector<type::type_id>                                  m_base_class_list;      //!< This list contains for every type the id's of it's base classes (a.k.a. parent class)
        std::vector<type::type_id>                                  m_derived_class_list;   //!< This list contains for every type the id's of it's derived classes (a.k.a child class)
        std::vector<get_derived_info_func>                          m_get_derived_info_func_list;
        std::vector<rttr_cast_func>                                 m_conversion_list;      //!< This list contains for every type a conversion function to its base classes

        std::vector<type::type_id>                                  m_raw_type_list;
        std::vector<type::type_id>                                  m_array_raw_type_list;
        std::vector<variant_create_func>                            m_variant_create_func_list; //!< This list contains a function to create from an 'argument' a variant

        std::vector<bool>                                           m_is_class_list;
        std::vector<bool>                                           m_is_enum_list;
        std::vector<bool>                                           m_is_array_list;
        std::vector<bool>                                           m_is_pointer_list;
        std::vector<bool>                                           m_is_primitive_list;
        std::vector<bool>                                           m_is_function_pointer_list;
        std::vector<bool>                                           m_is_member_object_pointer_list;
        std::vector<bool>                                           m_is_member_function_pointer_list;
        std::vector<std::size_t>                                    m_pointer_dim_list;
                                                                    
         std::vector<metadata_container>                            m_meta_data_list;

        std::vector<std::unique_ptr<constructor_container_base>>    m_constructor_list;     //!< This list stores all constructors
        std::vector<std::unique_ptr<destructor_container_base>>     m_destructor_list;      //!< This list stores all destructor's
        std::vector<std::unique_ptr<property_container_base>>       m_property_list;        //!< This list stores all properties
        std::vector<std::unique_ptr<method_container_base>>         m_method_list;          //!< This list stores all methods
        std::vector<std::unique_ptr<enumeration_container_base>>    m_enumeration_list;     //!< This list stores all enumerations

        std::vector<global_item_data>                               m_global_property_list;
        std::vector<global_item_data>                               m_global_method_list;


        //type::type_id                                       m_base_class_list[RTTR_MAX_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT];        // this list contains for every type its base classes
        //type::type_id                                       m_derived_class_list[RTTR_MAX_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT];     // this list contains for every type its derived classes
        //rttr_cast_func                                      m_conversion_list[RTTR_MAX_TYPE_COUNT * RTTR_MAX_INHERIT_TYPES_COUNT];        // this list contains for every type a conversion function to its base classes
        //get_derived_info_func                               m_get_derived_info_func_list[RTTR_MAX_TYPE_COUNT];
        //detail::variant_create_func                         m_variant_create_func_list[RTTR_MAX_TYPE_COUNT];                              // this list contains for every type a create function to a variant
        //type::type_id                                       m_raw_type_list[RTTR_MAX_TYPE_COUNT];
        //type::type_id                                       m_array_raw_type_list[RTTR_MAX_TYPE_COUNT];
        //std::size_t                                         m_pointer_dim_list[RTTR_MAX_TYPE_COUNT];
       

        /*
        std::unique_ptr<class_data>                         m_class_data_list[RTTR_MAX_TYPE_COUNT];
        std::unique_ptr<constructor_container_base>         m_constructor_list[RTTR_MAX_TYPE_COUNT];
        std::unique_ptr<destructor_container_base>          m_destructor_list[RTTR_MAX_TYPE_COUNT];
        std::unique_ptr<enumeration_container_base>         m_enumeration_list[RTTR_MAX_TYPE_COUNT];
        std::vector<std::unique_ptr<type_converter_base>>   m_type_converter_list[RTTR_MAX_TYPE_COUNT];
        property_map                                        m_global_properties;
        method_map                                          m_global_methods;

        metadata_container                                  m_meta_data_list[RTTR_MAX_TYPE_COUNT];

        constructor_container                               m_constructor_container_list;
        destructor_container                                m_destructor_container_list;
        method_container                                    m_method_list;
        property_container                                  m_property_list;
        */
        std::mutex                                          m_register_type_mutex;
};

} // end namespace detail
} // end namespace rttr

#endif // RTTR_REFLECTION_DATABASE_P_H_
