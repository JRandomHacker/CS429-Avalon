#include "serverInfo.hpp"

template < typename T >
static void randomizeVector( std::vector< T >& vec, std::mt19937* rng ) {
    std::shuffle( vec.begin(), vec.end(), *rng );
}