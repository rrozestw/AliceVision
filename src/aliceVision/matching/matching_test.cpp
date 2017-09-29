// This file is part of the AliceVision project and is made available under
// the terms of the MPL2 license (see the COPYING.md file).

#include "aliceVision/numeric/numeric.hpp"
#include "aliceVision/matching/ArrayMatcher_bruteForce.hpp"
#include "aliceVision/matching/ArrayMatcher_kdtreeFlann.hpp"
#include "aliceVision/matching/ArrayMatcher_cascadeHashing.hpp"
#include <iostream>

#define BOOST_TEST_MODULE matching
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

using namespace std;
using namespace aliceVision;
using namespace matching;

BOOST_AUTO_TEST_CASE(Matching_ArrayMatcher_bruteForce_Simple_Dim1)
{
  const float array[] = {0, 1, 2, 3, 4};
  ArrayMatcher_bruteForce<float> matcher;
  BOOST_CHECK( matcher.Build(array, 5, 1) );

  const float query[] = {2};
  int nIndice = -1;
  float fDistance = -1.0f;
  BOOST_CHECK( matcher.SearchNeighbour( query, &nIndice, &fDistance) );

  BOOST_CHECK_EQUAL( 2, nIndice); // index of the found nearest neighbor
  BOOST_CHECK_SMALL(static_cast<double>(fDistance), 1e-8); //distance
}

BOOST_AUTO_TEST_CASE(Matching_ArrayMatcher_bruteForce_NN)
{
  const float array[] = {0, 1, 2, 5, 6};
  // no 3, because it involve the same dist as 1,1
  ArrayMatcher_bruteForce<float> matcher;
  BOOST_CHECK( matcher.Build(array, 5, 1) );

  const float query[] = {2};
  IndMatches vec_nIndice;
  vector<float> vec_fDistance;
  BOOST_CHECK( matcher.SearchNeighbours(query,1, &vec_nIndice, &vec_fDistance, 5) );

  BOOST_CHECK_EQUAL( 5, vec_nIndice.size());
  BOOST_CHECK_EQUAL( 5, vec_fDistance.size());

  // Check distances:
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[0]- Square(2.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[1]- Square(1.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[2]- Square(0.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[3]- Square(5.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[4]- Square(6.0f-2.0f)), 1e-6);

  // Check indexes:
  BOOST_CHECK_EQUAL(IndMatch(0,2), vec_nIndice[0]);
  BOOST_CHECK_EQUAL(IndMatch(0,1), vec_nIndice[1]);
  BOOST_CHECK_EQUAL(IndMatch(0,0), vec_nIndice[2]);
  BOOST_CHECK_EQUAL(IndMatch(0,3), vec_nIndice[3]);
  BOOST_CHECK_EQUAL(IndMatch(0,4), vec_nIndice[4]);
}

BOOST_AUTO_TEST_CASE(Matching_ArrayMatcher_bruteForce_Simple_Dim4)
{
  const float array[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 11};
  ArrayMatcher_bruteForce<float> matcher;
  BOOST_CHECK( matcher.Build(array, 3, 4) );

  const float query[] = {4, 5, 6, 7};
  int nIndice = -1;
  float fDistance = -1.0f;
  BOOST_CHECK( matcher.SearchNeighbour( query, &nIndice, &fDistance) );

  BOOST_CHECK_EQUAL( 1, nIndice); // index of the found nearest neighbor
  BOOST_CHECK_SMALL(static_cast<double>(fDistance), 1e-8); //distance
}

BOOST_AUTO_TEST_CASE(Matching_ArrayMatcher_kdtreeFlann_Simple__NN)
{
  const float array[] = {0, 1, 2, 5, 6};
  // no 3, because it involve the same dist as 1,1

  ArrayMatcher_kdtreeFlann<float> matcher;
  BOOST_CHECK( matcher.Build(array, 5, 1) );

  const float query[] = {2};
  IndMatches vec_nIndice;
  vector<float> vec_fDistance;
  const int NN = 5;
  BOOST_CHECK( matcher.SearchNeighbours(query, 1, &vec_nIndice, &vec_fDistance, NN) );

  BOOST_CHECK_EQUAL( 5, vec_nIndice.size());
  BOOST_CHECK_EQUAL( 5, vec_fDistance.size());

  // Check distances:
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[0]- Square(2.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[1]- Square(1.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[2]- Square(0.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[3]- Square(5.0f-2.0f)), 1e-6);
  BOOST_CHECK_SMALL(static_cast<double>(vec_fDistance[4]- Square(6.0f-2.0f)), 1e-6);

  // Check indexes:
  BOOST_CHECK_EQUAL(IndMatch(0,2), vec_nIndice[0]);
  BOOST_CHECK_EQUAL(IndMatch(0,1), vec_nIndice[1]);
  BOOST_CHECK_EQUAL(IndMatch(0,0), vec_nIndice[2]);
  BOOST_CHECK_EQUAL(IndMatch(0,3), vec_nIndice[3]);
  BOOST_CHECK_EQUAL(IndMatch(0,4), vec_nIndice[4]);
}

//-- Test LIMIT case (empty arrays)

BOOST_AUTO_TEST_CASE(Matching_ArrayMatcher_bruteForce_Simple_EmptyArrays)
{
  std::vector<float> array;
  ArrayMatcher_bruteForce<float> matcher;
  BOOST_CHECK(! matcher.Build(&array[0], 0, 4) );

  int nIndice = -1;
  float fDistance = -1.0f;
  BOOST_CHECK(! matcher.SearchNeighbour( &array[0], &nIndice, &fDistance) );
}

BOOST_AUTO_TEST_CASE(Matching_ArrayMatcher_kdtreeFlann_Simple_EmptyArrays)
{
  std::vector<float> array;
  ArrayMatcher_kdtreeFlann<float> matcher;
  BOOST_CHECK(! matcher.Build(&array[0], 0, 4) );

  int nIndice = -1;
  float fDistance = -1.0f;
  BOOST_CHECK(! matcher.SearchNeighbour( &array[0], &nIndice, &fDistance) );
}

BOOST_AUTO_TEST_CASE(Matching_Cascade_Hashing_Simple_EmptyArrays)
{
  std::vector<float> array;
  ArrayMatcher_cascadeHashing<float> matcher;
  BOOST_CHECK(! matcher.Build(&array[0], 0, 4) );

  int nIndice = -1;
  float fDistance = -1.0f;
  BOOST_CHECK(! matcher.SearchNeighbour( &array[0], &nIndice, &fDistance) );
}
