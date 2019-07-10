#ifndef BOOST_REAL_REAL_EXPLICIT_HPP
#define BOOST_REAL_REAL_EXPLICIT_HPP

#include <vector>
#include <iostream>
#include <initializer_list>
#include <string>
#include <regex>

#include <real/real_exception.hpp>
#include <real/interval.hpp>
#include <real/exact_number.hpp>

namespace boost {
    namespace real {

        /**
         * @author Laouen Mayal Louan Belloli
         *
         * @brief boost::real::real_explicit is a C++ class that fully represents real numbers as
         * a vector of digits, a sign and an exponent.
         */

        class real_explicit {

            // Number representation as a vector of digits with an integer part and a sign (+/-)
            // TODO: Add normalizations to the constructors
            exact_number explicit_number;

        public:

            /**
             * @brief *Default constructor:* Constructs an empty boost::real::real_explicit with
             * undefined representation and behaviour.
             */
            real_explicit() = default;

            /**
             * @brief *Copy constructor:* Creates a copy of the boost::real::real_explicit number other.
             *
             * @param other - the boost::real::real instance to copy.
             */
            real_explicit(const real_explicit& other)  = default;

            /**
             * @brief *String constructor:* Creates a boost::real::real_explicit instance by
             * parsing the string. The string must have a valid number, otherwise, the
             * constructor will throw an boost::real::invalid_string_number exception.
             *
             * @param number - a valid string representing a number.
             *
             * @throws boost::real::invalid_string_number exception
             */
            explicit real_explicit(const std::string& number) {
                std::regex decimal("((\\+|-)?[[:digit:]]*)(\\.(([[:digit:]]+)?))?((e|E)(((\\+|-)?)[[:digit:]]+))?");
                if (!std::regex_match (number, decimal))
                    throw boost::real::invalid_string_number_exception();
                //Know at this point that representation is valid
                std::string decimal_part = regex_replace(number, decimal, "$5");
                std::string integer_part = regex_replace(number, decimal, "$1");
                std::string exp = regex_replace(number, decimal, "$8");
                int add_exponent = exp.length() == 0 ? 0 : std::stoi(exp);
                if (integer_part[0] == '+') {
                    explicit_number.positive = true;
                    integer_part = integer_part.substr(1);
                }
                else if (integer_part[0] == '-') {
                    explicit_number.positive = false;
                    integer_part = integer_part.substr(1);
                }
                integer_part = regex_replace(integer_part, std::regex("(0?+)([[:digit:]]?+)"), "$2");
                size_t i = decimal_part.length() - 1;
                while (decimal_part[i] == '0' && i > 0) {
                    --i;
                }
                decimal_part = decimal_part.substr(0, i + 1);
                //decimal and integer parts are stripped of zeroes
                int exponent = integer_part.length() + add_exponent;
                if (decimal_part.empty()) {
                    i = integer_part.length() - 1;
                    while (integer_part[i] == '0' && i > 0)
                        --i;
                    integer_part = integer_part.substr(0, i + 1);
                }
                if (integer_part.empty()) {
                    i = 0;
                    while (decimal_part[i] == '0' && i < decimal_part.length()) {
                        ++i;
                        --exponent;
                    }
                    decimal_part = decimal_part.substr(i);
                }
                if (integer_part.empty() && decimal_part.empty()) {
                    explicit_number.digits = {0};
                    explicit_number.exponent = 0;
                    return;
                }
                explicit_number.exponent = exponent;
                for (const auto& c : integer_part ) {
                    explicit_number.digits.push_back(c - '0');
                }
                for (const auto& c : decimal_part ) {
                    explicit_number.digits.push_back(c - '0');
                }
            }           

            /**
             * @brief *Initializer list constructor with exponent:* Creates a boost::real::real_explicit
             * instance that represents the number where the exponent is used to set the number
             * integer part and the elements of the digits list are the digits the number in the same order.
             * The number is set as positive.
             *
             * @param digits - an initializer_list<int> that represents the number digits.
             * @param exponent - an integer representing the number exponent.
             */
            real_explicit(std::initializer_list<int> digits, int exponent) : explicit_number(digits, exponent)
            {};

            /**
             * @brief *Initializer list constructor with exponent and sign:* Creates a
             * boost::real::real_explicit instance that represents the number where the exponent
             * is used to set the number integer part and the elements of the digit list are the
             * digits of the number in the same order. This constructor uses the sign to determine
             * if the number is positive or negative.
             *
             * @param digits - an initializer_list<int> that represents the number digits.
             * @param exponent - an integer representing the number exponent.
             * @param positive - a bool that represents the number sign. If positive is set to true,
             * the number is positive, otherwise is negative.
             */
            real_explicit(std::initializer_list<int> digits, int exponent, bool positive) : 
                                                                                explicit_number(digits,exponent,positive)
            {};

            /**
             * @return An integer with the number exponent
             */
            int exponent() const {
                return explicit_number.exponent;
            }

            /**
             * @return A bool indicating if the number is positive (true) or negative (false)
             */
            bool positive() const {
                return explicit_number.positive;
            }

            /**
             * @return a const reference to the vector holding the number digits
             */
            const std::vector<int>& digits() const {
                return explicit_number.digits;
            }

            /**
             * @brief Constructs a new boost::real::real_explicit::const_precision_iterator that iterates the number
             * approximation intervals in increasing order according to the approximation precision.
             *
             * The iterator starts pointing to the interval with the maximum allowed precision.
             *
             * @return a boost::real::real_explicit::const_precision_iterator of the number.
             */

            /**
             * @brief Returns the n-th digit the number.
             *
             * @param n - an unsigned int number indicating the index of the requested digit.
             * @return an integer with the value of the number n-th digit.
             */
            int operator[](unsigned int n) const {
                if (n < explicit_number.digits.size()) {
                    return explicit_number.digits.at(n);
                }

                return 0;
            }

            /**
             * @brief It assigns a new copy of the other boost::real::real_explicit number in the *this boost::real::real_explicit number.
             *
             * @param other - the boost::real::real_explicit number to copy.
             * @return a reference of *this with the new represented number.
             */
            real_explicit& operator=(const real_explicit& other) = default;
        };
    }
}



#endif //BOOST_REAL_REAL_EXPLICIT_HPP
