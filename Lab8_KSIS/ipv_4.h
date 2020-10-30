#pragma once
//
// Created by wonde on 24.10.2020.
//

#ifndef UNTITLED_IPV_4_H
#define UNTITLED_IPV_4_H
#include <type_traits>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <regex>
#include <cassert>
#include <iostream>
namespace Lab8_KSIS
{

    typedef std::string address;

    class ipv4_excpetion
    {
        std::string message;
    public:
        explicit ipv4_excpetion(std::string message) : message(message) {}
        std::string get_message() noexcept { return  this->message; }
        ~ipv4_excpetion() {}
    };

    template<typename T>
    constexpr  bool is_unsigned = std::is_same<T, unsigned>::value;

    template<typename T>
    struct _address_
    {
        static_assert(is_unsigned<T>, "ipv4 must be unsigned");
        T first : 8;
        T second : 8;
        T third : 8;
        T last  : 8;
        friend const _address_& operator& (_address_ a1, _address_ a2) noexcept
        {
            _address_ a = { a1.first & a2.first,
                    a1.second & a2.second ,
                    a1.third & a2.third,
                    a1.last & a2.last };
            return a;
        }
        const _address_& operator~ () noexcept
        {
            return
            { ~this->first,
              ~this->second,
              ~this->third,
              ~this->last };
        }
        friend const _address_& operator| (_address_ a1, _address_ a2) noexcept
        {
            _address_ a = { a1.first | a2.first,
                    a1.second | a2.second ,
                    a1.third | a2.third,
                    a1.last | a2.last };
            return a;
        }
        std::string to_str() noexcept
        {
            std::string str;
            std::stringstream ss(str);
            ss << this->first << '.' << this->second << '.' << this->third << '.' << this->last;
            str = ss.str();
            return str;
        }
    };
    inline bool check_ip_address(const std::string& str_)
    {
        std::regex valid_ip("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$",
            std::regex_constants::ECMAScript);
        return std::regex_search(str_, valid_ip);

    }
    inline bool check_subnet_mask(const std::string& str_)
    {
        std::regex valid_ip("^(((255\\.){3}(255|254|252|248|240|224|192|128|0+))|((255\\.){2}(255|254|252|248|240|224|192|128|0+)\\.0)|((255\\.)(255|254|252|248|240|224|192|128|0+)(\\.0+){2})|((255|254|252|248|240|224|192|128|0+)(\\.0+){3}))$",
            std::regex_constants::ECMAScript);
        return std::regex_search(str_, valid_ip);

    }

    template<typename T>
    _address_<unsigned> string_to_address(std::string str)
    {
        _address_<unsigned> address_out;
        std::stringstream tmp_ip(str);
        unsigned int octet1; unsigned int octet2; unsigned int octet3; unsigned int octet4;
        char dot;
        tmp_ip >> octet1 >> dot >> octet2 >> dot >> octet3 >> dot >> octet4;
        address_out = { octet1, octet2, octet3, octet4 };
        return address_out;
    }
    template<typename T>
    class ipv_4
    {
        static_assert(is_unsigned<T>, "ipv4 must be unsigned");
    private:

        address ip;
    public:
        address get_ip() const
        {
            return ip;
        }

        void set_ip(const address& ip)
        {
            ipv_4::ip = ip;
        }

        address get_mask() const
        {
            return mask;
        }

        void set_mask(const address& mask)
        {
            ipv_4::mask = mask;
        }

        address get_subnet()
        {
            return string_to_address<unsigned>(ip) & string_to_address<unsigned>(mask);
        }

        address get_host() const
        {
            return string_to_address<unsigned>(ip) & ~string_to_address<unsigned>(mask);
        }

    private:
        address mask;
        address subnet;
        address host;
        address broadcast;
    public:
        address get_broadcast() const
        {
            return string_to_address<unsigned>(subnet) | ~string_to_address<unsigned>(mask);
        }

    public:
        ipv_4(const address& ip, const address& mask);
        ~ipv_4();

    public:
        friend std::ostream& operator<<(std::ostream& io, ipv_4<T> ipv_4_out) noexcept
        {
            return io << "IP: " << ipv_4_out.ip << std::endl <<
                "Mask: " << ipv_4_out.mask << std::endl <<
                "Subnet: " << ipv_4_out.subnet << std::endl <<
                "Host: " << ipv_4_out.host << std::endl <<
                "Broadcast: " << ipv_4_out.broadcast;
        }
    };


}

template<typename T>
Lab8_KSIS::ipv_4<T>::ipv_4(const address& ip, const address& mask)
{

    if (!check_ip_address(ip))
    {
        throw ipv4_excpetion("Invalid IP-address: " + ip);
    }
    if (!check_subnet_mask(mask))
    {
        throw ipv4_excpetion("Invalid subnet mask: " + mask);
    }
    this->ip = ip;
    this->mask = mask;
    _address_<unsigned> getip = string_to_address<unsigned>(ip);
    _address_<unsigned> getmask = string_to_address<unsigned>(mask);
    _address_<unsigned> getsub = getip & getmask;
    _address_<unsigned> gethost = getip & (~getmask);
    _address_<unsigned> getbroadcast = getsub | ~getmask;

    this->subnet = getsub.to_str();
    this->host = gethost.to_str();
    this->broadcast = getbroadcast.to_str();
}

template<typename T>
Lab8_KSIS::ipv_4<T>::~ipv_4()
{

}


#endif //UNTITLED_IPV_4_H

