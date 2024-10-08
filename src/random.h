// -*-c++-*-

/***************************************************************************
                                  random.h
                            Random Number Generator
                             -------------------
    begin                : 17-JAN-2002
    copyright            : (C) 2002 by The RoboCup Soccer Server
                           Maintainance Group.
    email                : sserver-admin@lists.sourceforge.net
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 3 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#ifndef RCSSSERVER_RANDOM_H
#define RCSSSERVER_RANDOM_H

#include <random>
#include <algorithm>
#include <memory>
#include <cstdlib>

// TODO: find a way to remove boost from this file

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>

class DefaultRNG
{
public:
    typedef std::mt19937 Engine;

private:
    Engine M_engine;

    DefaultRNG() = default;

public:
    static
        // DefaultRNG & instance()
        Engine &
        instance()
    {
        static DefaultRNG the_instance;
        return the_instance.M_engine;
    }

    static
        // DefaultRNG & instance( const std::mt19937::result_type & value )
        Engine &
        seed(const Engine::result_type &value)
    {
        instance().seed(value);
        return instance();
    }

    // For GCC, moving this function out-of-line prevents inlining, which may
    // reduce overall object code size.  However, MSVC does not grok
    // out-of-line definitions of member function templates.
    // template< class Generator >
    // static
    // DefaultRNG & instance( Generator & gen )
    //   {
    //       instance().engine().seed( gen() );
    //       return instance();
    //   }
};

// old random code
// #define RANDOMBASE		1000
// #define IRANDOMBASE		31

// #define drand(h,l)	(((((h)-(l)) * ((double)(random()%RANDOMBASE) / (double)RANDOMBASE))) + (l))
// #define	irand(x)	((random() / IRANDOMBASE) % (x))

inline int
irand(int x)
{
    if (x <= 1)
        return 0;

    std::uniform_int_distribution<> dst(0, x - 1);
    return dst(DefaultRNG::instance());
}

inline double
drand(double low, double high)
{
    if (low > high)
        std::swap(low, high);
    if (high - low < 1.0e-10)
        return (low + high) * 0.5;

    std::uniform_real_distribution<> rng(low, high);
    return rng(DefaultRNG::instance());
}

// TODO: drand hfo remove boost to comply with the new changes

// static inline double
// drand(double low, double high, boost::mt19937 &generator)
// {
//     if (low > high)
//         std::swap(low, high);
//     if (high - low < 1.0e-10)
//         return (low + high) * 0.5;
//     boost::uniform_real<> rng(low, high);
//     boost::variate_generator<boost::mt19937 &, boost::uniform_real<>>
//         gen(generator, rng);
//     return gen();
// }


static inline double drand(double low, double high, boost::mt19937 &generator)
{
    if (low > high)
        std::swap(low, high);  // Ensure low is less than high

    if (high - low < 1.0e-10)  // If the range is too small, return midpoint
        return (low + high) * 0.5;

    // Use boost::random::uniform_real_distribution for the range [low, high)
    boost::random::uniform_real_distribution<> dist(low, high);
    
    return dist(generator);  // Generate a random number
}

inline double
ndrand(double mean, double stddev)
{
    std::normal_distribution<> rng(mean, stddev);
    return rng(DefaultRNG::instance());
}
#endif
